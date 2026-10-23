#pragma once
#include <cstddef>

namespace core {

// Saída da aresta Core -> HardwareProfiler (init 1x)
struct WindowLimits {
    std::size_t max_window_slices; // m: teto fixo do Ring Buffer
    std::size_t vram_bytes;
    double pcie_bandwidth_gbps;
};

class HardwareProfiler {
public:
    // Mede VRAM, throughput do barramento e vértices/s da GPU.
    // Chamado uma única vez, antes do Render Loop começar.
    WindowLimits Profile();
};

} // namespace core
