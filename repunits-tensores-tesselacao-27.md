# Repunits, Séries Geradoras, Tensores e Tesselação

Um registro da conexão explorada, do caso mais simples (série geométrica) até o padrão geométrico que ela revela quando elevada ao quadrado.

---

## 0. Base algébrica: por que paridade determina o ponto médio inteiro

Antes de qualquer generalização geométrica, vale provar o fato mais elementar que sustenta a Seção 8: por que dois números de mesma paridade sempre somam um número par.

**Prova (por casos, direto da definição de paridade):**

- **$a$ par e $b$ par:** $a=2k$, $b=2m$ para algum $k,m \in \mathbb{Z}$. Então $a+b = 2(k+m)$, que é par, e $\dfrac{a+b}{2} = k+m$ é inteiro.
- **$a$ ímpar e $b$ ímpar:** $a=2k+1$, $b=2m+1$. Então $a+b = 2(k+m+1)$, que é par, e $\dfrac{a+b}{2} = k+m+1$ é inteiro.
- **Paridades diferentes** (um par, outro ímpar): $a=2k$, $b=2m+1$. Então $a+b = 2(k+m)+1$, que é ímpar, e $\dfrac{a+b}{2}$ não é inteiro.

**Teorema:** $a+b$ é par $\iff$ $a$ e $b$ têm a mesma paridade.

Verificado exaustivamente por computador para todos os pares $(a,b)$ com $a,b \in \{-6,\dots,6\}$ (169 combinações) — nenhuma exceção, como esperado de uma prova algébrica fechada (não é um padrão empírico, é necessidade lógica).

Esse fato — aplicado **independentemente a cada coordenada** — é exatamente o que gera as $2^d$ classes de paridade da Seção 8: cada eixo contribui um "par" ou "ímpar" de forma autônoma, e a combinação de $d$ escolhas independentes é o que tesselam $\mathbb{Z}^d$ em $2^d$ ladrilhos.

---

### 0.1 Caso gerador mínimo: p = 2n±1 e o papel de complemento

O menor espaçamento não trivial dentro da classe ímpar já basta para reconstruir a classe par inteira via ponto médio.

**Proposição:** para todo $n\in\mathbb{Z}$, o ponto médio de $2n-1$ e $2n+1$ é exatamente $2n$.

**Prova:** $\frac{(2n-1)+(2n+1)}{2}=\frac{4n}{2}=2n$. $\blacksquare$

Verificado para $n=1,\dots,7$: em cada caso, o ponto médio dos dois ímpares vizinhos é exatamente o par entre eles. Isso mostra algo que a Seção 8 não tornou explícito: as classes par e ímpar não são apenas disjuntas — a classe ímpar, tomada em pares consecutivos (espaçamento mínimo 2), **gera** a classe par inteira por ponto médio.

**A rede deslocada por meio período.** Escrever $2n-1=2\left(n-\tfrac12\right)$ revela a razão estrutural: pares e ímpares são a **mesma rede** $2\mathbb{Z}$, apenas deslocada por meio período antes de escalar — pares $=2\cdot\mathbb{Z}$, ímpares $=2\cdot\left(\mathbb{Z}+\tfrac12\right)$. Verificado computacionalmente: a união das duas cópias cobre $\{0,\dots,10\}$ exatamente uma vez cada elemento, sem sobra nem buraco.

**Princípio geral do encaixe sem falha.** Generalizando: $m$ classes tesselam $\mathbb{Z}$ sem sobreposição nem buraco se e somente se são deslocamentos de uma mesma rede $m\mathbb{Z}$ por frações exatas do período — $0,\frac1m,\frac2m,\dots,\frac{m-1}{m}$, igualmente espaçadas. Isso é, por definição, um **sistema completo de resíduos mod $m$** (a mesma estrutura da Seção 9). Qualquer deslocamento que não seja um múltiplo exato de $\frac1m$ do período produz falha (sobreposição ou lacuna) — não há alternativa que funcione "quase tão bem"; a divisão em frações exatas do período é a única forma de encaixe perfeito.

### 0.2 Por que primos "tocam" a tesselação sem pertencer a ela: o filtro mod 6 dos primos gêmeos

A pergunta natural é se $p$ ser primo altera a tesselação por paridade. A resposta é não, diretamente: o argumento do ponto médio vale para qualquer par de ímpares, primos ou não — primalidade é uma propriedade multiplicativa, a tesselação da Seção 0 é aditiva/modular, e uma não implica a outra.

Mas exigir que **ambos** os vizinhos $2n-1$ e $2n+1$ sejam primos (primos gêmeos) revela uma tesselação mais fina agindo como filtro prévio. Verificado computacionalmente para todos os pares de primos gêmeos até 200: com exceção do caso pequeno $(3,5)$, o ponto médio $n$ satisfaz sempre $n\equiv 0\pmod 6$.

**Prova do filtro:** para $n>4$, se $n\not\equiv 0\pmod 6$, então $n\bmod 6\in\{1,2,3,4,5\}$, e em cada um desses casos verifica-se que $2n-1$ ou $2n+1$ é múltiplo de 3 (e maior que 3), logo composto. Só quando $n\equiv 0\pmod 6$ nenhum dos dois vizinhos é automaticamente descartado por divisibilidade por 3.

**Conclusão sobre a relação entre primos e tesselação:** a tesselação mod 6 não "sabe" quais candidatos são primos — ela apenas elimina de antemão as classes onde a primalidade é impossível por divisibilidade forçada por 3. Primalidade age como uma peneira multiplicativa aplicada *depois*, dentro da classe residual já filtrada pela tesselação. A tesselação organiza o espaço de busca; não gera primos.

### 0.3 A base lógica ainda mais elementar: o princípio da casa dos pombos

As Seções 8, 9 e 11 usaram repetidamente o princípio da casa dos pombos (mais pontos que classes força repetição) sem nunca prová-lo — só aplicado a casos específicos. Ele é logicamente anterior a tudo isso: não depende de números, geometria ou paridade, só da definição de função injetora.

**Teorema:** se $n+1$ objetos são distribuídos em $n$ casas, pelo menos uma casa recebe 2 ou mais objetos.

**Prova (por contradição):** suponha que existe uma função $f:\{1,\dots,n+1\}\to\{1,\dots,n\}$ injetora. Como $f$ é injetora, sua imagem tem exatamente $n+1$ elementos distintos. Mas a imagem está contida no contradomínio $\{1,\dots,n\}$, que só tem $n$ elementos — um conjunto de $n+1$ elementos não pode caber dentro de um conjunto de $n$ elementos. Contradição. Logo $f$ não pode ser injetora. $\blacksquare$

**Verificação exaustiva:** testadas **todas** as funções possíveis de $n+1$ objetos em $n$ casas, para $n=1,2,3,4$ (até 1024 funções no caso $n=4$) — 0 funções injetoras em todos os casos, confirmando a prova sem depender de amostragem.

Essa é a raiz lógica, não escrita explicitamente até agora, por trás de: Seção 8 (5 pontos, 4 classes de paridade → repetição garantida), Seção 9 (generalização para $m^d+1$ pontos e $m^d$ classes), e Seção 11 (a grade par×ímpar do crivo força colisão de coluna). Nenhum desses resultados depende de mais nada além deste teorema — tudo o que eles acrescentam é a contagem específica de quantas "casas" existem em cada contexto.

---

## 1. Repunits como série geométrica truncada

Um repunit $R_n$ é o número formado por $n$ dígitos 1 (1, 11, 111, 1111, ...).

$$R_n = \frac{10^n - 1}{9} = \sum_{k=0}^{n-1} 10^k$$

Verificação para $n=4$: $1 + 10 + 100 + 1000 = 1111$ ✓

**Interpretação:** cada potência de 10 é um "ladrilho" que cabe exatamente 10 vezes dentro do próximo — a soma é uma tesselação da reta numérica em escalas geométricas, sem sobra e sem buraco. Todo sistema posicional de numeração é, nesse sentido, uma tesselação 1D.

---

## 2. Função geradora dos repunits

$$f(x) = \sum_{n=1}^{\infty} R_n\, x^n = \frac{x}{(1-x)(1-10x)}$$

Confirmado por expansão em série (SymPy):

$$f(x) = x + 11x^2 + 111x^3 + 1111x^4 + 11111x^5 + O(x^6)$$

Válida para $|x| < 1/10$ (raio de convergência).

---

## 3. Tensores: escalar como caso trivial

Um tensor de ordem $k$ é um objeto com $k$ índices que se transforma de forma específica sob mudança de base. Um **escalar é um tensor de ordem 0** — invariante por definição, não um objeto "à parte" dos tensores.

Hierarquia: escalar (ordem 0) → vetor (ordem 1) → matriz/tensor (ordem 2) → ...

Formalmente, vivem em $V^{\otimes p} \otimes (V^*)^{\otimes q}$.

**Analogia com séries:** essa hierarquia é estruturalmente parecida com os coeficientes de uma expansão de Taylor multivariada — valor (escalar), gradiente (vetor), Hessiana (tensor de ordem 2) — cada ordem guardando mais informação, e a série sendo truncada conforme a precisão necessária.

---

## 4. A ponte com tesselação: base = produto de Kronecker

Uma base é o que permite "cobrir" um espaço sem lacunas nem redundância — exatamente a definição de tesselação.

Testado computacionalmente: o produto de Kronecker das bases canônicas de $\mathbb{R}^1$ (eixo x) e $\mathbb{R}^1$ (eixo y) reproduz exatamente a base canônica de $\mathbb{R}^2$:

```
e_x ⊗ e_x = [1,0,0,0]
e_x ⊗ e_y = [0,1,0,0]
e_y ⊗ e_x = [0,0,1,0]
e_y ⊗ e_y = [0,0,0,1]
```

Ou seja: **a grade 2D (tesselação em quadrados) nasce do produto tensorial de duas retas 1D.**

---

## 5. O resultado central: R_n² como tesselação 2D

Fazendo o produto tensorial da série geométrica 1D consigo mesma, cada célula $(i,j)$ de uma grade recebe o peso $10^i \cdot 10^j$:

```
[1,     10,    100  ]
[10,    100,   1000 ]
[100,   1000,  10000]
```

Somando todos os ladrilhos da grade $3\times3$: **12321**

Comparando: $R_3^2 = 111^2 = 12321$ ✓ (verificado computacionalmente)

### Por que isso funciona

$$R_n^2 = \left(\sum_i 10^i\right)\left(\sum_j 10^j\right) = \sum_{i,j} 10^i \cdot 10^j$$

que é exatamente a soma de todos os ladrilhos da grade tensorial 2D. O padrão palindrômico de $R_n^2$ (1,2,3,...,3,2,1 nos dígitos) é a assinatura da convolução de $(1,1,1,\dots)$ consigo mesma — o mesmo mecanismo por trás da multiplicação de polinômios com coeficientes unitários.

---

## 6. Demonstração formal: generalização para $R_n^p$ (tensor de ordem $p$)

O padrão observado para $R_n^2$ não é coincidência — é uma consequência direta da propriedade distributiva da multiplicação, e se generaliza para qualquer potência.

### Proposição

Para qualquer $n \geq 1$ e $p \geq 1$:
$$R_n^p = \sum_{i_1=0}^{n-1} \sum_{i_2=0}^{n-1} \cdots \sum_{i_p=0}^{n-1} 10^{i_1} \cdot 10^{i_2} \cdots 10^{i_p}$$

isto é, $R_n^p$ é exatamente a soma de todos os pesos de uma **grade tensorial $p$-dimensional** de lado $n$, onde a célula $(i_1,\dots,i_p)$ vale $10^{i_1+i_2+\cdots+i_p}$.

### Demonstração

Por definição, $R_n = \sum_{i=0}^{n-1} 10^i$. Elevando à potência $p$ e usando a propriedade distributiva (produto de $p$ somas é a soma de todos os produtos possíveis, um termo de cada fator):

$$R_n^p = \left(\sum_{i_1=0}^{n-1} 10^{i_1}\right)\left(\sum_{i_2=0}^{n-1} 10^{i_2}\right)\cdots\left(\sum_{i_p=0}^{n-1} 10^{i_p}\right) = \sum_{i_1,\dots,i_p=0}^{n-1} \prod_{r=1}^{p} 10^{i_r}$$

Cada termo do somatório múltiplo é exatamente o peso de uma célula da grade $p$-dimensional — isso é, por construção, a definição de um **tensor de ordem $p$** obtido pelo produto tensorial (Kronecker) de $p$ cópias da sequência geométrica 1D $(10^0, 10^1, \dots, 10^{n-1})$. $\blacksquare$

Essa é a prova algébrica geral: não depende de $p=2$ ou $p=3$ especificamente, vale para qualquer ordem.

### Verificação computacional para $p=3$ (caso $n=3$)

Construí a grade tensorial de ordem 3 (cubo $3\times3\times3$), onde cada célula $(i,j,k)$ vale $10^i \cdot 10^j \cdot 10^k$, e somei todas as 27 células:

```
Soma da grade 3D (tensor de ordem 3): 1367631
R_3^3 = 111^3 = 1367631
Iguais? True
```

Confirma a proposição: **o cubo do repunit é a "soma de volume" da tesselação/tensor de ordem 3.**

### Generalização do padrão geométrico

| Ordem $p$ | Objeto geométrico | Fórmula | Exemplo ($n=3$) |
|---|---|---|---|
| 1 | Linha (tesselação 1D) | $R_n$ | $111$ |
| 2 | Grade/plano (tesselação 2D) | $R_n^2$ | $12321$ |
| 3 | Cubo (tesselação 3D) | $R_n^3$ | $1367631$ |
| $p$ | Hipercubo (tesselação $p$-D) | $R_n^p$ | — |

Ou seja: **$R_n^p$ tem uma dupla identidade** — é ao mesmo tempo uma potência aritmética simples e a soma total de um tensor de ordem $p$ construído por produto de Kronecker de $p$ cópias da mesma série geométrica. A prova algébrica (distributividade) garante que essa identidade vale para todo $p$, não é um padrão observado só nos casos pequenos testados.

---

## 7. O passo mais básico: a demonstração não depende da base 10

Antes de ir mais longe, vale checar o alicerce: será que essa mecânica é uma peculiaridade da base 10, ou é estrutural?

### Repunit em base $b$ qualquer

$$R_n^{(b)} = \sum_{i=0}^{n-1} b^i = \frac{b^n - 1}{b - 1}$$

Calculado para $n=1,\dots,5$ em várias bases:

| base $b$ | $R_1,\dots,R_5$ |
|---|---|
| 2  | 1, 3, 7, 15, 31 |
| 3  | 1, 4, 13, 40, 121 |
| 7  | 1, 8, 57, 400, 2801 |
| 10 | 1, 11, 111, 1111, 11111 |
| 16 | 1, 17, 273, 4369, 69905 |

(Só em base 10 o repunit "parece" ter dígitos 1 repetidos — nas outras bases é a mesma soma geométrica, mas o valor decimal não tem essa aparência visual. A propriedade é aritmética, não decorativa.)

### A grade tensorial 2D também vale em qualquer base

Repeti a verificação de $R_3^2$ = soma da grade tensorial $3\times3$ com pesos $b^i \cdot b^j$, para $b=2,7,10$:

```
base 2:  R_3=7,   R_3^2=49,   soma_grade=49,   match=True
base 7:  R_3=57,  R_3^2=3249, soma_grade=3249, match=True
base 10: R_3=111, R_3^2=12321,soma_grade=12321,match=True
```

### Por que isso era esperado

A demonstração da Seção 6 usa apenas a propriedade distributiva de $\left(\sum_i b^i\right)^p$ — em nenhum momento o valor de $b$ entra no argumento. Logo a identidade

$$R_n^{(b)\,p} = \sum_{i_1,\dots,i_p=0}^{n-1} b^{\,i_1+\cdots+i_p}$$

