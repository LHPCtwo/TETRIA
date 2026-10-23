// main.cpp
// Núcleo do grafo radial ("Render Loop"). Cada aresta do grafo module_graph_radial.dot
// que sai de Core corresponde exatamente a uma chamada aqui, na mesma ordem:
//
//   Core -> HardwareProfiler   (init 1x)      -> HardwareProfiler::Profile()
//   Core -> Disco              (I/O)          -> SliceStreamer lê disco
//   Core -> SliceStreamer      (por frame)    -> SliceStreamer::PullSlice()
//   Core -> WindowController   (por frame)    -> WindowController::Slide()
//   Core -> BoundingReference  (por frame)    -> BoundingReference::Evaluate()
//   Core -> ProjectivePipeline (por frame)    -> Draw() OU Compile() [ramo mutuamente exclusivo, README v22/v23]
//   Core -> Framebuffer        (present)      -> Present() [só no ramo Draw()]
//
// Se um módulo novo virar vértice do polígono, ele entra aqui como mais uma chamada
// dentro do loop (ou na inicialização, se for 1x), na mesma ordem do .dot.

#include "core/hardware_profiler.hpp"
#include "stream/slice_streamer.hpp"
#include "stream/window_controller.hpp"
#include "render/bounding_reference.hpp"
#include "render/projective_pipeline.hpp"
#include "render/cylindrical_photo_compiler.hpp"

#include <chrono>
#include <thread>
#include <cstdio>

int main() {
    // ---- 1. Core -> HardwareProfiler (init 1x) ----
    core::HardwareProfiler profiler;
    core::WindowLimits limits = profiler.Profile();       // mede VRAM, PCIe, throughput
    const std::size_t m = limits.max_window_slices;       // teto fixo da janela m

    // ---- 2. Reserva de memória: Ring Buffer de tamanho m ----
    stream::WindowController windowController(m);
    stream::SliceStreamer sliceStreamer("assets/world.stream"); // abre o "Storage/Disco"

    render::BoundingReference boundingRef;
    render::ProjectivePipeline pipeline(limits);
    render::CylindricalPhotoCompiler photoCompiler; // README v22/v23

    bool running = true;
    Vec3 cameraPos{0.0f, 0.0f, 0.0f};
    int photo_count = 0;

    // ---- Render Loop: cada iteração percorre as arestas do núcleo, em ordem ----
    while (running) {
        // AdvanceFrame() no início da iteração: sincroniza a numeração
        // de frame usada por Present() (nome do arquivo) e por
        // ConsumePhotoRequest() (gatilho do fallback simulado) — evita
        // o off-by-one que apareceu quando o incremento ficava
        // espalhado dentro de Present() (só chamado num dos dois ramos).
        pipeline.AdvanceFrame();

        // Core -> Disco -> SliceStreamer (por frame): puxa a fatia sob a posição atual
        stream::Slice slice = sliceStreamer.PullSlice(cameraPos);

        // Core -> WindowController (por frame): scroll do ring buffer,
        // descarta fatia defasada, insere a nova
        windowController.Slide(cameraPos, slice);

        // Core -> BoundingReference (por frame): aplica L² - termos de corte
        // sobre a janela ativa devolvida pelo WindowController
        auto correctedGeometry = boundingRef.Evaluate(windowController.ActiveWindow());

        // MODO FOTO (README v22/v23): mutuamente exclusivo com Draw()
        // normal — o mesmo trade-off testado em
        // tests/test_photo_mode_exclusion.cpp (28 Draw + 2 Compile = 30
        // frames, nunca os dois no mesmo frame). Consome o pedido (tecla
        // real 'p' ou fallback simulado headless) ANTES de decidir a
        // ramificação, não depois.
        if (pipeline.ConsumePhotoRequest()) {
            std::vector<double> slice_scales;
            slice_scales.reserve(correctedGeometry.metrics.size());
            for (const auto& metric : correctedGeometry.metrics) {
                slice_scales.push_back(metric.a); // mesma escala usada por Draw()
            }
            auto photo = photoCompiler.Compile(windowController.ActiveWindow(), slice_scales);
            ++photo_count;
            char path[128];
            snprintf(path, sizeof(path), "/tmp/photo_%03d.ppm", photo_count);
            photo.SavePPM(path);
            // Draw() NÃO é chamado neste frame — exclusão mútua real,
            // não só documentada.
        } else {
            // Core -> ProjectivePipeline (por frame): projeção linear direta
            pipeline.Draw(correctedGeometry, cameraPos);

            // Core -> Framebuffer (present)
            pipeline.Present();
        }

        // AdvanceFrame() já rodou no início desta iteração (ver acima).

        // realimentação: nova posição da câmera fecha o loop (aresta pontilhada do grafo)
        cameraPos = pipeline.PollCameraInput();
        running = !pipeline.ShouldClose();
    }

    return 0;
}
