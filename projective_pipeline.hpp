#pragma once
#include "../core/vec3.hpp"
#include "../core/hardware_profiler.hpp"
#include "bounding_reference.hpp"
#include "keyboard_input.hpp"
#include "vanishing_point.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace render {

// Framebuffer RGB real em memória (não é GPU/VRAM — é RAM do processo,
// documentado como limitação: ver README v8). Sem API gráfica real
// (Vulkan/DX12) disponível neste ambiente (headless, sem display), esta
// é uma implementação HONESTA de rasterização por software: real, testável,
// produz imagem de verdade em disco — não é mais um stub vazio.
struct Framebuffer {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> pixels; // RGB, width*height*3 bytes

    void Init(int w, int h);
    void Clear(uint8_t r, uint8_t g, uint8_t b);
    void FillRect(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);
    bool SavePPM(const std::string& path) const;
};

class ProjectivePipeline {
public:
    explicit ProjectivePipeline(const core::WindowLimits& limits);

    // Aresta Core -> ProjectivePipeline (por frame): transformação linear
    // projetiva direta 3D->2D (projeção ortográfica simples), rasterizada
    // em software no framebuffer_ interno. AGORA mede o padrão de escala
    // real entre as fatias da janela ativa (README v21,
    // VanishingPointAnalyzer) antes de projetar — em vez de assumir
    // ponto de fuga clássico, decide entre modelo linear e log-linear
    // (Seção 19.5 do relatório) por medição, igual o HardwareProfiler
    // mede RAM em vez de assumir um valor fixo.
    void Draw(const CorrectedGeometry& geometry, const Vec3& cameraPos);

    // Último padrão de escala detectado (para inspeção/teste) — não
    // escondido dentro de Draw, exposto pra quem quiser auditar a
    // decisão tomada.
    const ScalePattern& LastScalePattern() const { return last_pattern_; }

    // Aresta Core -> Framebuffer (present): grava o frame atual em disco
    // (PPM) a cada N frames, para inspeção real do resultado.
    void Present();

    // Realimentação (aresta pontilhada do grafo): fecha o loop com a
    // posição atualizada. AGORA input REAL de teclado (README v19,
    // KeyboardInput: termios raw mode, leitura de bytes de stdin) —
    // funciona com um humano digitando num terminal interativo. Se
    // nenhuma tecla chegar (stdin não é TTY, ex.: rodando sem terminal
    // anexado), cai em fallback de voo automático simulado, para o loop
    // continuar exercitável em testes automatizados — a distinção entre
    // os dois modos é reportada por InputModeIsReal().
    Vec3 PollCameraInput();

    bool InputModeIsReal() const { return using_real_input_; }

    // Pedido real de foto (tecla 'p', README v23): consome e reseta a
    // flag. Fallback documentado para modo headless (sem TTY, sem tecla
    // real chegando): dispara sozinho a cada N frames, mesmo padrão de
    // honestidade já usado no fallback de movimento — para que
    // engine_fluxo rodando sem terminal anexado ainda exercite o modo
    // foto de ponta a ponta (não é um humano pedindo, documentado).
    bool ConsumePhotoRequest();

    // Incrementa a contagem de frame — chamado 1x por iteração do loop,
    // independente de qual ramo rodou (Draw()+Present() ou modo foto).
    // Separado de Present() de propósito: ver .cpp para o bug real que
    // essa separação corrige.
    void AdvanceFrame();

    bool ShouldClose() const;

    int FrameCount() const { return frame_count_; }

private:
    core::WindowLimits limits_;
    Framebuffer framebuffer_;
    KeyboardInput keyboard_;
    ScalePattern last_pattern_{ScalePattern::Model::kNeither, 0.0, 0.0, 0.0};
    int frame_count_ = 0;
    int max_frames_;      // loop termina após esse número de frames OU tecla 'q'
    Vec3 sim_camera_;     // estado da câmera (real, via teclado, ou fallback simulado)
    float sim_velocity_z_ = 1.0f;
    bool using_real_input_ = false;
    bool quit_requested_ = false;
};

} // namespace render