vale para **qualquer base $b \geq 2$**, o que confirma numericamente: o fenômeno é uma propriedade da estrutura de série geométrica em si, não um acidente da base 10 ou da aparência visual dos "uns repetidos".

Isso é o alicerce mais básico da cadeia inteira — antes de tensores de ordem 3, hipercubos etc., o que garante tudo isso funcionar é só: **produto de somas = soma de produtos**, independente da base numérica escolhida.

---

## 8. Tesselação por ponto médio: paridade como classificação geométrica

### Propriedade 1 — Regra do ponto médio inteiro

Dados dois pontos de rede (coordenadas inteiras) $P=(x_1,y_1)$ e $Q=(x_2,y_2)$, o ponto médio $M = \left(\frac{x_1+x_2}{2}, \frac{y_1+y_2}{2}\right)$ é também um ponto de rede **se e somente se** $x_1 \equiv x_2 \pmod 2$ **e** $y_1 \equiv y_2 \pmod 2$ — ou seja, mesma paridade em cada coordenada, separadamente.

**Correção de hipótese inicial:** a investigação original supôs 3 combinações (par-par, ímpar-ímpar, par-ímpar), o que é correto em 1D (uma única coordenada, 2 classes → 3 combinações possíveis entre elas). Mas em 2D a paridade é independente por eixo, então não são 3 combinações — são **4 classes de paridade**: $(par,par)$, $(par,ímpar)$, $(ímpar,par)$, $(ímpar,ímpar)$. Testado computacionalmente com pares de pontos: o ponto médio só é inteiro quando ambos os pontos caem exatamente na mesma classe das 4 — confirmado em todos os casos testados.

Essas 4 classes tesselam $\mathbb{Z}^2$ como um padrão $2\times2$ que se repete indefinidamente — um "tabuleiro" de 4 cores, generalização do tabuleiro de xadrez (2 cores).

### Propriedade 2 — Princípio da casa dos pombos e generalização por dimensão

Como existem exatamente 4 classes em 2D, entre **quaisquer 5 pontos de rede** no plano, pelo menos dois pertencem à mesma classe — e portanto o ponto médio entre eles é garantidamente um ponto de rede. Verificado computacionalmente: gerando 5 pontos aleatórios, sempre há repetição de classe, e o ponto médio do par repetido é sempre inteiro.

O mesmo argumento generaliza para qualquer dimensão $d$: existem $2^d$ classes de paridade (uma combinação par/ímpar por eixo), então entre $2^d + 1$ pontos de rede em $\mathbb{Z}^d$, pelo menos dois compartilham classe e têm ponto médio inteiro.

| dimensão $d$ | classes $2^d$ | pontos necessários para garantir repetição |
|---|---|---|
| 1 | 2 | 3 |
| 2 | 4 | 5 |
| 3 | 8 | 9 |
| 4 | 16 | 17 |
| 5 | 32 | 33 |

Verificado computacionalmente para $d=3$: gerando 9 pontos aleatórios em $\mathbb{Z}^3$, houve repetição de classe entre 8 possíveis, e o ponto médio do par repetido teve as três coordenadas inteiras, como previsto.

### Conexão com o resto do documento

Essa tesselação por paridade é estruturalmente do mesmo tipo que a tesselação tensorial das seções 4–7: em ambos os casos, uma propriedade 1D (paridade de um inteiro; potência de uma série geométrica) se combina **por produto cartesiano/tensorial independente por eixo** para gerar uma classificação ou peso em dimensão $d$. Aqui o "produto tensorial" é o produto cartesiano das classes de resíduo $\{0,1\}^d$, em vez do produto de pesos $b^i$ — mas o mecanismo de "cada eixo contribui uma escolha independente, e a combinação total tesselam o espaço" é o mesmo.

---

## 9. Generalização: de paridade (mod 2) para resíduos mod m

A Seção 0 provou o caso $m=2$. Aqui generalizamos: paridade não é especial — é a instância mais simples de uma regra sobre **resíduos módulo $m$**.

### Prova algébrica

**Teorema:** dados inteiros $a, b$ e $m \geq 2$, o segmento entre $a$ e $b$ admite uma **$m$-seção em pontos de rede** (isto é, existem $m-1$ pontos intermediários igualmente espaçados, todos inteiros) se e somente se $a \equiv b \pmod m$.

**Demonstração:** Suponha $a \equiv b \pmod m$, isto é, $b - a = qm$ para algum $q \in \mathbb{Z}$. Definindo o passo $s = q = \frac{b-a}{m}$, os pontos $a, a+s, a+2s, \dots, a+ms=b$ são todos inteiros, pois $s \in \mathbb{Z}$. Reciprocamente, se existe uma $m$-seção inteira, o passo $s=(b-a)/m$ precisa ser inteiro, o que exige $m \mid (b-a)$, ou seja, $a \equiv b \pmod m$. $\blacksquare$

Note que $m=2$ recupera exatamente a Seção 0: "mesma paridade" é "$a \equiv b \pmod 2$", e a $2$-seção é o próprio ponto médio.

### Verificação computacional

| $a$ | $b$ | $m$ | mesma classe mod $m$? | $m$-seção |
|---|---|---|---|---|
| 2 | 11 | 3 | sim | $[2,5,8,11]$ |
| 5 | 17 | 4 | sim | $[5,8,11,14,17]$ |
| 0 | 10 | 2 | sim | $[0,5,10]$ |
| 3 | 10 | 3 | não | (não existe) |

Confirma a prova: só há $m$-seção inteira quando as classes coincidem.

### Generalização da tesselação por dimensão e módulo

Em $d$ dimensões, cada eixo tem $m$ classes de resíduo independentes, então o espaço $\mathbb{Z}^d$ é tesselado em $m^d$ classes (generalização direta do tabuleiro de $2^d$ cores da Seção 8). Pelo princípio da casa dos pombos, entre $m^d+1$ pontos de rede em $\mathbb{Z}^d$, ao menos dois compartilham classe — e portanto admitem $m$-seção inteira em todas as coordenadas.

| módulo $m$ | dimensão $d$ | classes $m^d$ | pontos p/ garantir repetição |
|---|---|---|---|
| 2 | 1 | 2 | 3 |
| 2 | 2 | 4 | 5 |
| 2 | 3 | 8 | 9 |
| 3 | 2 | 9 | 10 |
| 3 | 3 | 27 | 28 |
| 4 | 3 | 64 | 65 |

### Conexão com o resto do documento

Isso fecha o ciclo: a Seção 8 (paridade) é o caso $m=2$ desta seção; e a contagem $m^d$ — produto de $d$ escolhas independentes de $m$ opções cada — é estruturalmente idêntica ao mecanismo tensorial das Seções 4–7, onde uma grade $p$-dimensional nasce do produto de $p$ cópias de uma mesma estrutura 1D (lá, pesos $b^i$; aqui, classes de resíduo). O fio condutor de todo o documento é este: **produto cartesiano/tensorial de estruturas 1D independentes por eixo gera tesselações em dimensão superior**, seja a estrutura uma série geométrica (repunits) ou uma partição por resíduo (paridade generalizada).

---

## 10. Conexões com investigações relacionadas (arranjos, (a−b)ⁿ, reticulado de divisores, Bézout)

Esta seção documenta pontes genuínas identificadas entre este material e um conjunto de investigações relacionadas (arranjos como tensores, identidades de $(a-b)^n$, Bézout/frações contínuas, decomposição em repunits por base, reticulado de divisores, diferença de quadrados).

### 10.1 Paridade como classe de equivalência — a ponte mais direta

O resultado central da Seção 0/8 deste documento (ponto médio $\frac{a+b}{2}$ é inteiro $\iff a\equiv b \pmod 2$) é **a mesma estrutura matemática** do teorema central sobre diferença de quadrados: $n=a^2-b^2$ tem solução inteira via um par de divisores $(d_1,d_2)$ de $n$ com $d_1 d_2=n$ **se e somente se** $d_1\equiv d_2\pmod 2$, com $a=\frac{d_1+d_2}{2}$, $b=\frac{d_2-d_1}{2}$.

Verificado computacionalmente para $n=15,45,91,105$: em cada representação encontrada, o par de divisores usado satisfaz exatamente a condição de mesma classe mod 2 provada na Seção 0 — não é uma condição nova, é a mesma regra de paridade aplicada a pares de divisores em vez de pares de inteiros arbitrários.

### 10.2 Hipercubo: três instâncias do mesmo objeto combinatório

O hipercubo de dimensão $d$ aparece, com significados diferentes, em três lugares:

| Contexto | O que os $2^d$ (ou $2^k$) rótulos representam |
|---|---|
| Seção 8 deste documento | classes de paridade $(par,ímpar)^d$ em $\mathbb{Z}^d$ |
| $(a-b)^n$ (identidade de potências) | escolhas $\{a,-b\}$ em cada um dos $n$ fatores da expansão |
| Reticulado de divisores $D_n$ | presença/ausência de cada primo distinto, quando $n$ é livre de quadrados |

Em todos os três, a estrutura subjacente é a mesma: **produto cartesiano de $k$ cópias de um conjunto de 2 elementos**, e a contagem por "camada" (número de elementos com exatamente $i$ escolhas de um certo tipo) é sempre o coeficiente binomial $\binom{k}{i}$ — no primeiro caso, o número de pontos de rede numa classe; no segundo, os coeficientes do binômio de Newton; no terceiro, o número de divisores com exatamente $i$ fatores primos (contados com multiplicidade, restrito ao caso livre de quadrados).

### 10.3 Repunits em base $b$: mesmo resultado, ângulo complementar

A generalização da Seção 9 (paridade → resíduo mod $m$) e a decomposição em cascata via repunits generalizados $R_j^{(b)}=\frac{b^j-1}{b-1}$ chegam à mesma identidade $b^i = R_{i+1}^{(b)}-R_i^{(b)}$ já usada na Seção 2 deste documento. A contribuição complementar é a soma de Abel $N=\sum_j c_j R_j^{(b)}$ com $c_j=a_{j-1}-a_j$, que para $b=2$ coincide com a recodificação de Booth usada em hardware — um caso em que a mesma identidade algébrica de repunits reaparece como algoritmo de otimização de multiplicação binária.

### 10.4 Refinamento: gcd(a+b,a−b) como tesselação recursiva sobre o núcleo coprimo

A proposição sobre $\gcd(a+b,a-b)$ do texto de Bézout/frações contínuas (rotulada no próprio texto como "Esboço") precisou de um refinamento, encontrado ao testar computacionalmente contraexemplos como $a=8,b=6$: mesma paridade de $a,b$, mas $\gcd(a+b,a-b)=2\neq 2\gcd(a,b)=4$.

**Regra correta:** sejam $g=\gcd(a,b)$ e $(a',b')=(a/g,\,b/g)$ o núcleo coprimo. Então

$$\gcd(a+b,a-b) = \begin{cases} 2g & \text{se } a' \text{ e } b' \text{ são ambos ímpares} \\ g & \text{caso contrário} \end{cases}$$

Note que $a',b'$ nunca podem ser ambos pares (seriam coprimos por construção), então a condição relevante não é a paridade de $a,b$ em si, mas a paridade do **núcleo coprimo** — o par depois de remover a escala $g$. Verificado em 12 casos, incluindo os que antes falhavam: $(8,6)\to(4,3)$ misto $\to g=2$ ✓; $(8,4)\to(2,1)$ misto $\to g=4$ ✓; $(10,2)\to(5,1)$ ambos ímpares $\to 2g=4$ ✓.

**Por que isso é tesselação, e não só uma correção pontual:** $a+b$ e $a-b$ são um par de complementos em torno de $(a,b)$ — a mesma estrutura da Seção 0 (soma e diferença como reflexões). A classe de paridade que decide o resultado não é a de $(a,b)$ diretamente, mas a do núcleo coprimo $(a',b')$ obtido depois de fatorar a escala comum $g$ — exatamente a "Técnica 2" (redução por fator comum) do documento de $(a-b)^n$. Ou seja: a tesselação por paridade da Seção 8 se aplica de forma **recursiva**, primeiro reduzindo pela escala $g$ (o mesmo passo do algoritmo de Euclides) e só então testando a classe — o mesmo padrão de "reduzir ao núcleo antes de aplicar a regra local" que estrutura o algoritmo de Euclides e as frações contínuas.

---

## 11. O crivo com roda: por que a diagonal falha, e o ladrilhamento correto

O documento `crivo.pdf` descreve a construção do crivo com roda: expandir $S_{k-1}$ para $S_{k,\text{expandido}} = \{s + N_{k-1}\cdot j \mid s\in S_{k-1},\, 0\le j<p_k\}$ (uma grade $\phi(N_{k-1})\times p_k$), e remover os múltiplos de $p_k$ via $p_k\times S_{k-1}\bmod N_k$.

### 11.1 A diagonal só é limpa em grade quadrada

Testando a remoção para $N_3\to N_4$ ($S_3$ com 8 elementos, $p_4=7$ colunas), os pontos removidos caem em posições $(\text{índice}, j)$ que **não** formam uma diagonal (não seguem $i=j$ nem $i=\text{const}-j$), e duas linhas diferentes (índices 0 e 7) caem na mesma coluna $j=3$ — uma colisão.

Isso contrasta com a construção por gnômons de `arranjos_bordas.pdf`, onde a diagonal principal de uma matriz $x\times x$ toca cada gnômon em exatamente um ponto — uma bijeção perfeita, possível **porque a grade é quadrada**.

### 11.2 A causa: a grade do crivo é sempre par × ímpar

Verificado para $k=1,\dots,8$: $\phi(N_{k-1})$ é sempre par (o produto $\prod(p_i-1)$ contém ao menos um fator par, pois todo $p_i-1$ com $p_i$ ímpar é par), enquanto $p_k$ é sempre ímpar para $k\geq 2$ (todo primo além do 2 é ímpar). A grade é, portanto, estruturalmente **par × ímpar** para todo $k\geq3$ — nunca quadrada.

Como uma diagonal limpa exige bijeção linha↔coluna (mesmo número de linhas e colunas), e aqui os números nunca coincidem, o princípio da casa dos pombos garante colisão: pelo menos uma coluna recebe duas linhas. A falha da diagonal não é um acidente do exemplo $N_3\to N_4$ — é consequência estrutural da paridade de $\phi(N_{k-1})$ versus $p_k$.

### 11.3 O ladrilhamento correto: dois tamanhos de bloco via divisão euclidiana

Se a diagonal (ladrilho de tamanho 1, uniforme) não serve, a pergunta é qual ladrilhamento *funciona* sem sobra nem buraco. A resposta é a divisão euclidiana de $\phi(N_{k-1})$ por $p_k$: escrevendo $\phi(N_{k-1}) = q\cdot p_k + r$, a grade se particiona exatamente em $r$ colunas com $q+1$ elementos e $p_k - r$ colunas com $q$ elementos — uma partição completa, só que com **dois tamanhos de bloco** em vez de um só.

| $k$ | $p_k$ (colunas) | $\phi(N_{k-1})$ (linhas) | $q$ | $r$ | partição |
|---|---|---|---|---|---|
| 4 | 7 | 8 | 1 | 1 | 1 coluna com 2, 6 colunas com 1 |
| 5 | 11 | 48 | 4 | 4 | 4 colunas com 5, 7 colunas com 4 |
| 6 | 13 | 480 | 36 | 12 | 12 colunas com 37, 1 coluna com 36 |

### 11.4 Generalização da Seção 9: tesselação não-uniforme

