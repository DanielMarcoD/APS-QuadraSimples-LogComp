/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "parser.y"

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "codegen.h"

#line 56 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    VAR = 258,                     /* VAR  */
    IF = 259,                      /* IF  */
    ELSE = 260,                    /* ELSE  */
    WHILE = 261,                   /* WHILE  */
    LOG = 262,                     /* LOG  */
    PRINT = 263,                   /* PRINT  */
    SESSAO = 264,                  /* SESSAO  */
    ETAPA = 265,                   /* ETAPA  */
    DESCANSO = 266,                /* DESCANSO  */
    SAQUE = 267,                   /* SAQUE  */
    RALLY = 268,                   /* RALLY  */
    START = 269,                   /* START  */
    POINT = 270,                   /* POINT  */
    ACE = 271,                     /* ACE  */
    FAULT = 272,                   /* FAULT  */
    DOUBLEFAULT = 273,             /* DOUBLEFAULT  */
    SCORE = 274,                   /* SCORE  */
    RENAME = 275,                  /* RENAME  */
    CONFIG = 276,                  /* CONFIG  */
    NO_AD = 277,                   /* NO_AD  */
    SHOT_CLOCK = 278,              /* SHOT_CLOCK  */
    PLAYER_A = 279,                /* PLAYER_A  */
    PLAYER_B = 280,                /* PLAYER_B  */
    TRUE = 281,                    /* TRUE  */
    FALSE = 282,                   /* FALSE  */
    READLINE = 283,                /* READLINE  */
    LAST_RALLY = 284,              /* LAST_RALLY  */
    TOTAL_POINTS = 285,            /* TOTAL_POINTS  */
    IDENT = 286,                   /* IDENT  */
    STRING = 287,                  /* STRING  */
    NUMBER = 288,                  /* NUMBER  */
    OR = 289,                      /* OR  */
    AND = 290,                     /* AND  */
    EQ3 = 291,                     /* EQ3  */
    LT = 292,                      /* LT  */
    GT = 293,                      /* GT  */
    PLUS = 294,                    /* PLUS  */
    MINUS = 295,                   /* MINUS  */
    TIMES = 296,                   /* TIMES  */
    DIV = 297,                     /* DIV  */
    NOT = 298,                     /* NOT  */
    ASSIGN = 299,                  /* ASSIGN  */
    LPAREN = 300,                  /* LPAREN  */
    RPAREN = 301,                  /* RPAREN  */
    LBRACE = 302,                  /* LBRACE  */
    RBRACE = 303,                  /* RBRACE  */
    COMMA = 304,                   /* COMMA  */
    SEMI = 305,                    /* SEMI  */
    LOWER_THAN_ELSE = 306          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "parser.y"

  int   ival;
  char* sval;

#line 129 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
