#include "window_controller.hpp"
#include "../core/fast_math.hpp"

namespace stream {

WindowController::WindowController(std::size_t m) : m_(m) {
    ring_.resize(m_);
}

void WindowController::Slide(const Vec3& cameraPos, const Slice& newSlice) {
    // Resíduo via truncamento por MÁSCARA (core::FastMath): m_ deve ser
    // potência de 2. Aqui o objetivo é indexação circular (custo), não
    // magnitude — por isso máscara, não shift (ver Seção 7).
    const std::size_t idx = core::TruncaPorMascara(static_cast<uint32_t>(head_), __builtin_ctz(static_cast<uint32_t>(m_)));
    ring_[idx] = newSlice; // descarta a fatia antiga nesse slot, insere a nova
    ++head_;
}

} // namespace stream
