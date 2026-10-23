# engine-fluxo — Log de Desenvolvimento

Este README é atualizado a cada mudança relevante. Cada entrada é datada e
descreve o que foi feito, o que foi validado, e o que ficou pendente.
O relatório matemático (`relatorio_consolidado.{md,tex,pdf}`) documenta a
teoria; este arquivo documenta a implementação em C++ (a engine) e sua
relação com essa teoria.

## Como o projeto está organizado

```
src/
├── core/
│   ├── vec3.hpp
│   ├── hardware_profiler.{hpp,cpp}   — mede VRAM/PCIe, define m (janela)
│   └── fast_math.hpp                 — máscara vs shift (Seção 7)
├── stream/
│   ├── slice_streamer.{hpp,cpp}      — puxa fatia, classifica via quadtree
│   ├── window_controller.{hpp,cpp}   — ring buffer de tamanho m
│   ├── union_find.hpp                — Union-Find + shift (Seção 8)
│   ├── spatial_partition.hpp         — QuadNode/OctNode (Seções 5, 12)
│   ├── hybrid_partition.hpp          — union-find grosseiro + quadtree fino (Seção 17)
│   └── vertex_delta_codec.hpp        — codec vértice+delta, formato WFLV (Seção 9 / README v27)
├── render/
│   ├── bounding_reference.{hpp,cpp}  — L² - correção (Seção 13)
│   ├── metric_recovery.hpp           — recuperação por partição/escala (Seção 16)
│   └── projective_pipeline.{hpp,cpp} — projeção + present (stub, sem API gráfica real)
├── fold/
│   └── fold_validation.hpp           — Kawasaki + Maekawa, pré-condição de dobra plana (Seção 22.1)
├── symmetry/
│   ├── mirror_catalog.hpp            — as 6 operações de espelho nomeadas (Seção 22.3-22.4)
│   └── horner_parity.hpp             — colapso de Horner por raízes simétricas (Seção 22.6)
└── main.cpp                          — Render Loop (núcleo do grafo radial)

docs/
├── module_graph.dot / .svg / .png            — grafo hierárquico dos módulos
└── module_graph_radial.dot / .svg / .png     — grafo radial (núcleo + polígono)

py/  — os 15 scripts de validação matemática (fonte da verdade dos números do relatório)
```

Cada módulo em `src/render` e `src/stream` cita explicitamente a seção do
relatório de onde a matemática vem. Isso é intencional: qualquer alteração
na teoria (relatório) deve ser rastreável até o código, e vice-versa.

---

## Log de mudanças

### [28] — Seção 22: dobra plana, catálogo de espelhos, e colapso de Horner por paridade, portados à engine
**O que foi pedido:** consolidar na engine C++ um conjunto de resultados de
geometria dinâmica (dobra/origami, ramificação recursiva, reflexão) que
vinha sendo desenvolvido em paralelo em quatro documentos de apoio
(`ladrilhamento_origami_ramificacao.md`, `secao_pentagono_trapezio.md`,
`glossario_espelho_letras.md`, `secao_espelhamento_horner.pdf` — este
último era, na verdade, um pacote zip de imagens+texto por página, não um
PDF padrão; extraído com `unzip` antes da leitura). O código Python de
referência já existia consolidado em `geometria_engine.py`; esta entrada
faz o porte para C++ e a incorporação formal ao relatório.

**Adicionada Seção 22 ao relatório** (`relatorio_consolidado.pdf`, agora 31
páginas — 27 originais + 4 do apêndice novo, concatenadas via `pdfunite`
para preservar exatamente a tipografia LaTeX original em vez de
reconstruir o documento inteiro a partir de texto extraído, que teria
introduzido artefatos de hifenização/quebra de página). Seções internas:

- **22.1** Validação de dobra plana (Kawasaki + Maekawa) — necessárias mas
  não suficientes; usada como pré-condição antes de gerar dobra
  procedural.
- **22.2** Multiplicação por reflexões sucessivas (leque/flor).
- **22.3** Catálogo das 6 operações de espelho (vertical, horizontal,
  rotação 180°, auto-simetria, sobreposição-vs-translação, composição) —
  testadas em glifos reais (fonte DejaVu Sans); confirma, entre outros,
  que m→w **não** é reflexão limpa nesta fonte (erro 0,142), ao contrário
  de b→d, b→p, b→q, n→u.
- **22.4** Espelho transladado: prova que alinhar (em vez de centralizar)
  o trapézio da Seção 10 colapsa o hexágono em pentágono (ou triângulo,
  se n=1) por colinearidade de arestas, e que espelhar sem sobrepor
  sempre restaura o hexágono — verificado exaustivamente (20 casos, via
  `scipy.spatial.ConvexHull`), sem exceção.
- **22.5** Ramificação recursiva (árvore orgânica) — o mesmo mecanismo de
  composição L→V (22.3) aplicado recursivamente a cada vértice-filho.
- **22.6** Colapso de Horner por paridade: raízes em pares simétricos
  reduzem a cadeia de Horner de 2k para k passos, com o ganho herdado
  integralmente pela truncagem modular (Seção 1.7/1.8 do relatório) —
  conecta-se diretamente à distinção palíndromo par/ímpar já registrada na
  Seção 19.

**Portado para C++:** `fold::FoldValidation` (`src/fold/fold_validation.hpp`),
`symmetry::MirrorCatalog` (`src/symmetry/mirror_catalog.hpp`),
`symmetry::HornerParity` (`src/symmetry/horner_parity.hpp`).

**Validado por paridade C++/Python** (`test_section22_engine.cpp`, 5/5
casos): Kawasaki do hexágono regular (true) e do caso inválido
70/110/70/110 (false, 140≠220 nos alternados); composição L→V (6 pontos,
2× o L original); colapso de Horner no caso de referência
(raízes {7,5,3,1}, c=4 → P(40)=1.666.665, coeficientes de Q(v)=[1,-10,9]);
truncagem modular (mod 9 = 0). Todos batem exatamente com
`geometria_engine.py`.

**Pendente:** `gera_arvore_organica` (Seção 22.5) e
`multiplica_por_reflexoes` (Seção 22.2) ainda não portados para C++ —
ficaram só em `geometria_engine.py` nesta entrada, pois não têm ainda um
consumidor na engine (nenhum sistema de vegetação/ladrilho procedural
existe em `src/` hoje); portar quando houver esse consumidor, em vez de
antecipar uma interface sem uso real.

---

### [27] — Formato WFLV (vértice+delta): esparso vence o denso em espaço também, com um incidente real de perda de ambiente no meio
**O que foi pedido:** modelar a esparsidade pelos vértices, de forma
mais estrutural — usando o índice único de vértice ($val=yW+x+1$,
Seção 9 do documento "Números Primos como Interseção de Triângulos")
em vez de pares $(x,y)$ separados, explorando que a lista ordenada
permite comprimir por delta entre índices consecutivos (varint/LEB128,
técnica clássica de posting-list).

**Medido antes de implementar** (`py/verifica_esparsidade_por_vertice.py`,
dado real do bunny): razão bruta 0,251 contra o formato WFLS anterior.
Contando de forma justa, com cabeçalho e índice de offsets reais: **3.648
bytes** (L=32) contra 12.844 do WFLS (3,7x menor) e — o resultado mais
importante — contra **4.108 bytes do bitmap denso WFLB** (0,888x, ou
seja, **vence o denso também**, não só o esparso anterior). Em L=64:
13.413 contra 32.780 do denso (0,409x). Essa é a primeira vez que o
formato esparso vence o denso em espaço em disco — antes (v16) ele
sempre perdia nessa métrica, só vencia em trabalho de construção da
árvore.

**Implementado:** `stream::VertexDeltaCodec` (`vertex_delta_codec.hpp`),
novo formato de arquivo `WFLV`, integrado ao `SliceStreamer` com
detecção automática de magic (coexiste com `WFLX`/`WFLB`/`WFLS`, não
quebra nenhum formato anterior).

**Validado em três camadas:** round-trip exato (Python, 0 falhas em
32+64 fatias, comparado como conjunto — a ordem muda porque o índice
de vértice ordena por y primeiro, não por x), round-trip em C++
(`test_vertex_delta_codec.cpp`, 0 falhas), e paridade na engine real
contra o WFLS e o gabarito conhecido (`test_vertexdelta_engine.cpp`,
6/6 fatias batendo).

