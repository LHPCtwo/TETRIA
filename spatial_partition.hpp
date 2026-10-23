#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <algorithm>
#include <vector>

namespace stream {

// Função de ocupação/valor da malha: value(x, y) -> uint8_t
using ValueFn = std::function<uint8_t(int x, int y)>;

// Nó de quadtree: folha (valor único) ou 4 filhos.
// Réplica direta do compress_demo.py descrito na Seção 5 do relatório:
//   - região homogênea (4 cantos + centro com mesmo valor) vira folha, nunca expande;
//   - blocos de tamanho <= 2 usam checagem exata célula a célula, porque a
//     hipotenusa é uma fronteira fina que pode escapar da amostragem por cantos
//     (Seção 9: é justamente essa amostragem heurística que causa a sensibilidade
//     ao alinhamento binário de k).
struct QuadNode {
    int x0, y0, size;
    bool is_leaf = true;
    uint8_t value = 0;
    std::unique_ptr<QuadNode> children[4]; // NW, NE, SW, SE

    int NodeCount() const {
        if (is_leaf) return 1;
        int total = 1;
        for (const auto& c : children) if (c) total += c->NodeCount();
        return total;
    }
};

class SpatialPartition {
public:
    // Constrói o quadtree completo para uma malha size x size usando `value`.
    static std::unique_ptr<QuadNode> Build(int x0, int y0, int size, const ValueFn& value) {
        auto node = std::make_unique<QuadNode>();
        node->x0 = x0; node->y0 = y0; node->size = size;

        if (size <= 2) {
            // checagem exata célula a célula (Seção 5, nota de rodapé)
            uint8_t first = value(x0, y0);
            bool homogeneous = true;
            for (int y = y0; y < y0 + size && homogeneous; ++y)
                for (int x = x0; x < x0 + size; ++x)
                    if (value(x, y) != first) { homogeneous = false; break; }

            if (homogeneous) {
                node->is_leaf = true;
                node->value = first;
                return node;
            }
        } else {
            // amostragem por 4 cantos + centro (heurística, sujeita ao viés
            // de alinhamento binário descrito na Seção 9)
            uint8_t c00 = value(x0, y0);
            uint8_t c10 = value(x0 + size - 1, y0);
            uint8_t c01 = value(x0, y0 + size - 1);
            uint8_t c11 = value(x0 + size - 1, y0 + size - 1);
            uint8_t cc  = value(x0 + size / 2, y0 + size / 2);

            if (c00 == c10 && c10 == c01 && c01 == c11 && c11 == cc) {
                node->is_leaf = true;
                node->value = c00;
                return node;
            }
        }

        // não homogêneo: subdivide em 4
        node->is_leaf = false;
        const int half = size / 2;
        node->children[0] = Build(x0,        y0,        half, value); // NW
        node->children[1] = Build(x0 + half, y0,        half, value); // NE
        node->children[2] = Build(x0,        y0 + half, half, value); // SW
        node->children[3] = Build(x0 + half, y0 + half, half, value); // SE
        return node;
    }

    // Reconstrução: desce a árvore até a folha correspondente, sem
    // materializar nenhum array L x L (Seção 5, "regra de reconstrução").
    static uint8_t Query(const QuadNode& node, int x, int y) {
        if (node.is_leaf) return node.value;
        const int half = node.size / 2;
        const bool east  = x >= node.x0 + half;
        const bool south = y >= node.y0 + half;
        const int idx = (south ? 2 : 0) + (east ? 1 : 0);
        return Query(*node.children[idx], x, y);
    }
};

// Função de ocupação/valor do volume: value(x, y, z) -> uint8_t
using ValueFn3D = std::function<uint8_t(int x, int y, int z)>;

// Octree: generalização direta do QuadNode para 3D (Seção 12 do relatório,
// réplica de octree3d.py). Mesmo critério de homogeneidade — agora 8 cantos
// + centro do cubo — exato apenas para blocos de tamanho <= 2.
struct OctNode {
    int x0, y0, z0, size;
    bool is_leaf = true;
    uint8_t value = 0;
    std::unique_ptr<OctNode> children[8]; // ordem: dx,dy,dz em {0,half}, dz mais externo