A Seção 9 tratou apenas o caso em que o módulo $m$ divide exatamente o domínio, gerando $m$ classes de mesmo tamanho (sistema completo de resíduos uniforme). O crivo mostra o caso geral, quando a divisão **não** é exata: o único ladrilhamento sem sobra nem buraco passa a exigir dois tamanhos de bloco, $q$ e $q+1$, na proporção ditada pelo quociente e resto da divisão euclidiana — o mesmo algoritmo que abre o `unificacao.pdf` (Seção 1, Bézout/Euclides) e a base da Seção 9.

**Síntese do princípio geral de encaixe (revisado):** quando as dimensões coincidem exatamente (múltiplo do módulo), o ladrilhamento é uniforme, um só tamanho de bloco (Seção 0.1/9). Quando não coincidem, o único ladrilhamento perfeito possível é o gerado pela divisão euclidiana — não existe alternativa que evite ter dois tamanhos de bloco convivendo; é essa mistura, e não a diagonal, que "sempre encaixa" mesmo quando o módulo não divide o domínio.

---

## 12. Ladrilhamento por restrição local (Wang tiles): que padrões são possíveis

Diferente da tesselação por resíduo (Seções 8–9), que exige uma regra algébrica global, existe uma segunda família de tesselação: **ladrilhamento por restrição local**, onde peças só precisam bater com os vizinhos já colocados. Essa seção documenta as propriedades verificadas dessa família.

### 12.1 A ideia central

Cada peça tem 4 bordas rotuladas (N, E, S, W), e duas peças só podem ser vizinhas se a borda compartilhada tiver o mesmo rótulo. Isso é uma tesselação por **restrição local** — bem diferente do produto tensorial (Seções 4–7) e da classe residual (Seções 0, 8–9), que são regras globais aplicadas uniformemente.

### 12.2 Propriedade 1 — conjunto completo garante encaixe sempre

Com o conjunto **completo** de tiles (todas as $m^4$ combinações possíveis de rótulos, para $m$ cores de borda), o preenchimento de uma grade $G\times G$ nunca falha: testado para $m=3$ (81 tipos), grade $16\times16$ — 0 falhas em 256 posições, 0 erros de encaixe na grade final. Isso vale independentemente da ordem de visita (testado tanto em varredura linha-por-linha quanto em espiral) — a ordem afeta *quantos* vizinhos já colocados cada peça precisa respeitar (2 na varredura raster, até 4 na espiral), mas não afeta a garantia de sucesso quando o conjunto é completo.

### 12.3 Propriedade 2 — limiar de densidade (comportamento tipo percolação)

Quando o conjunto de tiles é uma **amostra parcial** do total (nem todas as $m^4$ combinações), a taxa de falha de encaixe local depende fortemente da densidade da amostra:

| densidade do conjunto | nº de tipos de tile | falhas (grade 12×12, 144 posições) |
|---|---|---|
| 5% | 4 | 109 |
| 10% | 8 | 64 |
| 20% | 16 | 14 |
| 30% | 24 | 0 |
| 50% | 40 | 0 |
| 70% | 56 | 0 |
| 100% | 81 | 0 |

Existe um **limiar** perto de 20–30% de densidade abaixo do qual falhas se tornam frequentes e acima do qual o encaixe passa a ser garantido na prática — um comportamento qualitativamente parecido com percolação em redes aleatórias, embora não tenhamos verificado o valor exato do limiar nem sua dependência com $m$ ou o tamanho da grade.

### 12.4 Propriedade 3 — ordem de visita não muda a validade, só a legibilidade

Testado sistematicamente com 5 ordens de preenchimento distintas — raster (linha por linha), colunas, espiral, diagonal (a mesma lógica de varredura por diagonais anti-principais dos gnômons de `arranjos_bordas.pdf`), e ordem totalmente aleatória — todas produziram **0 falhas e 0 erros** com o conjunto completo de tiles, numa grade $10\times10$.

Isso não é coincidência das 5 ordens testadas: com o conjunto completo ($m^4$ combinações de rótulo para $m$ cores de borda), qualquer combinação de vizinhos já colocados sempre tem pelo menos uma peça compatível, porque o conjunto contém, por construção, toda combinação possível dos 4 rótulos — a garantia de sucesso é uma consequência direta de $m^4$ ser o produto cartesiano completo, não uma propriedade da ordem escolhida.

O que muda entre ordens é estrutural, não lógico: cada ordem exige checar um número diferente de vizinhos já colocados por peça (raster e colunas exigem no máximo 2; espiral e diagonal podem exigir até 4, dependendo da posição), o que afeta o custo computacional mas não a validade do resultado.

### 12.5 Propriedade 4 — a escolha entre candidatos válidos é independente da validade

Quando existe mais de uma peça compatível numa posição, a escolha entre elas (aleatória ou determinística) não afeta se o encaixe é válido — afeta apenas a aparência do resultado. Colorir pelo índice arbitrário de enumeração da peça produz um padrão visualmente ilegível (mesmo com encaixe perfeito por baixo), enquanto colorir por uma grandeza que reflete a estrutura de bordas (ex: média das 4 bordas) revela continuidade espacial genuína. Isso é consistente com a Seção 10 (permutação preserva entropia mas destrói coerência espacial): a validade lógica do ladrilhamento e a legibilidade visual do resultado são propriedades independentes.

### 12.6 Classificação dos tipos de padrão possíveis nessa família

| Tipo de padrão | Como obter | Regularidade resultante |
|---|---|---|
| Periódico uniforme | Conjunto completo + escolha determinística fixa (ex: sempre a peça de menor id compatível) | Alta — repete em ciclo previsível |
| Não-periódico mas válido | Conjunto completo + escolha aleatória entre candidatos | Média — sem repetição óbvia, mas sem estrutura visível a menos que se colorir pela grandeza certa (12.5) |
| Falho/incompleto | Conjunto parcial abaixo do limiar de densidade (12.3) | Precisa de relaxação de regra (fallback) para completar — não é um ladrilhamento "puro" |
| Ilegível mas válido | Conjunto completo + coloração por índice arbitrário | Validade garantida, mas indescritível visualmente (12.5) — é o caso que motivou esta seção |

### 12.7 Bloco mínimo auto-consistente e periódico: a virada real

A tentativa inicial de reduzir trabalho por simetria (12.6, espelhar metade de uma grade grande por transposição) falhou na costura da diagonal — a condição de encaixe na fronteira entre as duas metades não decorre automaticamente de reflexão aplicada depois do preenchimento. A correção foi buscar, desde o início, o **menor bloco que já nasce satisfazendo todas as condições simultaneamente**, e repeti-lo.

**Construção:** busca exaustiva por um bloco $2\times2$ de tiles $(t_{00}, t_{01}, t_{10}, t_{11})$, com $t_{10}=\text{transposta}(t_{01})$, satisfazendo três condições ao mesmo tempo:
1. **Consistência interna** — bordas compartilhadas dentro do bloco batem (0 erros de encaixe internamente).
2. **Auto-transposição na diagonal** — $t_{00}$ e $t_{11}$ satisfazem $N=W$ e $E=S$ (são seu próprio espelho).
3. **Periodicidade** — a borda direita do bloco bate com a borda esquerda de uma cópia repetida à direita, e o mesmo verticalmente (condição de contorno periódica, como um toro discreto).

Para $m=2$ cores de borda, a busca exaustiva encontrou **14 soluções não-triviais** (excluindo a solução trivial onde todas as bordas têm o mesmo rótulo).

**Verificação:** repetindo um desses blocos $2\times2$ por um mosaico $4\times4$ (grade final $8\times8=64$ células), o resultado tem **0 erros de encaixe em toda a grade** — incluindo as fronteiras entre cópias repetidas do bloco, que é exatamente onde a tentativa anterior falhava.

**Propriedades verificadas desta técnica:**

- **Propriedade 1 (economia radical).** Apenas as $2\times2=4$ células do bloco original precisam ser de fato escolhidas por busca; as demais 60 células da grade $8\times8$ são cópias diretas — 94% de economia computacional, contra 44% da tentativa de espelhamento simples (12.6) e 0% do preenchimento célula a célula (Seções 12.2–12.4).
- **Propriedade 2 (a periodicidade precisa ser garantida na origem, não remendada depois).** A causa raiz da falha em 12.6 era tratar consistência interna e periodicidade como problemas separados (preencher, depois espelhar). Aqui as três condições (interna, diagonal, periódica) são exigidas *simultaneamente* na busca do bloco — é isso que elimina os erros de fronteira.
- **Propriedade 3 (escala com o tamanho do bloco, não da grade).** Uma vez encontrado um bloco válido de tamanho $k\times k$, o custo de preencher uma grade $N\times N$ é $O(k^2)$ (busca do bloco) $+\ O(N^2/k^2)$ (cópias), em vez de $O(N^2)$ células escolhidas individualmente — quanto menor o bloco mínimo válido, maior a economia relativa em grades grandes.
- **Propriedade 4 (existência resolvida: sempre existe, mas a riqueza depende do tamanho do bloco).** Testado para blocos $1\times1$: a condição de periodicidade se reduz a $E=W$ e $N=S$ (a peça precisa bater consigo mesma nos dois eixos), e essa condição **sempre** tem soluções não-triviais para todo $m\geq2$ testado ($m=2,3,4,5$) — inclusive $m^2$ combinações distintas de $(N,E)$ válidas para $m=4$. Porém, um bloco $1\times1$, usado sozinho, produz apenas um padrão **uniforme** (um único tipo de célula repetido) — não há como misturar duas soluções $1\times1$ diferentes na mesma tela sem quebrar a periodicidade global. A riqueza visual (mais de um tipo de célula coexistindo de forma consistente) exige blocos maiores, como o $2\times2$ usado nesta seção. Existência de bloco válido, portanto, **não é o fator limitante** — é sempre garantida a partir de $1\times1$; o que de fato varia com $m$ e o tamanho do bloco é a diversidade/riqueza do padrão resultante, não a mera possibilidade de ladrilhar sem erro.

**Conexão com o resto do documento:** isso é a mesma lógica da Seção 9 (sistema completo de resíduos) e do Painel 3 do teste de Coxeter — um objeto pequeno e bem escolhido (a célula unitária, a frequência de uma função invariante, o bloco $2\times2$) gera, por repetição/simetria, uma estrutura arbitrariamente grande sem custo proporcional ao tamanho final. A diferença aqui é que a condição de contorno periódica precisou ser resolvida como parte da busca do objeto pequeno, não como uma etapa posterior — a lição central desta seção.

---

## 13. Riqueza do bloco mínimo: diversidade máxima e limiar de dificuldade

Retomando a questão em aberto da Seção 12.7 (Propriedade 4): em vez de "existe bloco válido?" (sempre existe, a partir de $1\times1$), a pergunta certa é **quanta diversidade** (tipos de célula distintos coexistindo) um bloco $k\times k$ periódico consegue sustentar.

### 13.1 Metodologia

Busca por backtracking (não aleatória — testes anteriores confirmaram que busca aleatória cega se torna inviável rapidamente) de blocos $k\times k$ com condição de contorno **toroidal**: a borda direita da última coluna precisa bater com a borda esquerda da primeira coluna, e o mesmo verticalmente — a condição exata para que o bloco se repita infinitamente sem erro em qualquer direção.

### 13.2 Resultado: diversidade máxima $=k^2$ é alcançável para $k$ pequeno

| $m$ | $k$ | diversidade máxima encontrada | $k^2$ |
|---|---|---|---|
| 2 | 1 | 1 | 1 |
| 2 | 2 | 4 | 4 |
| 2 | 3 | 9 | 9 |
| 3 | 1 | 1 | 1 |
| 3 | 2 | 4 | 4 |
| 3 | 3 | 9 | 9 |

Para $k=1,2,3$ e $m=2,3$, a busca sempre encontrou um bloco onde **todas** as $k^2$ células são de tipos distintos — a diversidade máxima teórica é atingível, não apenas aproximada.

### 13.3 A métrica real: razão de saturação e limiar de dificuldade (conexão com phase transitions em CSP)

A tentativa inicial de nomear o caso $k^2=m^4$ como "de Bruijn torus" (Seção 13.3, versão anterior) usou um termo impreciso, aplicado de memória sem verificação — corrigido após checagem externa (a definição real de de Bruijn torus é sobre janelas deslizantes, não sobre "cada peça aparece uma vez"; a ponte entre os dois problemas existe mas exige tradução explícita, não uma equivalência direta).

O que a investigação de fato revelou é uma **métrica própria**: a razão de saturação $\rho = k^2/m^4$ — quantas células a grade $k\times k$ precisa preencher em relação ao total de tiles distintos disponíveis. Medindo o custo de busca (número de chamadas de backtracking até encontrar a diversidade máxima $k^2$, com orçamento de 50.000 chamadas) em função de $\rho$:

| $k$ | $m$ | $\rho=k^2/m^4$ | sucesso | chamadas até resolver |
|---|---|---|---|---|
| 3 | 4 | 0,035 | sim | 10 |
| 2 | 3 | 0,049 | sim | 5 |
| 1 | 2 | 0,062 | sim | 2 |
| 3 | 3 | 0,111 | sim | 203 |
| 2 | 2 | 0,250 | sim | 5 |
| 3 | 2 | 0,562 | sim | 12 |
| **4** | **2** | **1,000** | **não** | **>50.000 (esgotou o orçamento)** |

**O que isso mostra:** a dificuldade não cresce gradualmente com $\rho$ — o caso $\rho=1$ (pedir a diversidade máxima teórica, sem nenhuma folga) é qualitativamente diferente de todos os outros, inclusive de $\rho=0{,}562$, que resolveu em poucas dezenas de chamadas. Isso é consistente com o fenômeno de **"sharp threshold" em problemas de satisfação de restrições (CSP)** — um campo estabelecido e verificado (k-SAT, coloração de grafos, etc.), onde a probabilidade de sucesso de um CSP aleatório cai abruptamente perto de uma razão crítica de restrições, em vez de degradar suavemente. A fonte confirma: "*the random CSP is satisfiable with high probability for $\alpha<\alpha_{sat}$, and unsatisfiable with high probability for $\alpha>\alpha_{sat}$*" — o mesmo padrão qualitativo dos nossos dados, com $\rho$ no papel do parâmetro de restrição $\alpha$.

**Distinção importante:** isso não prova que $\rho=1$ seja *impossível* — só que a busca por backtracking ingênuo, sem heurística dedicada, se torna computacionalmente inviável perto desse ponto, exatamente o comportamento que a literatura de CSP documenta para buscas não-informadas perto do limiar. Não afirmamos (nem testamos) se existe algoritmo dedicado capaz de resolver $\rho=1$ de forma eficiente — isso fica como questão em aberto, sem o rótulo impreciso da versão anterior.

### 13.4 Síntese da relação tamanho do bloco × riqueza

| Regime | Diversidade alcançável | Como obter |
|---|---|---|
| $k^2 < m^4$ (bloco pequeno relativo ao espaço de tiles) | Máxima ($k^2$), fácil de achar | Backtracking simples, poucas tentativas |
| $k^2 = m^4$ (bloco do tamanho exato do espaço de tiles) | Máxima ($k^2=m^4$) teoricamente, mas exige construção dedicada | Algoritmo de de Bruijn torus, não busca ingênua |
| $k^2 > m^4$ | Diversidade limitada a $m^4$ por definição (não há tiles suficientes para não repetir) | Necessariamente há repetição de tipo de célula |

Isso fecha a questão da Seção 12.7 de forma mais precisa: a riqueza de um bloco periódico cresce livremente até o limite $m^4$ (o número total de tiles distintos possíveis), e o regime de "usar cada tile exatamente uma vez" tem nome e teoria próprios (de Bruijn tori) — não precisa ser redescoberto do zero, mas também não é trivial de alcançar por busca aleatória, sendo o único regime, entre os testados, que exigiu mais do que backtracking simples.

---

## 14. Episódio de verificação cruzada: contagem de soluções na grade 2×2 (k=2, m=2)