**Incidente real registrado, não escondido:** no meio desta
implementação, o ambiente de trabalho local (`/home/claude`) perdeu o
diretório `src` inteiro entre chamadas de ferramenta (reset de sandbox,
não perda de histórico de chat) — o trabalho ainda não salvo em
`outputs` (o codec novo, as mudanças no `SliceStreamer`) sumiu.
Recuperado restaurando a base de `outputs` (que estava íntegra, pois
cada entrada anterior já era salva imediatamente) e recriando as
mudanças desta sessão a partir do que já estava validado na conversa —
confirmado que a recriação bateu exatamente com o que tinha sido
testado antes (mesmos números). Reforça a prática já em vigor: salvar
em `outputs` a cada arquivo novo, não só no fim da tarefa.

**Estado da compilação:** novo target `test_vertexdelta_engine` no
CMake. Regressão completa (`test_sparse_engine`, `test_bunny_engine`,
`test_main_photo_integration`) rodada após a integração — nada quebrou.

---

### [26] — Seção 21: completando trapézios/hexágonos com critério de primalidade, vãos e sólidos duais
**O que foi feito:** o documento "Números Primos como Interseção de
Triângulos Semelhantes" retomava exatamente a construção da Seção 19.1
($N=T_{k+1}-T_{k-1}$) e completava a peça que faltava lá: essa
decomposição é única precisamente quando $N$ é primo. Acrescentada
Seção 21 ao relatório com o teorema completo, mais as extensões que o
documento trazia (números hexagonais via Delaunay real, múltiplas
tesselações sobre a mesma malha — losango/hexágono/triângulo —, e
dualidade cubo/octaedro via projeção no eixo de simetria 3).

**Tudo verificado nesta consolidação antes de escrever, não só
aceito por leitura** (`py/verifica_primos_trapezio.py`,
`py/verifica_piramide_hexagono.py`, `py/verifica_vaos_solidos.py`,
`py/verifica_pontos_fuga.py`):
- Teorema de unicidade (primalidade ↔ decomposição única): confirmado
  por busca exaustiva, $N=3$ a $200$, incluindo a tabela exata do
  documento.
- Pirâmide simétrica ($=p^2$), $H(k)=3k^2+3k+1$, espelhamento do
  trapézio: confirmados para $k=0..29$.
- Proposição dos vãos ($6k^2$ triângulos elementares): confirmada por
  **triangulação de Delaunay real** (`scipy.spatial.Delaunay`), não
  recontagem de fórmula — $6,24,54,96$ para $k=1..4$, exatos.
- Projeção do octaedro no eixo de ordem 3 = hexágono regular:
  confirmada por geometria 3D direta (6 distâncias e 6 ângulos
  exatamente iguais), não aceita de olho.
- Pontos de fuga (Seção 7 do documento): testados os 4 fenômenos
  qualitativos com câmera sintética própria (números absolutos
  diferem do documento, orientação de câmera diferente, mas os 4
  fenômenos se confirmam de forma independente) — conectado ao
  `render::VanishingPointAnalyzer` já existente (Seção 20.7).

**Achado extra, verificado e descartado antes de escrever:** o
documento comenta a distinção entre o hexágono regular (malha
triangular verdadeira) e o hexágono achatado "baú" (linhas empilhadas
em grade quadrada ingênua). Testei se bastaria uma rotação pura para
converter um no outro — **não basta**: o "baú" tem lados de dois
tamanhos diferentes (2,0 e 1,118), o regular tem todos iguais (1,0);
rotação é isometria (preserva distâncias), não pode igualar
multiconjuntos de distância diferentes. É preciso escala não-uniforme
+ cisalhamento (mesma transformação já usada em
`py/verifica_tesselacoes.py`, Seção 19.3, pra gerar a malha triangular
a partir de grade quadrada). Não incorporado como afirmação no
relatório por não ter se confirmado como estava.

**Estado:** só documentação/relatório, nenhuma mudança de código nesta
entrada.

---

### [25] — Item 10 revisto: premissa errada corrigida + ganho real medido (36,8%)
**Achado antes de otimizar:** a via registrada como próximo passo do
item 10 ("reaproveitar ordenação Morton entre frames consecutivos")
partia de uma premissa errada sobre o padrão de acesso real. Verificado
(`bounding_reference.cpp`): a árvore de cada fatia é construída **uma
única vez** dentro de `SliceStreamer::PullSlice`, e descartada logo
depois (só o `nodeCount` sobrevive no `Slice`) — `BoundingReference`
nunca reconstrói nada, só lê `width`/`height` já guardados. Como a
câmera percorre `z` monotonicamente (sem revisitar fatias), não há
reconstrução redundante entre frames pra evitar. Corrigido o
entendimento antes de tentar resolver o problema errado.

**O que realmente se aplica:** `SpatialPartitionSparse::Build` (v16)
aloca 4 `std::vector` novos a cada nó da árvore — overhead de heap, não
de "toques por ponto" (esse fica em ~5,6-6,6x mesmo, é inerente à
profundidade da árvore, não muda). Implementada `BuildInPlace`:
particiona os pontos **no mesmo buffer** via `std::partition` (3
passadas por nó, sem alocar nada novo), reaproveitando o padrão real de
uso (`points` em `SliceStreamer` já é um buffer novo e descartável a
cada frame — candidato perfeito pra partição in-place sem cópia).

**Validado em duas camadas:**
- **Paridade lógica** (`tests/test_inplace_parity.cpp`): 32/32 fatias
  batem exatamente contra `Build` (mesma contagem de nós, mesma
  contagem de toques — 17.444 = 17.444 — confirmando que só mudou a
  alocação, não a lógica).
- **Tempo de parede real** (`tests/test_wallclock_sparse.cpp`,
  `std::chrono`, 2000 repetições sobre as 64 fatias de
  `world_bunny_sparse_64.stream`, medindo só a construção da árvore —
  não a leitura de disco): **36,8% mais rápido** (2365ms vs 3744ms para
  o mesmo trabalho). Medi primeiro incluindo o custo de uma cópia
  (ganho de 17,6%, mais conservador) e depois sem essa cópia artificial
  (ganho real de 36,8%, cenário que reflete o uso de verdade no
  `SliceStreamer` — não escondi a diferença entre as duas medições).

**Integrado**: `SliceStreamer` agora usa `BuildInPlace` no formato
`WFLS` (esparso). Regressão completa após a troca: `engine_fluxo` roda
limpo, `test_sparse_engine` (denso vs esparso, 6/6), `test_bunny_engine`
(6/6 contra gabarito Python), `test_main_photo_integration` (57 Draw +
3 Photo = 60, sem quebrar o que a v24 tinha corrigido).

**O que NÃO mudou:** o fator ~5,6-6,6x de toques por ponto continua
igual — essa parte é inerente à profundidade da árvore de partição
espacial, não foi reduzida (só o custo de alocação por toque, que é um
eixo de custo diferente). Se quiser reduzir o fator de toques em si,
precisaria de uma estrutura fundamentalmente diferente (ex.: índice
persistente entre múltiplas consultas — que não se aplica aqui, como
já estabelecido acima).

**Estado da compilação:** dois novos targets no CMake
(`test_inplace_parity`, `test_wallclock_sparse`). Todo o projeto
recompilado e re-testado.

---

### [24] — Modo foto integrado de verdade no main.cpp (gatilho real, não só teste isolado)
**O que foi pedido:** ligar `CylindricalPhotoCompiler`/`Homography`
(README v22) ao loop real da engine — antes só existiam testados
isoladamente, `main.cpp` nunca os chamava.

**O que foi feito:**
- `KeyboardInput` ganhou a tecla `p` (pedido de foto), sem duplicar
  leitura de stdin (single-owner do fd, evita condição de corrida entre
  dois leitores).
- `ProjectivePipeline::ConsumePhotoRequest()`: consome o pedido real
  primeiro; se não há tecla real E não há TTY interativo, cai num
  fallback simulado (dispara a cada 20 frames) — mesmo padrão de
  honestidade já usado no fallback de movimento (v19).
- `main.cpp` decide a ramificação Draw() vs Compile() por frame,
  extraindo as escalas reais (`metric.a`, mesma fonte que `Draw()` já
  usava) da geometria já calculada — sem recalcular nada.

**Dois bugs reais pegos rodando de verdade (não em teoria):**
1. **Loop infinito real, travou o ambiente.** `frame_count_` só
   incrementava dentro de `Present()`, que o ramo de foto pula — então
   `frame_count_` nunca avançava, o gatilho do fallback (`%20==0`)
   ficava sempre verdadeiro, e `ShouldClose()` nunca fechava. Rodei sem
   `timeout` e o processo gerou **5.168 arquivos PPM** antes de eu
   interromper, enchendo `/tmp` a 100% e quebrando a compilação
   seguinte (`No space left on device`). Corrigido com `AdvanceFrame()`
   novo, chamado sempre, nos dois ramos.
