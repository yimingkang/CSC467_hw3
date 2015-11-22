%{
/***********************************************************************
 * --YOUR GROUP INFO SHOULD GO HERE--
 * 
 *   Interface to the parser module for CSC467 course project.
 * 
 *   Phase 2: Implement context free grammar for source language, and
 *            parse tracing functionality.
 *   Phase 3: Construct the AST for the source language program.
 ***********************************************************************/

/***********************************************************************
 *  C Definitions and external declarations for this module.
 *
 *  Phase 3: Include ast.h if needed, and declarations for other global or
 *           external vars, functions etc. as needed.
 ***********************************************************************/

#include <string.h>

#include "common.h"
#include "ast.h"
#include "symbol.h"
#include "semantic.h"

#define YYERROR_VERBOSE
#define yTRACE(x)    { if (traceParser) fprintf(traceFile, "%s\n", x); }

void yyerror(char* s);    /* what to do in case of error            */
int yylex();              /* procedure for calling lexical analyzer */
extern int yyline;        /* variable holding current line number   */

enum {
  DP3 = 0, 
  LIT = 1, 
  RSQ = 2
};

%}

/***********************************************************************
 *  Yacc/Bison declarations.
 *  Phase 2:
 *    1. Add precedence declarations for operators (after %start declaration)
 *    2. If necessary, add %type declarations for some nonterminals
 *  Phase 3:
 *    1. Add fields to the union below to facilitate the construction of the
 *       AST (the two existing fields allow the lexical analyzer to pass back
 *       semantic info, so they shouldn't be touched).
 *    2. Add <type> modifiers to appropriate %token declarations (using the
 *       fields of the union) so that semantic information can by passed back
 *       by the scanner.
 *    3. Make the %type declarations for the language non-terminals, utilizing
 *       the fields of the union as well.
 ***********************************************************************/

%{
#define YYDEBUG 1
%}

// defines the yyval union
%union {
  int as_int;
  int as_vec;
  float as_float;
  char *as_str;
  int as_func;
  node *as_ast;
}

%token          FLOAT_T
%token          INT_T
%token          BOOL_T
%token          CONST
%token          FALSE_C TRUE_C
%token          FUNC
%token          IF ELSE
%token          AND OR NEQ EQ LEQ GEQ

// links specific values of tokens to yyval
%token <as_vec>   VEC_T
%token <as_vec>   BVEC_T
%token <as_vec>   IVEC_T
%token <as_float> FLOAT_C
%token <as_int>   INT_C
%token <as_str>   ID


// operator precdence
%left     OR                        // 7
%left     AND                       // 6
%left     EQ NEQ '<' LEQ '>' GEQ    // 5
%left     '+' '-'                   // 4
%left     '*' '/'                   // 3
%right    '^'                       // 2
%right    '!' UMINUS                // 1
%left     '(' '['                   // 0

// resolve dangling else shift/reduce conflict with associativity
%left     WITHOUT_ELSE
%left     WITH_ELSE

// type declarations
// TODO: fill this out
%type <as_ast> expression
%type <as_ast> scope
%type <as_ast> declarations
%type <as_ast> statements
%type <as_ast> declaration
%type <as_ast> statement
%type <as_ast> type
%type <as_ast> variable
%type <as_ast> program
%type <as_ast> arguments_opt
%type <as_str> FUNC
%type <as_ast> arguments

// expect one shift/reduce conflict, where Bison chooses to shift
// the ELSE.
%expect 1

%start    program

%%

/***********************************************************************
 *  Yacc/Bison rules
 *  Phase 2:
 *    1. Replace grammar found here with something reflecting the source
 *       language grammar
 *    2. Implement the trace parser option of the compiler
 ***********************************************************************/
program
  : scope 
      {
      ast = ast_allocate(PROGRAM, $1);
      yTRACE("program -> scope\n") 
      } 
  ;

scope
  : '{' declarations statements '}'
      {
      $$ = ast_allocate(SCOPE, $2, $3);
      yTRACE("scope -> { declarations statements }\n") 
      }
  ;

declarations
  : declarations declaration
      {
      $$ = ast_allocate(DECLARATIONS, $1, $2);
      yTRACE("declarations -> declarations declaration\n") }
  | 
      {
      $$ = NULL;
      yTRACE("declarations -> \n")
      }
  ;