    int NodeCount() const {
        if (is_leaf) return 1;
        int total = 1;
        for (const auto& c : children) if (c) total += c->NodeCount();
        return total;
    }
};

class SpatialPartition3D {
public:
    static std::unique_ptr<OctNode> Build(int x0, int y0, int z0, int size, const ValueFn3D& value) {
        auto node = std::make_unique<OctNode>();
        node->x0 = x0; node->y0 = y0; node->z0 = z0; node->size = size;

        if (size <= 2) {
            uint8_t first = value(x0, y0, z0);
            bool homogeneous = true;
            for (int z = z0; z < z0 + size && homogeneous; ++z)
                for (int y = y0; y < y0 + size && homogeneous; ++y)
                    for (int x = x0; x < x0 + size; ++x)
                        if (value(x, y, z) != first) { homogeneous = false; break; }

            if (homogeneous) {
                node->is_leaf = true;
                node->value = first;
                return node;
            }
        } else {
            // amostragem por 8 cantos + centro (mesma heurística do QuadNode,
            // generalizada; sujeita ao mesmo viés de alinhamento da Seção 9)
            const int s1 = size - 1;
            uint8_t corners[8] = {
                value(x0,      y0,      z0),
                value(x0 + s1, y0,      z0),
                value(x0,      y0 + s1, z0),
                value(x0,      y0,      z0 + s1),
                value(x0 + s1, y0 + s1, z0),
                value(x0 + s1, y0,      z0 + s1),
                value(x0,      y0 + s1, z0 + s1),
                value(x0 + s1, y0 + s1, z0 + s1),
            };
            uint8_t center = value(x0 + size / 2, y0 + size / 2, z0 + size / 2);

            bool homogeneous = true;
            for (uint8_t c : corners) if (c != corners[0]) { homogeneous = false; break; }
            if (homogeneous && center == corners[0]) {
                node->is_leaf = true;
                node->value = corners[0];
                return node;
            }
        }

        node->is_leaf = false;
        const int half = size / 2;
        int idx = 0;
        for (int dx : {0, half})
            for (int dy : {0, half})
                for (int dz : {0, half})
                    node->children[idx++] = Build(x0 + dx, y0 + dy, z0 + dz, half, value);
        return node;
    }

