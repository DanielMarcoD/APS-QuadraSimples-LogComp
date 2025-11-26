%option noyywrap
%option yylineno
%option noinput
%option nounput

%{
  #define _POSIX_C_SOURCE 200809L
  #include "parser.tab.h"
  #include <stdio.h>
  #include <unistd.h>
  #include <string.h>
  #include <stdlib.h>
  static char* sdup(const char* s){size_t n=strlen(s)+1;char* p=(char*)malloc(n);if(p)memcpy(p,s,n);return p;}
%}

WS   [ \t\r\n]+
ID   [A-Za-z][A-Za-z0-9_]*
NUM  [0-9]+
STR  \"([^\\\n\"]|\\.)*\"

%%

"var"           { return VAR; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"log"           { return LOG; }
"print"         { return PRINT; }
"true"          { return TRUE; }
"false"         { return FALSE; }

"sessao"        { return SESSAO; }
"etapa"         { return ETAPA; }
"descanso"      { return DESCANSO; }
"saque"         { return SAQUE; }
"rally"         { return RALLY; }
"start_match"   { return START; }
"point"         { return POINT; }
"ace"           { return ACE; }
"fault"         { return FAULT; }
"double_fault"  { return DOUBLEFAULT; }
"score"         { return SCORE; }
"rename"        { return RENAME; }
"config"        { return CONFIG; }
"no_ad"         { return NO_AD; }
"shot_clock"    { return SHOT_CLOCK; }
"A"             { return PLAYER_A; }
"B"             { return PLAYER_B; }

"last_rally"    { return LAST_RALLY; }
"total_points"  { return TOTAL_POINTS; }

"readline"      { return READLINE; }

"||"            { return OR; }
"&&"            { return AND; }
"==="           { return EQ3; }
">"             { return GT; }
"<"             { return LT; }
"="             { return ASSIGN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return DIV; }
"!"             { return NOT; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
","             { return COMMA; }
";"             { return SEMI; }

{NUM}           { yylval.ival = atoi(yytext); return NUMBER; }
{STR}           { yylval.sval = sdup(yytext); return STRING; }
{ID}            { yylval.sval = sdup(yytext); return IDENT;  }

"//".*          { }
{WS}            { }

.               { fprintf(stderr,"[Lexer] invalid char '%s' (linha %d)\n", yytext, yylineno); return yytext[0]; }

%%
