#pragma once
#include <vector>
#include <unordered_set>
#include <functional>
#include <cmath>
#include <limits>

namespace procedural {

struct Cell {
    int x, y;
    bool operator==(const Cell& o) const { return x == o.x && y == o.y; }
};

struct CellHash {
    size_t operator()(const Cell& c) const {
        return (static_cast<size_t>(c.x) << 32) ^ static_cast<size_t>(c.y);
    }
};

struct DiagonalFirstResult {
    std::vector<Cell> caminho;
    int passos_diag = 0;
    int passos_reto = 0;
    int saltos = 0;
};

// Regra de caminhamento anisotrópico preferencial (Relatório, Seção 25):
// sempre move pro vizinho diagonal não-visitado mais próximo; só usa reto
// (H/V) quando nenhuma diagonal livre existe; salta pra célula não-visitada
// mais próxima quando nem diagonal nem reto estão livres. `mascara` (opcional)
// restringe as células válidas -- ex.: cunha afunilada para o efeito
// "reflexo de sol na água, transversal" (Seção 25.3).
class DiagonalFirst {
public:
    using Mascara = std::function<bool(int, int)>;

    static DiagonalFirstResult Gera(int n_col, int n_lin, Cell origem = {0, 0},
                                     Mascara mascara = nullptr) {
        std::unordered_set<Cell, CellHash> celulas_validas;
        for (int x = 0; x < n_col; ++x)
            for (int y = 0; y < n_lin; ++y)
                if (!mascara || mascara(x, y)) celulas_validas.insert({x, y});

        DiagonalFirstResult r;
        std::unordered_set<Cell, CellHash> visitado;
        visitado.insert(origem);
        r.caminho.push_back(origem);
        Cell atual = origem;

        auto viz_diag = [](Cell p) {
            return std::vector<Cell>{{p.x+1,p.y+1},{p.x+1,p.y-1},{p.x-1,p.y+1},{p.x-1,p.y-1}};
        };
        auto viz_reto = [](Cell p) {
            return std::vector<Cell>{{p.x+1,p.y},{p.x-1,p.y},{p.x,p.y+1},{p.x,p.y-1}};
        };
        auto livre = [&](Cell c) {
            return celulas_validas.count(c) && !visitado.count(c);
        };

        const size_t total = celulas_validas.size();
        while (visitado.size() < total) {
            Cell prox{};
            bool achou = false;

            for (auto& v : viz_diag(atual)) {
                if (livre(v)) { prox = v; achou = true; r.passos_diag++; break; }
            }
            if (!achou) {
                for (auto& v : viz_reto(atual)) {
                    if (livre(v)) { prox = v; achou = true; r.passos_reto++; break; }
                }
            }
            if (!achou) {
                double melhor_dist = std::numeric_limits<double>::infinity();
                for (auto& c : celulas_validas) {
                    if (visitado.count(c)) continue;
                    double dist = std::hypot(c.x - atual.x, c.y - atual.y);
                    if (dist < melhor_dist) { melhor_dist = dist; prox = c; achou = true; }
                }
                r.saltos++;
            }

            visitado.insert(prox);
            r.caminho.push_back(prox);
            atual = prox;
        }
        return r;
    }

    // Máscara em cunha (afunila esquerda->direita) para o efeito
    // "reflexo de sol na água, transversal" (Seção 25.3).
    static Mascara MascaraCunhaTransversal(int n_col, int n_lin,
                                            double largura_min = 0.6,
                                            double largura_max = 6.0) {
        double cy = n_lin / 2.0;
        return [=](int x, int y) {
            double largura = largura_min + (largura_max - largura_min) * (double(x) / n_col);
            return std::abs(y - cy) <= largura;
        };
    }
};

} // namespace procedural
