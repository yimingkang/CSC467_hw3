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
          ast->scope.statements = va_arg(args, std::vector<node*>*);
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

void node_next(node* ast, void (*visitor)(node* n)){
    node_kind kind = ast->kind;
     switch (kind){
    
      case PROGRAM:
          visitor(ast->program.scope);
          break;

      case SCOPE:
          for (std::vector<node*>::iterator it = ast->scope.declarations->begin(); it!=ast->scope.declarations->end();++it){
              visitor(*it);
          }
          for (std::vector<node*>::iterator it = ast->scope.statements->begin(); it!=ast->scope.statements->end();++it){
              visitor(*it);
          }
          break;

      case DECLARATION:
          visitor(ast->declaration.type);
          visitor(ast->declaration.expression);
          break;

      case STATEMENT_ASSIGN:
          visitor(ast->statement_assign.variable);
          visitor(ast->statement_assign.expression);
          break;

      case STATEMENT_IF:
          visitor(ast->statement_if.condition_expression);
          visitor(ast->statement_if.if_body);
          visitor(ast->statement_if.else_body);
          break;
    
      case TYPE_INT:
      case TYPE_IVEC:
      case TYPE_BOOL:
      case TYPE_BVEC:
      case TYPE_FLOAT:
      case TYPE_VEC:
          break;

      case EXPRESSION_CONSTRUCTOR:
          visitor(ast->expression_constructor.type);
          for (std::vector<node*>::iterator it = ast->expression_constructor.args->begin(); it!=ast->expression_constructor.args->end();++it){
              visitor(*it);
          }
          break;

      case EXPRESSION_FUNC:
          for (std::vector<node*>::iterator it = ast->expression_func.args->begin(); it!=ast->expression_func.args->end();++it){
              visitor(*it);
          }
          break;

      case EXPRESSION_UNARY_A:
      case EXPRESSION_UNARY_L:
          visitor(ast->expression_unary.expression);
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
          visitor(ast->expression_two_op_operation.left);;
          visitor(ast->expression_two_op_operation.right);;
          break;

      case EXPRESSION_BOOL_C:
      case EXPRESSION_INT_C:
      case EXPRESSION_FLOAT_C:
      case EXPRESSION_VARIABLE:
          break;

      default: break;
    }

   

}


void print_action(node * ast) {
    node_kind kind = ast->kind;
    switch (kind){
    
      case PROGRAM:
          printf("PROGRAM\n");
          break;

      case SCOPE:
          printf("SCOPE\n");
          break;

      case DECLARATION:
          if (ast->declaration.const_f ==1){
              printf("DECLARATION const %s\n", ast->declaration.id); 
          }else{
              printf("DECLARATION %s\n",ast->declaration.id);
          }
          break;

      case STATEMENT_ASSIGN:
          printf("ASSIGN\n");
          break;

      case STATEMENT_IF:
          if (ast->statement_if.else_body){
              printf("IF...THEN...ELSE...\n");
          }else{
              printf("IF...THEN...\n");
          }
          break;
    
      case TYPE_INT:
          printf("int\n");
          break;
      case TYPE_IVEC:
          printf("ivec%d\n",ast->type.param);
          break;
      case TYPE_BOOL:
          printf("bool\n");
          break;
      case TYPE_BVEC:
          printf("bvec%d\n",ast->type.param);
          break;
      case TYPE_FLOAT:
          printf("float\n");
          break;
      case TYPE_VEC:
          printf("vec%d\n", ast->type.param);
          break;

      case EXPRESSION_CONSTRUCTOR:
          printf("CONSTRUCTOR\n");
          break;

      case EXPRESSION_FUNC:
          switch(ast->expression_func.func){
              case 0:
                printf("CALL dp3\n");
                break;
              case 1:
                printf("CALL lit\n");
                break;
              case 2:
                printf("CALL rsq\n");
                break;
            }
          break;

      case EXPRESSION_UNARY_A:
          printf("UNARY -\n");
          break;
      case EXPRESSION_UNARY_L:
          printf("UNARY !\n");
          break;
      
      case EXPRESSION_AND:
          printf("BINARY and\n");
          break;
      case EXPRESSION_OR:
          printf("BINARY or\n");
          break;
      case EXPRESSION_EQ:
          printf("BINARY eq\n");
          break;
      case EXPRESSION_NEQ:
          printf("BINARY neq\n");
          break;
      case EXPRESSION_GT:
          printf("BINARY ge\n");
          break;
      case EXPRESSION_LT:
          printf("BINARY le\n");
          break;
      case EXPRESSION_GET:
          printf("BINARY geq\n");
          break;
      case EXPRESSION_LET:
          printf("BINARY leq\n");
          break;
      case EXPRESSION_ADD:
          printf("BINARY add\n");
          break;
      case EXPRESSION_SUB:
          printf("BINARY sub\n");
          break;
      case EXPRESSION_MUL:
          printf("BINARY mul\n");
          break;
      case EXPRESSION_DIV:
          printf("BINARY div\n");
          break;
      case EXPRESSION_POW:
          printf("BINARY pow\n");
          break;

      case EXPRESSION_BOOL_C:
          if (ast->expression_bool_c.value == 1){
              printf("true\n");
          }else{
              printf("false\n");
          }
          break;

      case EXPRESSION_INT_C:
          printf("%d\n",ast->expression_int_c.value);
          break;
      case EXPRESSION_FLOAT_C:
          printf("%f\n", ast->expression_float_c.value);
          break;

      case EXPRESSION_VARIABLE:
          if (ast->expression_variable.is_array ==1){
                printf("INDEX %s %d\n", ast->expression_variable.id, ast->expression_variable.index);
          }else{
                printf("VARIABLE %s\n", ast->expression_variable.id);
          break;

          }
       default: break;
    }

}

void ast_print(node * ast) {
   print_action(ast);
   node_next(ast, &ast_print); 
}
