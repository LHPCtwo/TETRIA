#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <functional>

namespace symmetry {

struct Point2 { double x, y; };

// Catálogo das 6 operações de espelho nomeadas (Relatório, Seção 22.3).
// Antes eram usadas de forma implícita e ambígua ("espelhar X") — aqui
// cada uma é uma função nomeada, para o resto da engine escolher a
// operação certa por nome em vez de reimplementar localmente.
class MirrorCatalog {
public:
    // Operação 1: reflexão no eixo Y. (x,y) -> (-x,y).
    static std::vector<Point2> EspelhoVertical(const std::vector<Point2>& pts) {
        std::vector<Point2> out = pts;
        for (auto& p : out) p.x = -p.x;
        return out;
    }

    // Operação 2: reflexão no eixo X. (x,y) -> (x,-y).
    static std::vector<Point2> EspelhoHorizontal(const std::vector<Point2>& pts) {
        std::vector<Point2> out = pts;
        for (auto& p : out) p.y = -p.y;
        return out;
    }

    // Operação 3: rotação de 180 graus. Composição de 1+2, NÃO é
    // reflexão simples.
    static std::vector<Point2> Rotacao180(const std::vector<Point2>& pts) {
        std::vector<Point2> out = pts;
        for (auto& p : out) { p.x = -p.x; p.y = -p.y; }
        return out;
    }

    // Operação 5b: espelha e TRANSLADA (nao sobrepoe) — dobra a
    // contagem de pontos exatamente e restaura o numero de lados
    // perdido por alinhamento (Relatório, Seção 22.4).
    static std::vector<Point2> EspelhaTransladado(const std::vector<Point2>& pts,
                                                   char eixo = 'x',
                                                   double deslocamento = -1.0) {
        std::vector<Point2> out = pts;
        std::vector<Point2> espelho = pts;
        for (auto& p : espelho) {
            if (eixo == 'x') p.x = -p.x + deslocamento;
            else p.y = -p.y + deslocamento;
        }
        out.insert(out.end(), espelho.begin(), espelho.end());
        return out;
    }

    // Operação 6: composição — junta peças já transformadas (com
    // deslocamento opcional), NAO é uma reflexão 1-para-1. Ex.: V = 2
    // L's (um espelhado verticalmente) unidos por vertice comum.
    static std::vector<Point2> ComposePecas(const std::vector<std::vector<Point2>>& pecas,
                                             const std::vector<Point2>& deslocamentos = {}) {
        std::vector<Point2> out;
        for (size_t i = 0; i < pecas.size(); ++i) {
            Point2 d = (i < deslocamentos.size()) ? deslocamentos[i] : Point2{0, 0};
            for (const auto& p : pecas[i]) {
                out.push_back({p.x + d.x, p.y + d.y});
            }
        }
        return out;
    }

    // Operação 2 aplicada PERIODICAMENTE: constrói uma onda triangular
    // por reflexão pura (rampa linear refletida a cada meio-período),
    // sem nenhuma função trigonométrica. Uma única rampa (nao completando
    // o periodo) e monotonica -- 0 extremos locais, o caso "agua"
    // (Seção 25.3). Ver ReflexaoRecursivaMultiescala abaixo para o caso
    // "fumaca" (N niveis desta mesma construcao, empilhados).
    static std::vector<double> OndaTriangularPorReflexao(int n_col, double periodo, double amplitude) {
        std::vector<double> perfil(n_col);
        for (int x = 0; x < n_col; ++x) {
            double t = std::fmod(double(x), periodo) / periodo;
            perfil[x] = (t < 0.5) ? amplitude * (t / 0.5)
                                   : amplitude * (1.0 - (t - 0.5) / 0.5);
        }
        return perfil;
    }

    // "Fumaça" (Seção 29): N níveis da mesma reflexão (22.3, Operação 2),
    // cada nível nascendo do anterior em escala menor -- mesma lógica
    // recursiva da árvore (Seção 22.5), aplicada a perfil de largura em
    // vez de bifurcação de segmentos. "Água" (Seção 25.3) é o caso
    // degenerado n_niveis=1 (ou uma unica rampa nao completada): 0
    // extremos, monotonica -- ver 22.4 desta mesma família.
    static std::vector<double> ReflexaoRecursivaMultiescala(int n_col, int n_niveis,
                                                              double periodo_base,
                                                              double amplitude_base,
                                                              double fator_reducao,
                                                              unsigned seed) {
        std::mt19937 rng(seed);
        std::vector<double> perfil(n_col, 0.0);
        double periodo = periodo_base;
        double amplitude = amplitude_base;
        for (int nivel = 0; nivel < n_niveis; ++nivel) {
            std::uniform_real_distribution<double> fase_dist(0.0, periodo);
            double fase = fase_dist(rng);
            auto onda = OndaTriangularPorReflexao(n_col, std::max(2.0, periodo), amplitude);
            for (int x = 0; x < n_col; ++x) {
                int idx = int(std::round(x + fase)) % n_col;
                perfil[x] += onda[idx];
            }
            amplitude *= fator_reducao;
            std::uniform_real_distribution<double> jitter(-0.3, 0.3);
            periodo /= (2.3 + jitter(rng));
        }
        return perfil;
    }

