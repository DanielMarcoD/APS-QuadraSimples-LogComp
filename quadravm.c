#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024
#define MAX_VARS   128
#define MAX_NAME   64
#define MAX_LINES  2048
#define MAX_LABELS 512

typedef struct {
    char name[MAX_NAME];
    int  value;
} Var;

typedef struct {
    int no_ad;
    int shot_clock;
} Config;

typedef struct {
    char player_A_name[MAX_NAME];
    char player_B_name[MAX_NAME];
    int  points_A;    /* 0, 15, 30, 40 */
    int  points_B;
    int  games_A;
    int  games_B;
    int  sets_A;
    int  sets_B;
    int  match_started;
    int  target_sets;   /* sets para vencer */
    int  target_games;  /* games por set */
} MatchState;

typedef struct {
    int  stack[STACK_SIZE];
    int  sp;
    Var  vars[MAX_VARS];
    int  var_count;
    Config cfg;
    MatchState match;
    
    /* Registradores de propósito geral (visíveis ao programador) */
    int R0;
    int R1;
    
    /* Sensores read-only (atualizados pela VM, lidos pela linguagem) */
    int last_rally_exchanges;   /* número de trocas do último rally */
    int total_points_played;    /* total de pontos disputados na partida */
} VM;

typedef struct {
    char name[MAX_NAME];
    int  index;   /* índice da linha LABEL no programa */
} Label;

/* Programa carregado (linhas de assembly) */
static char  *prog[MAX_LINES];
static int    prog_len = 0;

static Label  labels[MAX_LABELS];
static int    label_count = 0;

/* ====== VM: pilha e variáveis ====== */

static void vm_init(VM *vm) {
    vm->sp = 0;
    vm->var_count = 0;
    vm->cfg.no_ad = 0;
    vm->cfg.shot_clock = 0;
    
    /* Inicializa registradores */
    vm->R0 = 0;
    vm->R1 = 0;
    
    /* Inicializa sensores read-only */
    vm->last_rally_exchanges = 0;
    vm->total_points_played = 0;
    
    /* Inicializa estado da partida */
    strcpy(vm->match.player_A_name, "Jogador A");
    strcpy(vm->match.player_B_name, "Jogador B");
    vm->match.points_A = 0;
    vm->match.points_B = 0;
    vm->match.games_A = 0;
    vm->match.games_B = 0;
    vm->match.sets_A = 0;
    vm->match.sets_B = 0;
    vm->match.match_started = 0;
    vm->match.target_sets = 3;
    vm->match.target_games = 6;
}

static void push(VM *vm, int v) {
    if (vm->sp >= STACK_SIZE) {
        fprintf(stderr, "[VM] Stack overflow\n");
        exit(1);
    }
    vm->stack[vm->sp++] = v;
}

static int pop(VM *vm) {
    if (vm->sp <= 0) {
        fprintf(stderr, "[VM] Stack underflow\n");
        exit(1);
    }
    return vm->stack[--vm->sp];
}

static int *find_var(VM *vm, const char *name) {
    for (int i = 0; i < vm->var_count; i++) {
        if (strcmp(vm->vars[i].name, name) == 0) {
            return &vm->vars[i].value;
        }
    }
    if (vm->var_count >= MAX_VARS) {
        fprintf(stderr, "[VM] Muitas variáveis\n");
        exit(1);
    }
    strncpy(vm->vars[vm->var_count].name, name, MAX_NAME - 1);
    vm->vars[vm->var_count].name[MAX_NAME - 1] = '\0';
    vm->vars[vm->var_count].value = 0;
    vm->var_count++;
    return &vm->vars[vm->var_count - 1].value;
}

/* Extrai primeira string entre aspas */
static const char *extract_quoted(const char *line, char *dest, size_t max) {
    const char *p = strchr(line, '"');
    if (!p) {
        dest[0] = '\0';
        return NULL;
    }
    p++;
    const char *q = strchr(p, '"');
    if (!q) {
        dest[0] = '\0';
        return NULL;
    }
    size_t n = (size_t)(q - p);
    if (n >= max) n = max - 1;
    memcpy(dest, p, n);
    dest[n] = '\0';
    return q + 1;
}