Este episódio é registrado por seu valor metodológico: um resultado externo (gerado por outro sistema, "Gemini") afirmou um número sem demonstração, e a verificação revelou exatamente onde o raciocínio subjacente falhava — não apenas que o número estava errado, mas por quê.

### 14.1 A alegação original

Para a grade toroidal $2\times2$ com $m=2$ (contexto da Seção 13), foi afirmado que existem **48 soluções** com diversidade máxima (as 4 células ocupadas por tiles todos distintos), sem demonstração — apenas o enunciado das 8 equações de conexão e o resultado final.

### 14.2 Primeira verificação: força bruta exaustiva

Testando todas as $16^4=65536$ atribuições possíveis de tile às 4 posições (não amostragem — o espaço é pequeno o suficiente para busca completa), filtrando pelas 8 equações de conexão e depois pela condição de diversidade máxima:

- Soluções válidas no total (qualquer diversidade): **256**
- Soluções com diversidade máxima (4 tiles distintos): **152**

Nem o total (256 vs. o "64" implícito na justificativa) nem o resultado final (152 vs. 48) coincidiram com a alegação original.

### 14.3 Segunda verificação, independente: contagem de graus de liberdade

Quando questionada, a justificativa recebida argumentava que a grade $2\times2$ tem "6 variáveis livres" (logo $2^6=64$ configurações totais, das quais 16 teriam repetição, sobrando $64-16=48$). Essa alegação foi verificada diretamente nas 8 equações de conexão fornecidas: as 16 variáveis de borda (4 tiles × 4 rótulos $N,E,S,W$) foram checadas uma a uma, contando em quantas equações cada uma aparece.

**Resultado:** cada uma das 16 variáveis aparece em **exatamente uma** das 8 equações — nenhuma redundância, um emparelhamento perfeito. Isso implica $16-8=8$ graus de liberdade, não 6, e portanto $2^8=256$ configurações totais, não $2^6=64$.

### 14.4 Conclusão do episódio

Duas verificações totalmente independentes — força bruta completa sobre todo o espaço de busca, e contagem algébrica direta de graus de liberdade a partir das próprias equações fornecidas — concordam exatamente em **256 soluções totais** e **152 com diversidade máxima**. A alegação de 48 foi rastreada até sua origem: uma contagem incorreta de graus de liberdade (6 em vez de 8) na etapa inicial do argumento, não um erro sutil de uma etapa avançada.

**Lição registrada:** um resultado numérico sem demonstração — mesmo quando parece formal ou vem acompanhado de equações corretas — não deve ser aceito até que cada etapa do argumento seja reproduzida de forma independente. Neste caso, o problema não estava nas equações de conexão (que estavam corretas e foram usadas identicamente nas duas verificações), mas numa contagem de graus de liberdade feita sem checar se as equações eram de fato independentes.

---

## 15. O triângulo de Pascal é isósceles, e o tetraedro de Pascal tem 4 faces (3 iguais, 1 diferente)

### 15.1 Por que o triângulo de Pascal é isósceles, não equilátero

O binômio de Newton $(a+b)^n=\sum_k\binom{n}{k}a^{n-k}b^k$ tem coeficientes simétricos: $\binom{n}{k}=\binom{n}{n-k}$, verificado exaustivamente para $n=1,\dots,6$ (cada linha é um palíndromo).

Ao desenhar o triângulo de Pascal no arranjo padrão (cada linha centralizada, descendo uma unidade por vez), essa simetria algébrica se traduz numa simetria geométrica de espelho em torno do eixo vertical central — o que força os dois lados que saem do topo a terem o mesmo comprimento. Verificado numericamente (arranjo com $N=8$ linhas): lado esquerdo e lado direito medem exatamente $7{,}8262$, enquanto a base mede $7{,}0000$ — **isósceles**, não equilátero. A simetria $\binom{n}{k}=\binom{n}{n-k}$ (grupo de reflexão $\mathbb{Z}_2$) é exatamente o suficiente para igualar os dois lados, mas não diz nada sobre o comprimento da base — daí a forma isósceles, e não equilátera.

### 15.2 O tetraedro de Pascal: quatro faces, três delas Pascal comum, uma delas diferente

O binômio generaliza para o trinômio $(a+b+c)^n=\sum_{i+j+k=n}\binom{n}{i,j,k}a^ib^jc^k$, com coeficiente multinomial $\binom{n}{i,j,k}=\frac{n!}{i!\,j!\,k!}$. Empilhando as camadas $n=0,1,2,\dots,N$ (cada camada sendo um triângulo de coeficientes, como o Pascal, mas com 3 índices), o resultado é um sólido tetraédrico — o **tetraedro de Pascal**. Ele tem exatamente 4 faces triangulares.

**Faces 1, 2 e 3** (fixando $i=0$, $j=0$ ou $k=0$, respectivamente, em todas as camadas): cada uma reproduz **exatamente** o triângulo de Pascal binomial padrão. Verificado linha por linha até $n=6$: fixar uma variável em zero colapsa $(a+b+c)^n$ para $(a+b)^n$, recuperando $\binom{n}{j}$. Essas 3 faces têm a mesma simetria isósceles da Seção 15.1 (grupo $\mathbb{Z}_2$, reflexão simples).

**Face 4** (a camada mais externa, $n=N$ inteira, sozinha): é um objeto genuinamente diferente — o **triângulo trinomial**. Testado para $n=6$:

```
[1, 6, 15, 20, 15, 6, 1]
[6, 30, 60, 60, 30, 6]
[15, 60, 90, 60, 15]
[20, 60, 60, 20]
[15, 30, 15]
[6, 6]
[1]
```

Diferente das faces 1-3, essa face é invariante sob **todas as 6 permutações** das 3 coordenadas $(i,j,k)$ — verificado exaustivamente para $n=4$: $\binom{n}{i,j,k}=\binom{n}{\sigma(i,j,k)}$ para toda permutação $\sigma\in S_3$, não apenas a reflexão simples. É uma simetria estritamente mais rica (grupo $S_3$, ordem 6, contra $\mathbb{Z}_2$, ordem 2).

### 15.3 Síntese: "além de Pascal e Newton" são 3 cópias de Pascal + 1 objeto novo

A observação de que o tetraedro tem "mais 3 faces" além do Pascal/Newton está certa, mas de forma mais precisa: 3 dessas faces **são** o mesmo Pascal (não são objetos novos, são cópias do binomial vistas de ângulos diferentes do sólido 3D), e apenas **uma** — a face externa, o triângulo trinomial — é um objeto genuinamente distinto, com uma simetria de grupo maior.

### 15.4 Generalização confirmada: (a+b+c+d)ⁿ e o padrão indutivo geral

Testando o próximo passo da recursão — o simplex 4D de $(a+b+c+d)^n$ — confirma que o padrão "3 cópias do caso anterior + 1 objeto novo" (Seção 15.3) não é peculiar do trinômio; é o início de uma indução.

**Verificação das facetas laterais:** as 4 facetas obtidas fixando cada uma das variáveis $i,j,k,l$ em 0 reproduzem **exatamente** o tetraedro de Pascal (trinomial) da Seção 15.2 — testado para todas as combinações até $n=5$, sem exceção.

**Verificação da faceta nova:** a camada externa ($n=N$ inteira) tem coeficientes quadrinomiais $\binom{n}{i,j,k,l}$, invariantes sob as **24 permutações** do grupo $S_4$ — testado exaustivamente nas 56 tuplas de $n=5$, confirmando a generalização da simetria $S_3\to S_4$.

**O padrão indutivo geral, confirmado em dois passos consecutivos:**

| Termos ($r$) | Objeto | Facetas | $r-1$ facetas = caso anterior | 1 faceta nova, simetria |
|---|---|---|---|---|
| 2 | Pascal (linha) | 2 pontas | — (caso base) | $\mathbb{Z}_2$, ordem 2 |
| 3 | Tetraedro de Pascal | 4 | 3 cópias do Pascal binomial | $S_3$, ordem 6 |
| 4 | Simplex 4D | 5 | 4 cópias do tetraedro trinomial | $S_4$, ordem 24 |
| $r$ | Simplex $(r-1)$-D | $r$ | $r-1$ cópias do caso $r-1$ | $S_r$, ordem $r!$ |

O padrão geral (última linha) é a indução natural sugerida pelos dois casos verificados: um multinômio de $r$ termos gera um simplex de dimensão $r-1$ com $r$ facetas — $r-1$ delas idênticas ao simplex de $r-1$ termos (por fixar uma variável em zero, colapsando o termo extra), e exatamente 1 faceta nova (a camada externa), cuja simetria de permutação cresce de $S_{r-1}$ para $S_r$ a cada passo. Os casos $r=2,3,4$ foram verificados computacionalmente; $r$ geral seria o próximo passo de indução formal (não testado para $r\geq5$).

### 15.6 Demonstração formal geral (para todo $r$, não apenas os casos testados)

A tabela da Seção 15.4 foi verificada computacionalmente para $r=2,3,4$ (e agora também $r=5$, testado com $n=4$: 70 tuplas, invariância sob as 120 permutações de $S_5$ confirmada, assim como a redução de faceta). Isso sugere fortemente o padrão, mas não o prova para todo $r$. As duas propriedades centrais, no entanto, têm demonstração direta a partir da definição do coeficiente multinomial — sem depender de indução computacional.

**Definição:** para $i_1+\cdots+i_r=n$, o coeficiente multinomial é $\displaystyle\binom{n}{i_1,\dots,i_r}=\frac{n!}{i_1!\,i_2!\cdots i_r!}$.

**Teorema 1 (redução de faceta).** Se $i_r=0$, então $\displaystyle\binom{n}{i_1,\dots,i_{r-1},0}=\binom{n}{i_1,\dots,i_{r-1}}$ — a faceta obtida fixando qualquer uma das $r$ variáveis em zero é, termo a termo, exatamente o coeficiente multinomial de $r-1$ termos.

**Prova:** por definição, $\displaystyle\binom{n}{i_1,\dots,i_{r-1},0}=\frac{n!}{i_1!\cdots i_{r-1}!\,0!}$. Como $0!=1$ por convenção, isso é exatamente $\displaystyle\frac{n!}{i_1!\cdots i_{r-1}!}=\binom{n}{i_1,\dots,i_{r-1}}$. $\blacksquare$

Essa prova não depende de $r$ — vale para qualquer número de termos, porque usa só a definição de fatorial de zero. Isso demonstra, de uma vez por todas, que as $r$ facetas laterais do simplex de $r$ termos são sempre cópias exatas do simplex de $r-1$ termos, resolvendo formalmente a coluna "$r-1$ facetas = caso anterior" da tabela para todo $r$.

**Teorema 2 (simetria $S_r$ da faceta nova).** O coeficiente multinomial $\binom{n}{i_1,\dots,i_r}$ é invariante sob qualquer permutação $\sigma\in S_r$ dos índices: $\binom{n}{i_1,\dots,i_r}=\binom{n}{i_{\sigma(1)},\dots,i_{\sigma(r)}}$.

**Prova:** o denominador $i_1!\,i_2!\cdots i_r!$ é um produto de $r$ fatores. A multiplicação é comutativa, então reordenar os fatores segundo qualquer permutação $\sigma$ não altera o produto: $i_1!\cdots i_r! = i_{\sigma(1)}!\cdots i_{\sigma(r)}!$. Como o numerador $n!$ não depende dos índices, o quociente inteiro é idêntico para qualquer reordenação. $\blacksquare$

Essa prova também não depende de $r$ — vale para qualquer número de variáveis, porque usa só a comutatividade da multiplicação. Isso demonstra, para todo $r$, que a faceta nova (a camada externa) tem simetria de grupo $S_r$ (ordem $r!$), resolvendo formalmente a última coluna da tabela.

**Conclusão:** ao contrário do que a tabela da Seção 15.4 sugeria (um padrão indutivo verificado apenas em casos pequenos), as duas propriedades centrais — redução de faceta e simetria $S_r$ — têm demonstração direta e geral, válida para todo $r\geq2$ sem exceção. A verificação computacional ($r=2,3,4,5$) deixa de ser a evidência principal e passa a ser apenas uma confirmação consistente de um teorema já provado algebricamente.

### 15.7 Conexão com o resto do documento

Isso se encaixa diretamente na Seção 10.2 (hipercubo como estrutura comum entre $(a-b)^n$, paridade e reticulado de divisores): o coeficiente binomial contava "camadas" de um hipercubo de escolhas $\{a,-b\}$; aqui, o coeficiente multinomial conta camadas de um **simplex** de escolhas $\{a,b,c\}$ — a generalização natural de hipercubo (produto de fatores $\{0,1\}$) para simplex (partição em $i+j+k=n$). O padrão geral: um binômio gera um hipercubo com $\mathbb{Z}_2$ de simetria por eixo; um multinômio de $r$ termos gera um simplex $(r-1)$-dimensional com simetria de permutação $S_r$ — a mesma estrutura de "produto/partição gerando tesselação com grupo de simetria" que atravessa todo o documento, agora vista em sua forma combinatória mais rica até aqui.

---

## 16. LFSR e cycle joining: a construção real de de Bruijn que faltava na Seção 13

A Seção 13.3 deixou em aberto a construção de um de Bruijn torus 2D genuíno (não a dobra ingênua que falhou). O documento `teoria_lfsr_cycle_joining.md` (revisado nesta integração) traz o método real e verificado para o caso 1D — sequências de de Bruijn via LFSR e *cycle joining* — que é o alicerce sobre o qual as construções 2D publicadas (Seção 13.3) se apoiam.

### 16.1 O método, resumido e verificado

Um LFSR de comprimento $n$ com polinômio característico **primitivo** sobre $\mathbb{F}_2$ particiona $\mathbb{F}_2^n$ em um ciclo máximo ($2^n-1$ estados não-nulos) e o estado nulo isolado. O método de *cycle joining* insere uma perturbação não-linear local que costura o estado nulo de volta ao ciclo máximo, produzindo um único ciclo hamiltoniano de comprimento $2^n$ — que é, por construção, uma sequência de de Bruijn.

**Correção feita durante a revisão:** a derivação original usava $x^3+x^2+x+1=0$, que fatora como $(x+1)^3$ sobre $\mathbb{F}_2$ — não é primitivo, não pode gerar ciclo maximal. O polinômio correto, usado consistentemente após a correção, é $x^3+x+1=0$ (irredutível, verificado, período $2^3-1=7$).

### 16.2 Nota estrutural sobre $\mathbb{F}_2$: por que este método nunca produz números irracionais

Em $\mathbb{F}_2$, $-1\equiv1\pmod2$, logo $x^3+x+1$ e $x^3-x-1$ são **o mesmo polinômio** — verificado computacionalmente. Sobre os reais, são polinômios diferentes, cada um com sua própria raiz irracional. A distinção $+/-$, que importa profundamente sobre os reais, simplesmente não existe em característica 2. Essa é a razão estrutural pela qual toda a construção de LFSR/de Bruijn deste documento permanece exata e finita (resolvida por busca em $\{0,1\}^n$, nunca por fórmula fechada) — não por conveniência, mas porque o domínio não admite outra possibilidade.

### 16.3 Contraste esclarecedor: quando a mesma ideia de substituição ($k$) encontra um obstáculo genuíno (Abel-Ruffini)

A técnica de substituir uma potência alta por uma variável $k$ de grau menor (Seção 4.1 do documento LFSR) tem uma motivação mais ampla do que só simplificar LFSRs: a mesma ideia, aplicada a $x^5-x-1=0$ sobre os **reais/complexos** (não $\mathbb{F}_2$), esbarra num obstáculo genuinamente diferente.

Verificado computacionalmente:
- $x^5-x-1=0$ tem exatamente 5 raízes: **1 real** ($\approx1{,}1673$) e 4 complexas (2 pares conjugados).
- O grupo de Galois desse polinômio é $S_5$ (ordem 120), confirmado **não solúvel** por radicais (teorema de Abel-Ruffini).

