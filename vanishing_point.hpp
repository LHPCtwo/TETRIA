#pragma once
#include <vector>
#include <cmath>
#include <limits>

namespace render {

// Detector de padrão de escala projetiva (README v21, Seção 19.5 do
// relatório / "Registro de Progresso", Rota B): dada uma sequência de
// amostras (posição, escala aparente) — ex.: uma fatia por nível de
// LOD/tesselação —, decide por MEDIÇÃO, não suposição, se o padrão é:
//
//   (a) linear — retas convergem a um ponto de fuga clássico
//       (inclinação constante em (x,y));
//   (b) log-linear — relação exponencial x_i = x0*r^i, que só fica
//       linear em (ln x, y) (Seção 19.5), típica de escala geométrica
//       repetida como LOD/quadtree/m-tree (fator m^i entre níveis).
//
// Isso importa porque a tesselação por níveis desta engine (ring buffer
// de fatias, quadtree/m-tree por potências) tem exatamente essa forma
// geométrica repetida — assumir "ponto de fuga clássico" ao projetar
// entre níveis seria assumir o modelo errado por construção. A engine
// mede, como já faz o HardwareProfiler para decidir a janela m.
struct ScalePattern {
    enum class Model { kLinear, kLogLinear, kBothDegenerate, kNeither };
    Model model;
    double slope;         // inclinação do modelo vencedor
    double var_linear;
    double var_log;
};

class VanishingPointAnalyzer {
public:
    struct Point { double x, y; };

    static double Mean(const std::vector<double>& v) {
        double s = 0;
        for (double x : v) s += x;
        return s / v.size();
    }

    static double Variance(const std::vector<double>& v) {
        const double m = Mean(v);
        double s = 0;
        for (double x : v) s += (x - m) * (x - m);
        return s / v.size();
    }

    static std::vector<double> InclinacoesConsecutivas(const std::vector<Point>& pts) {
        std::vector<double> out;
        for (size_t i = 0; i + 1 < pts.size(); ++i) {
            const double dx = pts[i+1].x - pts[i].x;
            if (dx == 0.0) out.push_back(std::numeric_limits<double>::infinity());
            else out.push_back((pts[i+1].y - pts[i].y) / dx);
        }
        return out;
    }

    // tolerancia_relativa: mesmo parâmetro do protótipo Python
    // (py/detecta_padrao_escala.py), mantido igual para paridade.
    static ScalePattern DetectPattern(const std::vector<Point>& points,
                                       double tolerancia_relativa = 0.02) {
        auto inclin_linear = InclinacoesConsecutivas(points);
        const double var_linear = Variance(inclin_linear);
        const double media_linear = Mean(inclin_linear);

        std::vector<Point> pontos_log;
        for (const auto& p : points) {
            if (p.x > 0) pontos_log.push_back({std::log(p.x), p.y});
        }
        auto inclin_log = InclinacoesConsecutivas(pontos_log);
        const double var_log = inclin_log.empty()
            ? std::numeric_limits<double>::infinity() : Variance(inclin_log);
        const double media_log = inclin_log.empty()
            ? std::numeric_limits<double>::quiet_NaN() : Mean(inclin_log);

        const double limiar_linear = (std::abs(media_linear) * tolerancia_relativa)
            * (std::abs(media_linear) * tolerancia_relativa) + 1e-9;
        const double limiar_log = (std::abs(media_log) * tolerancia_relativa)
            * (std::abs(media_log) * tolerancia_relativa) + 1e-9;

        const bool eh_linear = var_linear < limiar_linear;
        const bool eh_log = var_log < limiar_log;

        ScalePattern result;
        result.var_linear = var_linear;
        result.var_log = var_log;

        if (eh_linear && !eh_log) {
            result.model = ScalePattern::Model::kLinear;
            result.slope = media_linear;
        } else if (eh_log && !eh_linear) {
            result.model = ScalePattern::Model::kLogLinear;
            result.slope = media_log;
        } else if (eh_linear && eh_log) {
            result.model = ScalePattern::Model::kBothDegenerate;
            result.slope = media_linear;
        } else {
            result.model = ScalePattern::Model::kNeither;
            result.slope = std::numeric_limits<double>::quiet_NaN();
        }
        return result;
    }
};

} // namespace render
