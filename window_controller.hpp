#pragma once
#include "../core/vec3.hpp"
#include "slice_streamer.hpp"
#include <vector>
#include <cstddef>

namespace stream {

class WindowController {
public:
    explicit WindowController(std::size_t m);

    // Aresta Core -> WindowController (por frame):
    // libera a fatia mais antiga, insere a nova (scroll do ring buffer).
    void Slide(const Vec3& cameraPos, const Slice& newSlice);

    // Janela ativa atual, consumida por Render::BoundingReference
    const std::vector<Slice>& ActiveWindow() const { return ring_; }

private:
    std::size_t m_;      // teto fixo vindo do HardwareProfiler
    std::size_t head_ = 0;
    std::vector<Slice> ring_;
};

} // namespace stream
