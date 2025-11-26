    #include "codegen.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    static FILE *OUT = NULL;

    /* Contadores para labels de while/if */

    static int while_counter = 0;
    static int if_counter    = 0;

    #define MAX_NEST 64

    static char while_begin[MAX_NEST][32];
    static char while_end_label[MAX_NEST][32];
    static int  while_top = 0;

    static char if_else_label[MAX_NEST][32];
    static char if_end_label[MAX_NEST][32];
    static int  if_has_else[MAX_NEST];  /* 1 se tem else, 0 senão */
    static int  if_top = 0;

    /* ===== Inicialização / finalização ===== */

    void cg_init(FILE *out) {
        OUT = out;
        if (OUT) {
            fprintf(OUT, "# QuadraSimples assembly\n");
        }
    }

    void cg_finish(void) {
        if (OUT) {
            fprintf(OUT, "HALT\n");
        }
        OUT = NULL;
    }

    /* ===== EXPRESSÕES (VM em pilha) ===== */

    void cg_push_number(int n) {
        if (OUT) fprintf(OUT, "PUSH_NUM %d\n", n);
    }

    void cg_push_bool(int b) {
        if (OUT) fprintf(OUT, "PUSH_BOOL %d\n", b ? 1 : 0);
    }

    void cg_push_var(const char *name) {
        if (OUT) fprintf(OUT, "PUSH_VAR %s\n", name);
    }

    void cg_readline(void) {
        if (OUT) fprintf(OUT, "READLINE\n");
    }

    /* ===== SENSORES READ-ONLY ===== */

    void cg_push_last_rally(void) {
        if (OUT) fprintf(OUT, "PUSH_LAST_RALLY\n");
    }

    void cg_push_total_points(void) {
        if (OUT) fprintf(OUT, "PUSH_TOTAL_POINTS\n");
    }

    void cg_add(void)         { if (OUT) fprintf(OUT, "ADD\n"); }
    void cg_sub(void)         { if (OUT) fprintf(OUT, "SUB\n"); }
    void cg_mul(void)         { if (OUT) fprintf(OUT, "MUL\n"); }
    void cg_div(void)         { if (OUT) fprintf(OUT, "DIV\n"); }
    void cg_unary_minus(void) { if (OUT) fprintf(OUT, "UNARY_MINUS\n"); }
    void cg_not(void)         { if (OUT) fprintf(OUT, "NOT\n"); }

    void cg_eq(void)  { if (OUT) fprintf(OUT, "EQ\n"); }
    void cg_gt(void)  { if (OUT) fprintf(OUT, "GT\n"); }
    void cg_lt(void)  { if (OUT) fprintf(OUT, "LT\n"); }
    void cg_and(void) { if (OUT) fprintf(OUT, "AND\n"); }
    void cg_or(void)  { if (OUT) fprintf(OUT, "OR\n"); }

    /* ===== ATRIBUIÇÃO / LOG ===== */

    void cg_assignment(const char *name) {
        if (OUT) fprintf(OUT, "ASSIGN %s\n", name);
    }

void cg_log(void) {
    if (OUT) fprintf(OUT, "LOG\n");
}

