#pragma once
#include <cstdint>

namespace core {

// Seção 7 do relatório: duas técnicas de truncamento sem divisão inteira,
// que respondem perguntas DIFERENTES — nunca usar uma no lugar da outra.

// Truncamento por MÁSCARA (resíduo): i & Rk, com Rk = 2^k - 1.
// Calcula i mod 2^k. NÃO preserva magnitude — usar só quando o objetivo
// é reduzir custo de operação (ex.: índice circular em ring buffer),
// nunca para decidir se dois valores são "parecidos".
inline uint32_t TruncaPorMascara(uint32_t i, int k) {
    const uint32_t repunit = (1u << k) - 1u;
    return i & repunit;
}

// Truncamento por DESLOCAMENTO (shift): i >> k == floor(i / 2^k).
// Preserva magnitude — usar quando o objetivo é dimensionar/classificar
// (ex.: fundir blocos vizinhos com valor "parecido" no union-find).
inline uint32_t TruncaPorShift(uint32_t i, int k) {
    return i >> k;
}

// Verificado (Seção 7, testa_mascara.py): para k=4, os testes
// (i & Rk) <= 1  e  (i >> k) == 0  discordam em 128 dos 256 casos de
// i em [0,256) — não são intercambiáveis.

} // namespace core
