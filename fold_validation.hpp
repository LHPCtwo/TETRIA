#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <numeric>

namespace fold {

// Validação de dobra plana (Relatório, Seção 22.1). Kawasaki + Maekawa são
// NECESSÁRIAS mas não SUFICIENTES para garantir dobra plana sem
// auto-interseção do papel — usar como pré-condição antes de gerar
// qualquer geometria de dobra procedural, não como garantia completa.
class FoldValidation {
public:
    // angulos_graus: angulos ao redor de um vertice, em ordem, em graus.
    static bool TestaKawasaki(const std::vector<double>& angulos_graus) {
        const size_t n = angulos_graus.size();
        if (n % 2 != 0) return false;
        const double soma_total = std::accumulate(angulos_graus.begin(), angulos_graus.end(), 0.0);
        if (std::abs(soma_total - 360.0) > 1e-6) return false;

        double soma_impares = 0.0, soma_pares = 0.0;
        for (size_t i = 0; i < n; ++i) {
            if (i % 2 == 0) soma_impares += angulos_graus[i];
            else soma_pares += angulos_graus[i];
        }
        return std::abs(soma_impares - 180.0) < 1e-6 && std::abs(soma_pares - 180.0) < 1e-6;
    }

    // padrao_mv: string de 'M' (montanha) / 'V' (vale), uma por vinca.
    static bool TestaMaekawa(const std::string& padrao_mv) {
        int M = 0, V = 0;
        for (char c : padrao_mv) {
            if (c == 'M') ++M;
            else if (c == 'V') ++V;
        }
        return std::abs(M - V) == 2;
    }

    static bool VerticeEDobravel(const std::vector<double>& angulos_graus,
                                  const std::string& padrao_mv) {
        return TestaKawasaki(angulos_graus) && TestaMaekawa(padrao_mv);
    }
};

} // namespace fold