**Isso explica por que uma abordagem em série para essa equação "converge mas nunca fecha exatamente":** não é limitação de implementação ou erro de ponto flutuante evitável — é consequência direta de Abel-Ruffini. Não existe expressão fechada por radicais para a raiz real desse polinômio; qualquer série (Newton, ponto fixo) pode se aproximar dela com precisão arbitrária, mas nunca produzirá uma forma fechada, porque essa forma não existe.

**A distinção que fecha esta seção:** o mesmo símbolo $k$ (substituição de potência alta por variável auxiliar) opera em dois regimes completamente diferentes — sobre $\mathbb{F}_2$ (Seção 16.1–16.2), onde tudo é finito, exato e sempre resolúvel por busca; e sobre os reais/complexos aplicado a um polinômio de grau $\geq5$ com grupo de Galois não-solúvel (Seção 16.3), onde nenhuma quantidade de álgebra produz forma fechada, e aproximação numérica (com seu erro de ponto flutuante inerente) é a única via possível. Confundir os dois regimes — tentar "resolver exatamente" no segundo caso, ou temer imprecisão numérica desnecessariamente no primeiro — é o erro de categoria que esta seção documenta para evitar.

---

## 17. MDC entre pares de divisores: quando um fator quadrado se reparte

Esta seção nasceu de uma exploração concreta: fatorando $17649=111\times159$ via método de Fermat (Seção 14), notou-se que $\text{mdc}(111,159)=3$, não $1$ — diferente da maioria dos outros pares de divisores de $17649$. Isso revelou um padrão geral, não um acaso desse número específico.

### 17.1 O teorema

**Proposição:** Seja $N=p^{2m}\cdot M$, com $p$ primo, $M$ livre de quadrados e $\gcd(p,M)=1$. Para qualquer par de divisores $(d_1,d_2)$ com $d_1d_2=N$, o valor $\text{mdc}(d_1,d_2)$ assume **exatamente** os valores $p^0,p^1,\dots,p^m$ — nenhum outro.

**Ideia da prova:** todo divisor $d_1$ se escreve como $p^i\cdot e_1$ com $0\leq i\leq2m$ e $e_1\mid M$; correspondentemente $d_2=p^{2m-i}\cdot e_2$ com $e_1e_2=M$. Como $M$ é livre de quadrados, qualquer par de divisores $(e_1,e_2)$ de $M$ é automaticamente coprimo — logo $\text{mdc}(d_1,d_2)=p^{\min(i,\,2m-i)}\cdot\gcd(e_1,e_2)=p^{\min(i,2m-i)}$. Conforme $i$ varre $0,\dots,2m$, $\min(i,2m-i)$ varre exatamente $0,1,\dots,m,\dots,1,0$ — logo os valores possíveis de mdc são $\{p^0,\dots,p^m\}$.

### 17.2 Verificação computacional

| $N$ | Fatoração | mdc observados | mdc esperados | Confere? |
|---|---|---|---|---|
| $17649$ | $3^2\times37\times53$ | $\{1,3\}$ | $\{1,3\}$ | sim |
| $2835$ | $3^4\times5\times7$ | $\{1,3,9\}$ | $\{1,3,9\}$ | sim |
| $960$ | $2^6\times15$ | $\{1,2,4,8\}$ | $\{1,2,4,8\}$ | sim |
| $5625$ | $5^4\times3^2$ | $\{1,3,5,15,25,75\}$ | $\{1,5,25\}$ | **não** (esperado — viola a hipótese) |

O último caso viola deliberadamente a condição "$M$ livre de quadrados" ($M=9=3^2$ não é livre de quadrados) — e os valores extras de mdc ($3,15,75$) aparecem exatamente porque o próprio $M$ contribui sua própria estrutura de mdc entre pares de divisores, misturando-se com a do primo $p=5$. Isso não invalida a proposição — confirma que a hipótese é necessária, não decorativa.

### 17.3 Conexão com o resto do documento

Isso une dois resultados já estabelecidos:
- **Seção 14** (diferença de quadrados): garante que todo par de divisores de $N$ ímpar corresponde a uma representação $N=a^2-b^2$, com $a=\frac{d_1+d_2}2$, $b=\frac{d_2-d_1}2$.
- **Seção 10.4** (mdc$(a+b,a-b)$ via núcleo coprimo): já tínhamos mostrado que o mdc de uma soma/diferença depende de reduzir pelo núcleo coprimo antes de testar a classe de paridade.

A Seção 17 é a mesma ideia — "o mdc herda estrutura de como um fator se reparte" — vista agora do lado da fatoração de $N$ em vez da soma/diferença de dois números arbitrários: aqui, o "núcleo" que se reparte é a potência $p^{2m}$ de um fator primo repetido de $N$, e a forma como ele se distribui entre $d_1$ e $d_2$ determina exatamente o mdc do par — nem mais, nem menos valores do que $p^0,\dots,p^m$.

---

## 18. Investigação: repunits e tesselação mod m têm função geradora unificada?

### 18.1 A função geradora da tesselação por resíduo (Seção 9)

Para uma única classe residual (resto $r$, módulo $m$), a função geradora é

$$G_{m,r}(x)=\sum_{k=0}^{\infty}x^{r+km}=\frac{x^r}{1-x^m}$$

Verificado por expansão em série ($m=3,r=1$): produz exatamente os expoentes $1,4,7,10,13,\dots$ — todos $\equiv1\pmod3$, como esperado.

### 18.2 Conexão parcial real (não inventada)

A função geradora dos repunits (Seção 2), $f(x)=\dfrac{x}{(1-x)(1-bx)}$, decompõe-se em frações parciais como

$$f(x)=\frac{1}{b-1}\cdot\frac{1}{1-x}-\frac{1}{b-1}\cdot\frac{1}{1-bx}$$

O primeiro termo, $\dfrac{1}{1-x}$, é **literalmente** $G_{1,0}(x)$ — a função geradora da tesselação trivial de uma única classe (módulo $m=1$, todos os números, sem restrição). Essa parte da conexão é exata, não aproximada.

### 18.3 Por que a unificação completa falha (verificado, não suposto)

O segundo termo, $\dfrac{1}{1-bx}$, **não** é nenhum $G_{m,r}(x)$ da família de tesselação. A razão é estrutural, confirmada verificando a localização dos polos:

- **Polos da GF dos repunits:** $x=1$ e $x=1/b$ — o segundo polo é um número real fora (ou dentro, se $b<1$) do círculo unitário, **não** uma raiz da unidade.
- **Polos da GF de tesselação mod $m$:** todas as $m$ raízes de $x^m=1$ — todas têm $|x|=1$, sobre o círculo unitário, verificado explicitamente para $m=5$ (as 5 raízes têm módulo exatamente 1).

Essas são famílias de função racional **estruturalmente diferentes**: a tesselação mod $m$ é inteiramente sobre o círculo unitário (reflexo de sua periodicidade exata), enquanto o repunit tem um polo fora desse círculo (reflexo de seu crescimento geométrico, não periódico). Não existe substituição $b\to m$ que transforme uma família na outra.

### 18.4 Conclusão da investigação

A pergunta "existe uma função geradora unificada?" tem resposta **parcialmente sim, não totalmente**: as duas famílias compartilham um bloco básico comum ($1/(1-x)$, a tesselação trivial de uma classe), mas divergem estruturalmente a partir daí — repunits crescem geometricamente (polo fora do círculo unitário), tesselação mod $m$ é puramente periódica (polos sobre o círculo unitário). Isso não é uma falha da investigação; é uma resposta precisa e verificada sobre os limites da analogia entre os dois formalismos que atravessam este documento inteiro.

---

## 19. O limiar de densidade da Seção 12.3: medição precisa e dependência com m

### 19.1 Metodologia

Para $m=3,4,5$, medi a taxa de sucesso (fração de 40 tentativas independentes que preenchem uma grade $10\times10$ sem falha) em função da densidade $\rho$ do conjunto de tiles, com passo fino ($0{,}02$) e interpolação linear entre o último ponto abaixo de $50\%$ e o primeiro acima, para localizar o limiar com mais precisão que a Seção 12.3 original.

### 19.2 Resultado

| $m$ | tiles totais ($m^4$) | limiar $\rho_c$ (50% sucesso) | $\rho_c\cdot m$ | $\rho_c\cdot m^2$ |
|---|---|---|---|---|
| 3 | 81 | $0{,}2475$ | $0{,}7425$ | $2{,}2275$ |
| 4 | 256 | $0{,}1650$ | $0{,}6600$ | $2{,}6400$ |
| 5 | 625 | $0{,}1280$ | $0{,}6400$ | $3{,}2000$ |

($m=2$ foi excluído — o espaço total de apenas 16 tiles torna os passos de densidade grosseiros demais para uma medição confiável.)

### 19.3 Interpretação: mais perto de $C/m$ do que de $C/m^2$, mas sem expoente fechado

$\rho_c\cdot m$ é quase constante ($0{,}64$–$0{,}74$), enquanto $\rho_c\cdot m^2$ cresce claramente ($2{,}23\to3{,}20$) — isso descarta $\rho_c\sim C/m^2$ como boa aproximação e favorece algo próximo de $\rho_c\sim C/m$. Mas o ajuste de expoente por lei de potência ($\rho_c\sim C/m^p$), calculado entre cada par de pontos, dá $p=1{,}409$ (entre $m=3,4$), $p=1{,}291$ (entre $m=3,5$) e $p=1{,}138$ (entre $m=4,5$) — valores próximos de $1$, mas não idênticos, e com só 3 pontos (e ruído de Monte Carlo de 40 tentativas cada) não é possível fixar $p$ com precisão maior.

### 19.4 Heurística teórica (não uma derivação rigorosa)

Para uma peça já ter dois vizinhos fixados (oeste e norte, caso típico da varredura raster), o número esperado de candidatos compatíveis dentro de um conjunto de tamanho $\rho m^4$ é aproximadamente $\rho m^4/m^2=\rho m^2$ (cada uma das 2 restrições de borda reduz o conjunto por um fator $\sim1/m$, assumindo distribuição uniforme dos rótulos). Isso sugeriria $\rho_c\sim C/m^2$, não $C/m$ — o que **não bate** com os dados medidos (Seção 19.3). A discrepância provavelmente vem de os candidatos não serem estatisticamente independentes célula a célula (a restrição de vizinhança introduz correlação que o argumento ingênuo ignora), ou de o tamanho finito da grade ($10\times10$, não assintótico) afastar a medição do comportamento limite teórico.

### 19.5 Conclusão honesta

O limiar de densidade **depende de $m$ de forma previsível em ordem de grandeza** (decresce com $m$, mais rápido que $1/m^0$ constante, mais devagar que $1/m^2$), mas a forma exata da lei de potência não ficou determinada com os dados coletados — a heurística teórica simples (Seção 19.4) não reproduz o expoente observado, indicando que uma análise mais cuidadosa (correlações entre células, ou análise assintótica em grades maiores) seria necessária para uma conexão rigorosa com a teoria de percolação/CSP citada na Seção 13.3. Isso permanece um resultado empírico parcial, não uma lei fechada.

---

## 20. Forma complementar de x⁵-x-1=0: verificação e uma ressalva metodológica

### 20.1 A forma complementar, verificada

Partindo da representação $x=\cosh(t)$ já estabelecida (Seção 16, corrigindo o `Cos.pdf`), a "forma complementar" usa a outra função hiperbólica do par, $y=\sinh(t)$, ligada por $y^2=x^2-1$. Eliminando $x$ entre $x^5-x-1=0$ e $y^2=x^2-1$ (via resultante), obtém-se o polinômio que $y$ satisfaz:

$$y^{10}+5y^8+8y^6+4y^4-1=0$$

**Nota de processo:** a primeira tentativa de verificação numérica desta equação falhou (erro de $10^{-16}$) porque usei um valor de $y$ copiado do `Cos.pdf`, truncado numa casa decimal incorreta — refazendo o cálculo de $x$ e $y$ do zero, com 50 casas de precisão, o polinômio bateu exatamente ($\sim10^{-51}$). Isso reforça, mais uma vez, o princípio já estabelecido nesta investigação: recalcular do zero, não reaproveitar valores "quase certos" de fontes externas.

### 20.2 Por que o grau salta de 5 para 10

A eliminação captura as 5 raízes de $x^5-x-1=0$ (1 real, 4 complexas), e cada uma gera **duas** raízes em $y$ (via $y=\pm\sqrt{x^2-1}$) — daí grau $5\times2=10$. A forma complementar não é mais simples que a original: é estritamente mais complexa (grau maior), pela ambiguidade extra do sinal de $y$.

### 20.3 Ressalva metodológica: "melhor forma" não é absoluto, é proximidade

Vale registrar uma tensão importante, notada durante esta investigação: escolher entre a forma em $x$ (grau 5, mínima) e a forma complementar em $y$ (grau 10) não é uma questão de qual é "objetivamente melhor" — é a mesma lógica de conveniência por proximidade que já apareceu na Seção sobre truncagem aritmética (escolher decompor $a$ ou $b$, dependendo de qual gera a parte trivial). Aqui, a forma em $x$ é preferível **porque** tem grau menor e é a representação mínima — não por alguma virtude hiperbólica intrínseca do cosseno hiperbólico sobre o seno hiperbólico. Se por algum motivo específico do problema $y$ fosse mais acessível diretamente que $x$ (o que não é o caso aqui), a forma complementar passaria a ser a "melhor", pela mesma lógica de escolher o caminho mais barato disponível, não por uma hierarquia fixa entre as duas representações.

Isso é consistente com o padrão desta seção 16–20: nenhuma "roupagem" transcendental (radicais, $\cosh$, $\sinh$) resolve a impossibilidade de Abel-Ruffini — cada uma é só uma rota alternativa de mesma dificuldade estrutural, e a escolha de qual usar é sempre uma questão de conveniência local (grau mínimo, proximidade a algo já conhecido), nunca uma resolução genuína do problema de fundo.

### 20.4 Por que o grau 10 não é coincidência: teoria de extensões de corpo

Testei a fatoração do polinômio de grau 10 (e do de grau 5 em $z=y^2$) sobre $\mathbb{Q}$: **não fatora** — confirmado tanto por fatoração direta quanto por checagem explícita de irredutibilidade.

Isso não é um acidente algébrico — é consequência de teoria de extensões de corpo. $x$ gera uma extensão $\mathbb{Q}(x)/\mathbb{Q}$ de grau 5 (a quíntica original, irredutível). $y=\sqrt{x^2-1}$ exige adjuntar **mais uma raiz quadrada** além disso, então $[\mathbb{Q}(x,y):\mathbb{Q}]=[\mathbb{Q}(x,y):\mathbb{Q}(x)]\cdot[\mathbb{Q}(x):\mathbb{Q}]=2\times5=10$ — exatamente o grau observado do polinômio em $y$.

**A distinção precisa:** $y$ é "redutível" ao problema original no sentido de que toda sua informação vem de $x$ (não é independente, foi definido a partir dele) — mas **não é redutível em complexidade algébrica**. Pelo contrário: é uma extensão estritamente maior (grau 10, não 5), porque a extração de $\sqrt{x^2-1}$ carrega uma unidade extra de informação algébrica que o polinômio em $x$ sozinho não tinha. "Vem de" e "é mais simples que" são coisas diferentes — a forma complementar não é apenas uma rota alternativa de mesma dificuldade (como a Seção 20.3 sugeria de forma mais informal); é uma rota **estritamente mais difícil**, com a razão exata (grau dobrado pela raiz quadrada extra) agora precisa.

### 20.5 Forma fechada elegante (achada por regrupagem manual)

