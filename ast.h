
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>
#include <vector>

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

typedef enum {
  UNKNOWN,    

  PROGRAM,
  

  EXPRESSIONS,
  EXPRESSION,
  
  //STATEMENT
  STATEMENTS,
  STATEMENT_ASSIGN,
  STATEMENT_IF,
  SCOPE,

  //type should be in the same class
  TYPE_INT,
  TYPE_IVEC,
  TYPE_BOOL,
  TYPE_BVEC,
  TYPE_FLOAT,
  TYPE_VEC,

  DECLARATION,
  
  EXPRESSION_CONSTRUCTOR,

  EXPRESSION_FUNC,

  EXPRESSION_UNARY_A,//arithmatic
  EXPRESSION_UNARY_L, //logical

  //should be in the same class
  EXPRESSION_AND,
  EXPRESSION_OR,
  EXPRESSION_EQ,
  EXPRESSION_NEQ,
  EXPRESSION_GT,
  EXPRESSION_LT,
  EXPRESSION_GET,
  EXPRESSION_LET,
  EXPRESSION_ADD,
  EXPRESSION_SUB,
  EXPRESSION_MUL,
  EXPRESSION_DIV ,
  EXPRESSION_POW,

  //should be in the same class
  EXPRESSION_BOOL_C,
  EXPRESSION_INT_C,
  EXPRESSION_FLOAT_C, 

  EXPRESSION_VARIABLE,

} node_kind;

struct node_ {

  // an example of tagging each node with a type
  node_kind kind;

  union {
    struct {
        node* scope;
    } program;

    struct{
        std::vector<node*>* declarations;
        std::vector<node*>* statements;
    } scope;
    
    struct{
        node* type;
        char* id;
        node* expression;
        int const_f;
    } declaration;

    struct{
        node* variable;
        node* expression;
    } statement_assign;

    struct{
        node* condition_expression;
        node* if_body;
        node* else_body;
    } statement_if;

    struct{
        int param;
    } type;
    
    struct{
        node* type;
        std::vector<node*>* args;
    } expression_constructor;

    struct{
        int func;
        std::vector<node*>* args;
    } expression_func;


    struct {
      node *expression;
    } expression_unary;

    struct {
      node *left;
      node *right;
    } expression_two_op_operation;

    struct{
      int value;
    } expression_bool_c;

    struct{
      int value;
    } expression_int_c;

    struct{
      float value;
    } expression_float_c;
    
    struct{
      char* id;
      int is_array;
      int index;
    } expression_variable;



    // etc.
  };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);

#endif /* AST_H_ */
