%code requires {
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
}
%code {
  int yylex(void);
  void yyerror(const char* s);
}
%define parse.error detailed

%union {
  int   ival;
  char* sval;
}

%token VAR IF ELSE WHILE LOG
%token SESSAO ETAPA DESCANSO SAQUE RALLY
%token START POINT ACE FAULT DOUBLEFAULT SCORE
%token RENAME CONFIG NO_AD SHOT_CLOCK
%token PLAYER_A PLAYER_B
%token TRUE FALSE
%token READLINE

%token <sval> IDENT STRING
%token <ival> NUMBER

%token OR AND EQ3 LT GT
%token PLUS MINUS TIMES DIV NOT
%token ASSIGN
%token LPAREN RPAREN LBRACE RBRACE COMMA SEMI

%start program

%%

program
  : %empty
  | program statement
  ;

statement
  : simple_stmt SEMI
  | block SEMI
  | SEMI
  ;

simple_stmt
  : assignment
  | log_stmt
  | while_stmt
  | if_stmt
  | sessao_stmt
  | etapa_stmt
  | descanso_stmt
  | saque_stmt
  | rally_stmt
  | start_stmt
  | point_stmt
  | ace_stmt
  | fault_stmt
  | doublefault_stmt
  | score_stmt
  | rename_stmt
  | config_stmt
  ;

block
  : LBRACE stmts RBRACE
  ;

stmts
  : %empty
  | stmts statement
  ;

assignment   : IDENT ASSIGN bool_expr ;
log_stmt     : LOG LPAREN bool_expr RPAREN ;
while_stmt   : WHILE LPAREN bool_expr RPAREN statement ;
if_stmt      : IF LPAREN bool_expr RPAREN statement
             | IF LPAREN bool_expr RPAREN statement ELSE statement ;

sessao_stmt      : SESSAO LPAREN STRING RPAREN block ;
etapa_stmt       : ETAPA LPAREN STRING COMMA IDENT ASSIGN expression RPAREN ;
descanso_stmt    : DESCANSO LPAREN expression RPAREN ;
saque_stmt       : SAQUE LPAREN player COMMA STRING RPAREN ;
rally_stmt       : RALLY LPAREN expression RPAREN ;
start_stmt       : START LPAREN expression COMMA expression RPAREN ;
point_stmt       : POINT LPAREN player RPAREN ;
ace_stmt         : ACE LPAREN player RPAREN ;
fault_stmt       : FAULT LPAREN player RPAREN ;
doublefault_stmt : DOUBLEFAULT LPAREN player RPAREN ;
score_stmt       : SCORE LPAREN RPAREN ;
rename_stmt      : RENAME LPAREN player COMMA STRING RPAREN ;
config_stmt      : CONFIG LPAREN cfgkey ASSIGN bool_expr RPAREN ;

cfgkey  : NO_AD | SHOT_CLOCK ;
player  : PLAYER_A | PLAYER_B ;

bool_expr
  : bool_term
  | bool_expr OR bool_term
  ;

bool_term
  : rel_expr
  | bool_term AND rel_expr
  ;

rel_expr
  : expression
  | rel_expr EQ3 expression
  | rel_expr GT  expression
  | rel_expr LT  expression
  ;

expression
  : term
  | expression PLUS  term
  | expression MINUS term
  ;

term
  : factor
  | term TIMES factor
  | term DIV   factor
  ;

factor
  : NUMBER
  | TRUE
  | FALSE
  | IDENT
  | READLINE LPAREN RPAREN
  | LPAREN bool_expr RPAREN
  | PLUS  factor
  | MINUS factor
  | NOT   factor
  ;

%%
