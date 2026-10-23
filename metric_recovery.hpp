#pragma once
#include <cmath>

namespace render {

// Implementa a Seção 15 do relatório (Recuperação Geométrico-Métrica: Método
// da Partição e Escala): dado a área A de um retângulo e sua diagonal D,
// recupera os lados (a, b) sem passar pela equação quadrática explícita
// x^2 - Sx + A = 0.
//
// Premissas: a*b = A, a^2 + b^2 = D^2, com a >= b > 0.
//
// Passo 1 - altura relativa à diagonal:   hD = A / D
// Passo 2 - quadrado composto:            Q  = D^2 + 2A            (= (a+b)^2)
// Passo 3 - semiperímetro por partição:   S  = sqrt(Q)             (raiz de quadrado perfeito quando exato)
// Passo 4 - perímetro:                    P  = 2S
// Extra   - diferença via D^2 - 2A = (a-b)^2, permite separar a e b individualmente.
struct MetricRecovery {
    double height_to_diagonal; // hD
    double composed_square;    // Q = (a+b)^2
    double semi_perimeter;     // S = a+b
    double perimeter;          // P = 2S
    double a;                  // lado maior
    double b;                  // lado menor
};

inline MetricRecovery RecoverFromAreaAndDiagonal(double A, double D) {
    MetricRecovery r{};
    r.height_to_diagonal = A / D;                 // Passo 1
    r.composed_square     = D * D + 2.0 * A;       // Passo 2: (a+b)^2
    r.semi_perimeter       = std::sqrt(r.composed_square); // Passo 3
    r.perimeter             = 2.0 * r.semi_perimeter;        // Passo 4

    const double diff_sq = D * D - 2.0 * A;        // (a-b)^2
    const double diff    = diff_sq > 0.0 ? std::sqrt(diff_sq) : 0.0;
    r.a = (r.semi_perimeter + diff) / 2.0;
    r.b = (r.semi_perimeter - diff) / 2.0;
    return r;
}

} // namespace render
