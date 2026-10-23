#include "slice_streamer.hpp"
#include "spatial_partition.hpp"
#include <cstring>
#include <cmath>
#include <algorithm>

namespace stream {

namespace {
constexpr char kMagicParametric[4] = {'W', 'F', 'L', 'X'};
constexpr char kMagicBitmap[4]     = {'W', 'F', 'L', 'B'};
constexpr char kMagicSparse[4]     = {'W', 'F', 'L', 'S'};
constexpr char kMagicVertexDelta[4] = {'W', 'F', 'L', 'V'};
}

SliceStreamer::SliceStreamer(std::string path) : path_(std::move(path)) {
    file_.open(path_, std::ios::binary);
    if (file_.is_open()) {
        if (!ReadHeader()) {
            file_.close();
        }
    }
}

bool SliceStreamer::ReadHeader() {
    char magic[4];
    file_.read(magic, 4);
    if (!file_) return false;

    WorldStreamFormat fmt = WorldStreamFormat::kNone;
    if (std::memcmp(magic, kMagicParametric, 4) == 0) fmt = WorldStreamFormat::kParametric;
    else if (std::memcmp(magic, kMagicBitmap, 4) == 0) fmt = WorldStreamFormat::kBitmap;
    else if (std::memcmp(magic, kMagicSparse, 4) == 0) fmt = WorldStreamFormat::kSparse;
    else if (std::memcmp(magic, kMagicVertexDelta, 4) == 0) fmt = WorldStreamFormat::kVertexDelta; // WFLV (README v27)
    else return false;

    int32_t L = 0, num_slices = 0;
    file_.read(reinterpret_cast<char*>(&L), sizeof(L));
    file_.read(reinterpret_cast<char*>(&num_slices), sizeof(num_slices));
    if (!file_ || L <= 0 || num_slices <= 0) return false;

    header_.L = L;
    header_.num_slices = num_slices;
    header_.format = fmt;
    header_.data_start = file_.tellg();

    if (fmt == WorldStreamFormat::kBitmap) {
        const size_t bytes_per_slice = (static_cast<size_t>(L) * L + 7) / 8;
        bitmap_buffer_.resize(bytes_per_slice);
    } else if (fmt == WorldStreamFormat::kSparse || fmt == WorldStreamFormat::kVertexDelta) {
        // índice: num_slices offsets int64, logo após o header (WFLS e WFLV)
        sparse_index_.resize(num_slices);
        file_.read(reinterpret_cast<char*>(sparse_index_.data()), sizeof(int64_t) * num_slices);
        if (!file_) return false;
    }
    return true;
}

bool SliceStreamer::ReadSliceParams(int z, int32_t& k, int32_t& c) {
    if (z < 0 || z >= header_.num_slices) return false;
    const std::streamoff offset = header_.data_start + static_cast<std::streamoff>(z) * 8;
    file_.seekg(offset);
    if (!file_) return false;
    file_.read(reinterpret_cast<char*>(&k), sizeof(k));
    file_.read(reinterpret_cast<char*>(&c), sizeof(c));
    return static_cast<bool>(file_);
}

bool SliceStreamer::ReadSliceBitmap(int z, std::vector<uint8_t>& out) {
    if (z < 0 || z >= header_.num_slices) return false;
    const size_t bytes_per_slice = bitmap_buffer_.size();
    const std::streamoff offset = header_.data_start
        + static_cast<std::streamoff>(z) * static_cast<std::streamoff>(bytes_per_slice);
    file_.seekg(offset);
    if (!file_) return false;
    out.resize(bytes_per_slice);
    file_.read(reinterpret_cast<char*>(out.data()), bytes_per_slice);
    return static_cast<bool>(file_);
}

bool SliceStreamer::ReadSliceSparse(int z, std::vector<SpatialPartitionSparse::Point>& out) {
    if (z < 0 || z >= static_cast<int>(sparse_index_.size())) return false;
    file_.seekg(static_cast<std::streamoff>(sparse_index_[z]));
    if (!file_) return false;
    int32_t count = 0;
    file_.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!file_ || count < 0) return false;
    out.resize(count);
    for (int i = 0; i < count; ++i) {
        uint16_t x, y;
        file_.read(reinterpret_cast<char*>(&x), sizeof(x));
        file_.read(reinterpret_cast<char*>(&y), sizeof(y));
        out[i] = {x, y};
    }
    return static_cast<bool>(file_) || count == 0;
}