Reagrupando os termos do polinômio como $y^8(y^2+5)+y^4(8y^2+4)=1$ e fatorando $y^4$ do segundo bloco, o termo interno $y^6+5y^4+8y^2+4$ (em $w=y^2$: $w^3+5w^2+8w+4$) fatora de forma limpa:

$$w^3+5w^2+8w+4=(w+1)(w+2)^2$$

o que dá a forma fechada, verificada simbolicamente (expansão bate exatamente com o polinômio original) e numericamente (diferença $\sim10^{-51}$ com 50 casas de precisão):

$$y^4(y^2+1)(y^2+2)^2=1$$

**Isso não contradiz a irredutibilidade da Seção 20.4** — o polinômio completo (incluindo o $-1$ do lado direito subtraído) continua irredutível sobre $\mathbb{Q}$; o que foi fatorado é o lado esquerdo antes da subtração, uma identidade estrutural diferente e mais reveladora do que a expansão bruta em 5 termos. É uma contribuição genuína: reduz a equação a um produto de 3 fatores crescentes ($y^4$, $y^2+1$, $(y^2+2)^2$) que precisam se equilibrar contra $1$ — uma leitura geométrica mais direta do que os coeficientes $1,5,8,4,-1$ dispersos.

---

## 21. Correção de terminologia: tesselação vs. ladrilhamento, e o mecanismo de não-repetição do de Bruijn

Um documento de referência trazido à conversa (`gnomon-tesselacao-ladrilhamento-dissecacao.md`) formaliza uma distinção que este documento usou de forma solta até aqui — vale corrigir explicitamente em vez de reescrever cada ocorrência individualmente.

### 21.1 A distinção precisa

| Conceito | O que é | Este documento usou como |
|---|---|---|
| **Tesselação** | Descritivo/combinatório — o vocabulário de quais formas *podem* se encaixar sem sobra nem buraco (propriedade angular/métrica). Não é uma ação. | Frequentemente como **verbo de ação** ("tesselam $\mathbb{Z}^d$ em $2^d$ classes") |
| **Ladrilhamento** | A **execução espacial** — o ato de efetivamente cobrir um domínio usando as peças que a tesselação permite. | Usado corretamente na Seção 12 (Wang tiles, "ladrilhamento por restrição local") |

**Correção retroativa:** em praticamente todas as Seções 0–19, quando o texto diz "tesselam" ou "tesselação" no sentido de *ação de cobrir* (ex: "as $m$ classes tesselam $\mathbb{Z}$ sem sobreposição", "a grade 2D nasce do produto tensorial"), o termo tecnicamente correto seria **ladrilhamento** (a execução) — "tesselação" deveria ficar reservado para a propriedade combinatória que garante que esse ladrilhamento é possível (ex: a demonstração de sistema completo de resíduos, Seção 0.1, é a tesselação propriamente dita; o ato de efetivamente cobrir $\mathbb{Z}^d$ com as classes é o ladrilhamento). Isso não muda nenhum resultado matemático do documento — é uma correção de precisão terminológica, não de conteúdo.

### 21.2 O mecanismo de não-repetição do de Bruijn: onde a distinção teve consequência real

O documento de referência é explícito: o mecanismo de não-repetição de um de Bruijn torus é **exaustão combinatória completa** — construção determinística que garante o resultado por construção, não busca.

Isso identifica precisamente o que estava errado na tentativa da Seção 13.3: usei **backtracking aleatório com orçamento limitado** para tentar resolver o caso $\rho=1$ — um mecanismo fundamentalmente diferente (busca probabilística, sem garantia) do mecanismo correto (exaustão determinística). A falha registrada ali (>50.000 tentativas sem sucesso) misturava dois problemas distintos: (a) possível dificuldade genuína do problema, e (b) uso do mecanismo errado para resolvê-lo. A Seção 16 já havia corrigido isso na prática (usando circuito euleriano via Hierholzer — exaustão combinatória completa, exatamente como a definição correta) e obtido sucesso garantido no caso 1D, sem nenhuma falha — confirmando, agora com a terminologia certa, que o problema real da Seção 13.3 era o mecanismo, não (só) a dificuldade estrutural do caso $\rho=1$.

**Lição registrada:** ao buscar um objeto cuja definição já especifica "gerado por construção determinística" (de Bruijn, cycle joining, sistema completo de resíduos), buscar por tentativa aleatória é o mecanismo errado desde o início — não é uma questão de dar mais tentativas, é trocar de método.

---

## 22. Identidades S,P e fatoração por agrupamento: funções simétricas encontram a Seção 15

O documento `Identidades_SP_e_Fatoracao.md` traz duas identidades de compressão algébrica — verificadas computacionalmente, incluindo a recorrência para 2 variáveis e sua generalização para 3.

### 22.1 $x^3+y^3=S^3-3PS$: o caso $n=2$ das funções simétricas elementares

Com $S=x+y$, $P=xy$: $x^3+y^3=S^3-3PS$, e mais geralmente $p_n=S\cdot p_{n-1}-P\cdot p_{n-2}$ (soma de potências via recorrência, sem nunca resolver $x,y$ individualmente). Verificado numericamente para $n=2,\dots,5$ com $x=2{,}5,y=1{,}5$ — bate exatamente com o cálculo direto em todos os passos.

**A conexão com a Seção 15:** $S$ e $P$ são os **polinômios simétricos elementares** de 2 variáveis ($e_1=S$, $e_2=P$). A recorrência acima é o caso $n=2$ das **identidades de Newton**, que relacionam somas de potências $p_n=\sum x_i^n$ aos polinômios simétricos elementares $e_1,\dots,e_n$. Testei a generalização para 3 variáveis — a mesma família de objeto que gera o triângulo trinomial da Seção 15 (onde a simetria $S_3$ dos coeficientes multinomiais decorre exatamente de $e_1,e_2,e_3$ serem invariantes sob permutação das variáveis):

$$p_3=e_1p_2-e_2p_1+3e_3$$

Verificado com $x=1,y=2,z=3$: $e_1=6,e_2=11,e_3=6$, $p_1=6,p_2=14$, e $e_1p_2-e_2p_1+3e_3=6\times14-11\times6+18=84-66+18=36=p_3$ (calculado diretamente como $1^3+2^3+3^3=36$). Confirma.

**Por que isso é a mesma simetria da Seção 15.6:** ali provamos que o coeficiente multinomial $\binom{n}{i_1,\dots,i_r}$ é invariante sob $S_r$ porque o denominador é um produto (comutativo) de fatoriais. Aqui, $S$ e $P$ (e, em geral, $e_1,\dots,e_r$) são invariantes sob $S_r$ porque são **somas/produtos simétricos** das variáveis por construção — a mesma razão de fundo (comutatividade da soma/produto), aplicada a um objeto diferente (funções simétricas em vez de coeficientes multinomiais), mas com a mesma assinatura de grupo.

### 22.2 Fatoração por agrupamento: a mesma distributividade da Seção 20

$a^2+2ab+b^2=(a+b)^2$, deduzida por agrupamento e extração de fator comum $(a+b)$ — verificada para 5 pares $(a,b)$ incluindo negativos e zero, sempre batendo.

**Conexão direta com a Seção 20:** essa é exatamente a propriedade distributiva ($uk+vk=(u+v)k$) que a Seção 20.3 já tinha identificado como o mecanismo real por trás da "truncagem" (não a identidade de Bézout, como eu erroneamente tinha nomeado antes de ser corrigido). O documento traz uma segunda instância clara do mesmo princípio: extrair fator comum reduz custo operacional (4 multiplicações + 2 somas → 1 multiplicação + 1 soma, Seção 2.2 do documento original), e sob uma restrição ($a+b=S$ fixo), a expressão colapsa para uma constante — o mesmo fenômeno de "restrição elimina grau de liberdade" que atravessa este documento (Seção 0.1, sistema completo de resíduos; Seção 12.7, bloco mínimo determina o resto).

### 22.3 Síntese da integração

| Documento novo | Conecta com | Mecanismo compartilhado |
|---|---|---|
| $S,P$ e identidades de Newton (Seção 1 do doc.) | Seção 15.6 (simetria $S_r$ do multinomial) | Invariância sob permutação por comutatividade de soma/produto |
| Fatoração por agrupamento (Seção 2 do doc.) | Seção 20.3 (distributividade, não Bézout) | $uk+vk=(u+v)k$, redução de custo operacional |
| Restrição colapsa grau de liberdade (Seção 2.3 do doc.) | Seção 0.1, 12.7 | Fixar um invariante elimina variação aparente dos componentes livres |

---

## 23. Combinando S,P (Seção 22) com repunits: economia de ciclos real

### 23.1 Repunits satisfazem exatamente a recorrência de Newton

Os polos da função geradora dos repunits (Seção 2/18) são $x=1$ e $x=1/b$ — ou seja, $R_n$ é uma combinação linear de $1^n$ e $b^n$, com "raízes características" $1$ e $b$. Isso significa que $R_n$ satisfaz a mesma recorrência de Newton da Seção 22.1, com $S=1+b$, $P=b$:

$$R_n=(b+1)R_{n-1}-bR_{n-2},\qquad R_0=0,\ R_1=1$$

Verificado exatamente em 4 bases ($b=2,7,10,16$), $n=2,\dots,7$ — bate em todos os casos.

### 23.2 Vantagem real: elimina divisão, útil em aritmética modular

A fórmula fechada $(b^n-1)/(b-1)$ exige dividir por $b-1$ — o que **falha** em aritmética modular quando $b-1$ não é invertível módulo $m$ (ex: $\gcd(b-1,m)\neq1$). Testado: $b=7$, módulo $6$ (onde $b-1=6$ não tem inverso mod 6) — a recorrência de Newton, que nunca divide, calcula $R_n\bmod6$ corretamente em todos os passos, batendo exatamente com o valor real do repunit reduzido módulo 6 a posteriori. A fórmula fechada simplesmente não seria aplicável nesse regime modular; a recorrência é.

### 23.3 Fast doubling: de O(n) para O(log n)

Como $R_n$ é combinação linear de duas raízes características ($1$ e $b$), admite identidades de duplicação análogas às de Fibonacci:

$$R_{2n}=R_n(b^n+1),\qquad R_{2n+1}=bR_{2n}+1$$

Implementado e verificado até $n=1000$ (bate exatamente com o cálculo direto). Contagem de operações: fast doubling usa $\sim2\log_2n$ multiplicações ($\sim20$ para $n=1000$), contra $n$ passos da recorrência linear simples ($1000$ para o mesmo caso) — uma redução real de $O(n)$ para $O(\log n)$.

### 23.4 Síntese

| Método | Custo | Funciona em aritmética modular geral? |
|---|---|---|
| Fórmula fechada $(b^n-1)/(b-1)$ | $O(\log n)$ (exponenciação) + 1 divisão | Não (falha se $b-1$ não invertível) |
| Recorrência de Newton, passo a passo | $O(n)$, sem divisão | Sim |
| Fast doubling da recorrência de Newton | $O(\log n)$, sem divisão | Sim |

A combinação pedida — usar o formalismo $S,P$ (Seção 22) sobre repunits — produz o melhor dos dois mundos: velocidade de $O(\log n)$ **e** ausência de divisão, superando tanto a fórmula fechada (rápida mas frágil em contexto modular) quanto a recorrência ingênua (robusta mas lenta). Isso conecta diretamente com o tema de $\mathbb{F}_2$/LFSR da Seção 16, onde evitar divisão é frequentemente essencial (corpos finitos nem sempre têm inversos convenientes, e hardware favorece multiplicação sobre divisão).

---

## 24. Paridade em palavras: nota de correção e referência

**Correção importante:** esta seção, na sua primeira versão, redescobriu — com menos precisão — um resultado já documentado extensivamente em `glossario_espelho_letras_atualizado.md` ("Paridade em Reflexão de Strings: o Caso hoje → hojeejoh / hojejoh"). Em vez de duplicar o conteúdo, esta seção agora referencia a fonte correta e registra a imprecisão cometida na primeira versão.

**O que a primeira versão desta seção errou:** tratei a construção "sem fusão" (concatenação, sempre par) e "com fusão" (sobrepondo a letra do meio, sempre ímpar) como igualmente naturais para qualquer palavra. O documento de referência mostra que isso é impreciso: para uma palavra de comprimento **ímpar**, a letra central já é, por construção, o próprio eixo de simetria — a fusão é genuína (ex: "arara", palíndromo natural, confirmado). Para uma palavra de comprimento **par**, não existe letra central genuína — o eixo cai *entre* duas letras — então "fundir" força artificialmente uma letra a servir de eixo compartilhado que não existia antes. O resultado ainda é um palíndromo válido (verificado: "hoje"→fusão→"ejohoje", confirmado palíndromo), mas a costura é arbitrária, não uma propriedade natural da palavra original, como o documento de referência formaliza na tabela da seção citada.

**A generalização correta** (do documento de referência, não desta seção): a mesma dicotomia sobreposição-vs-translação, e a mesma condição de paridade, aparece em pelo menos três instâncias já verificadas naquele documento — dobra de Kawasaki (paridade posicional de ângulos), "par mata primalidade" (paridade aritmética de contagem), e reflexão de strings (paridade de comprimento decidindo se o eixo cai sobre uma letra ou entre duas). As três compartilham a mesma estrutura de fundo que a Seção 0 deste documento já formaliza para inteiros — mas a articulação completa, com as três instâncias e a ressalva sobre quando a condição é trivial vs. não-trivial, está no documento de referência, não aqui.

---

## 25. Domínio fundamental e economia computacional: o grupo diedral como "gerar uma vez, repetir o resto"

### 25.1 O domínio fundamental de $D_5$ é exatamente $1/10$

O pentagrama tem simetria de grupo diedral $D_5$, de ordem $2n=10$ (5 rotações × reflexão). Isso significa que o **domínio fundamental** — a menor peça que, repetida pela ação do grupo, reconstrói a figura inteira — é exatamente $1/10$ da figura. Verificado por construção: um único triângulo (centro, uma ponta, um vértice interno adjacente) foi rotacionado ($0°,72°,144°,216°,288°$) e refletido em cada uma dessas posições, produzindo exatamente **10 cópias** que juntas reconstroem a estrela inteira sem nenhum ponto adicional desenhado à mão.

### 25.2 Por que isso é economia computacional real, não só elegância

Testei diretamente: recalcular seno/cosseno a cada uma das 5 rotações, 100.000 vezes, levou $2{,}13$s; pré-computar as 5 matrizes de rotação **uma vez** e só multiplicar o domínio fundamental por elas levou $0{,}59$s — **3,6× mais rápido**. Isso confirma numericamente a intuição de "gerar uma vez, o resto é repetição": o custo computacional real está em computar o domínio fundamental (ou as transformações do grupo) uma única vez; aplicá-lo repetidamente é multiplicação barata, não recomputação.

**Conexão direta com o resto do documento:** essa é a mesma lógica da Seção 12.7 (bloco $2\times2$ mínimo, repetido por uma tela inteira sem recalcular cada célula) e da Seção 23 (fast doubling de repunits, reaproveitando resultados intermediários em vez de recalcular potências do zero). O padrão geral, que atravessa o documento inteiro: identificar o menor objeto que gera tudo por simetria/repetição, e pagar o custo computacional só uma vez por esse objeto mínimo.

---

## 26. Saber sem calcular: simetria, Teorema de Lucas, e o Sierpinski escondido em Pascal

Esta seção reúne três níveis crescentes de "conhecer o valor pela forma, sem fazer a conta", em resposta direta à pergunta de saber "por consequência da forma do triângulo".

### 26.1 Nível 1: simetria de espelho (revisita a Seção 15.1)

