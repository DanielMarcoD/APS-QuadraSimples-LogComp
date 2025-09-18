# Linguagem para facilitar quem joga tênis.

```ebnf
PROGRAM        = { DECL | STMT } ;
DECL           = "var", IDENT, [ "=", EXPR ], ";" ;
STMT           = ASSIGN, ";"
               | PRINT, ";"
               | IF
               | WHILE
               | SESSAO
               | ETAPA, ";"
               | DESCANSO, ";"
               | SAQUE, ";"
               | RALLY, ";"
               | START, ";"
               | POINT, ";"
               | ACE, ";"
               | FAULT, ";"
               | DOUBLEFAULT, ";"
               | SCORE, ";"
               | RENAME, ";"
               | CONFIG, ";"
               | BLOCK
               | ";" ;                        
ASSIGN         = IDENT, "=", EXPR ;
PRINT          = "print", "(", [ EXPR ], ")" ;
IF             = "if", "(", EXPR, ")", STMT, [ "else", STMT ] ;
WHILE          = "while", "(", EXPR, ")", STMT ;
BLOCK          = "{", { DECL | STMT }, "}" ;
SESSAO         = "sessao", "(", STRING, ")", BLOCK ;
ETAPA          = "etapa", "(", STRING, ",", "dur", "=", EXPR, ")"
               | "etapa", "(", STRING, ",", "reps", "=", EXPR, ")" ;
DESCANSO       = "descanso", "(", EXPR, ")" ;             
SAQUE          = "saque", "(", PLAYER, ",", STRING, ")" ;  
RALLY          = "rally", "(", EXPR, ")" ;               
START          = "start_match", "(", EXPR, ",", EXPR, ")" ; 
POINT          = "point", "(", PLAYER, ")" ;
ACE            = "ace", "(", PLAYER, ")" ;
FAULT          = "fault", "(", PLAYER, ")" ;
DOUBLEFAULT    = "double_fault", "(", PLAYER, ")" ;
SCORE          = "score", "(", ")" ;
RENAME         = "rename", "(", PLAYER, ",", STRING, ")" ;
CONFIG         = "config", "(", CFGKEY, "=", EXPR, ")" ;
CFGKEY         = "no_ad" | "shot_clock" ;
PLAYER         = "A" | "B" ;
EXPR           = OR ;
OR             = AND, { "||", AND } ;
AND            = EQ , { "&&", EQ } ;
EQ             = REL, { ("==" | "!="), REL } ;
REL            = ADD, { ("<" | "<=" | ">" | ">="), ADD } ;
ADD            = MUL, { ("+" | "-"), MUL } ;
MUL            = UN , { ("*" | "/" | "%"), UN } ;
UN             = [ ("+" | "-" | "!") ], PRI ;
PRI            = NUMBER
               | STRING
               | "true" | "false"
               | IDENT
               | "(", EXPR, ")" ;
IDENT          = LETTER, { LETTER | DIGIT | "_" } ;
NUMBER         = DIGIT, { DIGIT } ;
STRING         = "\"", { /* qualquer caractere exceto " */ }, "\"" ;
LETTER         = "A" | ... | "Z" | "a" | ... | "z" ;
DIGIT          = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

```