2. **Off-by-one na numeração dos arquivos**, efeito colateral da
   correção acima: `frame_000.ppm` aparecia (não devia) e
   `frame_060.ppm` sumia. Corrigido movendo `AdvanceFrame()` para o
   início da iteração, sincronizando a numeração usada por `Present()`
   e por `ConsumePhotoRequest()`.

**Validado com números exatos, não só "não travou"**
(`tests/test_main_photo_integration.cpp`, réplica fiel do loop de
`main.cpp`, com limite de segurança de 200 iterações caso `ShouldClose`
falhasse de novo):
- 60 iterações totais, **57 Draw + 3 Photo = 60** (soma exata, sem
  sobreposição nem lacuna).
- Terminou por `ShouldClose()` de verdade (60 < limite de segurança de
  200), não pela rede de segurança.
- Com tecla real `p` enviada via pipe: **4 fotos** (3 do fallback nos
  múltiplos de 20 + 1 extra da tecla real) — confirma que o caminho de
  input real soma corretamente, não é ignorado nem substitui o
  fallback silenciosamente.
- Regressão completa: `test_bunny_engine` (6/6), `test_homography`
  (todos os casos), `test_photo_mode_exclusion` (28+2=30, teste
  isolado pré-existente) — nada quebrou.

**Estado da compilação:** novo target `test_main_photo_integration` no
CMake. Todos os binários recompilados e re-testados após os dois fixes.

---

### [22] — Scroll como cilindro: homografia local + projeção cilíndrica global (modo foto)
**O que foi pedido:** implementar um "scroll" que renderiza e compila,
observando que um scroll é semelhante a um anel e um cilindro, que isso
gera homografia, e que no "modo foto" o jogo inteiro deixa de ser
renderizado normalmente para renderizar a foto/cenário.

**A ligação correta (confirmada, não só afirmada):** o `WindowController`
já É um anel (ring buffer) — cada posição corresponde a um ângulo, como
um cilindro. Quando uma câmera gira em torno do MESMO centro óptico
(que é o caso de um scroll circular), a relação entre duas capturas em
ângulos diferentes é, por resultado clássico de visão computacional,
sempre uma **homografia** ($H = K \cdot R \cdot K^{-1}$) — isso está
matematicamente certo, confirmado numericamente antes de implementar
(ver abaixo).

**O que foi feito:**
- `render::Homography` (novo): matriz 3x3 de homografia por rotação
  pura. Validada em Python primeiro (`py/verifica_homografia.py`),
  depois portada para C++ com paridade exata (erro ~1e-14, precisão de
  ponto flutuante) contra os mesmos casos.
- **Achado real, medido, não suposto:** homografia de plano único
  degenera perto de 90° — `det(H)` explode de 0,92 (10°) para 73,7
  (91°), confirmado numericamente nos dois lados (Python e C++). Por
  isso homografia de plano único **não serve** para desenrolar o anel
  inteiro (360°) — só serve para alinhar fatias vizinhas (ângulo
  pequeno, $2\pi/m$).
- `render::CylindricalPhotoCompiler` (novo): compila o anel inteiro
  numa foto panorâmica via **projeção cilíndrica** (não-linear, sem o
  problema de degeneração) para o posicionamento global, usando
  homografia só para o ajuste local entre vizinhos.
- **Trade-off implementado de verdade, não só documentado:**
  `tests/test_photo_mode_exclusion.cpp` simula um loop com gatilho de
  "tirar foto" a cada 10 frames — cada frame toma OU o caminho normal
  (`ProjectivePipeline::Draw`) OU o modo foto (`Compile`), nunca os
  dois. Confirmado por contagem exata: 28 frames normais + 2 fotos = 30,
  sem sobreposição.

**Bugs reais pegos no processo (não escondidos):**
1. Erro de compilação real (referência circular no valor default de
   `Params` do construtor) — corrigido.
2. Minha primeira expectativa de teste estava errada (`frame%10==0`
   para `frame>0` em `0..29` só bate 2 vezes, não 3 como escrevi
   primeiro) — corrigido o teste, não o código.
3. **Achado real na primeira geração de imagem:** o panorama saiu com
   todo o conteúdo espremido num canto e uma linha fina do resto —
   não era bug de matemática, era a demonstração usando só 64 frames
   simulados contra um anel de `m=1024` posições reais (medido via
   `HardwareProfiler`, v13) — 960 posições ficaram com o valor
   default (vazio). Corrigido rodando frames suficientes pra preencher
   o anel de verdade. Resultado: 4 repetições do mesmo padrão
   visualmente coerentes (`1024/256=4`, o ciclo do `k=k_max-z` do
   asset sintético se repete 4x ao preencher o anel) — confirma que a
   projeção cilíndrica reflete corretamente o dado real subjacente,
   não um artefato.

**Estado da compilação:** dois novos targets (`test_homography`,
`test_photo_mode_exclusion`). Confirmado que nada quebrou no resto do
projeto (`engine_fluxo` roda limpo).

---

### [23] — Anomalia do m-tree (v20) investigada: NÃO é bug, é alinhamento real

**A anomalia:** no teste m-tree (v20), k/L=1/3, 1/9 e 1/27 em L=243,
m=3 davam exatamente o mesmo número de nós (1090), apesar de k ser bem
diferente em cada caso (81, 27, 9). Anotado como pendência aberta,
suspeita de bug.

**Investigado agora:** confirmado que **não é bug**. Comparação
independente contra o baseline `k=0` (sem bloco nenhum, só a
hipotenusa) deu **exatamente o mesmo 1090**. Isso prova que nos três
casos (k=81,27,9), o bloco tem custo marginal **zero** sobre a árvore —
suas bordas coincidem exatamente com linhas de subdivisão que a
hipotenusa já exigia naquele nível, então adicionar o bloco não força
nenhuma subdivisão extra.

**Mapeado sistematicamente** (varredura k=0 a 242, passo 3): os valores
de k que alinham "de graça" são exatamente as potências de 3 —
`{0, 3, 9, 27, 81}` — confirmando que é o mesmo fenômeno de alinhamento
já estabelecido na Seção 9.1 (lá para base 2, aqui replicado para base
3), agora generalizado: **múltiplos k distintos podem cair no mesmo
baseline por coincidência de alinhamento com a base m**, não é uma
propriedade de um único k "mágico".

**Nuance real encontrada, não escondida:** k=80 (perturbado de 81 por
-1) também bateu no baseline, mas k=79 não — efeito de arredondamento
inteiro em `c=(L-k)//2` que ocasionalmente faz k vizinhos também
alinharem por coincidência. Não caracterizei essa borda até o fim
(retorno decrescente para o esforço) — fica registrado como detalhe
observado, não resolvido por completo.

**Conclusão para a Seção 20.6:** os resultados numéricos lá continuam
válidos — não foram invalidados pela anomalia, que era só uma
coincidência de alinhamento múltiplo, não um erro de contagem.


---

### [21] — VanishingPointAnalyzer: enquadramento de ponto de fuga ligado a tesselação e ao princípio "medir, não assumir"
**O que foi pedido:** acrescentar à engine uma forma de enquadrar
pontos de fuga que também faz parte de tesselação, explicando por que
renderização/decisões de estrutura são baseadas em processamento do
hardware.

**A ligação conceitual:** a Seção 19.5 do relatório (documento
"Registro de Progresso") já tinha testado se uma escala geométrica
repetida ($x_i=x_0 r^i$, $y_i=y_0-di$) gera um ponto de fuga projetivo
clássico -- e mostrou que **não gera**: os pontos não são colineares em
$(x,y)$, só em $(\ln x, y)$ (inclinação medida $-5{,}7708$). Essa é
exatamente a forma da escala entre níveis de LOD/tesselação (quadtree
por potências de 2, m-tree por potências de $m$, Seção 20.6): um fator
de escala repetido $m^i$ entre níveis, geometricamente idêntico à
construção da Seção 19.5. Ou seja: **assumir ponto de fuga clássico ao
projetar entre níveis de tesselação seria assumir o modelo errado por
construção** -- a engine precisa medir qual padrão realmente ocorre,
mesmo princípio já usado pelo `HardwareProfiler` (mede RAM real em vez
de assumir) e pela decisão de subdivisão $m$-ária (Seção 20.6, testada
por medição, não suposta).

