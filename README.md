# QuadraSimples

**QuadraSimples** é uma DSL (Domain-Specific Language) para descrição de treinos e partidas de tênis. A linguagem permite criar rotinas de treino estruturadas, simular partidas com placar completo e combinar lógica de programação (loops, condicionais, variáveis) com comandos específicos do domínio de tênis.

## Índice

- [Arquitetura Geral](#arquitetura-geral)
- [Gramática da Linguagem](#gramática-da-linguagem)
- [Arquitetura do Compilador](#arquitetura-do-compilador)
- [Arquitetura da QuadraVM](#arquitetura-da-quadravm)
- [Turing-Completude](#turing-completude)
- [Como Compilar e Executar](#como-compilar-e-executar)
- [Exemplos de Programas](#exemplos-de-programas)
- [Estrutura dos Arquivos](#estrutura-dos-arquivos)

## Arquitetura Geral

```
┌─────────────────┐
│  Código .qs     │  (QuadraSimples - linguagem de alto nível)
│  (demo.qs)      │
└────────┬────────┘
         │
         │ compilação
         ↓
┌─────────────────┐
│  quadrasimples  │  (Compilador: Flex + Bison + codegen.c)
│  (executável)   │
└────────┬────────┘
         │
         │ gera
         ↓
┌─────────────────┐
│  out.qvm        │  (Assembly intermediário da QuadraVM)
└────────┬────────┘
         │
         │ execução
         ↓
┌─────────────────┐
│  quadravm       │  (Máquina Virtual em C)
│  (executável)   │
└────────┬────────┘
         │
         ↓
    Saída no terminal
```

## Gramática da Linguagem

### EBNF Completa

```ebnf
PROGRAM = { STATEMENT } ;

STATEMENT = BLOCK 
          | IF_STMT 
          | WHILE_STMT 
          | SESSAO_STMT 
          | TENNIS_CMD 
          | ASSIGNMENT 
          | EXPRESSION, ";" ;

BLOCK = "{", { STATEMENT }, "}" ;

IF_STMT = "if", "(", BOOL_EXPR, ")", BLOCK, [ "else", BLOCK ], ";" ;

WHILE_STMT = "while", "(", BOOL_EXPR, ")", BLOCK, ";" ;

SESSAO_STMT = "sessao", "(", STRING, ")", BLOCK, ";" ;

TENNIS_CMD = START_CMD 
           | POINT_CMD 
           | ACE_CMD 
           | DOUBLEFAULT_CMD 
           | SAQUE_CMD 
           | RALLY_CMD 
           | SCORE_CMD 
           | CONFIG_CMD 
           | ETAPA_CMD 
           | DESCANSO_CMD 
           | RENAME_CMD ;

START_CMD = "start", "(", STRING, ",", STRING, ")", ";" ;
POINT_CMD = "point", "(", PLAYER, ")", ";" ;
ACE_CMD = "ace", "(", PLAYER, ")", ";" ;
DOUBLEFAULT_CMD = "doublefault", "(", PLAYER, ")", ";" ;
SAQUE_CMD = "saque", "(", PLAYER, ",", STRING, ")", ";" ;
RALLY_CMD = "rally", "(", PLAYER, ",", STRING, ")", ";" ;
SCORE_CMD = "score", "(", ")", ";" ;
CONFIG_CMD = "config", "(", STRING, ",", EXPRESSION, ")", ";" ;
ETAPA_CMD = "etapa", "(", STRING, [ ",", PARAM ], ")", ";" ;
DESCANSO_CMD = "descanso", "(", EXPRESSION, ")", ";" ;
RENAME_CMD = "rename", "(", PLAYER, ",", STRING, ")", ";" ;

ASSIGNMENT = IDENTIFIER, "=", EXPRESSION, ";" ;

BOOL_EXPR = REL_EXPR, { ("&&" | "||"), REL_EXPR } ;

REL_EXPR = EXPRESSION, [ ("==" | "!=" | "<" | ">" | "<=" | ">="), EXPRESSION ] ;

EXPRESSION = TERM, { ("+" | "-"), TERM } ;

TERM = FACTOR, { ("*" | "/"), FACTOR } ;

FACTOR = "(", EXPRESSION, ")" 
       | NUMBER 
       | IDENTIFIER 
       | READLINE_EXPR 
       | STRING 
       | PLAYER ;

READLINE_EXPR = "readline", "(", ")" ;

PLAYER = "A" | "B" ;

PARAM = "dur", "=", NUMBER 
      | "reps", "=", NUMBER ;

NUMBER = DIGIT, { DIGIT } ;

DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

IDENTIFIER = LETTER, { LETTER | DIGIT | "_" } ;

LETTER = "a" | ... | "z" | "A" | ... | "Z" ;

STRING = '"', { CHARACTER }, '"' ;
```

> **Documentação completa**: [docs/gramatica-ebnf.md](docs/gramatica-ebnf.md)

### Exemplo de Sintaxe

```qs
// Mensagens e feedback
print("=== Bem-vindo ao QuadraSimples! ===");

// Sessão de treino
sessao("Aquecimento") {
  etapa("Mobilidade", dur=180);
  etapa("Shadow", reps=40);
  descanso(30);
};

// Variáveis e controle de fluxo
series = 2;
s = 0;

while (s < series) {
  saque(A, "deuce");
  
  print("Quantos acertos você fez?");
  acertos = readline();
  
  if (acertos > 5) {
    print("Excelente!");
  }
  else {
    print("Continue treinando");
  };
  
  s = s + 1;
};

// Simulação de partida
config(no_ad = true);
start_match(3, 6);
point(A);
ace(B);
score();

print("Treino concluído!");
log(acertos);  // Mostra valor numérico
```

## Arquitetura do Compilador

O compilador **quadrasimples** é implementado em C usando:

### 1. Analisador Léxico (Flex)
- **Arquivo**: `lexer.l`
- **Função**: Transforma o código-fonte `.qs` em uma sequência de tokens
- **Tokens reconhecidos**: palavras-chave (`if`, `while`, `sessao`), operadores (`+`, `-`, `===`), literais (números, strings), identificadores

### 2. Analisador Sintático (Bison)
- **Arquivo**: `parser.y`
- **Função**: Verifica a estrutura sintática do código e constrói a árvore de derivação
- **Ações semânticas**: Durante o parsing, chama funções de `codegen.c` para gerar código assembly

### 3. Gerador de Código
- **Arquivo**: `codegen.c` / `codegen.h`
- **Função**: Gera instruções assembly para a QuadraVM (arquivo `out.qvm`)
- **Estratégia**: Geração de código baseada em pilha com labels para controle de fluxo

### 4. Programa Principal
- **Arquivo**: `main.c`
- **Função**: Inicializa o compilador, abre o arquivo `.qs`, chama o parser e finaliza a compilação

### Pipeline de Compilação

```
.qs → [Flex: lexer.l] → Tokens → [Bison: parser.y] → Ações semânticas
                                                      ↓
                                           [codegen.c] → out.qvm
```

## Arquitetura da QuadraVM

A **QuadraVM** é uma máquina virtual baseada em pilha, implementada em C (`quadravm.c`), que executa o assembly gerado pelo compilador.

### Registradores (Writeable Registers)

A VM mantém estado interno modificável através das instruções:

- **Registradores de propósito geral**:
  - `R0`: Registrador 0 (inteiro de 32 bits)
  - `R1`: Registrador 1 (inteiro de 32 bits)
  - Acessíveis via: `LOAD_R0`, `LOAD_R1`, `PUSH_R0`, `PUSH_R1`, `INC_R0`, `DEC_R0`, `INC_R1`, `DEC_R1`

- **Estado da partida de tênis**:
  - `points_A`, `points_B`: Pontos no game atual (0, 15, 30, 40)
  - `games_A`, `games_B`: Games vencidos no set atual
  - `sets_A`, `sets_B`: Sets vencidos na partida
  - `player_A_name`, `player_B_name`: Nomes dos jogadores
  
- **Configurações**:
  - `no_ad`: Modo sem vantagem (0 ou 1)
  - `shot_clock`: Shot clock ativado (0 ou 1)

### Memória

#### 1. Pilha de Operandos
- **Uso**: Armazenamento temporário para cálculos e expressões
- **Operações**: `PUSH_NUM`, `PUSH_BOOL`, `PUSH_VAR`, `ADD`, `SUB`, `MUL`, `DIV`, `EQ`, `GT`, `LT`, `AND`, `OR`
- **Tamanho**: 1024 elementos (inteiros)

#### 2. Tabela de Variáveis
- **Uso**: Armazenamento de variáveis do programa (mapa nome → valor)
- **Operações**: `ASSIGN` (escreve), `PUSH_VAR` (lê)
- **Capacidade**: 128 variáveis

#### 3. Tabela de Labels
- **Uso**: Mapeamento de nomes de labels para índices de instrução
- **Operações**: Resolvida na fase de carregamento, usada por `JMP` e `JZ`
- **Capacidade**: 512 labels

### Sensores (Read-Only State)

Variáveis de estado que o programa não modifica diretamente, mas pode consultar através das instruções:

- **`last_rally`**: Número de trocas do último rally executado. Atualizado automaticamente pela instrução `RALLY`. Valor inicial: 0.
- **`total_points`**: Total de pontos disputados na partida. Incrementado automaticamente pelas instruções `POINT`, `ACE` e `DOUBLEFAULT`. Valor inicial: 0.
- **Placar atual** (pontos, games, sets) - lido por `SCORE`
- **Configurações** `no_ad` e `shot_clock` - modificadas por `CONFIG`
- **Estado de sessões e etapas** - gerenciado por `SESSAO_*` e `ETAPA_*`

#### Como usar sensores

Sensores são **somente leitura** - não podem ser modificados com `=` (atribuição). São atualizados automaticamente pela VM quando comandos de domínio são executados.

Exemplo de uso:

```qs
// Treino com análise de desempenho
rally(10);
trocas_anteriores = last_rally;  // Lê sensor (valor: 10)

point(A);
point(B);
ace(A);

total = total_points;  // Lê sensor (valor: 3)
log(total);
```

**Instruções da VM**:
- `PUSH_LAST_RALLY`: Empilha o valor do sensor `last_rally`
- `PUSH_TOTAL_POINTS`: Empilha o valor do sensor `total_points`

### Conjunto de Instruções (ISA)

#### Pilha e Expressões
| Instrução | Argumentos | Descrição |
|-----------|------------|-----------|
| `PUSH_NUM` | `<int>` | Empilha um número |
| `PUSH_BOOL` | `<0/1>` | Empilha um booleano |
| `PUSH_VAR` | `<nome>` | Empilha o valor de uma variável |
| `READLINE` | - | Lê um inteiro do usuário e empilha |
| `PUSH_LAST_RALLY` | - | Empilha valor do sensor `last_rally` |
| `PUSH_TOTAL_POINTS` | - | Empilha valor do sensor `total_points` |
| `ADD` | - | Desempilha 2, soma, empilha resultado |
| `SUB` | - | Desempilha 2, subtrai, empilha resultado |
| `MUL` | - | Desempilha 2, multiplica, empilha resultado |
| `DIV` | - | Desempilha 2, divide, empilha resultado |
| `UNARY_MINUS` | - | Desempilha 1, nega, empilha |
| `NOT` | - | Desempilha 1, inverte lógico, empilha |
| `EQ` | - | Desempilha 2, testa igualdade, empilha 0/1 |
| `GT` | - | Desempilha 2, testa maior que, empilha 0/1 |
| `LT` | - | Desempilha 2, testa menor que, empilha 0/1 |
| `AND` | - | Desempilha 2, AND lógico, empilha |
| `OR` | - | Desempilha 2, OR lógico, empilha |

#### Registradores
| Instrução | Argumentos | Descrição |
|-----------|------------|-----------|
| `LOAD_R0` | - | Desempilha e carrega em R0 |
| `LOAD_R1` | - | Desempilha e carrega em R1 |
| `PUSH_R0` | - | Empilha o valor de R0 |
| `PUSH_R1` | - | Empilha o valor de R1 |
| `INC_R0` | - | Incrementa R0 |
| `DEC_R0` | - | Decrementa R0 |
| `INC_R1` | - | Incrementa R1 |
| `DEC_R1` | - | Decrementa R1 |

#### Atribuição e Log
| Instrução | Argumentos | Descrição |
|-----------|------------|-----------|
| `ASSIGN` | `<nome>` | Desempilha valor e atribui à variável |
| `LOG` | - | Desempilha e imprime valor numérico |
| `PRINT` | `<"string">` | Imprime string literal |

#### Controle de Fluxo
| Instrução | Argumentos | Descrição |
|-----------|------------|-----------|
| `LABEL` | `<nome>` | Define um ponto de salto |
| `JMP` | `<label>` | Salta incondicionalmente para label |
| `JZ` | `<label>` | Desempilha; se 0, salta para label |
| `HALT` | - | Encerra a execução |

#### DSL de Tênis
| Instrução | Argumentos | Descrição |
|-----------|------------|-----------|
| `SESSAO_BEGIN` | `<"nome">` | Inicia sessão de treino |
| `SESSAO_END` | - | Finaliza sessão de treino |
| `ETAPA_DUR` | `<"nome">` | Define etapa com duração (desempilha segundos) |
| `ETAPA_REPS` | `<"nome">` | Define etapa com repetições (desempilha número) |
| `DESCANSO` | - | Período de descanso (desempilha segundos) |
| `SAQUE` | `<player> <"lado">` | Registra saque |
| `RALLY` | - | Rally (desempilha número de trocas) |
| `START_MATCH` | - | Inicia partida (desempilha games, sets) |
| `POINT` | `<player>` | Registra ponto para jogador |
| `ACE` | `<player>` | Registra ace para jogador |
| `FAULT` | `<player>` | Registra falta de saque |
| `DOUBLEFAULT` | `<player>` | Registra dupla falta |
| `SCORE` | - | Exibe placar completo da partida |
| `RENAME` | `<player> <"nome">` | Renomeia jogador |
| `CONFIG` | `<chave>` | Configura opção (desempilha valor 0/1) |

## Turing-Completude

A QuadraVM é **Turing-completa**, equivalente a uma **máquina de contadores de Minsky**, porque possui:

1. **Variáveis inteiras com operações**: Através de `ASSIGN`, `ADD`, `SUB`, `PUSH_NUM`, podemos implementar incremento e decremento de contadores.

2. **Teste condicional**: `EQ`, `GT`, `LT` permitem testar valores de variáveis.

3. **Saltos condicionais**: `JZ` (jump if zero) combinado com os testes relacionais permite desvios condicionais.

4. **Saltos incondicionais**: `JMP` permite loops e estruturas de controle arbitrárias.

5. **Memória ilimitada (teoricamente)**: A tabela de variáveis pode armazenar estado arbitrário.

Com esses elementos, é possível simular qualquer máquina de Turing, logo a linguagem QuadraSimples é Turing-completa.

## Como Compilar e Executar

### Compilação do Projeto

```bash
make
```

Este comando:
1. Gera `parser.tab.c` e `parser.tab.h` a partir de `parser.y` (Bison)
2. Gera `lex.yy.c` a partir de `lexer.l` (Flex)
3. Compila todos os `.c` e linka para criar os executáveis `quadrasimples` e `quadravm`

### Compilação de um Programa QuadraSimples

```bash
./quadrasimples examples/demo.qs
```

Este comando:
- Lê o arquivo `examples/demo.qs` (código-fonte QuadraSimples)
- Gera o arquivo `out.qvm` (assembly da QuadraVM)
- Exibe "OK: compilation successful." se não houver erros

### Execução na QuadraVM

```bash
./quadravm out.qvm
```

Este comando:
- Lê o arquivo `out.qvm`
- Executa as instruções na máquina virtual
- Exibe a saída do programa (logs, placares, etc.)

### Pipeline Completo

```bash
make
./quadrasimples examples/demo.qs
./quadravm out.qvm
```

## Exemplos de Programas

A pasta `examples/` contém 9 programas QuadraSimples que demonstram diferentes aspectos da linguagem:

### `examples/demo.qs`
**Descrição**: Demonstração geral da linguagem, combina sessão de treino, loops com condicionais e simulação de partida.

**Características**:
- Sessão estruturada com etapas e descanso
- Loop `while` com contador
- Condicional `if/else`
- Configuração e início de partida
- Comandos de jogo (saque, point, ace, score)

### `examples/treino_basico.qs`
**Descrição**: Foco em estrutura de treino progressivo com múltiplas séries.

**Características**:
- Sessão de aquecimento detalhada
- Loop para repetir séries de exercícios
- Uso de `etapa` com duração e repetições
- Períodos de descanso

### `examples/controle_fluxo.qs`
**Descrição**: Demonstração de estruturas de controle de fluxo.

**Características**:
- Loop `while` com entrada do usuário via `readline()`
- Condicional `if/else` para análise de valores
- Acumulação de resultados em variáveis
- Uso de `log()` para exibir resultados

### `examples/partida_completa.qs`
**Descrição**: Simulação de uma partida de tênis com placar.

**Características**:
- Configuração de regras (`no_ad`, `shot_clock`)
- Início de partida com `start_match()`
- Sequência de pontos, aces e faltas
- Exibição de placar com `score()`
- Uso de variáveis para controle de pontuação

### `examples/loops.qs`
**Descrição**: Demonstração de loops aninhados com interação do usuário.

**Características**:
- Loops `while` aninhados (loop dentro de loop)
- Entrada do usuário via `readline()`
- Condicionais aninhados (`if/else` dentro de `if/else`)
- Acumulação de estatísticas em variáveis
- Sessão de treino ao final

### `examples/match_simulation.qs`
**Descrição**: Simulação completa de 6 games com jogadores renomeados.

**Características**:
- Renomeação de jogadores para "Rafael Nadal" e "Roger Federer"
- Configuração de regras da partida
- Simulação de 6 games completos com rallies variados
- Uso extensivo de `score()` para mostrar evolução do placar
- Placar visual com nomes reais e emojis

### `examples/rally.qs`
**Descrição**: Treino interativo de rally com feedback dinâmico.

**Características**:
- Sessão de aquecimento inicial
- Loop com entrada do usuário (`readline()`)
- Meta progressiva que aumenta conforme performance
- Condicionais aninhados para feedback diferenciado
- Recompensa (sessão extra) por performance perfeita
- Demonstração de ponto ao final

### `examples/sensores_demo.qs`
**Descrição**: Demonstração completa do uso de sensores read-only (`last_rally` e `total_points`).

**Características**:
- Uso de `last_rally` para monitorar rallies executados
- Uso de `total_points` para estatísticas de partida
- Sensores em condicionais e expressões aritméticas
- Exemplo de treino com monitoramento de desempenho
- Demonstração de que sensores não podem ser modificados diretamente

### `examples/test_print.qs`
**Descrição**: Testes da instrução `print()` com diferentes tipos de conteúdo.

**Características**:
- Impressão de strings literais
- Demonstração de uso básico do comando `print()`

### Executando os Exemplos

Para executar qualquer exemplo:

```bash
./quadrasimples examples/<nome_do_arquivo>.qs
./quadravm out.qvm
```

Exemplos que requerem entrada do usuário (`controle_fluxo.qs`, `loops.qs`, `rally.qs`) solicitarão números durante a execução.

## Estrutura dos Arquivos

```
APS-QuadraSimples-LogComp/
│
├── README.md                 # Este arquivo
├── Makefile                  # Build system
│
├── docs/
│   └── gramatica-ebnf.md     # Gramática formal em EBNF
│
├── lexer.l                   # Analisador léxico (Flex)
├── parser.y                  # Analisador sintático (Bison)
├── codegen.c                 # Gerador de código assembly
├── codegen.h                 # Header do gerador
├── main.c                    # Programa principal do compilador
│
├── quadravm.c                # Implementação da máquina virtual
│
├── quadrasimples             # Executável do compilador (gerado)
├── quadravm                  # Executável da VM (gerado)
├── out.qvm                   # Assembly gerado (temporário)
│
└── examples/
    ├── demo.qs               # Demo geral
    ├── treino_basico.qs      # Treino estruturado
    ├── controle_fluxo.qs     # Controle de fluxo
    └── partida_completa.qs   # Simulação de partida
```

## Limpeza

Para remover arquivos gerados:

```bash
make clean
```

---

**Desenvolvido para a disciplina de Lógica da Computação - Insper**

### Exemplo de uso
O arquivo `examples/demo.qs` contém um exemplo completo que demonstra:
- Definição de sessões de treino
- Estruturas de controle (while, if/else)
- Operações aritméticas e lógicas
- Configurações específicas do tênis