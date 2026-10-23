# Leia-me da organização

Este arquivo explica **como esta pasta foi montada** a partir dos zips e arquivos
soltos que foram enviados, qual versão de cada arquivo foi escolhida e o que ainda falta.
Pode apagar quando tudo estiver completo.

## Resultado da verificação

Compilado com `cmake` + `g++ -std=c++20`. Tudo compila (20 arquivos `.cpp`).
Dos 14 testes registrados no `ctest`, **todos passam** (os outros dois, `test_wallclock_sparse` e
`test_formatos`, são lentos: cerca de 60 s e 10 s).

O `A.zip` que você mandou é o conjunto do **modo foto (v24)**: `projective_pipeline`,
`keyboard_input`, `homography`, `cylindrical_photo_compiler` e `main.cpp`. Já está integrado.
Os zips `files_1/2/3/6` também foram integrados (testes extras e scripts `verifica_*.py`).

## Como as versões foram escolhidas

Havia muitas cópias do mesmo arquivo (`nome (1).hpp`, `nome (2).cpp`...). Comparei o
conteúdo, não o nome.

| Arquivo | Escolhida | Motivo |
|---|---|---|
| `spatial_partition.hpp` | a de 618 linhas | única com Morton, `BuildInPlace` e pirâmide |
| `slice_streamer.hpp` | a que declara `WFLV` | única que bate com o `.cpp` v25 |
| `slice_streamer.cpp` | **feito aqui**, ver abaixo | o v25 não lia `WFLV` |
| `projective_pipeline.{hpp,cpp}`, `main.cpp` | conjunto **v24** (modo foto) | o mais novo; vem do `A.zip` / `files_1` |
| `geometria_engine.py` | a de 547 linhas | tem todas as funções das Seções 22 a 25 |
| `mirror_catalog.hpp` | a de 195 linhas | a mais completa; casa com `test_section29` |
| `relatorio_consolidado` | PDF v16 (40 páginas) | o mais novo; os `.md`/`.tex` são antigos |
| log de desenvolvimento | README com entradas até [28] | o mais completo |

As versões antigas dos relatórios ficaram em `docs/_versoes_antigas/`.

## O que foi criado ou alterado aqui (não veio exatamente assim de você)

- `src/stream/slice_streamer.cpp`: o v25 com leitura do formato `WFLV` acrescentada.
- `py/gera_wflv.py`: converte `WFLS` em `WFLV` e confere o round-trip.
- `tests/test_formatos.cpp`: compara a contagem de nós entre formatos, fatia por fatia.
- `assets/world_bunny_vertexdelta_64.stream` e `assets/world_bunny_sparse_32.stream`: gerados a partir dos seus arquivos do coelho.
- `CMakeLists.txt`, `.gitignore`, `README.md` e este arquivo.
- `tests/test_inplace_parity.cpp` e `tests/test_wallclock_sparse.cpp`: troquei o caminho `/home/claude/assets/` por `assets/`.
- `tests/test_render_loop.cpp`: era anterior ao v24 e **travava para sempre**, porque não chamava
  `pipeline.AdvanceFrame()` (no v24 o contador de frames avança ali, e não mais em `Present()`).
  Acrescentei a chamada e mudei a expectativa de 30 para 60 frames, como diz a entrada [12] do log.
- `extras/simulacao_radial.py`: tinha um `"""` solto na última linha (erro de sintaxe); removi só essa linha.

## O que falta (não estava em nenhum arquivo enviado)

1. **`hardware_profiler` v13** (mede a RAM real em `/proc/meminfo`). Está a versão antiga, com valores fixos.
2. **Scripts Python**: o `voxeliza_bunny_real.py` original se perdeu. Está em `py/` uma **reconstrução** que NÃO é idêntica
   (acerta cerca de 95% dos voxels em L=32 e 87% em L=64; rode `py/confere_voxelizacao.py`). Os seus `.stream` continuam
   sendo a referência. O `stanford-bunny.obj` foi baixado de `github.com/alecjacobson/common-3d-test-models` e está em `assets/`.
   Ainda faltam alguns `verifica_*.py` citados no README.
3. **Testes C++ citados no README**: `test_bitmap_pyramid`, `test_vertex_delta_codec`,
   `test_vertexdelta_engine`, `test_hardware_profiler`, `test_homography` e outros.
4. **Log do README**: termina na entrada [28]. As entradas [29] a [31] não vieram.

## Coisas para saber

- Os geradores `py/gera_world_*_stream.py` têm o caminho `/home/claude/stanford-bunny.obj` (o arquivo agora está em `assets/stanford-bunny.obj`) escrito dentro do código.
- O pipeline grava imagens em `/tmp/frame_NNN.ppm` e o `main.cpp` em `/tmp/photo_NNN.ppm` (caminho fixo no código).
- O `DecodeVarint` do `vertex_delta_codec.hpp` não checa o fim do buffer; um arquivo `WFLV` corrompido pode fazê-lo ler fora do vetor.
- As `extras/simulacao_radial*.py` são modelos de brinquedo: as conclusões impressas estão escritas no código, não medidas.