**O que foi feito:**
- `render::VanishingPointAnalyzer` (novo, `vanishing_point.hpp`): dado
  uma sequência de amostras (posição, escala aparente), testa
  colinearidade em $(x,y)$ [modelo linear] e em $(\ln x, y)$ [modelo
  log-linear] e decide qual se aplica por variância medida, não
  suposição.
- Prototipado e validado primeiro em Python
  (`py/detecta_padrao_escala.py`), com **validação cruzada contra o
  valor já publicado**: a inclinação log-linear detectada para a Rota B
  bateu exatamente com $-5{,}7708$ da Seção 19.5, tanto em Python quanto
  no port C++ (`tests/test_vanishing_point.cpp`, 4/4 casos corretos:
  linear clássico, Rota B, escala tipo LOD, ruído sem padrão).
- Integrado ao `ProjectivePipeline::Draw`: mede o padrão real entre as
  fatias da janela ativa a cada frame, exposto via `LastScalePattern()`.

**Bug/limitação real pega no processo:** a primeira integração deu
sempre resultado "degenerado" (`slope=0`, variância zero) -- não porque
o detector estivesse errado, mas porque `slice.width`/`height` no
`SliceStreamer` eram sempre fixados em `L` (constante), sem variação
real entre fatias para medir. Corrigido: para o formato paramétrico
(`WFLX`), agora expõe `k` (não `L`) como escala aparente -- é `k`,
não `L`, que varia de verdade com `z` nesse formato
(`gera_world_stream.py`: `k = k_max - z`, decréscimo linear).

**Validado com previsão matemática verificável, não só rodou sem
erro:** como `k = k_max - z` decresce exatamente 1 por frame, o modelo
correto a detectar é **linear**, com `slope=-1.0000` -- e foi
exatamente o que saiu (`tests/test_vanishing_point_engine.cpp`,
28/30 frames com padrão detectado, todos linear, slope=-1.0000 em
todos os pontos testados). Isso não foi ajustado para bater; é
consequência direta e prevista da matemática do próprio gerador.

**Estado da compilação:** dois novos targets no CMake
(`test_vanishing_point`, `test_vanishing_point_engine`). Confirmado que
nada quebrou: `engine_fluxo` roda limpo, `test_bunny_engine` continua
6/6.

---

### [20] — Hipótese do m-tree (item 7) testada: confirmação PARCIAL e delimitada
**O que foi testado:** a hipótese da Seção 20.2 — subdividir em $m$
partes por eixo (não necessariamente 2) alinhado ao $k/L$ típico dos
dados poderia melhorar a compressão — nunca tinha sido implementada
nem medida. `py/verifica_mtree.py` generaliza o teste de homogeneidade
geométrico exato (Seção 9.1) de subdivisão binária para $m$-ária.

**Primeira medição (com viés, pega antes de reportar):** testando
$m \in \{2,3,4,5,6,8\}$ com $L=256$ fixo para todos, $m=2$ venceu em
TODOS os casos, inclusive no pior caso do quadtree ($k/L=1/3$). Mas
esse teste tinha um viés real: para $m\ne$ potência de 2, a grade
precisa de padding até a próxima potência de $m$ (ex.: $m=3$ exige
grade $729$ para $L=256$, quase 3x maior) — isso infla os nós por
preenchimento, não pela hipótese em si.

**Corrigido:** re-testado com $L=m^{\text{depth}}$ EXATO para cada $m$
(sem sobra de padding). Resultado mudou: para $k/L=1/3$ especificamente,
$m=3$ **venceu** o quadtree — nós/L $=4{,}486$ contra $6{,}535$ do
$m=2$. Faz sentido: $1/3=3^{-1}$ é alinhado à base 3, do mesmo jeito
que $1/4=2^{-2}$ e $1/2=2^{-1}$ são alinhados à base 2 (Seção 9.1).

**Mas isso NÃO generaliza** — testado com as frações "naturais"
correspondentes para outros $m$:

| fração | nós/L (m=2) | nós/L (m natural) | m natural vence? |
|---|---|---|---|
| 1/3 (m=3) | 6,535 | 4,486 | **SIM** |
| 1/9 (m=3) | 4,332 | 4,486 | não |
| 1/5 (m=5) | 4,707 | 6,242 | não |
| 1/6 (m=6) | 5,223 | 7,338 | não |
| 1/7 (m=7) | 4,520 | 8,146 | não |

Só o caso $m=3,k/L=1/3$ venceu. Para $m=5,6,7$ com suas frações
naturais correspondentes, o quadtree continua melhor — o fator de
ramificação maior ($m^2$ filhos por divisão em vez de 4) custa mais
do que o alinhamento economiza, exceto nesse caso específico.

**Conclusão honesta:** a hipótese da Seção 20.2 estava certa em
princípio, mas otimista em escopo. Existe pelo menos um caso real onde
$m$-tree bate o quadtree (confirma que a ideia tem mérito), mas não é
uma melhoria geral aplicável trocando $m$ para "casar" com qualquer
$k/L$ — o custo extra de ramificação domina na maioria dos casos
testados. **Não portado para C++** (é resultado exploratório/matemático,
não uma melhoria de engine adotável sem mais investigação de quando
exatamente compensa).

---

### [19] — Input real de teclado (item 5), com fallback simulado explícito
**O que foi feito:** `render::KeyboardInput` (novo) — leitura real de
teclado via terminal (POSIX `termios`, modo raw + não bloqueante):
`w/s` move em z, `a/d` move em x, `q` encerra o loop. Isso é leitura
real de bytes de stdin, não um valor simulado — funciona com um
humano digitando num terminal interativo de verdade. Sem display
gráfico neste ambiente (confirmado antes, sem X server/GPU), input via
terminal é a via real disponível, não mouse/GUI.

`ProjectivePipeline::PollCameraInput` agora usa isso: se há tecla real
chegando, usa; senão (stdin fechado ou não-TTY sem dados), cai no voo
simulado antigo — a diferença entre os dois modos é reportável via
`InputModeIsReal()`, não escondida atrás de um valor só.

**Bug real pego durante o teste (não só no código, na minha primeira
tentativa de teste):** escrevi um teste esperando `x=3.0` para a
sequência `"wwwddq"`, mas contei errado — a string tem 2 `d`s, não 3.
Corrigido o teste, não o código (o código estava certo). Depois, um
segundo bug real, este no código: `PollAndApply` retornava `false` no
`case 'q'` **antes** de marcar `last_had_input_ = got_input`, então
`LastPollHadInput()` reportava `false` mesmo com input claramente
processado (posição mudou). Corrigido: `q` agora só marca uma flag
`quit`, o `last_had_input_` é sempre atualizado antes do retorno.

**Validado com números exatos** (`tests/test_keyboard_input.cpp`,
`tests/test_real_input_loop.cpp`), não só "não travou":
- `"wwwddq"` via pipe → `x=2.0, z=3.0`, `continuar=false` — bate exato.
- Loop completo sem stdin: 60 frames, todos fallback simulado,
  `InputModeIsReal()=false`, posição final `z=60`.
- Loop completo com 10 teclas `w` reais: as 10 chegam de uma vez no
  primeiro poll (pipe entrega tudo disponível), `z+=10`; os 59 frames
  restantes sem tecla nova caem no fallback (`z+=1` cada) — total
  `z=69`, exatamente como esperado pela mecânica descrita.

**O que isso NÃO prova:** que um ser humano de fato digitou — só que o
mecanismo de leitura de stdin funciona de ponta a ponta com bytes reais
(o mesmo código que rodaria com um humano num terminal interativo).
Rodar interativamente de verdade (`./engine_fluxo` num terminal, sem
pipe) não foi possível demonstrar aqui (ambiente sandboxed, sem
terminal interativo anexado à sessão) — registrado como limitação
honesta, não escondida.

**Estado da compilação:** dois novos targets no CMake
(`test_keyboard_input`, `test_real_input_loop`). Todo o projeto
recompila e roda após a mudança, nos dois modos (com e sem stdin).

---

### [18] — Morton code + busca binária testado para o item 10: resultado NEGATIVO (honesto)
**O que foi tentado:** reduzir o fator ~5,6-6,6x de toques por ponto da
construção esparsa (v16) usando ordem Morton (Z-order): ordenar os
pontos uma única vez por código Morton permite que cada divisão de
quadrante vire busca binária O(log N) em vez de reparticionar a lista
inteira. A propriedade central foi validada antes de implementar (0
falhas em 200 casos aleatórios, `py/verifica_morton.py`): pontos dentro
de uma região alinhada a potência de 2, ordenados por Morton, formam um
intervalo contendo exatamente os 4 quadrantes como 4 sub-blocos
contiguos.

