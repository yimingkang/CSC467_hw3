
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

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
  MINUS,
  NEGATION,
} unary_op;

typedef enum {
  PLUS,
  MINUS,
  MULT,
  DIV,

  POW,

  GEQ,
  GT,

  LT,
  LEQ,

  NEQ,
  EQ,

  AND,
  OR,
} binary_op;

typedef enum{
  INT_TYPE,
  IVEC_TYPE,
  BOOL_TYPE,
  BVEC_TYPE,
  FLOAT_TYPE,
  VEC_TYPE,
} type_name;

typedef union{
  int int_val;
  float float_val;
  char *char_val;
  int bool_val;
} type_value;

typedef enum {

  // Expression
  FUNC_EXPRESSION_NODE       ,
  TYPE_EXPRESSION_NODE       ,
  BINARY_EXPRESSION_NODE       ,
  UNARY_EXPRESSION_NODE       ,
  LITERAL_EXPRESSION_NODE,
  PAREN_EXPRESSION_NODE,
  VARIABLE_EXPRESSION_NODE,

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

  // statement
  ASSIGNMENT_STATEMENT,
  IF_ELSE_STATEMENT,
  IF_STATEMENT,
  SCOPE_STATEMENT,
  SEMICOLEN_STATEMENT,

  // declaration
  DECLARATION,
  INITIALIZED_DECLARATION,
  CONST_DECLARATION,

  // statements,
  STATEMENTS, // includes null

  // declarations
  DECLARATIONS, // includes null

  // scope
  SCOPE,

  // program
  PROGRAM,

  UNKNOWN               ,

} node_kind;

struct node_ {

  // an example of tagging each node with a type
  node_kind kind;

  union {
    struct {
      node *declarations;
      node *statements;
      // null node -> both are null
    } scope;

    struct {
      node *declaration;
      node *declarations;
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
      char *null = NULL;
    } semicolen_statement;

    struct {
      type_name type;
      type_value value;
      int multiplicity;
    } type;

    struct {
      node *type;
      node *arguments_opt;
    } type_expression_node;

    struct {
      char *func_name;
      node *arguments_opt;
    } func_expression_node;

    struct {
      unary_op op;
      node *expression;
    } unary_expression_node;

    struct {
      binary_op op;
      node *l_val;
      node *r_val;
    } binary_expression_node;

    struct {
      node *bool_int_float_type;  // can only be one of these types
    } literal_expression_node;

    struct {
      node *expressoin;
    } paren_expression_node;

    struct {
      node *variable;
    } variable_expression_node;

    struct {
      char *id;
    } singular_variable;

    struct {
      char *id;
      node *int_type;  // can only be int
    } array_variable;

    struct {
      node *arguments;
      node *expression;
    } binary_arguments;

    struct {
      node *expression;
    } unary_expression;

    struct {
      node *arguments;  // null -> epsilon
    } arguments_opt;
  };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);

#endif /* AST_H_ */