    static uint8_t Query(const OctNode& node, int x, int y, int z) {
        if (node.is_leaf) return node.value;
        const int half = node.size / 2;
        const int ix = (x >= node.x0 + half) ? 1 : 0;
        const int iy = (y >= node.y0 + half) ? 1 : 0;
        const int iz = (z >= node.z0 + half) ? 1 : 0;
        const int idx = ix * 4 + iy * 2 + iz; // mesma ordem de construção: dx,dy,dz
        return Query(*node.children[idx], x, y, z);
    }
};

// Teste de homogeneidade GEOMÉTRICO EXATO (Seção 9/17, "próximos passos"):
// decide homogeneidade por interseção analítica reta-retângulo em vez de
// amostragem por cantos+centro. O(1) por nó, sem chance de falso positivo.
//
// ACHADO (verificado nesta consolidação, ver homogeneidade_exata.py e
// README.md v5): este teste produz EXATAMENTE os mesmos números de nós que
// a amostragem heurística de SpatialPartition::Build, para todos os casos
// testados (Tabela 1 completa + Tabela 3). Isso contradiz a hipótese
// original da Seção 9 — a sensibilidade de nós/L ao alinhamento binário de
// k/L não vem de falso positivo da amostragem; é uma propriedade geométrica
// real de como a posição do bloco interage com a grade de subdivisão por
// potências de 2. A amostragem por cantos+centro já era, na prática, exata
// para esta geometria específica (fronteiras retas simples).
class SpatialPartitionExact {
public:
    // occ(x,y) só é chamado para decidir CLASSE via geometria fechada
    // (hipotenusa x+y=L, bloco [c,c+k)x[c,c+k)) — não precisa de uma
    // função de valor genérica porque o teste exige conhecer a estrutura
    // das fronteiras, não apenas amostrar pontos.
    static std::unique_ptr<QuadNode> Build(int L, int k, int c, int x0, int y0, int size) {
        auto node = std::make_unique<QuadNode>();
        node->x0 = x0; node->y0 = y0; node->size = size;

        int value;
        if (RegionValueExact(L, k, c, x0, y0, size, value)) {
            node->is_leaf = true;
            node->value = static_cast<uint8_t>(value);
            return node;
        }

        node->is_leaf = false;
        const int half = size / 2;
        node->children[0] = Build(L, k, c, x0,        y0,        half);
        node->children[1] = Build(L, k, c, x0 + half, y0,        half);
        node->children[2] = Build(L, k, c, x0,        y0 + half, half);
        node->children[3] = Build(L, k, c, x0 + half, y0 + half, half);
        return node;
    }

private:
    // Retorna true (e preenche `value`) se a região é exatamente
    // homogênea; false se cruza alguma fronteira (precisa subdividir).
    static bool RegionValueExact(int L, int k, int c, int x0, int y0, int size, int& value) {
        const int x1 = x0 + size - 1, y1 = y0 + size - 1;

        // 1. teste da hipotenusa: região cruza x+y=L?
        const int min_sum = x0 + y0;
        const int max_sum = x1 + y1;
        if (min_sum < L && L <= max_sum) return false; // cruza: parte dentro/fora do triângulo
        if (min_sum >= L) { value = 0; return true; }   // toda fora do triângulo

        // toda dentro do triângulo: só falta checar o bloco
        if (k <= 0) { value = 1; return true; }

        const int bx0 = c, bx1 = c + k - 1, by0 = c, by1 = c + k - 1;
        const int ox0 = std::max(x0, bx0), ox1 = std::min(x1, bx1);
        const int oy0 = std::max(y0, by0), oy1 = std::min(y1, by1);
        const bool overlapX = ox0 <= ox1, overlapY = oy0 <= oy1;

        if (!(overlapX && overlapY)) { value = 1; return true; } // fora do bloco inteiramente

        const bool insideBlock = (x0 >= bx0 && x1 <= bx1 && y0 >= by0 && y1 <= by1);
        if (insideBlock) { value = 2; return true; }              // dentro do bloco inteiramente

        return false; // overlap parcial: cruza a borda do bloco
    }
};

// Quadtree EXATO sobre um bitmap arbitrário (dado real, sem fórmula
// fechada) — ex.: uma fatia z do Stanford Bunny voxelizado (Seção 20 /
// README v13). Usa teste any/all sobre o bitmap, não amostragem por
// cantos+centro: já provamos (README v9, py/voxeliza_bunny_real.py) que
// a amostragem colapsa para 1 nó (falso positivo total) em dado real
// esparso, porque os cantos+centro do cubo inteiro quase sempre caem em
// espaço vazio numa casca fina. Aqui isso não é opcional — é o único
// teste correto para dado sem estrutura analítica conhecida.
//
// CORREÇÃO (README v15): a versão anterior (`Build`, mantida abaixo só
// para o teste de regressão) escaneava a região célula a célula a cada
// nó — O(size²) por consulta, não O(1). Isso foi apontado como erro
// real: medir uniformemente (célula a célula) contra toda a região,
// independente da escala, é o oposto do princípio de tesselação em
// múltiplas resoluções (mesma ideia de um pedreiro usando régua grossa
// pra planejar a fiada e fina só pro encaixe do canto — não a régua
// fina contra a parede inteira).
//
// A correção é uma PIRÂMIDE BOOLEANA (mipmap: OR-pool e AND-pool 2×2 por
// nível, bottom-up a partir do bitmap bruto), pré-computada 1x por fatia
// em O(L²) — inevitável, é preciso tocar cada voxel ao menos uma vez —,
// depois disso cada nó do quadtree decide homogeneidade com 1 lookup
// direto no nível certo da pirâmide. Isso é MEDIDO abaixo via
// `QueryCount`, não assumido: ver tests/test_bitmap_pyramid.cpp.
class BitmapPyramid {
public:
    // Constrói a pirâmide a partir do bitmap bruto L×L (mesmo layout do
    // BitmapQuadtree::Build). Custo: O(L²) total somando todos os níveis
    // (série geométrica 1+1/4+1/16+... converge a 4/3 * L², não explode).
    void Build(const uint8_t* bitmap, int L) {
        L_ = L;
        or_levels_.clear();
        and_levels_.clear();

        int size = L;
        std::vector<uint8_t> or_level(static_cast<size_t>(L) * L);
        std::vector<uint8_t> and_level(static_cast<size_t>(L) * L);
        for (int y = 0; y < L; ++y)
            for (int x = 0; x < L; ++x) {
                bool bit = GetBit(bitmap, L, x, y);
                or_level[y * L + x] = bit;
                and_level[y * L + x] = bit;
            }
        or_levels_.push_back(std::move(or_level));
        and_levels_.push_back(std::move(and_level));

        while (size > 1) {
            const int novo_size = size / 2;
            std::vector<uint8_t> novo_or(static_cast<size_t>(novo_size) * novo_size);
            std::vector<uint8_t> novo_and(static_cast<size_t>(novo_size) * novo_size);
            const auto& prev_or = or_levels_.back();
            const auto& prev_and = and_levels_.back();
            for (int y = 0; y < novo_size; ++y) {
                for (int x = 0; x < novo_size; ++x) {
                    bool o = prev_or[(2*y) * size + 2*x] || prev_or[(2*y) * size + 2*x+1] ||
                             prev_or[(2*y+1) * size + 2*x] || prev_or[(2*y+1) * size + 2*x+1];
                    bool a = prev_and[(2*y) * size + 2*x] && prev_and[(2*y) * size + 2*x+1] &&
                             prev_and[(2*y+1) * size + 2*x] && prev_and[(2*y+1) * size + 2*x+1];
                    novo_or[y * novo_size + x] = o;
                    novo_and[y * novo_size + x] = a;
                }
            }
            or_levels_.push_back(std::move(novo_or));
            and_levels_.push_back(std::move(novo_and));
            size = novo_size;
        }
    }

