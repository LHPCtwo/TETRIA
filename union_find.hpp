#pragma once
#include "../core/fast_math.hpp"
#include <vector>
#include <cstdint>

namespace stream {

// Union-Find clássico (path compression), réplica direta de merge_grafo.py.
class UnionFind {
public:
    explicit UnionFind(int n) : parent_(n) {
        for (int i = 0; i < n; ++i) parent_[i] = i;
    }

    int Find(int x) {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];
            x = parent_[x];
        }
        return x;
    }

    void Union(int a, int b) {
        int ra = Find(a), rb = Find(b);
        if (ra != rb) parent_[ra] = rb;
    }

private:
    std::vector<int> parent_;
};

// Truncamento por deslocamento (shift): preserva magnitude, técnica correta
// para dimensionar/fundir (Seção 7 do relatório) — diferente de máscara/resíduo.
// Delega a core::FastMath (fonte única da verdade para as duas técnicas).
inline uint8_t TruncaPorShift(uint8_t valor, int k) {
    return static_cast<uint8_t>(core::TruncaPorShift(valor, k));
}

} // namespace stream