    // Rotação genérica por ângulo arbitrário (generaliza Rotacao180,
    // que é o caso theta=pi). Necessária para expressar a construção de
    // V (Seção 22.3, "L->V") com ângulo de abertura variável, em vez de
    // fixo em 90/180 graus.
    static Point2 Rotaciona(const Point2& v, double theta) {
        double c = std::cos(theta), s = std::sin(theta);
        return {c * v.x - s * v.y, s * v.x + c * v.y};
    }

    // A primitiva "V" do catálogo (Seção 22.3: dois L's, um rotacionado
    // +theta e outro -theta a partir de uma direção comum, unidos no
    // vértice de origem). Retorna as duas pontas e as duas novas
    // direções -- usada recursivamente para gerar a árvore (Seção 22.5).
    struct ResultadoV {
        Point2 ponta1, ponta2, direcao1, direcao2;
    };
    static ResultadoV GeraV(const Point2& origem, const Point2& direcao_pai,
                             double comprimento, double angulo_abertura) {
        Point2 d1 = Rotaciona(direcao_pai, angulo_abertura);
        Point2 d2 = Rotaciona(direcao_pai, -angulo_abertura);
        return {
            {origem.x + comprimento * d1.x, origem.y + comprimento * d1.y},
            {origem.x + comprimento * d2.x, origem.y + comprimento * d2.y},
            d1, d2
        };
    }

    struct Segmento { double x0, y0, x1, y1; };

    // Árvore recursiva (Seção 22.5): aplica GeraV recursivamente em cada
    // ponta -- NÃO é uma primitiva nova, é a mesma composição L->V
    // (22.3) recursiva, exatamente como o glossário já descrevia em
    // prosa ("cada galho é um V cujo vértice é o pé do próximo par de
    // V's"). Verificado por reconstrução byte-a-byte contra a versão
    // original em Python (mesmos segmentos, mesma ordem de bifurcação).
    static std::vector<Segmento> ArvoreRecursiva(const Point2& origem, const Point2& direcao,
                                                   double comprimento, int profundidade,
                                                   double angulo_abertura_graus = 25.0,
                                                   double fator_reducao = 0.72) {
        std::vector<Segmento> segmentos;
        double theta = angulo_abertura_graus * M_PI / 180.0;

        Point2 ponta0 = {origem.x + comprimento * direcao.x, origem.y + comprimento * direcao.y};
        segmentos.push_back({origem.x, origem.y, ponta0.x, ponta0.y});

        std::function<void(Point2, Point2, double, int)> rec =
            [&](Point2 o, Point2 d, double c, int p) {
                if (p == 0 || c < 0.02) return;
                auto v = GeraV(o, d, c, theta);
                segmentos.push_back({o.x, o.y, v.ponta1.x, v.ponta1.y});
                segmentos.push_back({o.x, o.y, v.ponta2.x, v.ponta2.y});
                rec(v.ponta1, v.direcao1, c * fator_reducao, p - 1);
                rec(v.ponta2, v.direcao2, c * fator_reducao, p - 1);
            };
        rec(ponta0, direcao, comprimento * fator_reducao, profundidade - 1);
        return segmentos;
    }


    // cardinalidade/ordenação de entrada e saida da operacao).
    template <typename OpFn>
    static bool EAutoSimetrico(std::vector<Point2> pts, OpFn operacao, double tol = 1e-6) {
        std::vector<Point2> transformado = operacao(pts);
        auto cmp = [](const Point2& a, const Point2& b) {
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        };
        std::sort(pts.begin(), pts.end(), cmp);
        std::sort(transformado.begin(), transformado.end(), cmp);
        if (pts.size() != transformado.size()) return false;
        for (size_t i = 0; i < pts.size(); ++i) {
            if (std::abs(pts[i].x - transformado[i].x) > tol) return false;
            if (std::abs(pts[i].y - transformado[i].y) > tol) return false;
        }
        return true;
    }
};

} // namespace symmetry