**Primeira medição (contando só buscas binárias, não comparações
internas):** parecia uma vitória clara — 5.376 vs 17.444 (L=32),
69% de redução. **Mas essa contagem não era justa**: cada busca
binária custa O(log N) comparações internas, não contadas; e o `sort`
inicial (que a v16 não precisa pagar) também não estava sendo somado.

**Remedido com contagem justa** (comparações elementares nos dois
lados, incluindo o custo do `std::sort`, `tests/test_morton.cpp`):

| L | trabalho v16 (particionamento) | trabalho Morton+sort | razão |
|---|---|---|---|
| 32 | 17.444 | 39.268 | 2,25x (PIOR) |
| 64 | 82.137 | 176.818 | 2,15x (PIOR) |

**Causa raiz:** o `sort` custa O(N log N) e é pago do zero a cada fatia
(cada fatia é uma árvore nova, construída 1x, sem consultas repetidas
que amortizariam o custo do sort). Nesse padrão de uso (build-once,
não query-many), a vantagem teórica do Morton+busca binária nunca se
paga. Teria vantagem real se a mesma fatia fosse consultada múltiplas
vezes reaproveitando a mesma ordenação — não é o caso aqui.

**Decisão:** `SpatialPartitionMorton` mantido no código (compila,
corretude confirmada — paridade de resultado com v16 em todas as
fatias testadas), mas **não adotado** no `SliceStreamer` — resultado
negativo documentado, mesmo padrão de honestidade do
`hybrid_partition.hpp` (Seção 17 do relatório). O item 10 continua
aberto: a construção esparsa simples (v16) permanece a melhor opção
medida até agora.

---

### [17] — L=64 fecha os itens 11 e 12: esparsidade maior confirma a tendencia prevista
**O que foi feito:** gerados `world_bunny_64.stream` (denso) e
`world_bunny_sparse_64.stream` (esparso) a partir do mesmo mesh do
bunny, reusando os scripts já existentes. Medido com o mesmo
instrumental da v16 (`tests/test_medida_trabalho_geral.cpp`,
generalizado para receber `L` como parâmetro em vez de fixo em 32).

**Item 11 fechado — esparsidade maior, vantagem maior (confirmado, não
só esperado por extrapolação):**

| L | ocupação | trabalho esparso/pirâmide | economia |
|---|---|---|---|
| 32 | 9,50% | 0,3994 | 60,1% |
| 64 | 4,71% | 0,2350 | 76,5% |

Paridade de corretude confirmada também em L=64 (6/6 fatias testadas
em z=0,5,20,32,45,63, `tests/test_paridade_L64.cpp`).

**Item 12 fechado — ponto de equilíbrio de bytes calculado e
verificado:** o formato denso custa $L^2/8$ bytes fixos (1 bit/célula,
independe de ocupação); o esparso custa ~4 bytes/ponto. Igualando as
duas expressões, o ponto de equilíbrio teórico é $p = 1/32 = 3{,}125\%$
de ocupação — abaixo disso o esparso deveria ficar menor em disco que
o denso. **Verificado com os dois pontos de dado real, calculados de
forma independente da fórmula:** L=32 prevê 3,09%, L=64 prevê 3,11%
— ambos batem com a previsão teórica de 3,125% dentro de margem
pequena, confirmando o modelo, não só uma extrapolação única.

**Conclusão prática:** para o bunny (9,5% e 4,71% testados, ambos acima
do ponto de equilíbrio), o formato denso continua menor em disco; o
formato esparso ganha em disco só para dados mais esparsos que ~3,1%
de ocupação — mas o ganho em TRABALHO de construção da árvore (não
espaço em disco) já compensa nos dois casos testados, e cresce com a
esparsidade.

**Estado:** só testes/medição, nenhuma mudança estrutural no
`SliceStreamer` ou nos formatos.

---

### [16] — Construção esparsa de verdade (não toca grade densa) + bug real pego e corrigido no processo
**O que foi apontado (corretamente):** a piramide da v15 era O(1) por
consulta, mas para CONSTRUIR a pirâmide ela toca TODAS as O(L²) células
da grade, inclusive as vazias — não usa a esparsidade conhecida do
bunny (9,5% ocupado). Medido: 43.680 células tocadas contra 3.112
voxels de dado real (14x de desperdício). Isso é "adivinhar por força
bruta" sobre uma grade uniforme, não subtrair de uma referência como o
`SpatialPartitionExact` da malha sintética faz.

**Correção em duas etapas, com um erro real no meio do caminho:**

1. Novo formato `WFLS` (`py/gera_world_sparse_stream.py`): lista de
   coordenadas ocupadas por fatia, nunca materializa grade densa em
   nenhum ponto do pipeline (geração, disco, leitura). Achado honesto:
   em disco, esse formato é MAIOR que o bitmap denso nesta densidade
   (12.844 vs 4.108 bytes) — espaço em disco e trabalho de construção
   são eixos diferentes, só o segundo melhora aqui.

2. `stream::SpatialPartitionSparse::Build`: primeira versão filtrava a
   lista de pontos 4 vezes por nó (uma por filho, cada uma
   re-percorrendo tudo) — **medido em 60.440 pontos examinados, PIOR
   que a pirâmide densa (43.680)**. Bug real, pego pela própria
   medição, não por inspeção de código. Corrigido para 1 única passada
   por nó, particionando em 4 baldes de uma vez: **17.444 pontos
   examinados** — 2,5x melhor que a pirâmide densa.

**Medido, não suposto** (`tests/test_medida_trabalho.cpp`): tabela
comparativa final, todas as 32 fatias do bunny (L=32):

| Versão | Trabalho medido | Relação ao dado real (3.112 pontos) |
|---|---|---|
| Pirâmide densa (v15) | 43.680 | 14,0x |
| Esparso, 1ª tentativa (bug) | 60.440 | 19,4x (pior que a densa) |
| Esparso, corrigido | 17.444 | 5,6x |

Corretude preservada em todas as versões: 6/6 fatias batem exatamente
(z=0,1,10,16,20,31), confirmado em `tests/test_sparse_engine.cpp`.

**O resto do trabalho (5,6x, não 1x) não foi eliminado:** é custo
inerente de uma árvore de partição por comparação — cada ponto é
tocado uma vez por nível de profundidade até isolar-se num nó
homogêneo, não uma vez só. Reduzir isso further exigiria estrutura
adicional (ex.: índice espacial tipo k-d tree balanceado ou Morton
code com busca binária) — registrado como possível próximo passo, não
implementado.

---

### [15] — Correção real: BitmapQuadtree de O(size²) para O(1) por nó (medido, não suposto)
**O erro apontado, e por que era real:** a v14 introduziu
`BitmapQuadtree::Build` fazendo scan célula a célula da região a cada
nó do quadtree — O(size²) por consulta, não O(1) como o
`SpatialPartitionExact` da malha sintética. Eu tinha descrito isso como
"pendência" sem implementar a correção nem medir o custo de verdade —
só supus a ordem de grandeza. Foi apontado corretamente que isso é uma
medida uniforme (mesmo custo por célula, independente da escala da
região), o oposto do princípio de tesselação em múltiplas resoluções
já registrado na Seção 19/20 do relatório — análogo a um pedreiro que
usa régua em escalas diferentes (grosseira pra planejar a fiada, fina
só pro encaixe do canto), não a mesma precisão uniforme contra a parede
inteira.

**A correção:** `stream::BitmapPyramid` — mipmap booleano (OR-pool e
AND-pool 2×2 por nível, bottom-up a partir do bitmap bruto),
pré-computado 1x por fatia em O(L²) total (série geométrica
1+1/4+1/16+...→4/3·L², não explode). Depois disso, `BuildFromPyramid`
decide homogeneidade de qualquer nó com **1 lookup direto** no nível
certo da pirâmide — O(1) de verdade.

**Medido, não suposto** (`tests/test_bitmap_pyramid.cpp`,
`py/verifica_piramide_bitmap.py`, com contadores reais de leitura/lookup
instrumentados no código, não em comentário):
- scan antigo: 1024 leituras de bit pra um nó de size=32, 16 leituras
  pra um nó de size=4 — escala exatamente com size², como previsto.
- pirâmide nova: **1 lookup** para size=32 e **1 lookup** para size=4 —
  custo idêntico, confirmando O(1) por medição direta, não inferência.
- Paridade de resultado: 6/6 fatias do bunny (z=0,1,10,16,20,31) dão
  exatamente a mesma contagem de nós nas duas versões.