    // 1 lookup direto — O(1) de verdade, não scan. `query_count` é
    // incrementado para permitir medir isso em teste (não confiar sem medir).
    void Query(int x0, int y0, int size, bool& any_set, bool& all_set,
               long long* query_count = nullptr) const {
        if (query_count) ++(*query_count);
        int level = 0;
        for (int s = size; s > 1; s >>= 1) ++level;
        const int level_size = L_ >> level;
        const int nx = x0 / size, ny = y0 / size;
        if (nx >= level_size || ny >= level_size || level >= static_cast<int>(or_levels_.size())) {
            any_set = false; all_set = false; return;
        }
        any_set = or_levels_[level][ny * level_size + nx] != 0;
        all_set = and_levels_[level][ny * level_size + nx] != 0;
    }

private:
    int L_ = 0;
    std::vector<std::vector<uint8_t>> or_levels_;
    std::vector<std::vector<uint8_t>> and_levels_;

    static bool GetBit(const uint8_t* bitmap, int L, int x, int y) {
        if (x < 0 || x >= L || y < 0 || y >= L) return false;
        const int idx = y * L + x;
        return (bitmap[idx / 8] & (1 << (idx % 8))) != 0;
    }
};

class BitmapQuadtree {
public:
    // Versão CORRIGIDA (O(1) por nó de verdade): usa a pirâmide
    // pré-computada. `query_count`, se passado, conta quantos lookups
    // foram feitos no total — para medir (não assumir) que não escala
    // com size.
    static std::unique_ptr<QuadNode> BuildFromPyramid(const BitmapPyramid& pyramid,
                                                        int x0, int y0, int size,
                                                        long long* query_count = nullptr) {
        auto node = std::make_unique<QuadNode>();
        node->x0 = x0; node->y0 = y0; node->size = size;

        bool any_set, all_set;
        pyramid.Query(x0, y0, size, any_set, all_set, query_count);

        if (!any_set) { node->is_leaf = true; node->value = 0; return node; }
        if (all_set)  { node->is_leaf = true; node->value = 1; return node; }

        node->is_leaf = false;
        const int half = size / 2;
        node->children[0] = BuildFromPyramid(pyramid, x0,        y0,        half, query_count);
        node->children[1] = BuildFromPyramid(pyramid, x0 + half, y0,        half, query_count);
        node->children[2] = BuildFromPyramid(pyramid, x0,        y0 + half, half, query_count);
        node->children[3] = BuildFromPyramid(pyramid, x0 + half, y0 + half, half, query_count);
        return node;
    }

