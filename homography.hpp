#pragma once
#include <array>
#include <cmath>

namespace render {

// Homografia 3x3 por rotação pura (mesmo centro óptico) — README v22.
// Resultado clássico de visão computacional: a relação entre duas
// imagens tiradas do MESMO ponto focal, girando a câmera, é sempre uma
// homografia H = K·R·K⁻¹. Isso modela exatamente o WindowController
// (ring buffer / scroll): cada posição no anel é uma captura em ângulo
// diferente ao redor de um centro fixo — um cilindro.
//
// LIMITAÇÃO REAL, MEDIDA (não suposta, ver py/verifica_homografia.py):
// homografia de um plano único degenera perto de 90° (det(H) explode:
// 0,92 em 10° → 73,7 em 91°). Por isso esta classe só é usada para
// alinhar fatias VIZINHAS do anel (ângulo pequeno entre posições
// consecutivas) — o panorama global usa projeção cilíndrica separada
// (CylindricalPhotoCompiler), não uma homografia de plano único.
class Homography {
public:
    // matriz 3x3, row-major
    std::array<double, 9> m{1,0,0, 0,1,0, 0,0,1};

    static Homography Identity() { return Homography{}; }

    // K = intrínsecos da câmera (f = distância focal, cx,cy = centro óptico)
    // R = rotação em torno do eixo Y (vertical), ângulo theta
    // H = K · R · K⁻¹, normalizada por H[2][2]
    static Homography FromRotationY(double f, double cx, double cy, double theta) {
        const double c = std::cos(theta), s = std::sin(theta);
        // K
        const double K[9] = { f,0,cx,  0,f,cy,  0,0,1 };
        // R (rotação em Y)
        const double R[9] = { c,0,s,  0,1,0,  -s,0,c };
        // K^-1 (fórmula fechada para essa forma específica de K)
        const double Kinv[9] = { 1.0/f, 0, -cx/f,  0, 1.0/f, -cy/f,  0, 0, 1 };

        double KR[9];
        Mul3x3(K, R, KR);
        double H[9];
        Mul3x3(KR, Kinv, H);

        Homography result;
        const double norm = H[8]; // H[2][2]
        for (int i = 0; i < 9; ++i) result.m[i] = H[i] / norm;
        return result;
    }

    // Aplica a homografia a um ponto 2D (coordenadas de imagem),
    // devolvendo o ponto transformado (já des-homogeneizado).
    void Apply(double x, double y, double& out_x, double& out_y) const {
        const double px = m[0]*x + m[1]*y + m[2];
        const double py = m[3]*x + m[4]*y + m[5];
        const double pw = m[6]*x + m[7]*y + m[8];
        out_x = px / pw;
        out_y = py / pw;
    }

    Homography Compose(const Homography& other) const {
        double result[9];
        Mul3x3(m.data(), other.m.data(), result);
        Homography h;
        const double norm = result[8];
        for (int i = 0; i < 9; ++i) h.m[i] = result[i] / norm;
        return h;
    }

    double Determinant() const {
        return m[0]*(m[4]*m[8]-m[5]*m[7]) - m[1]*(m[3]*m[8]-m[5]*m[6]) + m[2]*(m[3]*m[7]-m[4]*m[6]);
    }

private:
    static void Mul3x3(const double* a, const double* b, double* out) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                double sum = 0;
                for (int k = 0; k < 3; ++k) sum += a[i*3+k] * b[k*3+j];
                out[i*3+j] = sum;
            }
    }
};

} // namespace render
