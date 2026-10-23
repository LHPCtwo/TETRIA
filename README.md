# engine-fluxo

Engine em C++20 que lê um mundo 3D fatia por fatia (streaming), comprime cada
fatia em quadtree e projeta o resultado. A matemática por trás (decomposição
geométrica de um semiprimo, quadtree exato, esparsidade por vértice, etc.) está
no relatório em `docs/relatorio_consolidado_v16.pdf`.

> Estado: **em desenvolvimento**. Veja `LEIA-ME_ORGANIZACAO.md` para saber o que
> está completo e o que ainda falta nesta pasta.

## Como compilar e testar

Precisa de `g++` com suporte a C++20 e `cmake` 3.20 ou mais novo.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

Rode sempre a partir da **raiz do projeto**, porque os testes leem arquivos de `assets/`.

Os testes `test_wallclock_sparse` (~60 s) e `test_formatos` (~10 s) medem tempo real e são lentos de propósito.
O pipeline grava imagens `.ppm` em `/tmp` (caminho fixo no código, em `projective_pipeline.cpp` e `main.cpp`).

## Pastas

| Pasta | O que tem |
|---|---|
| `src/core` | vetor 3D, perfil de hardware, matemática rápida (máscara/shift) |
| `src/stream` | leitura das fatias do disco, janela circular, quadtree, codec vértice+delta |
| `src/render` | métricas das fatias, projeção, teclado, detector de ponto de fuga, homografia e modo foto cilíndrico |
| `src/fold`, `src/symmetry`, `src/procedural` | dobra plana, espelhos, Horner por paridade, caminhamento diagonal |
| `tests/` | testes em C++ (registrados no `CMakeLists.txt`) |
| `py/` | scripts Python de validação e geradores de arquivos `.stream` |
| `assets/` | mundos de teste: sintético e coelho de Stanford em 4 formatos |
| `docs/` | relatório matemático, log de desenvolvimento, grafos dos módulos |
| `extras/` | coisas paralelas (formato TESS, mosaico, simulações radiais) que não fazem parte da engine |

## Formatos de arquivo do mundo (`assets/*.stream`)

| Magic | O que guarda | Coelho 64 |
|---|---|---|
| `WFLX` | parâmetros (k, c) por fatia, malha algébrica | (só no sintético) |
| `WFLB` | bitmap denso por fatia | 32.780 bytes |
| `WFLS` | lista de pontos (x, y) | 50.152 bytes |
| `WFLV` | índice de vértice + delta (varint) | **13.413 bytes** |

## Licença

Ainda não escolhida.
