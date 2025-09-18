# QuadraSimples

```ebnf
PROGRAM = { STATEMENT } ;
STATEMENT = ( ASSIGNMENT | LOG | WHILE | IF | BLOCK
            | SESSAO | ETAPA | DESCANSO | SAQUE | RALLY
            | START | POINT | ACE | FAULT | DOUBLEFAULT | SCORE
            | RENAME | CONFIG
            | ε ), ";" ;
ASSIGNMENT = IDENTIFIER, "=", BOOL_EXPR ;
LOG        = "log", "(", BOOL_EXPR, ")" ;
WHILE      = "while", "(", BOOL_EXPR, ")", STATEMENT ;
IF         = "if", "(", BOOL_EXPR, ")", STATEMENT, [ "else", STATEMENT ] ;
BLOCK      = "{", { STATEMENT }, "}" ;
SESSAO     = "sessao", "(", STRING, ")", BLOCK ;
ETAPA      = "etapa", "(", STRING, ",", ("dur" | "reps"), "=", EXPRESSION, ")" ;
DESCANSO   = "descanso", "(", EXPRESSION, ")" ;
SAQUE      = "saque", "(", PLAYER, ",", STRING, ")" ;      
RALLY      = "rally", "(", EXPRESSION, ")" ;
START      = "start_match", "(", EXPRESSION, ",", EXPRESSION, ")" ;
POINT      = "point", "(", PLAYER, ")" ;
ACE        = "ace", "(", PLAYER, ")" ;
FAULT      = "fault", "(", PLAYER, ")" ;
DOUBLEFAULT= "double_fault", "(", PLAYER, ")" ;
SCORE      = "score", "(", ")" ;
RENAME     = "rename", "(", PLAYER, ",", STRING, ")" ;
CONFIG     = "config", "(", CFGKEY, "=", BOOL_EXPR, ")" ;
CFGKEY     = "no_ad" | "shot_clock" ;
PLAYER     = "A" | "B" ;
BOOL_EXPR  = BOOL_TERM, { "||", BOOL_TERM } ;
BOOL_TERM  = REL_EXPR,  { "&&", REL_EXPR } ;
REL_EXPR   = EXPRESSION, { ("===" | ">" | "<"), EXPRESSION } ;
EXPRESSION = TERM, { ("+" | "-"), TERM } ;
TERM       = FACTOR, { ("*" | "/"), FACTOR } ;
FACTOR     = (("+" | "-" | "!"), FACTOR)
           | NUMBER
           | "(", BOOL_EXPR, ")"
           | IDENTIFIER
           | READ
           | "true" | "false" ;
READ       = "readline", "(", ")" ;
IDENTIFIER = LETTER, { LETTER | DIGIT | "_" } ;
NUMBER     = DIGIT, { DIGIT } ;
STRING     = "\"", { CHAR_EXCEPT_QUOTE }, "\"" ;
CHAR_EXCEPT_QUOTE = ? qualquer caractere exceto " ? ;
LETTER     = ( a | ... | z | A | ... | Z ) ;
DIGIT      = ( 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 ) ;


```