    // Versão ANTIGA (O(size²) por nó, scan célula a célula) — mantida
    // só para o teste de regressão comparar contra a nova e confirmar
    // paridade exata de resultado (README v15).
    static std::unique_ptr<QuadNode> Build(const uint8_t* bitmap, int L,
                                            int x0, int y0, int size,
                                            long long* bit_reads = nullptr) {
        auto node = std::make_unique<QuadNode>();
        node->x0 = x0; node->y0 = y0; node->size = size;

        bool any_set = false, all_set = true;
        for (int y = y0; y < y0 + size; ++y) {
            for (int x = x0; x < x0 + size; ++x) {
                if (bit_reads) ++(*bit_reads);
                const bool bit = GetBit(bitmap, L, x, y);
                any_set |= bit;
                all_set &= bit;
            }
        }

        if (!any_set) { node->is_leaf = true; node->value = 0; return node; }
        if (all_set)  { node->is_leaf = true; node->value = 1; return node; }

        node->is_leaf = false;
        const int half = size / 2;
        node->children[0] = Build(bitmap, L, x0,        y0,        half, bit_reads);
        node->children[1] = Build(bitmap, L, x0 + half, y0,        half, bit_reads);
        node->children[2] = Build(bitmap, L, x0,        y0 + half, half, bit_reads);
        node->children[3] = Build(bitmap, L, x0 + half, y0 + half, half, bit_reads);
        return node;
    }

    // Consulta de UM ÚNICO nó (sem recursão), pra medir o custo isolado
    // de uma consulta -- não o custo acumulado da árvore inteira.
    static void QuerySingleNodeScan(const uint8_t* bitmap, int L, int x0, int y0, int size,
                                     bool& any_set, bool& all_set, long long* bit_reads = nullptr) {
        any_set = false; all_set = true;
        for (int y = y0; y < y0 + size; ++y) {
            for (int x = x0; x < x0 + size; ++x) {
                if (bit_reads) ++(*bit_reads);
                const bool bit = GetBit(bitmap, L, x, y);
                any_set |= bit;
                all_set &= bit;
            }
        }
    }

private:
    static bool GetBit(const uint8_t* bitmap, int L, int x, int y) {
        if (x < 0 || x >= L || y < 0 || y >= L) return false; // fora da fatia: vazio
        const int idx = y * L + x;
        return (bitmap[idx / 8] & (1 << (idx % 8))) != 0;
    }
};

// Quadtree ESPARSO de verdade (README v16): ao contrário de
// BitmapPyramid, NUNCA toca a grade densa L×L — parte só da lista de
// pontos ocupados (conhecida, pequena), filtrando por quadrante a cada
// nível. O(1) por nó do BitmapPyramid tinha um custo escondido: montar
// a pirâmide exigia tocar TODAS as O(L²) células, mesmo as vazias — para
// o bunny (L=32, 9,5% ocupado), isso mede 14x mais células tocadas do
// que existe de dado real (medido em py/verifica_construcao_esparsa.py).
//
// Aqui o trabalho é proporcional ao número de pontos ocupados
// relevantes em cada região, não ao tamanho da região — análogo ao
// `SpatialPartitionExact` da malha sintética (que nunca varre células,
// só calcula com os parâmetros fechados L,k,c), mas aqui sem fórmula
// fechada: a "geometria de referência" é a própria lista ordenada de
// pontos, não uma equação.
class SpatialPartitionSparse {
public:
    struct Point { uint16_t x, y; };