statements
  : statements statement
      {
      $$ = ast_allocate(STATEMENTS, $1, $2);
      yTRACE("statements -> statements statement\n") 
      }
  | 
      {
      $$ = NULL;
      yTRACE("statements -> \n") 
      }
  ;

declaration
  : type ID ';' 
      {
      $$ = ast_allocate(DECLARATION, $1, $2);
      yTRACE("declaration -> type ID ;\n") 
      }
  | type ID '=' expression ';'
      {
      $$ = ast_allocate(INITIALIZED_DECLARATION, $1, $2, $4);
      yTRACE("declaration -> type ID = expression ;\n") 
      }
  | CONST type ID '=' expression ';'
      {
      $$ = ast_allocate(CONST_DECLARATION, $2, $3, $5);
      yTRACE("declaration -> CONST type ID = expression ;\n") 
      }
  ;

statement
  : variable '=' expression ';'
      {
      $$ = ast_allocate(ASSIGNMENT_STATEMENT, $1, $3);
      yTRACE("statement -> variable = expression ;\n") 
      }
  | IF '(' expression ')' statement ELSE statement %prec WITH_ELSE
      {
      $$ = ast_allocate(IF_ELSE_STATEMENT, $3, $5, $7);
      yTRACE("statement -> IF ( expression ) statement ELSE statement \n") 
      }
  | IF '(' expression ')' statement %prec WITHOUT_ELSE
      {
      $$ = ast_allocate(IF_STATEMENT, $3, $5);
      yTRACE("statement -> IF ( expression ) statement \n") 
      }
  | scope 
      {
      $$ = ast_allocate(SCOPE_STATEMENT, $1);
      yTRACE("statement -> scope \n") 
      }
  | ';'
      { yTRACE("statement -> ; \n") }
  ;

type
  : INT_T
      {
      $$ = ast_allocate(TYPE, INT_TYPE, 0);
      yTRACE("type -> INT_T \n") 
      }
  | IVEC_T
      {
      // ivecX would result in $1 == n - 1, src in scanner.l
      $$ = ast_allocate(TYPE, IVEC_TYPE, $1);
      yTRACE("type -> IVEC_T \n") 
      }
  | BOOL_T
      {
      $$ = ast_allocate(TYPE, BOOL_TYPE, 0);
      yTRACE("type -> BOOL_T \n") 
      }
  | BVEC_T
      { 
      $$ = ast_allocate(TYPE, BVEC_TYPE, $1);
      yTRACE("type -> BVEC_T \n") 
      }
  | FLOAT_T
      {
      $$ = ast_allocate(TYPE, FLOAT_TYPE, 0);
      yTRACE("type -> FLOAT_T \n") 
      }
  | VEC_T
      {
      $$ = ast_allocate(TYPE, VEC_TYPE, $1);
      yTRACE("type -> VEC_T \n") 
      }
  ;