/* ====== Funções auxiliares para placar de tênis ====== */

static const char* points_to_string(int p) {
    switch(p) {
        case 0: return "0";
        case 15: return "15";
        case 30: return "30";
        case 40: return "40";
        default: return "?";
    }
}

static void add_point_to_player(VM *vm, int is_player_A) {
    /* Incrementa contador de pontos totais (sensor read-only) */
    vm->total_points_played++;
    
    int *pts = is_player_A ? &vm->match.points_A : &vm->match.points_B;
    int *opp_pts = is_player_A ? &vm->match.points_B : &vm->match.points_A;
    int *games = is_player_A ? &vm->match.games_A : &vm->match.games_B;
    int *opp_games = is_player_A ? &vm->match.games_B : &vm->match.games_A;
    int *sets = is_player_A ? &vm->match.sets_A : &vm->match.sets_B;
    
    if (*pts < 30) {
        *pts += 15;
    } else if (*pts == 30) {
        *pts = 40;
    } else if (*pts == 40) {
        if (*opp_pts < 40) {
            /* Ganhou o game */
            (*games)++;
            *pts = 0;
            *opp_pts = 0;
            
            /* Verifica se ganhou o set */
            if (*games >= vm->match.target_games && (*games - *opp_games) >= 2) {
                (*sets)++;
                *games = 0;
                *opp_games = 0;
                printf("*** %s venceu o set! Placar de sets: %d-%d ***\n",
                       is_player_A ? vm->match.player_A_name : vm->match.player_B_name,
                       vm->match.sets_A, vm->match.sets_B);
            }
        } else {
            /* 40-40 */
            if (vm->cfg.no_ad) {
                /* No-ad: próximo ponto ganha o game */
                (*games)++;
                *pts = 0;
                *opp_pts = 0;
                
                if (*games >= vm->match.target_games && (*games - *opp_games) >= 2) {
                    (*sets)++;
                    *games = 0;
                    *opp_games = 0;
                    printf("*** %s venceu o set! Placar de sets: %d-%d ***\n",
                           is_player_A ? vm->match.player_A_name : vm->match.player_B_name,
                           vm->match.sets_A, vm->match.sets_B);
                }
            } else {
                /* Com vantagem: implementação simplificada - ganha direto */
                (*games)++;
                *pts = 0;
                *opp_pts = 0;
                
                if (*games >= vm->match.target_games && (*games - *opp_games) >= 2) {
                    (*sets)++;
                    *games = 0;
                    *opp_games = 0;
                    printf("*** %s venceu o set! Placar de sets: %d-%d ***\n",
                           is_player_A ? vm->match.player_A_name : vm->match.player_B_name,
                           vm->match.sets_A, vm->match.sets_B);
                }
            }
        }
    }
}

static void print_score(VM *vm) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║          PLACAR DA PARTIDA             ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║ %-20s │ Sets: %d │ Games: %d │\n", 
           vm->match.player_A_name, vm->match.sets_A, vm->match.games_A);
    printf("║ %-20s │ Sets: %d │ Games: %d │\n", 
           vm->match.player_B_name, vm->match.sets_B, vm->match.games_B);
    printf("╠════════════════════════════════════════╣\n");
    printf("║ Game atual: %-4s - %-4s              ║\n",
           points_to_string(vm->match.points_A),
           points_to_string(vm->match.points_B));
    printf("╠════════════════════════════════════════╣\n");
    printf("║ Configurações:                         ║\n");
    printf("║   No-Ad: %-5s  Shot Clock: %-5s    ║\n",
           vm->cfg.no_ad ? "ON" : "OFF",
           vm->cfg.shot_clock ? "ON" : "OFF");
    printf("╚════════════════════════════════════════╝\n\n");
}

/* ====== Tabela de labels ====== */

static void add_label(const char *name, int index) {
    if (label_count >= MAX_LABELS) {
        fprintf(stderr, "[VM] Labels demais\n");
        exit(1);
    }
    strncpy(labels[label_count].name, name, MAX_NAME - 1);
    labels[label_count].name[MAX_NAME - 1] = '\0';
    labels[label_count].index = index;
    label_count++;
}