    // points: ordenados por (x,y), formato do arquivo WFLS
    // (py/gera_world_sparse_stream.py). work_counter, se passado, soma
    // o número de pontos examinados em cada nível da recursão — medida
    // real de trabalho, não suposição.
    //
    // CORREÇÃO (README v16, medição pegou o erro): a primeira versão
    // filtrava a lista completa 4 vezes por nível (uma por filho, cada
    // uma re-percorrendo tudo) — medido em 60.440 pontos examinados
    // (todas as 32 fatias) contra 43.680 células tocadas pela pirâmide
    // densa, ou seja, PIOR que a abordagem que devia substituir. A causa
    // era re-filtragem redundante, não a ideia de usar dado esparso em
    // si. Corrigido: 1 única passada por nó, particionando os pontos nos
    // 4 baldes de uma vez (não 4 filtragens independentes).
    static std::unique_ptr<QuadNode> Build(const std::vector<Point>& points,
                                            int x0, int y0, int size,
                                            long long* work_counter = nullptr) {
        if (work_counter) *work_counter += static_cast<long long>(points.size());

        auto node = std::make_unique<QuadNode>();
        node->x0 = x0; node->y0 = y0; node->size = size;

        if (points.empty()) {
            node->is_leaf = true; node->value = 0; return node; // vazio, sem tocar células
        }
        if (static_cast<long long>(points.size()) == static_cast<long long>(size) * size) {
            node->is_leaf = true; node->value = 1; return node; // totalmente cheio
        }
        if (size == 1) {
            node->is_leaf = true; node->value = 1; return node;
        }

        node->is_leaf = false;
        const int half = size / 2;
        const int mx = x0 + half, my = y0 + half;

        // 1 única passada: particiona em 4 baldes de uma vez (não 4
        // filtragens independentes da lista inteira)
        std::vector<Point> nw, ne, sw, se;
        for (const auto& p : points) {
            const bool east = p.x >= mx;
            const bool south = p.y >= my;
            if (!east && !south) nw.push_back(p);
            else if (east && !south) ne.push_back(p);
            else if (!east && south) sw.push_back(p);
            else se.push_back(p);
        }

        node->children[0] = Build(nw, x0,   y0,   half, work_counter);
        node->children[1] = Build(ne, mx,   y0,   half, work_counter);
        node->children[2] = Build(sw, x0,   my,   half, work_counter);
        node->children[3] = Build(se, mx,   my,   half, work_counter);
        return node;
    }

    // Versão IN-PLACE (README v25): mesma contagem de toques por ponto
    // que Build() acima (não reduz o fator ~5,6-6,6x, isso é inerente à
    // profundidade da árvore — ver nota no README), mas evita alocar 4
    // std::vector NOVOS a cada nó. Em vez disso, particiona os índices
    // de um único buffer compartilhado (std::partition, 3 passadas por
    // nó em vez de 1 passada + 4 allocs) — troca "menos toques lógicos"
    // por "menos alocação de heap", um eixo de custo diferente (tempo
    // de parede real, não contagem de toques).
    static std::unique_ptr<QuadNode> BuildInPlace(std::vector<Point>& points,
                                                   size_t lo, size_t hi,
                                                   int x0, int y0, int size,
                                                   long long* work_counter = nullptr) {
        if (work_counter) *work_counter += static_cast<long long>(hi - lo);

        auto node = std::make_unique<QuadNode>();
        node->x0 = x0; node->y0 = y0; node->size = size;

        const size_t count = hi - lo;
        if (count == 0) { node->is_leaf = true; node->value = 0; return node; }
        if (count == static_cast<size_t>(size) * size) {
            node->is_leaf = true; node->value = 1; return node;
        }
        if (size == 1) { node->is_leaf = true; node->value = 1; return node; }

        node->is_leaf = false;
        const int half = size / 2;
        const int mx = x0 + half, my = y0 + half;

        // std::partition reordena IN-PLACE (sem alocar), 3 vezes: separa
        // west/east, depois norte/sul dentro de cada metade. O mesmo
        // buffer `points` é reusado por todas as chamadas recursivas via
        // referência + índices [lo,hi) — não há cópia nem alocação nova.
        auto east_begin = std::partition(points.begin() + lo, points.begin() + hi,
            [mx](const Point& p) { return p.x < mx; });
        size_t mid = east_begin - points.begin();

        auto nw_end = std::partition(points.begin() + lo, points.begin() + mid,
            [my](const Point& p) { return p.y < my; });
        size_t p1 = nw_end - points.begin();

        auto sw_end = std::partition(points.begin() + mid, points.begin() + hi,
            [my](const Point& p) { return p.y < my; });
        size_t p2 = sw_end - points.begin();

        node->children[0] = BuildInPlace(points, lo,  p1,  x0,        y0,        half, work_counter);
        node->children[1] = BuildInPlace(points, mid, p2,  mx,        y0,        half, work_counter);
        node->children[2] = BuildInPlace(points, p1,  mid, x0,        my,        half, work_counter);
        node->children[3] = BuildInPlace(points, p2,  hi,  mx,        my,        half, work_counter);
        return node;
    }
};

// Quadtree esparso via ORDEM MORTON (Z-order), README v18: reduz o
// custo de cada divisão de nó de O(N) (particionar a lista inteira) para
// O(log N) (busca binária), aproveitando a propriedade de que os pontos
// dentro de uma região alinhada a potência de 2, ordenados por código
// Morton, formam um intervalo CONTÍGUO — e os 4 quadrantes (NW/NE/SW/SE)
// são 4 sub-intervalos contíguos exatos dentro dele (verificado em
// py/verifica_morton.py, 0 falhas em 200 casos aleatórios).
class SpatialPartitionMorton {
public:
    struct Point { uint16_t x, y; };

