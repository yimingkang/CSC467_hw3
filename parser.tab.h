/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FLOAT_T = 258,
     INT_T = 259,
     BOOL_T = 260,
     CONST = 261,
     FALSE_C = 262,
     TRUE_C = 263,
     FUNC = 264,
     IF = 265,
     ELSE = 266,
     AND = 267,
     OR = 268,
     NEQ = 269,
     EQ = 270,
     LEQ = 271,
     GEQ = 272,
     VEC_T = 273,
     BVEC_T = 274,
     IVEC_T = 275,
     FLOAT_C = 276,
     INT_C = 277,
     ID = 278,
     UMINUS = 279,
     WITHOUT_ELSE = 280,
     WITH_ELSE = 281
   };
#endif
/* Tokens.  */
#define FLOAT_T 258
#define INT_T 259
#define BOOL_T 260
#define CONST 261
#define FALSE_C 262
#define TRUE_C 263
#define FUNC 264
#define IF 265
#define ELSE 266
#define AND 267
#define OR 268
#define NEQ 269
#define EQ 270
#define LEQ 271
#define GEQ 272
#define VEC_T 273
#define BVEC_T 274
#define IVEC_T 275
#define FLOAT_C 276
#define INT_C 277
#define ID 278
#define UMINUS 279
#define WITHOUT_ELSE 280
#define WITH_ELSE 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 62 "parser.y"
{
  int as_int;
  int as_vec;
  float as_float;
  char *as_str;
  int as_func;
  node *as_ast;
}
/* Line 1529 of yacc.c.  */
#line 110 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