static int find_label_index(const char *name) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, name) == 0) {
            return labels[i].index;
        }
    }
    fprintf(stderr, "[VM] Label não encontrado: %s\n", name);
    exit(1);
}

/* Faz um pré-passe nas linhas pra registrar LABELs */
static void build_labels(void) {
    for (int i = 0; i < prog_len; i++) {
        char *line = prog[i];
        char op[32];
        const char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '\0' || *s == '\n' || *s == '#') continue;

        if (sscanf(s, "%31s", op) != 1) continue;
        if (strcmp(op, "LABEL") == 0) {
            char name[MAX_NAME];
            if (sscanf(s, "LABEL %63s", name) == 1) {
                /* remove \n se vier junto */
                size_t len = strlen(name);
                if (len > 0 && (name[len-1] == '\n' || name[len-1] == '\r')) {
                    name[len-1] = '\0';
                }
                add_label(name, i);
            }
        }
    }
}

/* ====== Execução de instruções não-controladas (sem LABEL/JZ/JMP/HALT) ====== */

static void exec_non_control(VM *vm, const char *s) {
    char op[32];

    /* tirar espaços iniciais */
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '\0' || *s == '\n' || *s == '#') return;

    if (sscanf(s, "%31s", op) != 1) return;

    /* --- Pilha e aritmética --- */

    if (strcmp(op, "PUSH_NUM") == 0) {
        int n;
        if (sscanf(s, "PUSH_NUM %d", &n) == 1) {
            push(vm, n);
        }
    } else if (strcmp(op, "PUSH_BOOL") == 0) {
        int b;
        if (sscanf(s, "PUSH_BOOL %d", &b) == 1) {
            push(vm, b ? 1 : 0);
        }
    } else if (strcmp(op, "PUSH_VAR") == 0) {
        char name[MAX_NAME];
        if (sscanf(s, "PUSH_VAR %63s", name) == 1) {
            int *v = find_var(vm, name);
            push(vm, *v);
        }
    } else if (strcmp(op, "ASSIGN") == 0) {
        char name[MAX_NAME];
        if (sscanf(s, "ASSIGN %63s", name) == 1) {
            int val = pop(vm);
            int *v = find_var(vm, name);
            *v = val;
        }
    } else if (strcmp(op, "ADD") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, a + b);
    } else if (strcmp(op, "SUB") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, a - b);
    } else if (strcmp(op, "MUL") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, a * b);
    } else if (strcmp(op, "DIV") == 0) {
        int b = pop(vm), a = pop(vm);
        if (b == 0) {
            fprintf(stderr, "[VM] Divisão por zero\n");
            push(vm, 0);
        } else {
            push(vm, a / b);
        }
    } else if (strcmp(op, "UNARY_MINUS") == 0) {
        int a = pop(vm);
        push(vm, -a);
    } else if (strcmp(op, "NOT") == 0) {
        int a = pop(vm);
        push(vm, !a);
    } else if (strcmp(op, "LT") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, a < b);
    } else if (strcmp(op, "GT") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, a > b);
    } else if (strcmp(op, "EQ") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, a == b);
    } else if (strcmp(op, "AND") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, (a && b));
    } else if (strcmp(op, "OR") == 0) {
        int b = pop(vm), a = pop(vm);
        push(vm, (a || b));
    }

    /* --- READLINE / LOG --- */

    else if (strcmp(op, "READLINE") == 0) {
        int v;
        printf("input> ");
        fflush(stdout);
        if (scanf("%d", &v) != 1) v = 0;
        push(vm, v);
    } else if (strcmp(op, "LOG") == 0) {
        int v = pop(vm);
        printf("[LOG] %d\n", v);
    } else if (strcmp(op, "PRINT") == 0) {
        char msg[256];
        extract_quoted(s, msg, sizeof(msg));
        printf("%s\n", msg);
    }
    
    /* --- Registradores R0 e R1 --- */
    
    else if (strcmp(op, "LOAD_R0") == 0) {
        int v = pop(vm);
        vm->R0 = v;
    } else if (strcmp(op, "LOAD_R1") == 0) {
        int v = pop(vm);
        vm->R1 = v;
    } else if (strcmp(op, "PUSH_R0") == 0) {
        push(vm, vm->R0);
    } else if (strcmp(op, "PUSH_R1") == 0) {
        push(vm, vm->R1);
    } else if (strcmp(op, "INC_R0") == 0) {
        vm->R0++;
    } else if (strcmp(op, "DEC_R0") == 0) {
        vm->R0--;
    } else if (strcmp(op, "INC_R1") == 0) {
        vm->R1++;
    } else if (strcmp(op, "DEC_R1") == 0) {
        vm->R1--;
    }
    
    /* --- Sensores read-only --- */
    
    else if (strcmp(op, "PUSH_LAST_RALLY") == 0) {
        push(vm, vm->last_rally_exchanges);
    } else if (strcmp(op, "PUSH_TOTAL_POINTS") == 0) {
        push(vm, vm->total_points_played);
    }

    /* --- Sessões e etapas --- */

    else if (strcmp(op, "SESSAO_BEGIN") == 0) {
        char nome[128];
        extract_quoted(s, nome, sizeof(nome));
        printf("\n=== Sessão: %s ===\n", nome);
    } else if (strcmp(op, "SESSAO_END") == 0) {
        printf("--- Fim da sessão ---\n");
    } else if (strcmp(op, "ETAPA_DUR") == 0) {
        char nome[128];
        extract_quoted(s, nome, sizeof(nome));
        int d = pop(vm);
        printf("Etapa: %s (duração %d s)\n", nome, d);
    } else if (strcmp(op, "ETAPA_REPS") == 0) {
        char nome[128];
        extract_quoted(s, nome, sizeof(nome));
        int r = pop(vm);
        printf("Etapa: %s (%d repetições)\n", nome, r);
    } else if (strcmp(op, "DESCANSO") == 0) {
        int t = pop(vm);
        printf("Descanso: %d s\n", t);
    }

    /* --- Jogo de tênis --- */

    else if (strcmp(op, "SAQUE") == 0) {
        char player[16];
        char lado[64] = "";
        if (sscanf(s, "SAQUE %15s \"%63[^\"]", player, lado) >= 1) {
            const char *player_name = (strcmp(player, "A") == 0) 
                ? vm->match.player_A_name : vm->match.player_B_name;
            if (lado[0] != '\0')
                printf("🎾 Saque de %s no lado %s\n", player_name, lado);
            else
                printf("🎾 Saque de %s\n", player_name);
        }
    } else if (strcmp(op, "RALLY") == 0) {
        int n = pop(vm);
        /* Atualiza sensor read-only */
        vm->last_rally_exchanges = n;
        printf("⚡ Rally com %d trocas\n", n);
    } else if (strcmp(op, "START_MATCH") == 0) {
        int games = pop(vm);
        int sets  = pop(vm);
        vm->match.target_sets = sets;
        vm->match.target_games = games;
        vm->match.match_started = 1;
        vm->match.points_A = 0;
        vm->match.points_B = 0;
        vm->match.games_A = 0;
        vm->match.games_B = 0;
        vm->match.sets_A = 0;
        vm->match.sets_B = 0;
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║      INÍCIO DA PARTIDA DE TÊNIS        ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ %s vs %s\n", vm->match.player_A_name, vm->match.player_B_name);
        printf("║ Melhor de %d sets, %d games por set   ║\n", sets, games);
        printf("╚════════════════════════════════════════╝\n\n");
    } else if (strcmp(op, "POINT") == 0) {
        char player[16];
        if (sscanf(s, "POINT %15s", player) == 1) {
            int is_A = (strcmp(player, "A") == 0);
            add_point_to_player(vm, is_A);
            printf("✓ Ponto para %s\n", 
                   is_A ? vm->match.player_A_name : vm->match.player_B_name);
        }
    } else if (strcmp(op, "ACE") == 0) {
        char player[16];
        if (sscanf(s, "ACE %15s", player) == 1) {
            int is_A = (strcmp(player, "A") == 0);
            add_point_to_player(vm, is_A);
            printf("⚡ ACE de %s!\n", 
                   is_A ? vm->match.player_A_name : vm->match.player_B_name);
        }
    } else if (strcmp(op, "FAULT") == 0) {
        char player[16];
        if (sscanf(s, "FAULT %15s", player) == 1) {
            printf("✗ Fault de %s\n", 
                   (strcmp(player, "A") == 0) ? vm->match.player_A_name : vm->match.player_B_name);
        }
    } else if (strcmp(op, "DOUBLEFAULT") == 0) {
        char player[16];
        if (sscanf(s, "DOUBLEFAULT %15s", player) == 1) {
            int is_A = (strcmp(player, "A") == 0);
            /* Dupla falta dá ponto para o adversário */
            add_point_to_player(vm, !is_A);
            printf("✗✗ Dupla falta de %s (ponto para %s)\n", 
                   is_A ? vm->match.player_A_name : vm->match.player_B_name,
                   is_A ? vm->match.player_B_name : vm->match.player_A_name);
        }
    } else if (strcmp(op, "SCORE") == 0) {
        print_score(vm);
    } else if (strcmp(op, "RENAME") == 0) {
        char player[16];
        char nome[128];
        if (sscanf(s, "RENAME %15s \"%63[^\"]", player, nome) >= 2) {
            if (strcmp(player, "A") == 0) {
                strncpy(vm->match.player_A_name, nome, MAX_NAME - 1);
                vm->match.player_A_name[MAX_NAME - 1] = '\0';
            } else if (strcmp(player, "B") == 0) {
                strncpy(vm->match.player_B_name, nome, MAX_NAME - 1);
                vm->match.player_B_name[MAX_NAME - 1] = '\0';
            }
            printf("✎ Jogador %s renomeado para '%s'\n", player, nome);
        }
    }

    /* --- Configurações --- */

    else if (strcmp(op, "CONFIG") == 0) {
        char key[64];
        if (sscanf(s, "CONFIG %63s", key) == 1) {
            int val = pop(vm);
            if (strcmp(key, "no_ad") == 0) {
                vm->cfg.no_ad = val;
            } else if (strcmp(key, "shot_clock") == 0) {
                vm->cfg.shot_clock = val;
            }
            printf("Config %s = %s\n", key, val ? "true" : "false");
        }
    }

    /* LABEL, JZ, JMP, HALT não são tratados aqui */
}

