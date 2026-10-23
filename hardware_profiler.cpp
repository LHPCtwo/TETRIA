#include "hardware_profiler.hpp"

namespace core {

WindowLimits HardwareProfiler::Profile() {
    // TODO: substituir por queries reais de Vulkan/DX12
    // (VkPhysicalDeviceMemoryProperties, DXGI_ADAPTER_DESC, etc.)
    WindowLimits limits;
    limits.vram_bytes = 8ull * 1024 * 1024 * 1024; // placeholder: 8GB
    limits.pcie_bandwidth_gbps = 16.0;              // placeholder: PCIe 4.0 x16 ~ ida/volta
    limits.max_window_slices = 64;                  // m calculado a partir do acima
    return limits;
}

} // namespace core