    // Intercala os bits de x e y (Morton/Z-order encode), 16 bits por eixo.
    static uint32_t Encode(uint32_t x, uint32_t y) {
        auto parte1 = [](uint32_t n) -> uint32_t {
            n &= 0x0000FFFFu;
            n = (n | (n << 8)) & 0x00FF00FFu;
            n = (n | (n << 4)) & 0x0F0F0F0Fu;
            n = (n | (n << 2)) & 0x33333333u;
            n = (n | (n << 1)) & 0x55555555u;
            return n;
        };
        return parte1(x) | (parte1(y) << 1);
    }

    // points: já ORDENADOS por código Morton (não por x,y simples).
    // work_counter conta comparações de busca binária feitas, não
    // pontos "tocados" um a um — medida real do custo, comparável ao
    // contador de SpatialPartitionSparse.
    static std::unique_ptr<QuadNode> Build(const std::vector<Point>& sorted_points,
                                            size_t lo, size_t hi,
                                            int x0, int y0, int size,
                                            long long* work_counter = nullptr) {
        auto node = std::make_unique<QuadNode>();
        node->x0 = x0; node->y0 = y0; node->size = size;

        const size_t count = hi - lo;
        if (count == 0) { node->is_leaf = true; node->value = 0; return node; }
        if (count == static_cast<size_t>(size) * size) {
            node->is_leaf = true; node->value = 1; return node;
        }
        if (size == 1) { node->is_leaf = true; node->value = 1; return node; }

        node->is_leaf = false;
        const int half = size / 2;
        const uint32_t base = Encode(x0, y0);
        const size_t area_filho = static_cast<size_t>(half) * half;

        // 3 buscas binárias (não 4: os extremos lo/hi já são conhecidos)
        // para achar as 3 fronteiras internas dos 4 blocos contíguos.
        auto acha_fronteira = [&](uint32_t alvo) {
            size_t a = lo, b = hi;
            while (a < b) {
                if (work_counter) *work_counter += 1; // 1 comparação elementar
                size_t m = a + (b - a) / 2;
                if (Encode(sorted_points[m].x, sorted_points[m].y) < alvo) a = m + 1;
                else b = m;
            }
            return a;
        };

        const size_t p1 = acha_fronteira(base + 1 * area_filho);
        const size_t p2 = acha_fronteira(base + 2 * area_filho);
        const size_t p3 = acha_fronteira(base + 3 * area_filho);

        node->children[0] = Build(sorted_points, lo, p1, x0,        y0,        half, work_counter);
        node->children[1] = Build(sorted_points, p1, p2, x0 + half, y0,        half, work_counter);
        node->children[2] = Build(sorted_points, p2, p3, x0,        y0 + half, half, work_counter);
        node->children[3] = Build(sorted_points, p3, hi, x0 + half, y0 + half, half, work_counter);
        return node;
    }
};

} // namespace stream
