%code requires {
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "codegen.h"
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

/* Palavras-chave e tokens */
%token VAR IF ELSE WHILE LOG PRINT
%token SESSAO ETAPA DESCANSO SAQUE RALLY
%token START POINT ACE FAULT DOUBLEFAULT SCORE
%token RENAME CONFIG NO_AD SHOT_CLOCK
%token PLAYER_A PLAYER_B
%token TRUE FALSE
%token READLINE
%token LAST_RALLY TOTAL_POINTS

%token <sval> IDENT STRING
%token <ival> NUMBER

%token OR AND EQ3 LT GT
%token PLUS MINUS TIMES DIV NOT
%token ASSIGN
%token LPAREN RPAREN LBRACE RBRACE COMMA SEMI

%type <sval> cfgkey player

/* Precedência para resolver conflito dangling-else */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%start program

%%

/* =========================================================
 * Programa e lista de statements
 * ========================================================= */

program
  : stmt_list_opt
  ;

stmt_list_opt
  : /* empty */
  | stmt_list_opt stmt
  ;

stmt
  : expr_stmt        /* statements que terminam com ';' dentro da própria regra */
  | if_stmt
  | while_stmt
  | sessao_stmt
  | block
  | block SEMI       /* bloco com ';' opcional */
  ;

block
  : LBRACE stmt_list_opt RBRACE
  ;

/* =========================================================
 * Statements com ';'
 * ========================================================= */

expr_stmt
  : assignment SEMI
  | log_stmt SEMI
  | print_stmt SEMI
  | etapa_stmt SEMI
  | descanso_stmt SEMI
  | saque_stmt SEMI
  | rally_stmt SEMI
  | start_stmt SEMI
  | point_stmt SEMI
  | ace_stmt SEMI
  | fault_stmt SEMI
  | doublefault_stmt SEMI
  | score_stmt SEMI
  | rename_stmt SEMI
  | config_stmt SEMI
  ;

/* =========================================================
 * Atribuição e log
 * ========================================================= */

assignment
  : IDENT ASSIGN bool_expr {
      cg_assignment($1);
      free($1);
    }
  ;

log_stmt
  : LOG LPAREN bool_expr RPAREN {
      cg_log();
    }
  ;

print_stmt
  : PRINT LPAREN STRING RPAREN {
      cg_print($3);
      free($3);
    }
  ;

/* =========================================================
 * WHILE com bloco obrigatório
 * Sintaxe: while (cond) { ... };
 * ========================================================= */

while_stmt
  : WHILE
    {
      cg_while_start();            /* LABEL begin */
    }
    LPAREN bool_expr RPAREN
    {
      cg_while_after_cond();       /* JZ end se condição == 0 */
    }
    block SEMI
    {
      cg_while_end();              /* JMP begin + LABEL end */
    }
  ;

/* =========================================================
 * IF (sem else e com else), com bloco obrigatório
 * Sintaxe:
 *   if (cond) { ... };
 *   if (cond) { ... } else { ... };
 * ========================================================= */

if_stmt
  : IF LPAREN bool_expr RPAREN
    {
      cg_if_start();
      cg_if_after_cond();
    }
    block if_continuation SEMI
  ;

if_continuation
  /* sem else */
  : /* empty */
    {
      cg_if_end();
    }

  /* com else */
  | ELSE
    {
      cg_if_with_else_middle();
    }
    block
    {
      cg_if_end();
    }
  ;

/* =========================================================
 * Sessão de treino (possui bloco interno)
 * Sintaxe: sessao("Nome") { ... };
 * ========================================================= */

sessao_stmt
  : SESSAO LPAREN STRING RPAREN
    {
      cg_sessao_begin($3);
      free($3);
    }
    block SEMI
    {
      cg_sessao_end();
    }
  ;

/* =========================================================
 * Comandos de domínio QuadraSimples
 * ========================================================= */

etapa_stmt
  : ETAPA LPAREN STRING COMMA IDENT ASSIGN expression RPAREN {
      if (strcmp($5, "dur") == 0) {
        cg_etapa_dur($3);
      } else if (strcmp($5, "reps") == 0) {
        cg_etapa_reps($3);
      }
      free($3);
      free($5);
    }
  ;

descanso_stmt
  : DESCANSO LPAREN expression RPAREN {
      cg_descanso();  /* usa topo da pilha como segundos */
    }
  ;

saque_stmt
  : SAQUE LPAREN player COMMA STRING RPAREN {
      cg_saque($3, $5);
      free($5);
    }
  ;

rally_stmt
  : RALLY LPAREN expression RPAREN {
      cg_rally();     /* usa topo da pilha como nº de trocas */
    }
  ;

start_stmt
  : START LPAREN expression COMMA expression RPAREN {
      cg_start_match();  /* usa dois valores da pilha */
    }
  ;

point_stmt
  : POINT LPAREN player RPAREN {
      cg_point($3);
    }
  ;

ace_stmt
  : ACE LPAREN player RPAREN {
      cg_ace($3);
    }
  ;

fault_stmt
  : FAULT LPAREN player RPAREN {
      cg_fault($3);
    }
  ;

doublefault_stmt
  : DOUBLEFAULT LPAREN player RPAREN {
      cg_doublefault($3);
    }
  ;

score_stmt
  : SCORE LPAREN RPAREN {
      cg_score();
    }
  ;

rename_stmt
  : RENAME LPAREN player COMMA STRING RPAREN {
      cg_rename($3, $5);
      free($5);
    }
  ;

config_stmt
  : CONFIG LPAREN cfgkey ASSIGN bool_expr RPAREN {
      cg_config($3);  /* usa topo da pilha como 0/1 */
    }
  ;

/* =========================================================
 * Auxiliares: cfgkey e player
 * ========================================================= */

cfgkey
  : NO_AD      { $$ = "no_ad"; }
  | SHOT_CLOCK { $$ = "shot_clock"; }
  ;

player
  : PLAYER_A { $$ = "A"; }
  | PLAYER_B { $$ = "B"; }
  ;

/* =========================================================
 * Expressões (VM em pilha)
 * ========================================================= */

bool_expr
  : bool_term
  | bool_expr OR bool_term {
      cg_or();
    }
  ;

bool_term
  : rel_expr
  | bool_term AND rel_expr {
      cg_and();
    }
  ;

rel_expr
  : expression
  | rel_expr EQ3 expression {
      cg_eq();
    }
  | rel_expr GT  expression {
      cg_gt();
    }
  | rel_expr LT  expression {
      cg_lt();
    }
  ;

expression
  : term
  | expression PLUS  term {
      cg_add();
    }
  | expression MINUS term {
      cg_sub();
    }
  ;

term
  : factor
  | term TIMES factor {
      cg_mul();
    }
  | term DIV   factor {
      cg_div();
    }
  ;

factor
  : NUMBER {
      cg_push_number($1);
    }
  | TRUE {
      cg_push_bool(1);
    }
  | FALSE {
      cg_push_bool(0);
    }
  | IDENT {
      cg_push_var($1);
      free($1);
    }
  | READLINE LPAREN RPAREN {
      cg_readline();
    }
  | LAST_RALLY {
      cg_push_last_rally();
    }
  | TOTAL_POINTS {
      cg_push_total_points();
    }
  | LPAREN bool_expr RPAREN
  | PLUS  factor          /* unário +: nada extra */
  | MINUS factor {
      cg_unary_minus();
    }
  | NOT   factor {
      cg_not();
    }
  ;

%%
