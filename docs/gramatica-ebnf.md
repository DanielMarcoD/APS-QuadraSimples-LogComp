# Gramática EBNF da Linguagem QuadraSimples

Esta é a especificação formal da gramática da linguagem QuadraSimples, uma DSL (Domain-Specific Language) para descrição de treinos e partidas de tênis.

## Notação EBNF

- `{ }` : repetição zero ou mais vezes
- `[ ]` : opcional (zero ou uma vez)
- `|` : alternativa
- `( )` : agrupamento
- `,` : concatenação
- `;` : fim de regra

## Estrutura do Programa

```ebnf
program = { statement } ;
```

## Statements (Comandos)

```ebnf
statement =
      expr_stmt
    | if_stmt
    | while_stmt
    | sessao_stmt
    | block
    | block , ";" ;

block = "{" , { statement } , "}" ;
```

## Statements de Expressão

```ebnf
expr_stmt =
      assignment , ";"
    | log_stmt , ";"
    | print_stmt , ";"
    | etapa_stmt , ";"
    | descanso_stmt , ";"
    | saque_stmt , ";"
    | rally_stmt , ";"
    | start_stmt , ";"
    | point_stmt , ";"
    | ace_stmt , ";"
    | fault_stmt , ";"
    | doublefault_stmt , ";"
    | score_stmt , ";"
    | rename_stmt , ";"
    | config_stmt , ";" ;
```

## Atribuição e Log

```ebnf
assignment = identifier , "=" , bool_expr ;

log_stmt = "log" , "(" , bool_expr , ")" ;

print_stmt = "print" , "(" , string , ")" ;
```

## Estruturas de Controle de Fluxo

```ebnf
while_stmt = "while" , "(" , bool_expr , ")" , block , ";" ;

if_stmt = "if" , "(" , bool_expr , ")" , block , [ if_continuation ] , ";" ;

if_continuation = "else" , block ;
```

## Comandos de Domínio (DSL de Tênis)

### Sessão de Treino

```ebnf
sessao_stmt = "sessao" , "(" , string , ")" , block , ";" ;
```

### Etapas de Treino

```ebnf
etapa_stmt = "etapa" , "(" , string , "," , etapa_type , "=" , expression , ")" ;

etapa_type = "dur" | "reps" ;
```

### Descanso

```ebnf
descanso_stmt = "descanso" , "(" , expression , ")" ;
```

### Comandos de Jogo

```ebnf
saque_stmt = "saque" , "(" , player , "," , string , ")" ;

rally_stmt = "rally" , "(" , expression , ")" ;

start_stmt = "start_match" , "(" , expression , "," , expression , ")" ;

point_stmt = "point" , "(" , player , ")" ;

ace_stmt = "ace" , "(" , player , ")" ;

fault_stmt = "fault" , "(" , player , ")" ;

doublefault_stmt = "double_fault" , "(" , player , ")" ;

score_stmt = "score" , "(" , ")" ;

rename_stmt = "rename" , "(" , player , "," , string , ")" ;

config_stmt = "config" , "(" , cfgkey , "=" , bool_expr , ")" ;
```

### Auxiliares de Jogo

```ebnf
player = "A" | "B" ;

cfgkey = "no_ad" | "shot_clock" ;
```

## Expressões

### Expressões Booleanas

```ebnf
bool_expr = bool_term , { "||" , bool_term } ;

bool_term = rel_expr , { "&&" , rel_expr } ;

rel_expr = expression , [ rel_op , expression ] ;

rel_op = "===" | ">" | "<" ;
```

### Expressões Aritméticas

```ebnf
expression = term , { add_op , term } ;

add_op = "+" | "-" ;

term = factor , { mul_op , factor } ;

mul_op = "*" | "/" ;
```

### Fatores

```ebnf
factor =
      number
    | "true"
    | "false"
    | identifier
    | readline_call
    | sensor
    | "(" , bool_expr , ")"
    | "+" , factor
    | "-" , factor
    | "!" , factor ;

readline_call = "readline" , "(" , ")" ;

sensor = "last_rally" | "total_points" ;
```

## Tokens Léxicos