$\binom{20}{6}=\binom{20}{14}$ pode ser afirmado **antes** de calcular qualquer fatorial, só notando $6+14=20$ (posições espelhadas em torno do meio da linha 20). Verificado: ambos valem $38760$. Isso não é coincidência numérica — é a simetria isósceles já provada na Seção 15.1: a forma do triângulo *garante* a igualdade, o cálculo só confirma o que a forma já dizia.

### 26.2 Nível 2: paridade via Teorema de Lucas (bits, não fatoriais)

**Regra:** $\binom{n}{k}$ é ímpar se e somente se todo bit de $k$ (em binário) está contido nos bits de $n$ — isto é, $(n\ \text{AND}\ k)=k$. Nenhum fatorial envolvido, só uma operação bit a bit.

Verificado em 6 casos, incluindo $\binom{1{,}000{,}000}{524{,}288}$ — um número com centenas de milhares de dígitos, cuja paridade foi determinada em microssegundos pela regra de bits, sem jamais calcular o valor real (que seria computacionalmente caro de obter por inteiro).

### 26.3 Nível 3: a forma completa é Sierpinski, uma consequência recursiva

Colorindo cada $\binom{n}{k}$ do triângulo de Pascal por paridade (ímpar=preto), o padrão que emerge — construído para $n=0,\dots,63$ — é exatamente o **triângulo de Sierpinski**: um fractal autossimilar, onde cada triângulo menor dentro do grande é uma cópia idêntica dele mesmo.

**Por que isso fecha o ciclo com o resto do documento:** a autossimilaridade do Sierpinski é a mesma lógica do domínio fundamental da Seção 25 (uma peça mínima, repetida por simetria, gera a figura inteira) — só que aqui a "peça mínima" não é um triângulo geométrico fixo, é o próprio padrão de paridade se repetindo em escalas cada vez menores. E a regra de bits (26.2) é, por sua vez, uma tesselação por resíduo — mas em base 2 aplicada aos **bits de $k$ e $n$**, não ao valor de $\binom{n}{k}$ diretamente — conectando com a Seção 9 (tesselação mod $m$) de um ângulo novo: a paridade "esperada" na Seção 9 usava resíduo do próprio número; aqui usa resíduo posição-a-posição da representação binária.

**Síntese da resposta à pergunta original:** sim, dá pra saber sem contas — mas "sem contas" tem graus. Nível 1 evita computar metade da conta (usa simetria). Nível 2 evita computar o valor inteiro inteiramente (usa só os bits). Nível 3 mostra que a resposta a "qual é a paridade" para *qualquer* $(n,k)$ já está determinada, de uma vez por todas, pela posição no padrão fractal — não é uma pergunta que precisa ser refeita a cada novo par $(n,k)$, é uma leitura de mapa.

### 26.4 Generalização confirmada: qualquer primo $p$, não só $p=2$

O Teorema de Lucas tem uma forma geral: para qualquer primo $p$, $\binom{n}{k}\bmod p=\prod_i\binom{n_i}{k_i}\bmod p$, onde $n_i,k_i$ são os dígitos de $n,k$ na base $p$ (e o produto é zero assim que algum $k_i>n_i$). Verificado para $p=2,3,5,7$ em 5 casos, incluindo $\binom{1000}{333}\bmod7$ — todos batendo exatamente com o cálculo direto do resíduo.

Visualizando $\binom{n}{k}\bmod 3$ para $n=0,\dots,80$ ($=3^4-1$), emerge um fractal de **3 cores** (resíduos $0,1,2$), autossimilar em blocos de potências de 3 — visualmente diferente do Sierpinski binário (2 cores, blocos de potência de 2), mas gerado pela mesma lógica de fundo. Isso confirma que o "Sierpinski em Pascal" não é um fato isolado de $p=2$ — é o primeiro membro de uma família inteira de fractais, um por primo, todos legíveis diretamente dos dígitos de $n,k$ na base correspondente, sem jamais calcular o coeficiente binomial.

### 26.5 A dimensão fractal segue fórmula fechada: $D(p)=\log_p\frac{p(p+1)}{2}$

**Derivação:** dentro de cada bloco $p\times p$ de dígitos ($n_i,k_i\in\{0,\dots,p-1\}$), o Teorema de Lucas diz que $\binom{n_i}{k_i}$ é não-nulo exatamente quando $k_i\leq n_i$ — isso conta os pares $(n_i,k_i)$ na região triangular, dando $1+2+\dots+p=\frac{p(p+1)}{2}$ combinações não-nulas de $p^2$ possíveis. Cada uma dessas é uma cópia autossimilar do padrão inteiro, escalada por fator $p$. Por *box-counting*, a dimensão fractal é

$$D(p)=\frac{\log\left(\frac{p(p+1)}{2}\right)}{\log p}$$

**Verificação exaustiva (não aproximada):** contei todas as células não-nulas de $\binom{n}{k}\bmod p$ para $n=0,\dots,p^m-1$, $m=1,2,3,4$, em $p=2,3,5$. A razão de contagem entre escalas consecutivas foi **exatamente** $\frac{p(p+1)}{2}$ em todos os 9 casos testados (3 primos × 3 razões cada), sem nenhum desvio — confirmando a fórmula de forma combinatória exata, não só assintótica.

| $p$ | cópias autossimilares $\frac{p(p+1)}{2}$ | $D(p)$ |
|---|---|---|
| 2 | 3 | $1{,}584963$ (Sierpinski clássico, $\log_23$) |
| 3 | 6 | $1{,}630930$ |
| 5 | 15 | $1{,}682606$ |
| 7 | 28 | $1{,}712414$ |
| 11 | 66 | $1{,}747222$ |

**Padrão observado:** $D(p)$ cresce lentamente e monotonicamente com $p$, sempre entre $1$ (uma linha) e $2$ (um triângulo cheio). Isso é consistente com a fração de células preenchidas, $\frac{p(p+1)/2}{p^2}=\frac{p+1}{2p}$, que na verdade **decresce** com $p$ — de $75\%$ em $p=2$ até o limite $50\%$ quando $p\to\infty$ (verificado: $p=5\to60\%$, $p=1000\to50{,}05\%$). Ou seja: o fractal fica *proporcionalmente mais esparso* conforme $p$ cresce (menor fração de células não-nulas), mesmo com $D(p)$ crescendo — porque $D(p)$ mede a taxa de crescimento entre escalas, não a densidade absoluta num bloco fixo; os dois fatos não se contradizem, mas não são a mesma coisa, e eu os confundi na primeira redação desta seção.

### 26.6 Comportamento assintótico: $D(p)\to2$ quando $p\to\infty$

Como $D(p)=2+\frac{\log\left(\frac{p+1}{2p}\right)}{\log p}$ e $\frac{p+1}{2p}\to\frac12$, a correção $2-D(p)$ deveria convergir pra $\frac{\log2}{\log p}$. Verificado numericamente para $p=11,101,1009,10007,100003$: os dois valores coincidem cada vez com mais precisão conforme $p$ cresce (diferença de $0{,}036$ em $p=11$ caindo pra $0{,}000001$ em $p=100003$), confirmando $D(p)\to2$ — o fractal se aproxima de preencher o plano por completo (dimensão 2) conforme o primo cresce, mesmo a densidade absoluta de células não-nulas convergindo pra apenas $50\%$ (Seção 26.5). Isso não é contraditório: dimensão fractal mede taxa de crescimento entre escalas, não fração absoluta preenchida num bloco fixo.

---

## 27. 1729 e o Terceiro Problema de Hilbert: dois pares de cubos são sempre remontáveis um no outro

### 27.1 O número e a pergunta

$1^3+12^3=9^3+10^3=1729$ (o número táxi de Ramanujan). A pergunta natural: existe uma transformação geométrica (corte e remontagem) que transforma o par de cubos $\{1^3,12^3\}$ no par $\{9^3,10^3\}$?

### 27.2 A resposta, via Dehn e Sydler (verificado contra a literatura)

Confirmado por busca: o **invariante de Dehn** (Max Dehn, 1901, solucionando o Terceiro Problema de Hilbert) determina se dois poliedros de mesmo volume podem ser remontados um no outro por dissecação finita ("congruência por tesoura"). Todo cubo tem ângulos diedros de $90°$ — um múltiplo racional de $\pi$ — e o invariante de Dehn é **sempre zero** para poliedros com esses ângulos. O **Teorema de Sydler** (1965) prova que volume + invariante de Dehn determinam completamente a classe de congruência por tesoura.

**Aplicando:** os dois conjuntos $\{1^3,12^3\}$ e $\{9^3,10^3\}$ têm o mesmo volume ($1729$) e o mesmo invariante de Dehn ($0$, por serem só cubos). Pelo Teorema de Sydler, **existe garantidamente** uma dissecação finita que transforma um conjunto no outro — não é uma possibilidade, é uma certeza matemática.

**Ressalva importante:** o teorema é de existência, não construtivo — garante que a dissecação existe, mas não diz quais cortes fazer nem quantas peças são necessárias. Encontrar essa dissecação explícita é um problema separado, muito mais difícil na prática.

### 27.3 A assimetria visual

Visualizando os dois pares lado a lado: $\{1,12\}$ é um par extremo (um cubo minúsculo ao lado de um grande); $\{9,10\}$ é quase simétrico (dois cubos quase do mesmo tamanho). Mesma soma de volume, formas geometricamente muito diferentes — o que torna a garantia de Sydler ainda mais notável: mesmo par tão visualmente diferentes, a remontagem é sempre possível.

---

## 28. Resolver $a^3+b^3=N$ via $S,P$ e divisores de $N$ (não por força bruta)

### 28.1 O método

Usando $S=a+b$, $P=ab$ (Seção 22) e a identidade $a^3+b^3=S^3-3PS=S(S^2-3P)$: como $S$ divide $N=a^3+b^3$, o espaço de busca se reduz de $O(\sqrt[3]{N})$ pares $(a,b)$ testados diretamente para apenas $d(N)$ candidatos de $S$ (os divisores de $N$).

**Algoritmo:**
1. Para cada divisor $S$ de $N$: calcular $Q=N/S$ e $P=\frac{S^2-Q}{3}$ (só aceitar se inteiro).
2. Testar se $S^2-4P$ é quadrado perfeito (condição para $a,b$ inteiros via $t^2-St+P=0$).
3. Recuperar $a=\frac{S+\sqrt{S^2-4P}}2$, $b=\frac{S-\sqrt{S^2-4P}}2$.

### 28.2 Verificação com $N=1729$

Testados os 8 divisores de $1729$ ($1,7,13,19,91,133,247,1729$) como candidatos a $S$: exatamente 2 sobreviveram aos critérios — $S=13,P=12\to(a,b)=(12,1)$ e $S=19,P=90\to(a,b)=(10,9)$ — recuperando **as duas soluções conhecidas** de Ramanujan sem testar nenhum par $(a,b)$ diretamente.

### 28.3 Por que isso conecta com a cascata de divisores

O tamanho do espaço de busca desse método é **exatamente** $d(N)$ — o mesmo número que a cascata de divisores (documento `cascata_divisores.pdf`) decompõe em camadas por primo. Para $1729=7\times13\times19$ (cascata $[1,1,2,4]$, soma $8$), a busca por $S,P$ testou exatamente esses $8$ candidatos. Isso é uma ponte direta e concreta entre três peças do trabalho: a fatoração de $N$ (cascata) determina o custo de achar representações de $N$ como soma de dois cubos (Seção 28), via o formalismo $S,P$ de funções simétricas (Seção 22).

---

## 29. A cascata de turbulência (p-model) tem o triângulo de Pascal escondido dentro dela

### 29.1 O modelo real de turbulência

O "p-model" (Meneveau & Sreenivasan, 1987) é um modelo multiplicativo estabelecido para intermitência em turbulência: a energia total (=1) se reparte recursivamente — a cada nível, cada intervalo se divide em 2, com uma fração $p$ indo para um lado e $(1-p)$ para o outro. Depois de $n$ níveis, $2^n$ sub-intervalos, cada um com energia $p^k(1-p)^{n-k}$, onde $k$ é quantas vezes aquele caminho "escolheu" o lado $p$.

### 29.2 O triângulo de Pascal emerge exatamente

Testado para $n=5$: o número de sub-intervalos com exatamente $k$ escolhas do lado $p$ é **exatamente** $\binom{n}{k}$ — verificado sem exceção para $k=0,\dots,5$. A energia total de cada grupo, $\binom{n}{k}p^k(1-p)^{n-k}$, é o termo binomial completo, e a soma de todos os grupos fecha em exatamente $1{,}000000$ — o teorema binomial $(p+(1-p))^n=1^n=1$ aplicado à cascata.

### 29.3 A mesma decomposição linear+escalar da Seção 14 (`cascata_divisores.pdf`)

Os extremos ($k=0$ e $k=n$) têm sempre exatamente **1** caminho cada — crescimento "linear", nunca se ramificando. O meio ($0<k<n$) cresce **combinatoriamente**, com $\binom{n}{k}$ caminhos — a mesma dualidade tira-linear/bloco-escalar de $n^2=n+n(n-1)$ (Seção 14 do documento de cascata de divisores), agora encontrada dentro de um modelo real de física.

### 29.4 O que isso estabelece, com precisão

A cascata de turbulência (p-model) e a cascata de divisores não são o mesmo fenômeno físico — uma é transferência de energia entre escalas de turbilhões, a outra é contagem de divisores por fatoração. Mas **compartilham literalmente a mesma máquina combinatória**: o p-model é, matematicamente, uma distribuição binomial aplicada recursivamente — o mesmo objeto da Seção 15 (triângulo de Pascal) e da Seção 22 ($S,P$/funções simétricas). Isso não é mais analogia poética entre "cascata" como palavra — é uma identidade estrutural testada e confirmada: onde quer que uma quantidade se reparta recursivamente por um fator binário fixo, o triângulo de Pascal aparece por baixo, goste ou não do domínio (aritmética, física, ou qualquer outro).

---

## 30. Cascata multinomial com pesos físicos: água, cimento, areia (e brita)

### 30.1 Generalização confirmada: qualquer número de variáveis, qualquer proporção

Generalizando o p-model binário da Seção 29 para 3 variáveis (água, cimento, areia) e depois 4 (concreto, incluindo brita): a contagem de "caminhos de mistura" que levam a cada composição final $(k_1,\dots,k_r)$ segue **exatamente** o coeficiente multinomial $\binom{n}{k_1,\dots,k_r}$ — verificado para $n=5$ com 3 variáveis (21 combinações, todas batendo) e $n=4$ com 4 variáveis (35 combinações, todas batendo), com pesos físicos realistas ($0{,}15/0{,}25/0{,}35/0{,}25$ para o traço de concreto). A soma das proporções fecha em $1{,}000000$ exato nos dois casos — a mesma máquina combinatória do simplex de Pascal generalizado (Seção 15.4), agora escalada por pesos físicos desiguais em vez de $1/r$ uniforme.

### 30.2 Exemplo: como normalmente é feito

**Modo 1 (o comum):** decidir a cada "pazada" qual ingrediente pegar, guiado pela proporção-alvo, e confiar que depois de muitas pazadas a proporção realizada se aproxima do alvo — é a Lei dos Grandes Números aplicada à mistura. Testado com alvo $(0{,}15,\,0{,}25,\,0{,}35,\,0{,}25)$:

| pazadas ($n$) | erro máximo da proporção real vs. alvo |
|---|---|
| 10 | $0{,}0500$ |
| 100 | $0{,}0900$ |
| 1000 | $0{,}0280$ |
| 10000 | $0{,}0092$ |

O erro diminui com mais pazadas, mas **não é garantido, nem monótono** (o erro em $n=100$ foi pior que em $n=10$, por acaso da amostragem) — é convergência estatística, não exata.

### 30.3 Exemplo: como poderia ser feito