/* ====== Main: carrega programa, monta labels, executa com PC ====== */

int main(int argc, char **argv) {
    const char *fname = "out.qvm";
    if (argc > 1) {
        fname = argv[1];
    }

    FILE *f = fopen(fname, "r");
    if (!f) {
        perror(fname);
        return 1;
    }

    /* Carrega linhas do programa */
    char buf[512];
    while (prog_len < MAX_LINES && fgets(buf, sizeof buf, f)) {
        prog[prog_len] = strdup(buf);
        prog_len++;
    }
    fclose(f);

    /* Monta tabela de labels */
    build_labels();

    VM vm;
    vm_init(&vm);

    int pc = 0;
    while (pc < prog_len) {
        char *line = prog[pc];
        char op[32];
        const char *s = line;

        while (*s == ' ' || *s == '\t') s++;
        if (*s == '\0' || *s == '\n' || *s == '#') {
            pc++;
            continue;
        }

        if (sscanf(s, "%31s", op) != 1) {
            pc++;
            continue;
        }

        if (strcmp(op, "HALT") == 0) {
            break;
        } else if (strcmp(op, "LABEL") == 0) {
            pc++;
            continue;
        } else if (strcmp(op, "JMP") == 0) {
            char name[MAX_NAME];
            if (sscanf(s, "JMP %63s", name) == 1) {
                int idx = find_label_index(name);
                pc = idx;
                continue;
            } else {
                pc++;
                continue;
            }
        } else if (strcmp(op, "JZ") == 0) {
            char name[MAX_NAME];
            if (sscanf(s, "JZ %63s", name) == 1) {
                int cond = pop(&vm);
                if (cond == 0) {
                    int idx = find_label_index(name);
                    pc = idx;
                    continue;
                } else {
                    pc++;
                    continue;
                }
            } else {
                pc++;
                continue;
            }
        } else {
            exec_non_control(&vm, s);
            pc++;
        }
    }

    /* libera memória das linhas */
    for (int i = 0; i < prog_len; i++) {
        free(prog[i]);
    }

    return 0;
}