expression

  /* function-like operators */
  : type '(' arguments_opt ')' %prec '('
      {
      $$ = ast_allocate(TYPE_EXPRESSION_NODE, $1, $3);
      yTRACE("expression -> type ( arguments_opt ) \n") 
      }
  | FUNC '(' arguments_opt ')' %prec '('
      {
      $$ = ast_allocate(FUNC_EXPRESSION_NODE, $1, $3);
      yTRACE("expression -> FUNC ( arguments_opt ) \n") 
      }

  /* unary opterators */
  | '-' expression %prec UMINUS
      {
      $$ = ast_allocate(UNARY_EXPRESSION_NODE, UMINUS_OP, $2);
      yTRACE("expression -> - expression \n") 
      }
  | '!' expression %prec '!'
      {
      $$ = ast_allocate(UNARY_EXPRESSION_NODE, UNEGATION_OP, $2);
      yTRACE("expression -> ! expression \n") 
      }

  /* binary operators */
  | expression AND expression %prec AND
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, AND_OP, $3);
      yTRACE("expression -> expression AND expression \n") 
      }
  | expression OR expression %prec OR
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, OR_OP, $3);
      yTRACE("expression -> expression OR expression \n") 
      }
  | expression EQ expression %prec EQ
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, EQ_OP, $3);
      yTRACE("expression -> expression EQ expression \n") 
      }
  | expression NEQ expression %prec NEQ
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, NEQ_OP, $3);
      yTRACE("expression -> expression NEQ expression \n") 
      }
  | expression '<' expression %prec '<'
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, LT_OP, $3);
      yTRACE("expression -> expression < expression \n") 
      }
  | expression LEQ expression %prec LEQ
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, LEQ_OP, $3);
      yTRACE("expression -> expression LEQ expression \n") 
      }
  | expression '>' expression %prec '>'
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, GT_OP, $3);
      yTRACE("expression -> expression > expression \n") 
      }
  | expression GEQ expression %prec GEQ
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, GEQ_OP, $3);
      yTRACE("expression -> expression GEQ expression \n") 
      }
  | expression '+' expression %prec '+'
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, PLUS_OP, $3);
      yTRACE("expression -> expression + expression \n") 
      }
  | expression '-' expression %prec '-'
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, MINUS_OP, $3);
      yTRACE("expression -> expression - expression \n") 
      }
  | expression '*' expression %prec '*'
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, MULT_OP, $3);
      yTRACE("expression -> expression * expression \n") 
      }
  | expression '/' expression %prec '/'
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, DIV_OP, $3);
      yTRACE("expression -> expression / expression \n") 
      }
  | expression '^' expression %prec '^'
      {
      $$ = ast_allocate(BINARY_EXPRESSION_NODE, $1, POW_OP, $3);
      yTRACE("expression -> expression ^ expression \n") 
      }

  /* literals */
  | TRUE_C
      {
      $$ = ast_allocate(LITERAL_EXPRESSION_NODE, BOOL_LITERAL, 1);
      yTRACE("expression -> TRUE_C \n") 
      }
  | FALSE_C
      {
      $$ = ast_allocate(LITERAL_EXPRESSION_NODE, BOOL_LITERAL, 0);
      yTRACE("expression -> FALSE_C \n") 
      }
  | INT_C
      {
      $$ = ast_allocate(LITERAL_EXPRESSION_NODE, INT_LITERAL, $1);
      yTRACE("expression -> INT_C \n") 
      }
  | FLOAT_C
      {
      $$ = ast_allocate(LITERAL_EXPRESSION_NODE, FLOAT_LITERAL, $1);
      yTRACE("expression -> FLOAT_C \n") 
      }

  /* misc */
  | '(' expression ')'
      {
      $$ = ast_allocate(PAREN_EXPRESSION_NODE, $2);
      yTRACE("expression -> ( expression ) \n") 
      }
  | variable
    {
    $$ = ast_allocate(VARIABLE_EXPRESSION_NODE, $1);
    yTRACE("expression -> variable \n") 
    }
  ;

variable
  : ID
      {
      $$ = ast_allocate(SINGULAR_VARIABLE, $1);
      yTRACE("variable -> ID \n") 
      }
  | ID '[' INT_C ']' %prec '['
      {
      $$ = ast_allocate(ARRAY_VARIABLE, $1, $3);
      yTRACE("variable -> ID [ INT_C ] \n") 
      }
  ;

arguments
  : arguments ',' expression
      {
      $$ = ast_allocate(BINARY_ARGUMENT, $1, $3);
      yTRACE("arguments -> arguments , expression \n") 
      }
  | expression
      {
      $$ = ast_allocate(UNARY_ARGUMENT, $1);
      yTRACE("arguments -> expression \n") 
      }
  ;

arguments_opt
  : arguments
      {
      $$ = ast_allocate(ARGUMENT_OPT, $1);
      yTRACE("arguments_opt -> arguments \n") 
      }
  |
      {
      $$ = NULL;
      yTRACE("arguments_opt -> \n") 
      }
  ;

%%

/***********************************************************************ol
 * Extra C code.
 *
 * The given yyerror function should not be touched. You may add helper
 * functions as necessary in subsequent phases.
 ***********************************************************************/
void yyerror(char* s) {
  if(errorOccurred) {
    return;    /* Error has already been reported by scanner */
  } else {
    errorOccurred = 1;
  }

  fprintf(errorFile, "\nPARSER ERROR, LINE %d", yyline);
  
  if(strcmp(s, "parse error")) {
    if(strncmp(s, "parse error, ", 13)) {
      fprintf(errorFile, ": %s\n", s);
    } else {
      fprintf(errorFile, ": %s\n", s+13);
    }
  } else {
    fprintf(errorFile, ": Reading token %s\n", yytname[YYTRANSLATE(yychar)]);
  }
}

