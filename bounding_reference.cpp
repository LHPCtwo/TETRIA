#include "bounding_reference.hpp"
#include <cmath>

namespace render {

void BoundingReference::ExtractAreaDiagonal(const stream::Slice& slice, double& area, double& diagonal) {
    // A área da bounding box da fatia (equivalente ao "quadrado L^2 de
    // referência" da Seção 13) e sua diagonal, que alimentam o Passo 1-4
    // da Seção 15 (recuperação por partição e escala).
    const double w = slice.width;
    const double h = slice.height;
    area = w * h;
    diagonal = std::sqrt(w * w + h * h);
}

CorrectedGeometry BoundingReference::Evaluate(const std::vector<stream::Slice>& activeWindow) {
    CorrectedGeometry geometry;
    geometry.slices = activeWindow;
    geometry.metrics.reserve(activeWindow.size());

    for (const auto& slice : activeWindow) {
        double area = 0.0, diagonal = 0.0;
        ExtractAreaDiagonal(slice, area, diagonal);

        if (diagonal <= 0.0) {
            // fatia degenerada (sem extensão): nada a recuperar
            geometry.metrics.push_back(MetricRecovery{});
            continue;
        }

        // Passos 1-4 da Seção 15: hD, quadrado composto, semiperímetro, perímetro,
        // sem passar pela equação quadrática explícita x^2 - Sx + A = 0.
        geometry.metrics.push_back(RecoverFromAreaAndDiagonal(area, diagonal));
    }

    return geometry;
}

} // namespace render