void cg_print(const char *str) {
    if (OUT) fprintf(OUT, "PRINT %s\n", str);
}    /* ===== CONTROLE DE FLUXO: WHILE ===== */

    void cg_while_start(void) {
        if (!OUT) return;
        if (while_top >= MAX_NEST) {
            fprintf(stderr, "[codegen] While aninhados demais\n");
            exit(1);
        }

        int id = while_counter++;
        snprintf(while_begin[while_top], sizeof(while_begin[while_top]),
                "LWHILE_%d_BEGIN", id);
        snprintf(while_end_label[while_top], sizeof(while_end_label[while_top]),
                "LWHILE_%d_END", id);

        fprintf(OUT, "LABEL %s\n", while_begin[while_top]);
        while_top++;
    }

    void cg_while_after_cond(void) {
        if (!OUT) return;
        if (while_top <= 0) {
            fprintf(stderr, "[codegen] cg_while_after_cond sem while\n");
            return;
        }
        int idx = while_top - 1;
        fprintf(OUT, "JZ %s\n", while_end_label[idx]);
    }

    void cg_while_end(void) {
        if (!OUT) return;
        if (while_top <= 0) {
            fprintf(stderr, "[codegen] cg_while_end sem while\n");
            return;
        }
        int idx = --while_top;
        fprintf(OUT, "JMP %s\n", while_begin[idx]);
        fprintf(OUT, "LABEL %s\n", while_end_label[idx]);
    }

    /* ===== CONTROLE DE FLUXO: IF SEM ELSE ===== */

    void cg_if_no_else_start(void) {
        if (!OUT) return;
        if (if_top >= MAX_NEST) {
            fprintf(stderr, "[codegen] IF aninhados demais\n");
            exit(1);
        }

        int id = if_counter++;
        snprintf(if_end_label[if_top], sizeof(if_end_label[if_top]),
                "LIF_%d_END", id);
        if_top++;
    }

    void cg_if_no_else_after_cond(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_no_else_after_cond sem if\n");
            return;
        }
        int idx = if_top - 1;
        fprintf(OUT, "JZ %s\n", if_end_label[idx]);
    }

    void cg_if_no_else_end(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_no_else_end sem if\n");
            return;
        }
        int idx = --if_top;
        fprintf(OUT, "LABEL %s\n", if_end_label[idx]);
    }

    /* ===== CONTROLE DE FLUXO: IF COM ELSE ===== */

    void cg_if_else_start(void) {
        if (!OUT) return;
        if (if_top >= MAX_NEST) {
            fprintf(stderr, "[codegen] IF aninhados demais\n");
            exit(1);
        }

        int id = if_counter++;
        snprintf(if_else_label[if_top], sizeof(if_else_label[if_top]),
                "LIF_%d_ELSE", id);
        snprintf(if_end_label[if_top], sizeof(if_end_label[if_top]),
                "LIF_%d_END", id);
        if_top++;
    }

    void cg_if_else_after_cond(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_else_after_cond sem if\n");
            return;
        }
        int idx = if_top - 1;
        fprintf(OUT, "JZ %s\n", if_else_label[idx]);
    }

    void cg_if_else_after_then(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_else_after_then sem if\n");
            return;
        }
        int idx = if_top - 1;
        fprintf(OUT, "JMP %s\n", if_end_label[idx]);
        fprintf(OUT, "LABEL %s\n", if_else_label[idx]);
    }

    void cg_if_else_end(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_else_end sem if\n");
            return;
        }
        int idx = --if_top;
        fprintf(OUT, "LABEL %s\n", if_end_label[idx]);
    }

    /* ===== CONTROLE DE FLUXO: IF UNIFICADO ===== */

    void cg_if_start(void) {
        if (!OUT) return;
        if (if_top >= MAX_NEST) {
            fprintf(stderr, "[codegen] IF aninhados demais\n");
            exit(1);
        }

        int id = if_counter++;
        snprintf(if_else_label[if_top], sizeof(if_else_label[if_top]),
                "LIF_%d_ELSE", id);
        snprintf(if_end_label[if_top], sizeof(if_end_label[if_top]),
                "LIF_%d_END", id);
        if_has_else[if_top] = 0;  /* Assume sem else por padrão */
        if_top++;
    }

    void cg_if_after_cond(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_after_cond sem if\n");
            return;
        }
        int idx = if_top - 1;
        /* Gera JZ para o label else (mesmo se não houver else, será o fim) */
        fprintf(OUT, "JZ %s\n", if_else_label[idx]);
    }

    void cg_if_after_then_block(void) {
        /* Não faz nada aqui - apenas um marcador no parser */
    }

    void cg_if_with_else_middle(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_with_else_middle sem if\n");
            return;
        }
        int idx = if_top - 1;
        if_has_else[idx] = 1;  /* Marca que este if tem else */
        /* Pula o bloco else e coloca o label do else */
        fprintf(OUT, "JMP %s\n", if_end_label[idx]);
        fprintf(OUT, "LABEL %s\n", if_else_label[idx]);
    }

    void cg_if_end(void) {
        if (!OUT) return;
        if (if_top <= 0) {
            fprintf(stderr, "[codegen] cg_if_end sem if\n");
            return;
        }
        int idx = --if_top;
        /* Se não houver else, coloca o label else (que é onde o JZ vai) */
        if (!if_has_else[idx]) {
            fprintf(OUT, "LABEL %s\n", if_else_label[idx]);
        }
        /* Sempre coloca o label do fim */
        fprintf(OUT, "LABEL %s\n", if_end_label[idx]);
    }

    /* ===== COMANDOS DE DOMÍNIO (DSL de tênis) ===== */

    void cg_sessao_begin(const char *nome) {
        /* STRING já vem com aspas do lexer (ex: "Aquecimento") */
        if (OUT) fprintf(OUT, "SESSAO_BEGIN %s\n", nome);
    }

    void cg_sessao_end(void) {
        if (OUT) fprintf(OUT, "SESSAO_END\n");
    }

    void cg_etapa_dur(const char *nome) {
        if (OUT) fprintf(OUT, "ETAPA_DUR %s\n", nome);
    }

    void cg_etapa_reps(const char *nome) {
        if (OUT) fprintf(OUT, "ETAPA_REPS %s\n", nome);
    }

    void cg_descanso(void) {
        if (OUT) fprintf(OUT, "DESCANSO\n");
    }

    void cg_saque(const char *player, const char *lado) {
        /* lado também vem como STRING com aspas, ex: "deuce" */
        if (OUT) fprintf(OUT, "SAQUE %s %s\n", player, lado);
    }

    void cg_rally(void) {
        if (OUT) fprintf(OUT, "RALLY\n");
    }

    void cg_start_match(void) {
        if (OUT) fprintf(OUT, "START_MATCH\n");
    }

    void cg_point(const char *player) {
        if (OUT) fprintf(OUT, "POINT %s\n", player);
    }

    void cg_ace(const char *player) {
        if (OUT) fprintf(OUT, "ACE %s\n", player);
    }

    void cg_fault(const char *player) {
        if (OUT) fprintf(OUT, "FAULT %s\n", player);
    }

    void cg_doublefault(const char *player) {
        if (OUT) fprintf(OUT, "DOUBLEFAULT %s\n", player);
    }

    void cg_score(void) {
        if (OUT) fprintf(OUT, "SCORE\n");
    }

    void cg_rename(const char *player, const char *novo_nome) {
        if (OUT) fprintf(OUT, "RENAME %s %s\n", player, novo_nome);
    }

    void cg_config(const char *key) {
        if (OUT) fprintf(OUT, "CONFIG %s\n", key);
    }