// WFLV: por fatia, count (int32) | byte_length (int32) | bytes vértice+delta (varint).
// A decodificação devolve o mesmo CONJUNTO de pontos do WFLS, em ordem y-major
// (README v27) — BuildInPlace não depende da ordem de entrada.
bool SliceStreamer::ReadSliceVertexDelta(int z, std::vector<VertexDeltaCodec::Point>& out) {
    out.clear();
    if (z < 0 || z >= static_cast<int>(sparse_index_.size())) return false;
    file_.clear();
    file_.seekg(static_cast<std::streamoff>(sparse_index_[z]));
    if (!file_) return false;
    int32_t count = 0, byte_length = 0;
    file_.read(reinterpret_cast<char*>(&count), sizeof(count));
    file_.read(reinterpret_cast<char*>(&byte_length), sizeof(byte_length));
    if (!file_ || count < 0 || byte_length < 0) return false;
    std::vector<uint8_t> bytes(static_cast<size_t>(byte_length));
    if (byte_length > 0) {
        file_.read(reinterpret_cast<char*>(bytes.data()), byte_length);
        if (!file_) return false;
    }
    out = VertexDeltaCodec::Decode(bytes, header_.L);
    // confere a contagem declarada contra o que foi decodificado
    return static_cast<int32_t>(out.size()) == count;
}

Slice SliceStreamer::PullSlice(const Vec3& cameraPos) {
    Slice slice;
    slice.origin = cameraPos;

    if (!IsOpen()) {
        // Fallback: nenhum world.stream válido no path_ fornecido.
        int32_t L = 256, k = 85, c = (L - k) / 2;
        int size = 1; while (size < L) size *= 2;
        auto root = SpatialPartitionExact::Build(L, k, c, 0, 0, size);
        slice.width = slice.height = static_cast<float>(L);
        slice.compressed_data.resize(sizeof(int));
        int n = root->NodeCount();
        std::memcpy(slice.compressed_data.data(), &n, sizeof(int));
        return slice;
    }

    const int L = header_.L;
    int z = static_cast<int>(std::floor(cameraPos.z));
    z = std::clamp(z, 0, header_.num_slices - 1);

    int size = 1;
    while (size < L) size *= 2;

    int nodeCount = 0;

    if (header_.format == WorldStreamFormat::kParametric) {
        int32_t k = 0, c = 0;
        if (!ReadSliceParams(z, k, c)) { k = 0; c = 0; }
        auto root = SpatialPartitionExact::Build(L, k, c, 0, 0, size);
        nodeCount = root->NodeCount();
        slice.width  = static_cast<float>(k > 0 ? k : 1);
        slice.height = static_cast<float>(k > 0 ? k : 1);
    } else if (header_.format == WorldStreamFormat::kSparse) {
        std::vector<SpatialPartitionSparse::Point> points;
        ReadSliceSparse(z, points); // vazio em caso de falha: fatia vazia
        auto root = SpatialPartitionSparse::BuildInPlace(points, 0, points.size(), 0, 0, size);
        nodeCount = root->NodeCount();
    } else if (header_.format == WorldStreamFormat::kVertexDelta) {
        // WFLV: mesmo caminho do WFLS depois de decodificar (BuildInPlace, README v25).
        std::vector<VertexDeltaCodec::Point> decoded;
        ReadSliceVertexDelta(z, decoded); // vazio em caso de falha: fatia vazia
        std::vector<SpatialPartitionSparse::Point> points(decoded.size());
        for (size_t i = 0; i < decoded.size(); ++i) points[i] = {decoded[i].x, decoded[i].y};
        auto root = SpatialPartitionSparse::BuildInPlace(points, 0, points.size(), 0, 0, size);
        nodeCount = root->NodeCount();
    } else {
        if (!ReadSliceBitmap(z, bitmap_buffer_)) {
            std::fill(bitmap_buffer_.begin(), bitmap_buffer_.end(), 0);
        }
        BitmapPyramid pyramid;
        pyramid.Build(bitmap_buffer_.data(), L);
        auto root = BitmapQuadtree::BuildFromPyramid(pyramid, 0, 0, size);
        nodeCount = root->NodeCount();
    }

    if (header_.format != WorldStreamFormat::kParametric) {
        slice.width  = static_cast<float>(L);
        slice.height = static_cast<float>(L);
    }
    slice.compressed_data.resize(sizeof(int));
    std::memcpy(slice.compressed_data.data(), &nodeCount, sizeof(int));
    return slice;
}

} // namespace stream
