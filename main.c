#include <stdio.h>

int yyparse(void);
int yylex_destroy(void);
extern int yylineno;

void yyerror(const char* s) {
  fprintf(stderr, "[Parser] %s (linha %d)\n", s, yylineno);
}

int main(void) {
  int rc = yyparse();
  if (rc == 0) puts("OK: syntactic analysis successful.");
  else         puts("FAIL: syntax error.");
  yylex_destroy();
  return rc;
}
