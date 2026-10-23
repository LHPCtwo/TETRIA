#pragma once
#include <cstdint>
#include <vector>
#include <algorithm>

namespace stream {

// Codec vértice+delta (README v27): modela pontos esparsos por um
// ÍNDICE ÚNICO de vértice (val = y*W + x, análogo à Seção 9 do
// documento "Números Primos como Interseção de Triângulos" —
// val = y*W + x + 1 na malha quadrada de vértices), em vez de pares
// (x,y) de 16 bits cada. Como a lista de pontos ocupados já precisa
// ser processada de alguma forma, ordenar por esse índice único permite
// codificar apenas os DELTAS entre valores consecutivos via varint
// (LEB128) — técnica clássica de posting-list/bitmap esparso.
//
// MEDIDO, não suposto (py/verifica_esparsidade_por_vertice.py, dado
// real do Stanford Bunny, L=32): 3.492 bytes contra 12.844 do formato
// WFLS anterior (pares x,y fixos) e contra 4.108 do bitmap denso WFLB
// — a primeira vez que o formato esparso vence o denso em ESPAÇO em
// disco também, não só em trabalho de construção da árvore (que já
// vencia desde a v16/v25).
class VertexDeltaCodec {
public:
    struct Point { uint16_t x, y; };

    // Codifica uma lista de pontos (x,y) em bytes vértice+delta.
    // W: largura da grade (mesmo L da fatia).
    static std::vector<uint8_t> Encode(std::vector<Point> points, int W) {
        std::sort(points.begin(), points.end(), [W](const Point& a, const Point& b) {
            return (static_cast<uint32_t>(a.y) * W + a.x) <
                   (static_cast<uint32_t>(b.y) * W + b.x);
        });

        std::vector<uint8_t> out;
        uint32_t anterior = 0;
        for (const auto& p : points) {
            const uint32_t val = static_cast<uint32_t>(p.y) * W + p.x + 1;
            const uint32_t delta = val - anterior;
            anterior = val;
            EncodeVarint(delta, out);
        }
        return out;
    }

    // Decodifica de volta para a lista de pontos (ordem: crescente por
    // índice de vértice, ou seja, y-major — não necessariamente a mesma
    // ordem de entrada, mas o mesmo CONJUNTO de pontos; validado por
    // comparação de conjunto, não de ordem, em
    // py/verifica_esparsidade_por_vertice.py).
    static std::vector<Point> Decode(const std::vector<uint8_t>& bytes, int W) {
        std::vector<Point> points;
        size_t pos = 0;
        uint32_t anterior = 0;
        while (pos < bytes.size()) {
            const uint32_t delta = DecodeVarint(bytes, pos);
            const uint32_t val = anterior + delta;
            anterior = val;
            const uint32_t v0 = val - 1;
            const uint16_t y = static_cast<uint16_t>(v0 / W);
            const uint16_t x = static_cast<uint16_t>(v0 % W);
            points.push_back({x, y});
        }
        return points;
    }

private:
    static void EncodeVarint(uint32_t n, std::vector<uint8_t>& out) {
        while (true) {
            uint8_t byte = n & 0x7f;
            n >>= 7;
            if (n) {
                out.push_back(byte | 0x80);
            } else {
                out.push_back(byte);
                break;
            }
        }
    }

    static uint32_t DecodeVarint(const std::vector<uint8_t>& bytes, size_t& pos) {
        uint32_t result = 0;
        int shift = 0;
        while (true) {
            const uint8_t byte = bytes[pos++];
            result |= static_cast<uint32_t>(byte & 0x7f) << shift;
            if (!(byte & 0x80)) break;
            shift += 7;
        }
        return result;
    }
};

} // namespace stream
