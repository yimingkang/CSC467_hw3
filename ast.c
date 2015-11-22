#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

#define DEBUG_PRINT_TREE 0

node *ast = NULL;

node *ast_allocate(node_kind kind, ...) {
  va_list args;

  // make the node
  node *ast = (node *) malloc(sizeof(node));
  memset(ast, 0, sizeof *ast);
  ast->kind = kind;

  va_start(args, kind); 

  switch(kind) {
  
      case PROGRAM:
          ast->program.scope = va_arg(args, node *);
          break;

      case SCOPE:
          ast->scope.declarations = va_arg(args, std::vector<node*>*);
          ast->scope.declarations = va_arg(args, std::vector<node*>*);
          break;

      case DECLARATION:
          ast->declaration.type = va_arg(args, node*);
          ast->declaration.id = va_arg(args, char*);
          ast->declaration.expression = va_arg(args, node*);
          ast->declaration.const_f = va_arg(args, int);
          break;

      case STATEMENT_ASSIGN:
          ast->statement_assign.variable = va_arg(args, node*);
          ast->statement_assign.expression = va_arg(args, node*);
          break;

      case STATEMENT_IF:
          ast->statement_if.condition_expression = va_arg(args, node*);
          ast->statement_if.if_body = va_arg(args, node*);
          ast->statement_if.else_body = va_arg(args, node*);
          break;
    
      case TYPE_INT:
      case TYPE_IVEC:
      case TYPE_BOOL:
      case TYPE_BVEC:
      case TYPE_FLOAT:
      case TYPE_VEC:
          ast->type.param = va_arg(args, int);
          break;

      case EXPRESSION_CONSTRUCTOR:
          ast->expression_constructor.type = va_arg(args, node*);
          ast->expression_constructor.args = va_arg(args, std::vector<node*>*);
          break;

      case EXPRESSION_FUNC:
          ast->expression_func.func = va_arg(args, int);
          ast->expression_func.args = va_arg(args, std::vector<node*>*);
          break;

      case EXPRESSION_UNARY_A:
      case EXPRESSION_UNARY_L:
          ast->expression_unary.expression = va_arg(args, node *);
          break;
      
      case EXPRESSION_AND:
      case EXPRESSION_OR:
      case EXPRESSION_EQ:
      case EXPRESSION_NEQ:
      case EXPRESSION_GT:
      case EXPRESSION_LT:
      case EXPRESSION_GET:
      case EXPRESSION_LET:
      case EXPRESSION_ADD:
      case EXPRESSION_SUB:
      case EXPRESSION_MUL:
      case EXPRESSION_DIV:
      case EXPRESSION_POW:
          ast->expression_two_op_operation.left = va_arg(args, node*);
          ast->expression_two_op_operation.right = va_arg(args, node*);
          break;

      case EXPRESSION_BOOL_C:
          ast->expression_bool_c.value = va_arg(args, int);
          break;

      case EXPRESSION_INT_C:
          ast->expression_int_c.value = va_arg(args, int);
          break;
      case EXPRESSION_FLOAT_C:
          ast->expression_float_c.value = va_arg(args, float);
          break;

      case EXPRESSION_VARIABLE:
          ast->expression_variable.id = va_arg(args, char*);
          ast->expression_variable.is_array = va_arg(args, int);
          ast->expression_variable.index = va_arg(args, int);
          break;
 
  // ...

      default: break;
  }

  va_end(args);

  return ast;
}

void ast_free(node *ast) {

}

void ast_print(node * ast) {

}
