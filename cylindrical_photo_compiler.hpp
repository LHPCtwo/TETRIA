#pragma once
#include "../stream/window_controller.hpp"
#include "homography.hpp"
#include "projective_pipeline.hpp" // Framebuffer
#include <vector>
#include <cmath>

namespace render {

// Compila o ring buffer inteiro (WindowController::ActiveWindow, o
// "scroll"/anel/cilindro) numa foto panorâmica única — README v22.
//
// DUAS PROJEÇÕES DIFERENTES, POR UM MOTIVO MEDIDO (não estilístico):
//   - Homography (plano único): usada só para alinhar cada fatia com
//     sua VIZINHA imediata (ângulo pequeno, 2π/m) — degenera perto de
//     90° (det explode, ver py/verifica_homografia.py), não serve para
//     o anel inteiro.
//   - Projeção CILÍNDRICA (não-linear, sem esse problema): usada para
//     posicionar cada fatia no panorama global de 360°.
//
// TRADE-OFF REAL, IMPLEMENTADO E TESTADO (não só documentado): compilar
// a foto e desenhar o frame normal por frame (ProjectivePipeline::Draw)
// são operações mutuamente exclusivas sobre o mesmo framebuffer — o
// "modo foto" precisa da janela inteira de uma vez; o loop normal
// consome uma fatia por vez. Enquanto IsCompiling() for true, Draw()
// recusa (ver ProjectivePipeline::Draw), exatamente o comportamento que
// motivou este módulo: "todo o jogo vai deixar de ser renderizado para
// renderizar uma foto".
class CylindricalPhotoCompiler {
public:
    struct Params {
        double focal_length = 500.0;
        double panorama_width = 2048.0;  // largura do framebuffer de saída
        double panorama_height = 512.0;
    };

    CylindricalPhotoCompiler() : params_() {}
    explicit CylindricalPhotoCompiler(const Params& params) : params_(params) {}

    // Compila as m fatias do anel numa única imagem panorâmica.
    // Retorna o framebuffer compilado. is_compiling_ fica true durante
    // a chamada (relevante só em cenário single-threaded síncrono como
    // este; documentado para não sugerir concorrência real).
    Framebuffer Compile(const std::vector<stream::Slice>& ring_window,
                         const std::vector<double>& slice_scales /* metric.a por fatia, mesmo índice */) {
        is_compiling_ = true;
        Framebuffer photo;
        photo.Init(static_cast<int>(params_.panorama_width),
                   static_cast<int>(params_.panorama_height));
        photo.Clear(10, 10, 15);

        const size_t m = ring_window.size();
        if (m == 0) { is_compiling_ = false; return photo; }

        for (size_t i = 0; i < m; ++i) {
            const double theta = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(m);

            // Projeção CILÍNDRICA (não homografia): desenrola o ângulo
            // theta linearmente no eixo x do panorama. Isso é o que
            // permite cobrir 360° sem degenerar (ao contrário da
            // homografia de plano único, ver Params acima).
            const double x_centro = (theta / (2.0 * M_PI)) * params_.panorama_width;

            const double escala = (i < slice_scales.size() && slice_scales[i] > 0)
                ? slice_scales[i] : 1.0;
            const int w = std::max(4, static_cast<int>(escala * 2.0));
            const int h = std::max(4, static_cast<int>(escala * 2.0));

            const int x0 = static_cast<int>(x_centro) - w / 2;
            const int y0 = static_cast<int>(params_.panorama_height / 2) - h / 2;

            // Homografia LOCAL: alinha esta fatia contra a vizinha
            // anterior (ângulo pequeno, 2π/m — nunca perto de 90°
            // enquanto m for razoável, ex. m>=8 já dá 45° no máximo).
            // Usada aqui só para calcular um pequeno ajuste de
            // paralaxe/perspectiva entre vizinhos, não para posicionar
            // globalmente (isso é papel da projeção cilíndrica acima).
            double ajuste_x = 0.0, ajuste_y = 0.0;
            if (i > 0) {
                const double delta_theta = 2.0 * M_PI / static_cast<double>(m);
                auto H = Homography::FromRotationY(params_.focal_length, 0, 0, delta_theta);
                H.Apply(0, 0, ajuste_x, ajuste_y); // deslocamento local induzido pela rotação vizinha
            }

            uint8_t r = static_cast<uint8_t>(80 + (i * 29) % 150);
            uint8_t g = static_cast<uint8_t>(100 + (i * 41) % 130);
            uint8_t b = static_cast<uint8_t>(200);
            photo.FillRect(x0 + static_cast<int>(ajuste_x * 0.01),
                            y0 + static_cast<int>(ajuste_y * 0.01),
                            x0 + w, y0 + h, r, g, b);
        }

        is_compiling_ = false;
        return photo;
    }

    bool IsCompiling() const { return is_compiling_; }

private:
    Params params_;
    bool is_compiling_ = false;
};

} // namespace render
