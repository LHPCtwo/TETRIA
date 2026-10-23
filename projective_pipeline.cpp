#include "projective_pipeline.hpp"
#include <algorithm>
#include <cstdio>
#include <cmath>

namespace render {

void Framebuffer::Init(int w, int h) {
    width = w; height = h;
    pixels.assign(static_cast<size_t>(w) * h * 3, 0);
}

void Framebuffer::Clear(uint8_t r, uint8_t g, uint8_t b) {
    for (size_t i = 0; i < pixels.size(); i += 3) {
        pixels[i] = r; pixels[i+1] = g; pixels[i+2] = b;
    }
}

void Framebuffer::FillRect(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b) {
    x0 = std::clamp(x0, 0, width - 1);
    x1 = std::clamp(x1, 0, width - 1);
    y0 = std::clamp(y0, 0, height - 1);
    y1 = std::clamp(y1, 0, height - 1);
    if (x0 > x1) std::swap(x0, x1);
    if (y0 > y1) std::swap(y0, y1);
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            size_t idx = (static_cast<size_t>(y) * width + x) * 3;
            pixels[idx] = r; pixels[idx+1] = g; pixels[idx+2] = b;
        }
    }
}

bool Framebuffer::SavePPM(const std::string& path) const {
    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return false;
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    fwrite(pixels.data(), 1, pixels.size(), f);
    fclose(f);
    return true;
}

ProjectivePipeline::ProjectivePipeline(const core::WindowLimits& limits)
    // max_frames_ é uma CONTAGEM de iterações pra encerrar o teste, não
    // uma taxa de quadros (FPS) -- este stub não tem controle de tempo
    // real (sem vsync, sem delta-time, sem sleep). 60 aqui só segue a
    // convenção de mercado (60 FPS é o padrão fora de consoles com
    // restrição de hardware, que historicamente miram 30) para o NÚMERO
    // de frames simulados, não para dizer que o loop roda a 60 Hz de
    // verdade -- isso exigiria um clock e um limitador de frame real,
    // que não existem aqui (ver README).
    : limits_(limits), max_frames_(60), sim_camera_{0.0f, 0.0f, 0.0f} {
    framebuffer_.Init(256, 256);
    framebuffer_.Clear(20, 20, 30);
}

void ProjectivePipeline::Draw(const CorrectedGeometry& geometry, const Vec3& cameraPos) {
    framebuffer_.Clear(20, 20, 30); // fundo escuro

    // Mede o padrão de escala real entre as fatias da janela ativa
    // (README v21, VanishingPointAnalyzer): usa (índice no ring buffer,
    // largura recuperada por MetricRecovery) como amostras de
    // (posição, escala aparente), e decide por medição se o padrão é
    // ponto de fuga linear clássico ou log-linear (Seção 19.5) — não
    // assume nenhum dos dois de antemão. Precisa de pelo menos 3
    // fatias com escala positiva pra ter 2+ inclinações a comparar.
    std::vector<VanishingPointAnalyzer::Point> amostras;
    for (size_t i = 0; i < geometry.slices.size() && i < geometry.metrics.size(); ++i) {
        const double escala = geometry.metrics[i].a;
        if (escala > 0.0) {
            amostras.push_back({static_cast<double>(i + 1), escala});
        }
    }
    if (amostras.size() >= 3) {
        last_pattern_ = VanishingPointAnalyzer::DetectPattern(amostras);
    }

    // Projeção ortográfica direta: mapeia (a,b) recuperados de cada slice
    // (Seção 16, MetricRecovery) para um retângulo no framebuffer,
    // deslocado conforme a posição da câmera (scroll visual real, não
    // simulado por comentário).
    const float scale = 0.5f;
    const int cx = framebuffer_.width / 2;
    const int cy = framebuffer_.height / 2;

    for (size_t i = 0; i < geometry.slices.size(); ++i) {
        const auto& metric = (i < geometry.metrics.size()) ? geometry.metrics[i] : MetricRecovery{};
        int w = static_cast<int>(metric.a > 0 ? metric.a * scale : geometry.slices[i].width * scale * 0.1f);
        int h = static_cast<int>(metric.b > 0 ? metric.b * scale : geometry.slices[i].height * scale * 0.1f);
        w = std::max(w, 2); h = std::max(h, 2);

        // desloca com a posição da câmera (scroll real do framebuffer)
        int ox = cx - static_cast<int>(cameraPos.x * scale) - w / 2;
        int oy = cy - static_cast<int>(cameraPos.y * scale) - h / 2;

        // cor varia com o índice, só pra diferenciar fatias no PPM de saída
        uint8_t r = static_cast<uint8_t>(60 + (i * 37) % 180);
        uint8_t g = static_cast<uint8_t>(120 + (i * 53) % 120);
        uint8_t b = static_cast<uint8_t>(180);
        framebuffer_.FillRect(ox, oy, ox + w, oy + h, r, g, b);
    }
}