**`SliceStreamer` atualizado** para usar `BuildFromPyramid` por padrão;
`Build` (scan) mantido só para o teste de regressão comparar contra a
nova versão.

**Estado da compilação:** novo target `test_bitmap_pyramid` no CMake.
Todo o projeto recompila e roda após a mudança; `test_bunny_engine`
revalidado (continua batendo com o gabarito Python original).

---

### [14] — Bunny real chega até a engine C++ (fecha item 6 das pendências)
**O que foi feito:**
- Novo formato de asset `WFLB` (bitmap real por fatia), complementar ao
  `WFLX` (parâmetros algébricos): `py/gera_world_bunny_stream.py` grava
  cada fatia z do Stanford Bunny voxelizado (v9) como um bitmap L×L
  empacotado, não mais uma fórmula.
- `stream::BitmapQuadtree` (novo, em `spatial_partition.hpp`): quadtree
  com teste any/all **exato** sobre o bitmap — não amostragem por
  cantos+centro, porque já provamos (v9) que ela falha catastroficamente
  em dado real esparso (colapsava para 1 nó).
- `SliceStreamer` estendido: detecta `WFLX` vs `WFLB` pelo magic do
  header e usa `SpatialPartitionExact` ou `BitmapQuadtree`
  respectivamente — sem quebrar o formato antigo (`world.stream`
  sintético continua funcionando, testado).

**Validado:** paridade total C++/Python — 6/6 fatias testadas (z=0, 1,
10, 16, 20, 31) batem exatamente entre um gabarito calculado em Python
e o resultado do `SliceStreamer` em C++ (`tests/test_bunny_engine.cpp`),
incluindo o caso z=31 (fatia vazia, 1 nó — homogênea de verdade, não
falso positivo).

**Fecha a lacuna registrada no README v12, item 6:** a engine C++ agora
lê dado 3D real (não sintético) de ponta a ponta — disco → bitmap →
quadtree exato → contagem de nós —, não só o Python standalone da v9.

**Estado da compilação:** novo target `test_bunny_engine` no CMake.
`world.stream` (sintético) e `world_bunny_32.stream` (real) coexistem e
funcionam com o mesmo `SliceStreamer`.

---

### [13] — HardwareProfiler mede RAM real (não mais hardcoded)
**O que foi feito:** `HardwareProfiler::Profile()` tinha valores fixos
(VRAM=8GB, PCIe=16Gbps) desde a v1, nunca corrigido — pendência listada
explicitamente pela primeira vez na v12. Implementado: leitura real de
`/proc/meminfo` (Linux), extraindo `MemTotal`/`MemAvailable` em bytes.
`vram_bytes` agora é RAM real do sistema usada como **proxy documentado**
(não VRAM de verdade — sem GPU/display neste ambiente, ver v8/v12).
`max_window_slices` (m) é calculado a partir de 5% da RAM disponível
real, dividido por uma estimativa de 64KB/fatia, arredondado para baixo
até potência de 2 (exigência do `WindowController`), com teto [16,1024].

**O que continua hardcoded, declarado explicitamente:**
`pcie_bandwidth_gbps` — não há como medir throughput de barramento sem
acesso a hardware de GPU (Vulkan/DX12 indisponíveis aqui). Não escondido,
documentado no código com comentário.

**`ReadProcMeminfo` tornado público** (com path parametrizável,
default `/proc/meminfo`) especificamente para permitir testar o
caminho de fallback de verdade, não apenas assumir que funciona.

**Validado (`tests/test_hardware_profiler.cpp`):**
- Caminho normal: mede 4.192.006.144 bytes (4,19 GB) de RAM total real
  desta máquina — bem diferente do placeholder de 8GB antigo.
  `m=1024` (teto de segurança atingido; orçamento real chegaria a
  ~3024, limitado a 1024 por precaução).
- Fallback com path inexistente: retorna `false` corretamente, sem
  travar.
- Fallback com arquivo malformado (existe mas não é `/proc/meminfo`
  válido): retorna `false` corretamente.
- `m` é potência de 2 e está no intervalo esperado; RAM medida é
  plausível (>0, <1TB); `measured_ram_free <= measured_ram_total`.

**Estado da compilação:** `test_hardware_profiler` novo no CMake. Todo
o projeto recompila e roda após a mudança de visibilidade de
`ReadProcMeminfo` (privado → público).

---

### [12] — Correção: 30→60 frames (contagem de iterações, não FPS)
**O que foi apontado:** `max_frames_` estava fixo em 30, número que
carrega a conotação de "30 FPS" (padrão histórico de console com
hardware limitado) mesmo não sendo uma taxa de quadros — o
`ProjectivePipeline` não tem clock, vsync, delta-time nem sleep; é só
uma contagem de iterações do loop de teste, não um controle de tempo
real. Trocado para 60 (convenção de mercado fora de consoles
restritos), com o comentário no código deixando explícita a distinção
entre "quantidade de frames simulados" e "taxa de quadros real" — para
não sugerir uma característica de performance que não existe aqui.

**O que foi atualizado:** `ProjectivePipeline` (`max_frames_=60`,
salva PPM a cada 15 frames em vez de 10, pra manter ~4-5 amostras
proporcionalmente), `tests/test_render_loop.cpp` (asserts para 60),
frames de exemplo regenerados (`frame_001.png`, `frame_060.png`).

**Validado:** 60/60 frames rodados, 31 valores distintos de node_count
ao longo do voo (mais que os 16 vistos em 30 frames, consistente com
percurso maior em z).

---

### [11] — Fast doubling de repunits acrescentado à Seção 20.1
**O que foi feito:** a v10 tinha incluído só a recorrência linear de
Newton para repunits (Seção 20.1); faltava a parte que o usuário
apontou como ausente — a combinação com fast doubling (Seção 23 do
documento original), que reduz o custo de $O(n)$ para $O(\log n)$
multiplicações, ainda sem divisão. Acrescentado `py/verifica_fast_doubling_repunit.py`
e a Seção 20.1 do relatório atualizada com a identidade, a tabela
comparativa dos 3 métodos, e a verificação.

**Verificado:** 0 falhas em 200 casos (4 bases × 50 valores de n);
para n=1000, 28 multiplicações reais contadas (contra ~19,9 teórico —
mesma ordem de grandeza) vs. 1000 passos da recorrência simples.

---

### [10] — Duas pontes com "Repunits, Séries Geradoras, Tensores e Tesselação"
**O que foi feito:**
- Avaliado documento externo de 892 linhas (`repunits-tensores-tesselacao.md`)
  — majoritariamente outra linha de pesquisa (Wang tiles, tetraedro de
  Pascal, Galois da quíntica, LFSR/de Bruijn), sem relação com o
  semiprimo. Não incorporado em massa — só duas partes com conexão real
  e específica, adicionadas como Seção 20 do relatório:
  - **20.1** — recorrência de Newton para repunits em qualquer base,
    $R_n=(b+1)R_{n-1}-bR_{n-2}$, sem divisão (estende a Seção 7 já
    existente sobre repunits/máscara). Verificado nesta consolidação:
    bate contra cálculo direto em 4 bases, e funciona corretamente em
    aritmética modular onde a fórmula fechada falha (testado $b=7$,
    módulo 6, onde $b-1=6$ não é invertível mod 6).
  - **20.2** — tesselação por resíduo mod $m$ como teoria geral por trás
    do achado empírico da Seção 9.1 (viés de alinhamento binário do
    quadtree): o quadtree é tesselação com $m=2$, e $k/L$ alinhado a
    potências de 2 é o que determina se a fronteira cai sobre uma linha
    de corte ou força subdivisão extra.

**Bug corrigido no processo:** a primeira tentativa de inserir a Seção
20 no `.md` colocou o texto **antes** da Seção 19 por engano (a âncora
de texto usada no `str_replace` também existia mais cedo no documento).
Corrigido reordenando o arquivo antes de gerar o PDF final.

**Estado:** só documentação/relatório, nenhuma mudança de código nesta
entrada.

---

### [9] — Dado de ocupação real (Stanford Bunny), última pendência resolvida
**O que foi feito:**
- Baixado o mesh real do Stanford Bunny (35.947 vértices, 69.451
  triângulos, escaneado a laser em 1994 pelo Stanford Computer Graphics
  Laboratory) via
  `github.com/alecjacobson/common-3d-test-models` — dado real, não
  sintético/algébrico, resolvendo a última pendência da lista.
- `py/voxeliza_bunny_real.py`: voxeliza a superfície do mesh (rasterização
  de triângulo por interpolação baricêntrica) numa grade 32³/64³, e roda
  o mesmo octree já usado para esfera/terreno sintéticos, para comparação
  direta (mesmo algoritmo, dado diferente).

