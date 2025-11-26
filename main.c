#include <stdio.h>
#include "codegen.h"

int yyparse(void);
int yylex_destroy(void);
extern int yylineno;
extern FILE *yyin;

FILE *cg_out = NULL;

void yyerror(const char* s) {
  fprintf(stderr, "[Parser] %s (linha %d)\n", s, yylineno);
}

int main(int argc, char **argv) {
  // leitura opcional de arquivo da linha de comando
  if (argc > 1) {
    yyin = fopen(argv[1], "r");
    if (!yyin) {
      perror(argv[1]);
      return 1;
    }
  }

  cg_out = fopen("out.qvm", "w");
  if (!cg_out) {
    perror("out.qvm");
    return 1;
  }

  cg_init(cg_out);
  int rc = yyparse();
  cg_finish();

  fclose(cg_out);
  if (yyin && yyin != stdin) fclose(yyin);

  if (rc == 0) puts("OK: compilation successful.");
  else         puts("FAIL: syntax error.");

  yylex_destroy();
  return rc;
}

