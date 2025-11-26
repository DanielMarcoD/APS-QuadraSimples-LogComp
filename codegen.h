#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>

/* Inicialização / finalização */

void cg_init(FILE *out);
void cg_finish(void);

/* ===== EXPRESSÕES (VM em pilha) ===== */

void cg_push_number(int n);
void cg_push_bool(int b);
void cg_push_var(const char *name);
void cg_readline(void);

/* Sensores read-only */
void cg_push_last_rally(void);
void cg_push_total_points(void);

void cg_add(void);
void cg_sub(void);
void cg_mul(void);
void cg_div(void);
void cg_unary_minus(void);
void cg_not(void);

void cg_eq(void);
void cg_gt(void);
void cg_lt(void);
void cg_and(void);
void cg_or(void);

/* ===== ATRIBUIÇÃO E LOG ===== */

void cg_assignment(const char *name);  /* usa topo da pilha */
void cg_log(void);                     /* usa topo da pilha */
void cg_print(const char *str);        /* imprime string literal */

/* ===== CONTROLE DE FLUXO: WHILE ===== */

void cg_while_start(void);       /* gera LABEL início */
void cg_while_after_cond(void);  /* gera JZ fim */
void cg_while_end(void);         /* gera JMP início + LABEL fim */

/* ===== CONTROLE DE FLUXO: IF SEM ELSE ===== */

void cg_if_no_else_start(void);       /* aloca label fim */
void cg_if_no_else_after_cond(void);  /* JZ fim */
void cg_if_no_else_end(void);         /* LABEL fim */

/* ===== CONTROLE DE FLUXO: IF COM ELSE ===== */

void cg_if_else_start(void);          /* aloca labels else e fim */
void cg_if_else_after_cond(void);     /* JZ else */
void cg_if_else_after_then(void);     /* JMP fim + LABEL else */
void cg_if_else_end(void);            /* LABEL fim */

/* ===== CONTROLE DE FLUXO: IF UNIFICADO ===== */

void cg_if_start(void);               /* aloca labels (prepara para ambos os casos) */
void cg_if_after_cond(void);          /* JZ (para ser decidido depois) */
void cg_if_after_then_block(void);    /* chamado após bloco then */
void cg_if_with_else_middle(void);    /* JMP fim + LABEL else (chamado se houver else) */
void cg_if_end(void);                 /* LABEL fim */

/* ===== COMANDOS DE DOMÍNIO (DSL de tênis) ===== */

void cg_sessao_begin(const char *nome);
void cg_sessao_end(void);

void cg_etapa_dur(const char *nome);   /* usa topo da pilha */
void cg_etapa_reps(const char *nome);  /* usa topo da pilha */
void cg_descanso(void);                /* usa topo da pilha */

void cg_saque(const char *player, const char *lado);
void cg_rally(void);                   /* usa topo da pilha */
void cg_start_match(void);             /* usa 2 valores da pilha */

void cg_point(const char *player);
void cg_ace(const char *player);
void cg_fault(const char *player);
void cg_doublefault(const char *player);
void cg_score(void);
void cg_rename(const char *player, const char *novo_nome);
void cg_config(const char *key);       /* usa topo da pilha */

#endif