**Modo 2 (usando o que já provamos, Seção 25):** em vez de confiar em muitas pazadas aleatórias, achar o **núcleo mínimo exato** — o menor lote inteiro que já reproduz a proporção-alvo perfeitamente. Para $(0{,}15,0{,}25,0{,}35,0{,}25)$, o denominador comum mínimo é $20$, dando o núcleo exato $(3,5,7,5)$ pazadas — que reproduz a proporção com **erro zero**, não aproximado.

Para fazer um lote $N$ vezes maior, **repete-se o núcleo $N$ vezes** (a mesma lógica de "domínio fundamental repetido" da Seção 25) — a proporção permanece exatamente correta em qualquer escala, sem depender de quantas vezes você repete.

### 30.4 A comparação

| | Modo 1 (comum) | Modo 2 (núcleo exato) |
|---|---|---|
| Precisão | Aproximada, melhora com mais pazadas | Exata, sempre, em qualquer escala |
| Garantia | Estatística (Lei dos Grandes Números) | Determinística (aritmética de frações) |
| Trabalho por lote grande | Recalcular/reamostrar cada vez | Repetir o mesmo núcleo pequeno |

Isso é a mesma lição da Seção 25.2 (economia computacional: "gerar uma vez, repetir o resto") aplicada agora a um problema físico real — misturar concreto por proporção exata, em vez de por aproximação estatística repetida.

---

## 31. Números figurados e fatoração: trapezoidais, triangulares e hexagonais

### 31.1 Trapezoidais: o teorema do divisor ímpar

Um número $N$ admite uma representação trapezoidal (soma de $k\geq1$ inteiros positivos consecutivos, $N=a+(a+1)+\dots+(a+k-1)$) para cada divisor ímpar de $N$ — verificado exatamente em 8 casos ($N=9,15,20,28,32,45,64,90$), incluindo o caso extremo de potências de 2 ($32,64$): exatamente **1** representação (a trivial, $k=1$), batendo com "exatamente 1 divisor ímpar" (o próprio $1$). Também confirmado que $45$ e $90=2\times45$ têm a mesma contagem ($6$), pois compartilham a mesma parte ímpar.

### 31.2 Triangulares: fatoração via coprimalidade de $n,n+1$

Para $T_n=\frac{n(n+1)}2$: como $n$ e $n+1$ são sempre coprimos, e o fator $2$ do denominador divide exatamente um dos dois, $d(T_n)=d(n/2)\times d(n+1)$ (se $n$ par) ou $d(n)\times d((n+1)/2)$ (se $n$ ímpar) — verificado exatamente em 28 casos consecutivos ($n=2,\dots,29$), sem exceção.

### 31.3 Hexagonais: a versão mais limpa, sem casos

Para $H_n=n(2n-1)$: $\gcd(n,2n-1)=\gcd(n,n-1)=1$ sempre (prova direta, sem depender de paridade), logo $d(H_n)=d(n)\times d(2n-1)$ **sempre**, sem distinguir casos — verificado em 18 valores consecutivos de $n$, sem exceção. Essa é a versão mais simples das três, porque a construção hexagonal evita o fator $2$ problemático que os triangulares carregam.

### 31.4 O padrão geral: figurados "encaixam" com a cascata de divisores

As três famílias compartilham o mesmo mecanismo: cada número figurado é construído como produto (ou soma) de peças cuja coprimalidade (ou estrutura de paridade) é garantida pela própria definição geométrica da figura — e isso permite decompor $d(N)$ do número figurado em termos de $d$ dos componentes menores, sem nunca precisar fatorar o número grande diretamente. É a mesma lógica multiplicativa da cascata de divisores (`cascata_divisores.pdf`), agora aplicada à própria construção geométrica da figura, não só à fatoração bruta de um número arbitrário.

---

## 32. Diagonais de polígonos: por que ímpar nunca passa pelo centro

### 32.1 O experimento

Traçando todas as diagonais de um polígono regular de $n$ lados: para $n=5$ (pentágono), a figura interna resultante é o **pentagrama** (polígono estelar $\{5/2\}$), com um pentágono regular menor sobrando no núcleo. Para $n=8$ (octógono), desenhar todas as diagonais sobrepõe várias famílias de polígonos estelares diferentes ($\{8/2\}$, $\{8/3\}$, etc.) sem um nome único — o resultado visual é mais "cheio e uniforme", sem o núcleo pontudo deslocado do pentagrama.

### 32.2 A causa exata: diagonais centrais e paridade

Testado para $n=5,\dots,10$: o número de diagonais que passam **exatamente** pelo centro do polígono é

| $n$ | paridade | diagonais pelo centro |
|---|---|---|
| 5 | ímpar | 0 |
| 6 | par | 3 |
| 7 | ímpar | 0 |
| 8 | par | 4 |
| 9 | ímpar | 0 |
| 10 | par | 5 |

Para $n$ ímpar, **nenhum** vértice tem um oposto exato — logo nenhuma diagonal passa pelo centro, forçando todo o padrão de cruzamentos a se organizar **fora** do centro, o que cria o núcleo pequeno e deslocado (o "bico"/ponto de fuga visualmente notado). Para $n$ par, exatamente $n/2$ diagonais (uma por par de vértices opostos) atravessam o centro exatamente, dando uma simetria mais aberta e uniforme.

### 32.3 Conexão com o resto do documento

Essa é a mesma dualidade par/ímpar da Seção 0.1 e 24 (reflexão par sem ponto fixo vs. reflexão ímpar com ponto fixo) — aqui aplicada a diagonais de polígono em vez de letras ou pontos numéricos. Ímpar sempre "sobra" um núcleo deslocado sem centro exato; par sempre tem um conjunto de diagonais passando pelo centro. É também a mesma estrutura do domínio fundamental $D_5$ da Seção 25 — o pentagrama que aparece aqui é literalmente a mesma figura construída lá a partir do domínio fundamental $1/10$.

---

## 33. Partição de $[2,N]$ pelo menor fator primo: uma construção diferente de Erastóstenes e de Euler

### 33.1 A construção

Para cada primo $p\leq\sqrt N$, liste os **cofatores** — números coprimos com todo primo menor que $p$, no intervalo $[p,\lfloor N/p\rfloor]$. Multiplicando $p$ por cada cofator, obtém-se exatamente os compostos de $[2,N]$ cujo **menor** fator primo é $p$. Como todo composto tem exatamente um menor fator primo, essa construção **particiona** $[2,N]$ sem sobreposição — soma direta, sem inclusão-exclusão.

### 33.2 Exemplos verificados

**$N=30$:**

| $p$ | cofatores | contagem |
|---|---|---|
| $2$ | $2,3,\dots,15$ | $14$ |
| $3$ | $3,5,7,9$ | $4$ |
| $5$ | $5$ | $1$ |
| primos $>\sqrt{30}$ | $7,11,13,17,19,23,29$ (cada um só ele mesmo) | $7$ |

Soma: $14+4+1+7=26$... na verdade $14+4+1=19$ compostos, mais $3$ primos pequenos ($2,3,5$) mais $7$ primos grandes $=19+3+7=29=N-1$ — cobre $[2,30]$ exatamente, sem sobra nem falta.

**$N=40$:** cofatores dão $19,6,2$ para $p=2,3,5$ respectivamente; $9$ primos grandes ($7,\dots,37$); cobertura $19+6+2+3+9=39=N-1$. Ambos os casos verificados por computador, exatos.

### 33.3 Por que isso não é o Crivo de Eratóstenes

O crivo clássico **marca** múltiplos de cada primo, sem se importar com qual primo já marcou aquele número antes — um composto como $30=2\times3\times5$ é marcado **três vezes** (por $2$, por $3$, por $5$). Essa redundância é exatamente o que o `crivo_v3.pdf` mede via $E-V$ (Seção sobre o grafo bipartido). A construção desta seção é diferente: cada composto é listado **uma única vez**, sob o primo que é seu menor fator — não há marcação repetida para contar ou descartar.

### 33.4 Por que isso não é a função totiente de Euler / os conjuntos $S_k$

Os conjuntos $S_k$ do `crivo.pdf` (Seção 1) contam **sobreviventes** — números coprimos com o primorial $N_k$ inteiro, um conjunto fixo que não muda de significado conforme $k$ cresce. A partição desta seção é outra coisa: para cada primo $p$, pega-se uma **fatia restrita** de $S_{k-1}$ (coprimos com os primos menores que $p$) dentro de um intervalo específico $[p,\lfloor N/p\rfloor]$ — não é o conjunto de sobreviventes final, é uma peça de um mosaico que junto reconstrói $[2,N]$ inteiro. $\phi(N_k)$ responde "quantos sobrevivem a tudo"; esta construção responde "cada não-sobrevivente, atribuído a exatamente um primo culpado".

### 33.5 Sobre "não usar divisão": onde isso é verdade, com precisão

Os cofatores de cada primo $p$ são exatamente $S_{k-1}$ (já construído no `crivo.pdf`, Seção 2, **via bijeção $p_k\times S_{k-1}$, sem testar divisibilidade**) restrito ao intervalo $[p,\lfloor N/p\rfloor]$ — verificado exatamente para $p=3,N=30$: cofatores $\{3,5,7,9\}$ coincidem, ponto a ponto, com $S_1=\{1,3,5,7,9\}$ restrito a esse intervalo. Ou seja: **construir** a partição inteira não precisa de nenhum teste de divisibilidade, porque reaproveita os $S_k$ já construídos sem divisão. O único lugar onde uma divisão aparece é na etapa auxiliar de **verificação** (conferir que $\gcd$ é $1$, ou que $N/p$ dá o limite do intervalo) — não na construção em si. Confundir essas duas coisas (construir vs. verificar) é exatamente o mal-entendido mais comum sobre esse tipo de crivo.

---

## Síntese

| Conceito | Papel na conexão |
|---|---|
| Repunit $R_n$ | Tesselação 1D da reta em potências de 10 |
| Função geradora | Empacota a sequência infinita numa forma fechada (racional) |
| Tensor | Objeto que "vive" no produto de espaços vetoriais (bases) |
| Produto de Kronecker | Mecanismo algébrico que constrói uma base de dimensão maior a partir de bases menores |
| Tesselação 2D | Padrão geométrico gerado por esse produto tensorial |
| $R_n^2$ | Caso numérico mais simples onde a "área" da tesselação 2D aparece como um número reconhecível e verificável |

A cadeia de raciocínio completa: **série geométrica (1D) → produto tensorial de duas séries → grade 2D de pesos → soma da grade = quadrado do repunit.** O mesmo mecanismo, em dimensões mais altas, é o que gera redes cristalográficas (redes de Bravais) a partir de vetores de base repetidos.

---

## Próximos passos possíveis

- Testar se o número de famílias distintas de polígonos estelares (Seção 32.1) num $n$-gono segue uma fórmula simples em função de $n$ (relacionada a $\phi(n)/2$, a função totiente, já que cada família $\{n/k\}$ válida exige $\gcd(n,k)=1$).

- Testar se números pentagonais e outros figurados poligonais têm a mesma propriedade de fatoração coprima (Seção 31), e se existe uma fórmula geral para $d$(número $k$-gonal) em função de $d(n)$ e $d(\text{fator associado})$, válida para todo $k$.
- Investigar se o "núcleo mínimo exato" da Seção 30.3 (denominador comum das proporções) tem relação com o custo computacional de misturar $N$ variáveis em geral — por exemplo, se proporções "quase redondas" (frações simples) sempre dão núcleos pequenos, e proporções irracionais/complicadas nunca permitem núcleo exato algum.

- Testar o método da Seção 28 em outros números táxi ($4104$, $13832$, $20683$) e verificar se o custo de busca ($d(N)$ candidatos) prediz corretamente quantas soluções existem, ou se números com $d(N)$ grande podem ainda assim ter zero representações como soma de dois cubos.
- Investigar se a lei $-5/3$ de Kolmogorov (espectro de energia real) pode ser derivada do p-model binomial (Seção 29), conectando a constante empírica da turbulência real a uma propriedade combinatória do triângulo de Pascal.

- Investigar se existe uma dissecação explícita (não só a garantia de existência de Sydler) conectando $\{1^3,12^3\}$ a $\{9^3,10^3\}$ — o teorema garante que existe, mas não constrói.

- Investigar se existe uma função geradora fechada para $D(p)$ como série em $1/\log p$, unificando 26.5 e 26.6 numa única expressão assintótica completa.


- Explorar se a construção de palíndromo por fusão (Seção 24.1) generaliza para "fusão de $k$ letras" em vez de 1, e como isso se relaciona com os subgrupos de reflexão de ordem maior (diedral $D_n$).

- Implementar de fato o método de cycle joining (Seção 16.1) para construir uma sequência de de Bruijn 1D verificada, e então testar um dos métodos publicados de extensão 2D (Seção 13.3/16) sobre essa sequência, fechando o ciclo em aberto desde a Seção 13.
- Generalizar a Seção 17 para $N$ com múltiplos fatores primos repetidos simultaneamente (ex: $N=p^{2m}q^{2k}\cdot M$) e verificar se os mdc possíveis formam o produto cartesiano $\{p^0,\dots,p^m\}\times\{q^0,\dots,q^k\}$.
- Explorar se existe uma terceira família de função geradora (nem puramente geométrica como repunits, nem puramente periódica como tesselação mod $m$) que compartilhe a mesma lógica estrutural das duas, dado que a Seção 18 mostrou que elas não se unificam diretamente — mapeando semelhanças em vez de construir uma interpolação formal entre as duas.
- Refinar a medição da Seção 19 com grades maiores e mais tentativas por ponto, para reduzir o ruído de Monte Carlo e tentar fixar o expoente $p$ de $\rho_c\sim C/m^p$ com mais precisão; investigar por que a heurística teórica simples (19.4) não bate com o expoente observado.
- Verificar se o polinômio de grau 10 da Seção 20.1 fatora sobre $\mathbb{Q}$ (por exemplo, em dois fatores de grau 5 correspondendo a $y=+\sqrt{x^2-1}$ e $y=-\sqrt{x^2-1}$ separadamente) — verificado na Seção 20.4: não fatora (irredutível), mas a Seção 20.5 achou uma forma fechada elegante do lado esquerdo antes da subtração final.
- Investigar se a forma fechada $y^4(y^2+1)(y^2+2)^2=1$ (Seção 20.5) tem análogo para $x$ diretamente (uma fatoração parcial semelhante do lado esquerdo de $x^5-x=1$, antes de subtrair).

- Investigar se a tesselação por resíduo mod $m$ (Seção 9) tem uma função geradora análoga à dos repunits (Seção 2), unificando os dois formalismos numa única identidade.
- Explorar o caso onde $m$ varia por eixo (mod $m_1$ no eixo 1, mod $m_2$ no eixo 2, etc.) — generalização do teorema chinês do resto aplicado à tesselação.
- Formalizar a ponte do hipercubo (Seção 10.2) como um teorema único, com um isomorfismo explícito entre os três contextos.
- Investigar se a estrutura recursiva da Seção 10.4 (reduzir pela escala antes de testar a classe) aparece também na decomposição em cascata de repunits (Seção 10.3) — ambas envolvem "remover um fator comum, depois testar paridade/resíduo".
- Verificar se o ladrilhamento de dois blocos da Seção 11.3 (crivo) tem uma contagem análoga aos $R_n^p$ (repunits elevados a potência) — ambos nascem de uma divisão que não fecha exatamente.
- Determinar precisamente o limiar de densidade da Propriedade 2 (Seção 12.3) e verificar se ele depende de $m$ (número de cores de borda) de forma previsível — conectaria ladrilhamento por restrição local a teoria de percolação de forma rigorosa.
- Implementar um algoritmo de construção dedicado (não busca ingênua) para o regime $\rho=1$ (Seção 13.3), e testar se ele consegue resolver onde o backtracking ingênuo falhou — determinaria se a inviabilidade é do algoritmo de busca ou do problema em si.
