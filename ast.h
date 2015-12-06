
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>
#include "symbol.h"

// Dummy node just so everything compiles, create your own node/nodes
//
// The code provided below is an example ONLY. You can use/modify it,
// but do not assume that it is correct or complete.
//
// There are many ways of making AST nodes. The approach below is an example
// of a descriminated union. If you choose to use C++, then I suggest looking
// into inheritance.

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum{
  UMINUS_OP,
  UNEGATION_OP,
} unary_op;

typedef enum {
  PLUS_OP,
  MINUS_OP,
  MULT_OP,
  DIV_OP,

  POW_OP,

  GEQ_OP,
  GT_OP,

  LT_OP,
  LEQ_OP,

  NEQ_OP,
  EQ_OP,

  AND_OP,
  OR_OP,
} binary_op;

typedef enum{
    DP3,
    DP4,
    RSQ,
    LIT
}func_names;

typedef enum{
  INT_TYPE,
  IVEC_TYPE,
  BOOL_TYPE,
  BVEC_TYPE,
  FLOAT_TYPE,
  VEC_TYPE,
  ANY_TYPE
} type_name;

typedef enum{
  BOOL_LITERAL,
  INT_LITERAL,
  FLOAT_LITERAL,
} literal_name;

typedef enum {

  // program
  PROGRAM,   // 0

  // scope
  SCOPE,

  // declarations
  DECLARATIONS, // includes null

  // statements,
  STATEMENTS, // includes null

  // declaration
  DECLARATION,
  INITIALIZED_DECLARATION, //5
  CONST_DECLARATION,

  // statement
  ASSIGNMENT_STATEMENT,
  IF_ELSE_STATEMENT,
  IF_STATEMENT,
  SCOPE_STATEMENT,  // 10
  SEMICOLEN_STATEMENT,

  // Expression
  FUNC_EXPRESSION_NODE,
  TYPE_EXPRESSION_NODE,
  BINARY_EXPRESSION_NODE,
  UNARY_EXPRESSION_NODE,  // 15
  PAREN_EXPRESSION_NODE,
  VARIABLE_EXPRESSION_NODE,
  LITERAL_EXPRESSION_NODE,

  // variable
  SINGULAR_VARIABLE, 
  ARRAY_VARIABLE,

  // arguments
  BINARY_ARGUMENT,
  UNARY_ARGUMENT,

  // argument_opt
  ARGUMENT_OPT,

  // type
  TYPE,

  UNKNOWN               ,

} node_kind;

struct node_ {

  // an example of tagging each node with a type
  node_kind kind;
  int linenum;

  union {
    struct {
      node *scope;  
    } program;

    struct {
      node *declarations;
      node *statements;
      // null node -> both are null
    } scope;

    struct {
      node *declarations;
      node *declaration;
    } declarations;

    struct {
      node *statements;
      node *statement;
      // null node -> both are null
    } statements;

    struct {
      node *type;
      char *id;
    } declaration;

    struct {
      node *type;
      char *id;
      node *expression;
    } initialized_declaration;

    struct {
      node *type;
      char *id;
      node *expression;
    } const_declaration;

    struct {
      node *variable;
      node *expression;
    } assignment_statement;

    struct {
      node *if_condition;
      node *statement;
      node *else_statement;
    } if_else_statement;

    struct {
      node *if_condition;
      node *statement;
    } if_statement;

    struct {
      node *scope;
    } scope_statement;

    struct {
      char *null;
    } semicolen_statement;

    struct {
      int var_type;
      int multiplicity;
    } type;

    struct {
      int literal_type;
      union {
        int int_val;
        float float_val;
        int bool_val;
      } literal_value;
    } literal_expression_node;

    struct {
      node *type;
      node *arguments_opt;
    } type_expression_node;

    struct {
      int func_name;
      node *arguments_opt;
    } func_expression_node;

    struct {
      int op;
      node *expression;
    } unary_expression_node;

    struct {
      int op;
      node *l_val;
      node *r_val;
    } binary_expression_node;

    struct {
      node *expression;
    } paren_expression_node;

    struct {
      node *variable;
    } variable_expression_node;

    struct {
      char *id;
    } singular_variable;

    struct {
      char *id;
      int multiplicity;  // can only be int
    } array_variable;

    struct {
      node *arguments;
      node *expression;
    } binary_arguments;

    struct {
      node *expression;
    } unary_argument;

    struct {
      node *arguments;  // null -> epsilon
    } arguments_opt;
  };
};

struct ast_check_res {
    int invalid;
    union{
        struct symbol* sb_table; // returned by declaration/declarations 
        struct {  //returned by expression/variable/type
            int type_code;
            int multiplicity;
            int attr;
        } type;
    };
};
node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);
struct ast_check_res ast_check(node* ast,struct symbol* sb_table, int scope);
struct symbol* init_symbol_table();
#endif /* AST_H_ */