**Achado crítico não previsto:** a heurística de amostragem por
cantos+centro (usada em `octree3d.py` para os dados sintéticos)
**colapsa para 1 nó** no dado real — falso positivo total. Causa: o
bunny é uma casca fina e esparsa (9,5% ocupado em L=32); os 4 cantos +
centro da grade inteira caem quase sempre em espaço vazio, então a
heurística declara "tudo vazio" e nunca subdivide, apesar de existir
uma superfície real dentro. **Corrigido** com um teste de homogeneidade
exato sobre grade densa (`numpy.any()/all()` por região, não amostragem)
— viável nesse tamanho de grade (32³/64³ células cabem em memória).

**Resultado real (com o teste exato, comparação justa):**
- L=32: bunny real = 7.745 nós (razão 0,236) vs. esfera sintética =
  3.641 nós (razão 0,111) — **dado real comprime pior** que o sintético.
- L=64: bunny real = 33.473 nós (razão 0,128) vs. esfera sintética =
  14.377 nós (razão 0,055) — mesmo padrão.

**Interpretação:** a superfície real de um objeto escaneado é irregular
e não alinhada aos eixos, ao contrário da esfera analítica suave usada
como sintético — isso é evidência de que os números de compressão da
Seção 12 (razão nós/L² para dados sintéticos) são **otimistas**
comparados a dados reais de superfície. Atualiza a Seção 12 do
relatório com essa ressalva.

**Estado:** script Python standalone, não integrado à engine C++ (é uma
medição de validação, não um módulo de produção). Todos os itens da
lista de pendências original (README v6) agora estão resolvidos.

---

### [8] — ProjectivePipeline real: framebuffer por software, loop de múltiplos frames
**O que foi feito:**
- `render::Framebuffer`: buffer RGB real em memória (`std::vector<uint8_t>`),
  com `Clear`, `FillRect`, `SavePPM` — sem dependência gráfica externa
  (sem display no container, Vulkan/DX12 não têm onde rodar aqui).
- `ProjectivePipeline::Draw` agora rasteriza de verdade: projeta as
  dimensões recuperadas por `MetricRecovery` (Seção 16) de cada slice
  da janela ativa em retângulos no framebuffer, deslocados pela posição
  da câmera.
- `Present()` grava PPM em disco a cada 10 frames — I/O de saída real,
  testável (`/tmp/frame_NNN.ppm`).
- `PollCameraInput()`: **simulado** (voo automático em z, incrementando
  1 unidade por frame) — não é input de teclado/mouse real, documentado
  explicitamente no código e aqui, para não confundir com hardware real.
- `ShouldClose()`: agora `frame_count_ >= max_frames_` (30), não mais
  `true` fixo — o loop **roda de verdade por múltiplos frames** antes de
  fechar, em vez de fechar no frame 1 por design.
- `tests/test_render_loop.cpp`: valida numericamente (não só
  visualmente) que 30 frames rodam, que a contagem de nós do quadtree
  varia entre frames (prova de leitura real de fatias diferentes do
  disco) e que a câmera simulada avança de fato.

**Validado:** 30/30 frames rodados, 16 valores distintos de `node_count`
vistos ao longo do voo em z (de 30 frames — não é 1 valor fixo repetido),
`z` final = 30.0. Frames 1 e 30 exportados como PNG para inspeção visual
(`frame_001.png`, `frame_030.png` em `outputs/`) — cor muda entre eles
porque o ring buffer se preenche com fatias diferentes ao longo do voo.

**Limitação documentada, não escondida:** isto é rasterização por
software (CPU, RAM), não uma GPU real via Vulkan/DX12 — o container não
tem display nem driver de GPU acessível. É uma implementação real e
testável do papel estrutural do módulo (fechar o loop, produzir frames,
terminar), não uma simulação de API gráfica.

**Estado da compilação:** `test_render_loop` novo no CMake. Todos os
binários compilam e rodam a partir de `/home/claude` (path relativo do
asset).

---

### [7] — Registro de trapézios/tesselação incorporado ao relatório (Seção 19)
**O que foi feito:**
- `py/verifica_tesselacoes.py`: verificação independente de tesselações
  por dois métodos — (1) fechamento angular exato (só triângulo, quadrado
  e hexágono regular tesselam sozinhos; testadas as 11 configurações
  semi-regulares clássicas) e (2) rasterização independente (malha gerada
  por cisalhamento, testada contra lista de triângulos gerada à parte,
  não circular).
- Seção 19 nova no relatório consolidado: incorpora os Passos 1–7 do
  "Registro de Progresso" (decomposição de ímpares, números triangulares,
  gnômon, tesselação por losangos, emergência de forma por coloração,
  escala log-linear), filtrando o que é matemática fechada do que
  permanece em aberto — o próprio registro já havia se autocorrigido no
  ponto mais delicado (Rota B: não é ponto de fuga projetivo, é relação
  exponencial que vira linear em escala log).

**Nota de honestidade registrada no relatório:** a primeira tentativa de
rasterizar a malha triangular para verificar a tesselação tinha um bug
real (74,5% de cobertura, não 100%); a correção seguinte virou um teste
vazio que sempre "passava" sem testar nada; só a terceira tentativa
(cisalhamento padrão + lista de triângulos independente) produziu
resultado confiável (0% lacuna, 99,3% cobertura simples, 0,67% de
sobreposição esperada em arestas compartilhadas). Isso está documentado
na Seção 19.3 do relatório, não escondido.

**Estado da compilação:** não afeta a engine C++ (mudança é só
matemática/relatório); `py/verifica_tesselacoes.py` roda standalone.

---

### [6] — SliceStreamer lê disco de verdade (I/O real via seek)
**O que foi feito:**
- Definido um formato binário real, `assets/world.stream`: header
  (`"WFLX"` + `L` + `num_slices`) seguido de `(k,c)` por fatia — réplica
  em disco do streaming 3D da Seção 5/12 (bloco central encolhendo ao
  longo de z, como um cone extrudado).
- `py/gera_world_stream.py`: gera e verifica o asset.
- `SliceStreamer` reescrito: abre com `std::ifstream`, lê o header uma vez
  na construção, e `PullSlice` faz **seek direto** (não varredura linear)
  para a fatia correspondente ao `z` da câmera — custo O(1) por fatia,
  igual ring buffer.
- Fallback: se o arquivo não existir/for inválido, `IsOpen()` retorna
  `false` e a engine continua rodando com valores fixos (não trava),
  documentado como comportamento explícito, não silencioso.
- `tests/test_slice_streamer.cpp`: testa seeks fora de ordem
  (0→1→84→85→86→150→255→0→255→40), clamp de bordas (z negativo, z além
  do fim) e arquivo inexistente.

**Validado:** `z=0` (k=85) produz 1673 nós — bate exatamente com a
Tabela 1 do relatório (L=256, k=85 → 1673). Seeks fora de ordem
confirmam acesso aleatório real, não leitura sequencial disfarçada.

**Estado da compilação:** `engine_fluxo` e `test_slice_streamer` (novo
target no CMake) compilam e rodam a partir de `/home/claude` (onde
`assets/world.stream` está relativo ao path usado em `main.cpp`).

---

### [5] — Teste de homogeneidade geométrico exato (Seção 9/17) implementado
**O que foi feito:**
- `py/homogeneidade_exata.py`: teste de homogeneidade por interseção
  analítica reta-retângulo (hipotenusa + bordas do bloco), substituindo a
  amostragem por cantos+centro. O(1) por nó, nunca produz falso positivo.
- Portado para C++: `stream::SpatialPartitionExact::Build` em
  `spatial_partition.hpp`.
- `SliceStreamer::PullSlice` atualizado para usar a versão exata.

**Achado importante (contraria a Seção 9 original):** o teste exato produz
**exatamente os mesmos números de nós** que a heurística por amostragem,
em todos os casos testados — Tabela 1 inteira (L=16 a 4096) e Tabela 3
inteira (k/L de 1/6 a 5/8), diferença zero em contagem de nós. Isso muda
a conclusão: a sensibilidade de nós/L ao alinhamento de k/L **não vem da
amostragem ser imprecisa** (hipótese original da Seção 9) — é uma
propriedade geométrica real de como a posição do bloco interage com a
grade de subdivisão binária do quadtree, presente mesmo com um teste de
homogeneidade perfeito. O relatório (`relatorio_consolidado.md/.pdf`) foi
corrigido com uma nova Seção 9.1 registrando isso.

