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

#line 55 "parser.tab.h"

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
    SESSAO = 263,                  /* SESSAO  */
    ETAPA = 264,                   /* ETAPA  */
    DESCANSO = 265,                /* DESCANSO  */
    SAQUE = 266,                   /* SAQUE  */
    RALLY = 267,                   /* RALLY  */
    START = 268,                   /* START  */
    POINT = 269,                   /* POINT  */
    ACE = 270,                     /* ACE  */
    FAULT = 271,                   /* FAULT  */
    DOUBLEFAULT = 272,             /* DOUBLEFAULT  */
    SCORE = 273,                   /* SCORE  */
    RENAME = 274,                  /* RENAME  */
    CONFIG = 275,                  /* CONFIG  */
    NO_AD = 276,                   /* NO_AD  */
    SHOT_CLOCK = 277,              /* SHOT_CLOCK  */
    PLAYER_A = 278,                /* PLAYER_A  */
    PLAYER_B = 279,                /* PLAYER_B  */
    TRUE = 280,                    /* TRUE  */
    FALSE = 281,                   /* FALSE  */
    READLINE = 282,                /* READLINE  */
    IDENT = 283,                   /* IDENT  */
    STRING = 284,                  /* STRING  */
    NUMBER = 285,                  /* NUMBER  */
    OR = 286,                      /* OR  */
    AND = 287,                     /* AND  */
    EQ3 = 288,                     /* EQ3  */
    LT = 289,                      /* LT  */
    GT = 290,                      /* GT  */
    PLUS = 291,                    /* PLUS  */
    MINUS = 292,                   /* MINUS  */
    TIMES = 293,                   /* TIMES  */
    DIV = 294,                     /* DIV  */
    NOT = 295,                     /* NOT  */
    ASSIGN = 296,                  /* ASSIGN  */
    LPAREN = 297,                  /* LPAREN  */
    RPAREN = 298,                  /* RPAREN  */
    LBRACE = 299,                  /* LBRACE  */
    RBRACE = 300,                  /* RBRACE  */
    COMMA = 301,                   /* COMMA  */
    SEMI = 302                     /* SEMI  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 12 "parser.y"

  int   ival;
  char* sval;

#line 124 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