```ebnf
identifier = letter , { letter | digit | "_" } ;

number = digit , { digit } ;

string = '"' , { char_except_quote } , '"' ;

letter = "a" | "b" | ... | "z" | "A" | "B" | ... | "Z" ;

digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

char_except_quote = ? qualquer caractere exceto '"' e caracteres de escape ? ;
```

## Comentários

Comentários de linha única começam com `//` e vão até o final da linha.

```ebnf
comment = "//" , { any_char } , newline ;
```

## Palavras Reservadas

As seguintes palavras são reservadas e não podem ser usadas como identificadores:

- `var`, `if`, `else`, `while`, `log`, `print`
- `true`, `false`
- `sessao`, `etapa`, `descanso`, `saque`, `rally`
- `start_match`, `point`, `ace`, `fault`, `double_fault`, `score`, `rename`, `config`
- `no_ad`, `shot_clock`
- `A`, `B`
- `readline`
- `last_rally`, `total_points` (sensores read-only)

## Operadores

### Aritméticos
- `+` : adição (binário) ou positivo (unário)
- `-` : subtração (binário) ou negação (unário)
- `*` : multiplicação
- `/` : divisão inteira

### Relacionais
- `===` : igualdade
- `>` : maior que
- `<` : menor que

### Lógicos
- `&&` : AND lógico
- `||` : OR lógico
- `!` : NOT lógico (unário)

### Atribuição
- `=` : atribuição

## Delimitadores e Pontuação

- `(` `)` : parênteses
- `{` `}` : chaves
- `,` : vírgula
- `;` : ponto e vírgula

## Notas Semânticas

1. **Variáveis**: Não há declaração explícita de variáveis. Toda atribuição cria ou atualiza uma variável no escopo global da VM.

2. **Tipos**: A linguagem possui apenas dois tipos de valores na VM:
   - Inteiros (números)
   - Booleanos (`true` = 1, `false` = 0)
   
3. **Strings**: Strings são literais que podem ser usadas em:
   - `print()` - imprime a string na saída
   - Comandos específicos (sessao, etapa, saque, rename)
   - Não podem ser manipuladas como expressões ou armazenadas em variáveis

4. **Coerção**: Valores booleanos são tratados como inteiros (0 ou 1) nas operações aritméticas.

5. **Sensores (read-only)**: `last_rally` e `total_points` são palavras-chave especiais que retornam valores de estado interno da VM:
   - `last_rally`: Número de trocas do último comando `rally()` executado (inicialmente 0)
   - `total_points`: Total de pontos disputados na partida (incrementado por `point()`, `ace()`, `doublefault()`)
   - Esses valores podem ser lidos em expressões, mas **não** podem ser modificados diretamente (não aceitam atribuição)

5. **Divisão por zero**: Resulta em 0 na VM (comportamento definido).

6. **log() vs print()**:
   - `log(expr)` - avalia expressão numérica/booleana e imprime o valor
   - `print(string)` - imprime string literal diretamente

6. **Precedência de operadores** (do mais forte ao mais fraco):
   - Unários: `!`, `-`, `+`
   - Multiplicativos: `*`, `/`
   - Aditivos: `+`, `-`
   - Relacionais: `===`, `>`, `<`
   - Lógico AND: `&&`
   - Lógico OR: `||`

7. **Associatividade**: Todos os operadores binários são associativos à esquerda.

8. **Blocos**: Blocos `{ }` podem opcionalmente ser seguidos de `;`. Isso permite flexibilidade sintática.

## Exemplo Completo

```qs
// Treino e partida de tênis

print("=== Bem-vindo ao QuadraSimples! ===");
print("");

{
  sessao("Aquecimento") {
    etapa("Mobilidade", dur=180);
    etapa("Shadow", reps=40);
    descanso(30);
  };
};

series = 2;
alvo = 6;
s = 0;

while (s < series) {
  saque(A, "deuce");
  etapa("Saque no T", reps=10);
  
  print("Quantos acertos?");
  acertos = readline();

  if (acertos > alvo) {
    print("Excelente!");
    alvo = alvo + 1;
  }
  else {
    print("Continue treinando");
  };

  s = s + 1;
};

config(no_ad = true);
start_match(3, 6);
point(A);
ace(B);
score();

print("Treino finalizado!");
```

Esta gramática foi implementada usando **Flex** (analisador léxico) e **Bison** (analisador sintático) e gera código assembly para a **QuadraVM**.
