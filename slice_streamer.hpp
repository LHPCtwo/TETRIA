#pragma once
#include "../core/vec3.hpp"
#include "spatial_partition.hpp"
#include "vertex_delta_codec.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>

namespace stream {

// Uma fatia classificada (Fase 1: Shift Union-Find O(1); Fase 2: Quadtree/Octree O(L))
struct Slice {
    std::vector<uint8_t> compressed_data; // pico de memória = 1 fatia
    Vec3 origin;
    float width  = 0.0f; // extensão em x da bounding box da fatia
    float height = 0.0f; // extensão em y da bounding box da fatia
};

enum class WorldStreamFormat {
    kNone,
    kParametric, // "WFLX": (k,c) por fatia, malha algébrica (Seção 5)
    kBitmap,     // "WFLB": bitmap real por fatia, dado sem fórmula fechada (ex.: bunny)
    kSparse,     // "WFLS": lista de coordenadas ocupadas, nunca materializa grade densa
    kVertexDelta, // "WFLV": índice de vértice + delta varint (README v27) — vence WFLB em espaço também
};

// Cabeçalho comum a todos os formatos (ver py/gera_world_stream.py,
// py/gera_world_bunny_stream.py, py/gera_world_sparse_stream.py):
//   MAGIC (4 bytes) | L (int32) | num_slices (int32)
//   WFLX: por fatia, k (int32) | c (int32)              -- 8 bytes/fatia
//   WFLB: por fatia, bitmap L*L bits (ceil(L*L/8) B)     -- dado real
//   WFLS: índice de offsets (int64 x num_slices) + por fatia: count (int32) + count x (uint16,uint16)
//   WFLV: índice de offsets (int64 x num_slices) + por fatia: count (int32) + byte_length (int32) + bytes vértice+delta (varint)
struct WorldStreamHeader {
    int32_t L = 0;
    int32_t num_slices = 0;
    std::streamoff data_start = 0;
    WorldStreamFormat format = WorldStreamFormat::kNone;
};

class SliceStreamer {
public:
    explicit SliceStreamer(std::string path);

    // Aresta Core -> Disco -> SliceStreamer (por frame):
    // lê do disco (via seek) a fatia sob a posição atual e classifica.
    Slice PullSlice(const Vec3& cameraPos);

    bool IsOpen() const { return file_.is_open(); }
    const WorldStreamHeader& Header() const { return header_; }

private:
    std::string path_;
    std::ifstream file_;
    WorldStreamHeader header_;
    std::vector<uint8_t> bitmap_buffer_; // reutilizado a cada leitura (WFLB)
    std::vector<int64_t> sparse_index_;  // offsets por fatia (WFLS, WFLV)

    bool ReadHeader();
    bool ReadSliceParams(int z, int32_t& k, int32_t& c);       // WFLX
    bool ReadSliceBitmap(int z, std::vector<uint8_t>& out);    // WFLB
    bool ReadSliceSparse(int z, std::vector<SpatialPartitionSparse::Point>& out); // WFLS
    bool ReadSliceVertexDelta(int z, std::vector<VertexDeltaCodec::Point>& out);  // WFLV
};

} // namespace stream
