#pragma once
#include "../stream/slice_streamer.hpp"
#include "metric_recovery.hpp"
#include <vector>

namespace render {

// Geometria já resolvida contra a referência absoluta (L² - Termos_corte),
// pronta para ir ao vertex/mesh shader. Cada slice carrega também seus
// lados recuperados (Seção 15) a partir de área + diagonal, sem
// enumeração ponto a ponto.
struct CorrectedGeometry {
    std::vector<stream::Slice> slices;
    std::vector<MetricRecovery> metrics; // metrics[i] corresponde a slices[i]
};

class BoundingReference {
public:
    // Aresta Core -> BoundingReference (por frame)
    CorrectedGeometry Evaluate(const std::vector<stream::Slice>& activeWindow);

private:
    // Extrai (área, diagonal) de uma fatia a partir de seus bounds
    // (implementação depende de como Slice expõe extensão/bounding box).
    static void ExtractAreaDiagonal(const stream::Slice& slice, double& area, double& diagonal);
};

} // namespace render