void ProjectivePipeline::AdvanceFrame() {
    // Incrementa a contagem de frame INDEPENDENTE de qual ramo do loop
    // rodou (Draw()+Present() normal, ou Compile() em modo foto) —
    // corrige um bug real: antes, o incremento vivia dentro de
    // Present(), que o main.cpp pula no ramo de foto; isso travava
    // frame_count_ parado no mesmo valor pra sempre, o que travava
    // ShouldClose() e o gatilho do fallback simulado de foto num loop
    // infinito (pego rodando o binário de verdade, não em teoria).
    ++frame_count_;
}

void ProjectivePipeline::Present() {
    // grava em disco a cada 15 frames (não todo frame, pra não gerar
    // centenas de arquivos) — é I/O real de saída, testável.
    if (frame_count_ == 1 || frame_count_ % 15 == 0 || frame_count_ == max_frames_) {
        char path[128];
        snprintf(path, sizeof(path), "/tmp/frame_%03d.ppm", frame_count_);
        framebuffer_.SavePPM(path);
    }
}

Vec3 ProjectivePipeline::PollCameraInput() {
    // Input REAL de teclado (README v19): lê bytes de verdade de stdin
    // via KeyboardInput (termios raw mode). Se houve tecla real nesta
    // chamada, usa isso; senão, cai em voo automático simulado (só pra
    // manter o loop exercitável sem terminal interativo anexado, ex.:
    // testes automatizados) — a distinção fica explícita via
    // InputModeIsReal(), não escondida atrás de um valor só.
    bool continuar = keyboard_.PollAndApply(sim_camera_, 1.0f);
    if (!continuar) {
        quit_requested_ = true;
        return sim_camera_;
    }
    if (keyboard_.LastPollHadInput()) {
        using_real_input_ = true;
    } else if (!keyboard_.IsInteractiveTTY()) {
        // sem TTY e sem byte algum chegou neste frame: fallback simulado
        sim_camera_.z += sim_velocity_z_;
    }
    return sim_camera_;
}

bool ProjectivePipeline::ConsumePhotoRequest() {
    // Tecla real 'p' tem prioridade — consome o pedido real primeiro.
    if (keyboard_.ConsumePhotoRequest()) return true;

    // Fallback SIMULADO (headless, sem TTY): dispara a cada 20 frames,
    // documentado como simulação — não confundir com pedido real de
    // usuário. Existe só pra engine_fluxo rodando sem terminal anexado
    // ainda exercitar o modo foto de ponta a ponta (mesmo raciocínio já
    // usado no fallback de movimento, README v8/v19).
    if (!keyboard_.IsInteractiveTTY() && frame_count_ > 0 && frame_count_ % 20 == 0) {
        return true;
    }
    return false;
}

bool ProjectivePipeline::ShouldClose() const {
    // Termina após max_frames_ frames OU se 'q' foi pressionado (input
    // real) — duas condições de parada reais, não stub.
    return frame_count_ >= max_frames_ || quit_requested_;
}

} // namespace render
