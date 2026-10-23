#pragma once
#include "spatial_partition.hpp"
#include "union_find.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

namespace stream {

// Esquema híbrido proposto na Seção 17 ("próximos passos") de todas as
// versões do relatório, mas nunca implementado em nenhum dos 4 scripts
// (UnionFind só existe em merge_grafo.py; build_quadtree só nos outros).
//
// Passo 1 (grosseiro, O(1) classes): divide a malha em blocos de lado
//   block_size, trunca o valor de cada bloco por deslocamento e funde
//   blocos adjacentes com o mesmo valor truncado via union-find.
// Passo 2 (fino, O(L) por classe): para cada componente conexo resultante,
//   roda um quadtree só dentro da bounding box daquele componente, usando
//   o valor EXATO (não truncado) — recupera a fronteira exata perdida
//   na fusão grosseira.
//
// RESULTADO MEDIDO (não estava nos scripts, verificado nesta consolidação):
// para a malha do relatório (bloco central + T1/T2 espalhados pelos cantos
// opostos), o híbrido NÃO reduz nós em relação ao quadtree puro — pelo
// contrário: para L=256,k=85 o quadtree puro usa 1.673 nós (Tabela 1),
// mas o híbrido usa 2.010 (337 do bloco central + 1.673 do "resto").
// Causa: a fusão grosseira separa "bloco central" (bbox pequena, ganha)
// de "resto da malha" (T1+T2+fora do triângulo), mas esse segundo
// componente tem bounding box quase igual à malha inteira — T1 e T2 ficam
// em cantos opostos, então a bbox do componente cobre tudo entre eles.
// O híbrido só ganha quando os componentes fundidos formam regiões
// geometricamente compactas (bbox pequena); com regiões espalhadas em
// forma de T nos dois cantos, a bbox do componente "resto" degenera para
// o tamanho da malha inteira e o Passo 2 perde todo o ganho do Passo 1.
struct HybridResult {
    int num_components = 0;
    // Um quadtree por componente, cobrindo a bounding box daquele componente.
    std::vector<std::unique_ptr<QuadNode>> refined_trees;
    std::vector<int> total_nodes_per_component;
};

class HybridPartition {
public:
    static HybridResult Build(const ValueFn& exactValue, int L, int block_size, int k_shift) {
        const int nb = (L + block_size - 1) / block_size;

        // ---- Passo 1: classificação grosseira via union-find (Seção 8) ----
        std::vector<uint8_t> truncated(nb * nb);
        for (int by = 0; by < nb; ++by) {
            for (int bx = 0; bx < nb; ++bx) {
                int x0 = bx * block_size, y0 = by * block_size;
                truncated[by * nb + bx] = TruncaPorShift(exactValue(x0, y0), k_shift);
            }
        }

        UnionFind uf(nb * nb);
        auto idx = [nb](int bx, int by) { return by * nb + bx; };
        for (int by = 0; by < nb; ++by) {
            for (int bx = 0; bx < nb; ++bx) {
                if (bx + 1 < nb && truncated[idx(bx, by)] == truncated[idx(bx + 1, by)])
                    uf.Union(idx(bx, by), idx(bx + 1, by));
                if (by + 1 < nb && truncated[idx(bx, by)] == truncated[idx(bx, by + 1)])
                    uf.Union(idx(bx, by), idx(bx, by + 1));
            }
        }

        // agrupa blocos por componente e calcula bounding box de cada um
        struct BBox { int minx, miny, maxx, maxy; };
        std::unordered_map<int, BBox> boxes;
        for (int by = 0; by < nb; ++by) {
            for (int bx = 0; bx < nb; ++bx) {
                int root = uf.Find(idx(bx, by));
                int x0 = bx * block_size, y0 = by * block_size;
                int x1 = std::min(x0 + block_size, L) - 1;
                int y1 = std::min(y0 + block_size, L) - 1;
                auto it = boxes.find(root);
                if (it == boxes.end()) {
                    boxes[root] = BBox{x0, y0, x1, y1};
                } else {
                    it->second.minx = std::min(it->second.minx, x0);
                    it->second.miny = std::min(it->second.miny, y0);
                    it->second.maxx = std::max(it->second.maxx, x1);
                    it->second.maxy = std::max(it->second.maxy, y1);
                }
            }
        }

        // ---- Passo 2: refino fino por quadtree, um por componente (Seção 5) ----
        HybridResult result;
        result.num_components = static_cast<int>(boxes.size());

        for (const auto& [root, box] : boxes) {            // quadtree cobrindo a bounding box do componente, arredondada
            // para a próxima potência de 2 (exigência do quadtree binário)
            int span = std::max(box.maxx - box.minx + 1, box.maxy - box.miny + 1);
            int size = 1;
            while (size < span) size *= 2;

            int bx0 = box.minx, by0 = box.miny;
            auto localValue = [&exactValue, L](int x, int y) -> uint8_t {
                if (x >= L || y >= L) return 255; // fora da malha original: valor sentinela
                return exactValue(x, y);
            };
            // desloca a origem para (bx0,by0): quadtree local, fronteira exata
            auto shiftedValue = [&localValue, bx0, by0](int x, int y) {
                return localValue(bx0 + x, by0 + y);
            };

            auto tree = SpatialPartition::Build(0, 0, size, shiftedValue);
            result.total_nodes_per_component.push_back(tree->NodeCount());
            result.refined_trees.push_back(std::move(tree));
        }

        return result;
    }
};

} // namespace stream
