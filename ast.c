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
  int scope_level = 0;

  switch(kind) {
      // program
      case PROGRAM:
          ast->program.scope = va_arg(args, node *);
          break;
    
      // scope
      case SCOPE:
          scope_level++;
          ast->scope.declarations = va_arg(args, node *);
          ast->scope.statements = va_arg(args, node *);
          scope_level--;
          break;
    
      // declarations
      case DECLARATIONS: // includes null
          ast->declarations.declarations = va_arg(args, node *);
          ast->declarations.declaration = va_arg(args, node *);
          break;
    
      // statements:
      case STATEMENTS: // includes null
          ast->statements.statements = va_arg(args, node *);
          ast->statements.statement = va_arg(args, node *);
          break;
    
      // declaration
      case DECLARATION:
          ast->declaration.type = va_arg(args, node *);
          ast->declaration.id = va_arg(args, char *);
          break;

      case INITIALIZED_DECLARATION:
          ast->initialized_declaration.type = va_arg(args, node *);
          ast->initialized_declaration.id = va_arg(args, char *);
          ast->initialized_declaration.expression = va_arg(args, node *);
          break;

      case CONST_DECLARATION:
          ast->const_declaration.type = va_arg(args, node *);
          ast->const_declaration.id = va_arg(args, char *);
          ast->const_declaration.expression = va_arg(args, node *);
          break;
    
      // statement
      case ASSIGNMENT_STATEMENT:
          ast->assignment_statement.variable = va_arg(args, node *);
          ast->assignment_statement.expression = va_arg(args, node *);
          break;

      case IF_ELSE_STATEMENT:
          ast->if_else_statement.if_condition = va_arg(args, node *);
          ast->if_else_statement.statement = va_arg(args, node *);
          ast->if_else_statement.else_statement = va_arg(args, node *);
          break;

      case IF_STATEMENT:
          ast->if_statement.if_condition = va_arg(args, node *);
          ast->if_statement.statement = va_arg(args, node *);
          break;

      case SCOPE_STATEMENT:
          ast->scope_statement.scope = va_arg(args, node *);
          break;

      case SEMICOLEN_STATEMENT:
          // signifies end of statement
          ast->semicolen_statement.null = NULL; 
          break;
    
      // Expression
      case FUNC_EXPRESSION_NODE:
          ast->func_expression_node.func_name = va_arg(args, char *);
          ast->func_expression_node.arguments_opt = va_arg(args, node *);
          break;

      case TYPE_EXPRESSION_NODE:
          ast->type_expression_node.type = va_arg(args, node *);
          ast->type_expression_node.arguments_opt = va_arg(args, node *);
          break;

      case BINARY_EXPRESSION_NODE:
          ast->binary_expression_node.op = va_arg(args, int);
          ast->binary_expression_node.l_val = va_arg(args, node *);
          ast->binary_expression_node.r_val = va_arg(args, node *);
          break;

      case UNARY_EXPRESSION_NODE:
          ast->unary_expression_node.op = va_arg(args, int);
          ast->unary_expression_node.expression = va_arg(args, node *);
          break;

      case PAREN_EXPRESSION_NODE:
          ast->paren_expression_node.expression = va_arg(args, node *);
          break;

      case VARIABLE_EXPRESSION_NODE:
          ast->variable_expression_node.variable = va_arg(args, node *);
          break;

      case LITERAL_EXPRESSION_NODE:
          ast->literal_expression_node.literal_type = va_arg(args, int);
          switch (va_arg(args, int)){
              case INT_LITERAL:
                  ast->literal_expression_node.literal_value.int_val = va_arg(args, int);
                  break;

              case FLOAT_LITERAL:
                  ast->literal_expression_node.literal_value.float_val = va_arg(args, double);
                  break;

              case BOOL_LITERAL:
                  ast->literal_expression_node.literal_value.bool_val = va_arg(args, int);
                  break;

              default:
                  //ERROR
                  printf("!!!!!STOP, unrecognized literal in LITERAL_EXPRESSION_NODE");
          }
          break;
    
      // variable
      case SINGULAR_VARIABLE: 
          ast->singular_variable.id = va_arg(args, char *);
          break;

      case ARRAY_VARIABLE:
          ast->array_variable.id = va_arg(args, char *);
          ast->array_variable.multiplicity = va_arg(args, int) + 1;
          break;
    
      // arguments
      case BINARY_ARGUMENT:
          ast->binary_arguments.arguments = va_arg(args, node *);
          ast->binary_arguments.expression = va_arg(args, node *);
          break;

      case UNARY_ARGUMENT:
          ast->unary_argument.expression = va_arg(args, node *);
          break;
    
      // argument_opt
      case ARGUMENT_OPT:
          ast->arguments_opt.arguments = va_arg(args, node *);
          break;
    
      // type
      case TYPE:
          ast->type.type = va_arg(args, int);
          ast->type.multiplicity = va_arg(args, int);
          break;
    
      case UNKNOWN:
      default:
        printf("!!!!!STOP, unknown type!");
        break;
  }

  va_end(args);

  return ast;
}

void ast_free(node *ast) {

}

void ast_print(node * ast) {
    printf("TEST");

}
