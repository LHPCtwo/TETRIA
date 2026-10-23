#pragma once
#include <vector>
#include <cstdint>

namespace symmetry {

// Colapso de Horner por paridade (Relatório, Seção 22.6). Se as raízes de
// um polinômio vêm em k pares simétricos em torno de um centro c, o
// polinômio depende só de v=(x-c)^2 — corta a cadeia de Horner de 2k
// passos para k, e o ganho se propaga integralmente para a truncagem
// modular (mesma cadeia reduzida, mod m).
class HornerParity {
public:
    // Retorna os coeficientes (grau decrescente) de Q(v) = prod_i (v - d_i^2).
    static std::vector<double> CoeficientesQ(const std::vector<double>& deslocamentos) {
        std::vector<double> coefs = {1.0};
        for (double d : deslocamentos) {
            const double d2 = d * d;
            // multiplica coefs (em v) por (v - d2): convolucao com [1, -d2]
            std::vector<double> novo(coefs.size() + 1, 0.0);
            for (size_t i = 0; i < coefs.size(); ++i) {
                novo[i]     += coefs[i] * 1.0;
                novo[i + 1] += coefs[i] * (-d2);
            }
            coefs = novo;
        }
        return coefs;
    }

    static double Horner(const std::vector<double>& coefs, double valor) {
        double resultado = 0.0;
        for (double c : coefs) resultado = resultado * valor + c;
        return resultado;
    }

    // Avalia P(x) explorando a simetria: v=(x-c)^2, Horner em Q(v) — k
    // passos em vez de 2k. Retorna o valor; opcionalmente devolve os
    // coeficientes de Q via out_coefs (para reuso na truncagem modular).
    static double AvaliaPorParidade(double x, double c,
                                     const std::vector<double>& deslocamentos,
                                     std::vector<double>* out_coefs = nullptr) {
        const double u = x - c;
        const double v = u * u;
        auto coefs = CoeficientesQ(deslocamentos);
        if (out_coefs) *out_coefs = coefs;
        return Horner(coefs, v);
    }

    // Mesmo ganho de custo aplicado a truncagem modular: reduz v mod m
    // antes de rodar Horner mod m (k reducoes em vez de 2k).
    static int64_t TruncagemModularPorParidade(double x, double c,
                                                const std::vector<double>& deslocamentos,
                                                int64_t m) {
        const double u = x - c;
        int64_t v = static_cast<int64_t>(u * u) % m;
        if (v < 0) v += m;
        auto coefs = CoeficientesQ(deslocamentos);
        int64_t resultado = 0;
        for (double coef : coefs) {
            int64_t c_int = static_cast<int64_t>(coef);
            resultado = (resultado * v + c_int) % m;
            if (resultado < 0) resultado += m;
        }
        return resultado;
    }

    // Caso de raiz solta (numero impar de raizes): P(x) = u * Q(u^2) —
    // ganho parcial, nao pleno (Relatório, Seção 22.6, ressalva).
    static double RaizSoltaGanhoParcial(double x, double c,
                                         const std::vector<double>& deslocamentos) {
        const double u = x - c;
        return u * AvaliaPorParidade(x, c, deslocamentos);
    }
};

} // namespace symmetry