**Validado:** 0 falhas de reconstrução em 3000 amostras aleatórias
(múltiplos L,k), tanto em Python quanto em C++; paridade C++/Python
confirmada.

**Estado da compilação:** projeto inteiro recompila e roda após a mudança.

---

### [4] — Consolidação após perda parcial de contexto + FastMath centralizado
**O que foi feito:**
- Recuperados e revalidados os 11 scripts Python que haviam sido gerados mas
  nunca salvos em `outputs` (perda de contexto entre turnos): `constante_teorica.py`,
  `experimento.py`, `formula_hexagono.py`, `merge_grafo.py`, `octree3d.py`,
  `render_bench2.py`, `tabela_final.py`, `testa_mascara.py`, `transitivo.py`,
  `valida_alinhamento.py`, `valida_formula.py`, `verifica_escala-1.py`.
- Todos os 15 scripts foram executados de novo neste ambiente; os números
  batem exatamente com o `relatorio_consolidado.pdf` (Tabelas 1-6).
- Criado `core/fast_math.hpp`: antes `TruncaPorShift` existia solto dentro
  de `union_find.hpp`; agora é um módulo próprio com as duas técnicas da
  Seção 7 (`TruncaPorMascara`, `TruncaPorShift`), documentando explicitamente
  que não são intercambiáveis.
- `window_controller.cpp` e `union_find.hpp` atualizados para delegar a
  `core::FastMath` em vez de reimplementar o bit a bit localmente.
- Confirmado por execução direta (não só por comentário no código) o
  resultado negativo do `hybrid_partition.hpp` já registrado: para
  L=256, k=85, o esquema híbrido usa 2010 nós contra 1673 do quadtree puro
  — pior, não melhor, porque a bounding box do componente "T1+T2" degenera
  para quase o tamanho da malha inteira.

**Estado da compilação:** todos os `.cpp` compilam individualmente
(`g++ -std=c++20`), o binário linka e roda (`main.cpp` executa 1 frame e
fecha — `ShouldClose()` ainda é stub `true`, ver pendências).

**Pendente:** nenhum item novo; ver seção "Pendências" abaixo, que continua
igual à v3.

---

### [3] — Recuperação Geométrico-Métrica (Seção 16) integrada ao BoundingReference
**O que foi feito:**
- `render/metric_recovery.hpp`: implementa os 4 passos da Seção 16
  (altura relativa à diagonal, quadrado composto, semiperímetro por
  partição, perímetro), evitando a equação quadrática explícita.
- `bounding_reference.cpp` passou a extrair área/diagonal de cada `Slice`
  (via `width`/`height`, adicionados à struct `Slice`) e aplicar
  `RecoverFromAreaAndDiagonal`.
- Validado em Python (2000 casos aleatórios, 1 falha por ruído de ponto
  flutuante em caso degenerado a=b) e replicado em C++ isolado (0 falhas
  em 2000 casos, tolerância 1e-3). Bate o exemplo do relatório (A=60,
  D=13 → a=12, b=5, P=34).

---

### [2] — Grafo radial dos módulos (visualização)
**O que foi feito:**
- `docs/module_graph.dot`: grafo hierárquico (dependência de include +
  fluxo de runtime), renderizado com `dot`.
- `docs/module_graph_radial.dot`: grafo radial (`twopi`) com núcleo
  central (`Render Loop`) e módulos como vértices de um polígono — cada
  aresta nova a partir do núcleo é um lado a mais do polígono.
- Ambos exportados em `.dot` (fonte editável), `.svg` e `.png`.

---

### [1] — Esqueleto inicial do Render Loop
**O que foi feito:**
- Estrutura de pastas `src/{core,stream,render}` + `main.cpp`.
- Módulos stub: `HardwareProfiler`, `WindowController` (ring buffer real,
  índice via resíduo bit a bit), `SliceStreamer`, `BoundingReference`,
  `ProjectivePipeline`.
- `main.cpp` implementa o loop na ordem exata das arestas do grafo
  (`HardwareProfiler` 1x → por frame: `SliceStreamer → WindowController →
  BoundingReference → ProjectivePipeline → Present`).
- Confirmado compilando e rodando com `g++ -std=c++20`.

---

## Pendências (atualizado na v25)

1. ~~**Teste de homogeneidade geométrico exato**~~ — ✅ resolvido na v5.
2. ~~**`ProjectivePipeline` sem API gráfica real**~~ — ✅ resolvido
   parcialmente na v8/v12: framebuffer real por software (não GPU), loop
   roda 60 frames de verdade, `PollCameraInput` é voo simulado (não
   input real de teclado/mouse). Uma API gráfica real (Vulkan/DX12)
   continua fora de escopo sem display/GPU disponível neste ambiente.
3. ~~**`SliceStreamer::PullSlice` não lê disco de verdade**~~ — ✅
   resolvido na v6.
4. ~~**Dados de ocupação esparsa reais**~~ — ✅ resolvido na v9 (Python)
   e v14 (engine C++, ver abaixo).
5. ~~**Input de câmera real (teclado/mouse)**~~ — ✅ resolvido
   parcialmente na v19: `KeyboardInput` lê teclado real via terminal
   (termios raw mode), validado com bytes reais via pipe (não travou,
   e os números batem exato: `x=2.0,z=3.0` para `"wwwddq"`). Mouse/GUI
   continua fora de escopo (sem display real neste ambiente). Não
   demonstrado com um humano digitando interativamente (ambiente
   sandboxed sem terminal anexado) — só o mecanismo de leitura, que é
   o mesmo código que rodaria com input humano real.
6. ~~**`world.stream` real a partir do bunny voxelizado, lido pela
   engine C++**~~ — ✅ resolvido na v14: formato `WFLB`, `BitmapQuadtree`,
   paridade C++/Python confirmada em 6/6 fatias testadas.
7. ~~**O achado da Seção 20.2** (m-tree alinhado a k/L)~~ — ✅ testado
   na v20: confirmação PARCIAL. `m=3` bate o quadtree para `k/L=1/3`
   especificamente (4,49 vs 6,54 nós/L), mas não generaliza — `m=5,6,7`
   com suas frações naturais correspondentes todos perderam pro
   quadtree. Não portado para C++ (exploratório, escopo de quando
   compensa ainda não caracterizado).
8. ~~**`HardwareProfiler::Profile()` com valores fixos hardcoded**~~ —
   ✅ resolvido na v13: mede RAM real via `/proc/meminfo`, usada como
   proxy documentado de VRAM (sem GPU neste ambiente). `pcie_bandwidth_gbps`
   continua hardcoded, declarado explicitamente (sem forma de medir sem
   hardware de GPU).
9. ~~**`BitmapQuadtree::Build` é O(size²) por nó**~~ — ✅ resolvido na
   v15/v16: primeiro por pirâmide O(1)/consulta (v15), depois substituído
   por construção esparsa (v16) que nem toca a grade densa — 17.444
   pontos examinados contra 43.680 células tocadas pela pirâmide (2,5x
   melhor), medido diretamente.
10. ~~**A construção esparsa examina cada ponto ~5,6-6,6x**~~ — parcialmente
    resolvido na v25: a via "reaproveitar entre frames" registrada aqui
    partia de premissa errada (não há reconstrução redundante entre
    frames a evitar — cada fatia é construída 1x e descartada). O que
    de fato se aplica (custo de alocação, não de toques) foi resolvido:
    `BuildInPlace` elimina a alocação de 4 vetores por nó, medido 36,8%
    mais rápido em tempo de parede (não só teórico). **O fator ~5,6-6,6x
    de toques por ponto em si continua igual** — é inerente à
    profundidade da árvore, reduzir isso exigiria estrutura
    fundamentalmente diferente. Morton+sort (v18) continua descartado
    (resultado negativo, não muda).
11. ~~**Testar em L=64 (mais esparso)**~~ — ✅ resolvido na v17: vantagem
    do esparso sobre a pirâmide cresceu de 60,1% (L=32) para 76,5%
    (L=64), confirmando a tendência esperada, não só por suposição.
12. ~~**Ponto de equilíbrio esparso vs denso em bytes**~~ — ✅ resolvido
    na v17: calculado analiticamente (~3,125% de ocupação) e confirmado
    com dois pontos de dado real independentes (3,09% e 3,11%).
13. **Portar `gera_arvore_organica` e `multiplica_por_reflexoes` para C++**
    (Seção 22.2/22.5) — registrado na v28, ainda pendente: só existem em
    `geometria_engine.py` hoje, sem consumidor real em `src/` (nenhum
    sistema de vegetação/ladrilho procedural na engine ainda).

