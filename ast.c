#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

#define DEBUG_PRINT_TREE 0

node *ast = NULL;

char *bin_op_str(int);
char *u_op_str(int);
char *type_str(int, int);
char *get_func(int);

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
          ast->func_expression_node.func_name = va_arg(args, int);
          ast->func_expression_node.arguments_opt = va_arg(args, node *);
          break;

      case TYPE_EXPRESSION_NODE:
          ast->type_expression_node.type = va_arg(args, node *);
          ast->type_expression_node.arguments_opt = va_arg(args, node *);
          break;

      case BINARY_EXPRESSION_NODE:
          ast->binary_expression_node.l_val = va_arg(args, node *);
          ast->binary_expression_node.op = va_arg(args, int);
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
          switch (ast->literal_expression_node.literal_type){
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
                  printf("!!!!!STOP, unrecognized literal in LITERAL_EXPRESSION_NODE\n");
          }
          break;
    
      // variable
      case SINGULAR_VARIABLE: 
          ast->singular_variable.id = va_arg(args, char *);
          break;

      case ARRAY_VARIABLE:
          ast->array_variable.id = va_arg(args, char *);
          ast->array_variable.multiplicity = va_arg(args, int) ; // max index
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
          ast->type.var_type = va_arg(args, int);
          ast->type.multiplicity = va_arg(args, int);
          break;
    
      case UNKNOWN:
      default:
        printf("!!!!!STOP, unknown type!\n");
        break;
  }

  va_end(args);

  return ast;
}

void ast_free(node *ast) {

}

int print_level = 0;

void ast_print(node * ast) {
    /*
     * Perform a pre AND post order traversal
     *
     */

    if(ast==NULL)
        return;

    int kind;

    kind = ast->kind;

    int i = 0;
    for (i = 0; i < print_level; i ++){
       printf("    ");
    }

    switch(kind){
      // program
      case PROGRAM:
          ast_print(ast->program.scope);
          break;
    
      // scope
      case SCOPE:
          printf("( SCOPE ");
          print_level++;
          ast_print(ast->scope.declarations);
          ast_print(ast->scope.statements);
          print_level--;
          printf(")");
          break;
    
      // declarations
      case DECLARATIONS: // includes null
          printf("( DECLARATIONS ");
          ast_print(ast->declarations.declarations);
          ast_print(ast->declarations.declaration);
          printf(")");
          break;
    
      // statements:
      case STATEMENTS: // includes null
          printf("( STATEMENTS ");
          ast_print(ast->statements.statements );
          ast_print(ast->statements.statement );
          printf(")");
          break;
    
      // declaration
      case DECLARATION:
          printf("( DECLARATION ");
          printf(" ID=%s ", ast->declaration.id);
          ast_print(ast->declaration.type );
          printf(")");
          break;

      case INITIALIZED_DECLARATION:
          printf("( DECLARATION ");
          printf(" ID=%s ", ast->declaration.id);
          ast_print(ast->initialized_declaration.type );
          ast_print(ast->initialized_declaration.expression );
          printf(")");
          break;

      case CONST_DECLARATION:
          printf("( DECLARATION ");
          printf(" ID=%s ", ast->declaration.id);
          ast_print(ast->const_declaration.type );
          ast_print(ast->const_declaration.expression );
          printf(")");
          break;
    
      // statement
      case ASSIGNMENT_STATEMENT:
          printf("( ASSIGN ");
          ast_print(ast->assignment_statement.variable );
          ast_print(ast->assignment_statement.expression );
          printf(")");
          break;

      case IF_ELSE_STATEMENT:
          printf("( IF \n");
          print_level++;
          ast_print(ast->if_else_statement.if_condition );
          ast_print(ast->if_else_statement.statement );
          ast_print(ast->if_else_statement.else_statement );
          print_level--;
          printf(")\n");
          break;

      case IF_STATEMENT:
          printf("( IF \n");
          print_level++;
          ast_print(ast->if_statement.if_condition );
          ast_print(ast->if_statement.statement );
          print_level--;
          printf(")\n");
          break;

      case SCOPE_STATEMENT:
          ast_print(ast->scope_statement.scope );
          break;

      case SEMICOLEN_STATEMENT:
          // signifies end of statement
          // ast_print(ast->semicolen_statement.null );
          break;
    
      // Expression
      case FUNC_EXPRESSION_NODE:
          printf("( CALL %s ", get_func(ast->func_expression_node.func_name));
          ast_print(ast->func_expression_node.arguments_opt );
          printf(")");
          break;

      case TYPE_EXPRESSION_NODE:
          ast_print(ast->type_expression_node.type );
          ast_print(ast->type_expression_node.arguments_opt );
          break;

      case BINARY_EXPRESSION_NODE:
          printf("( BINARY ");
          ast_print(ast->binary_expression_node.l_val);
          printf(" %s ", bin_op_str(ast->binary_expression_node.op));
          ast_print(ast->binary_expression_node.r_val);
          printf(")");
          break;

      case UNARY_EXPRESSION_NODE:
          printf("( UNARY ");
          printf(" %s ", u_op_str(ast->unary_expression_node.op));
          ast_print(ast->unary_expression_node.expression );
          printf(")");
          break;

      case PAREN_EXPRESSION_NODE:
          printf("( ");
          ast_print(ast->paren_expression_node.expression );
          printf(")");
          break;

      case VARIABLE_EXPRESSION_NODE:
          ast_print(ast->variable_expression_node.variable );
          break;

      case LITERAL_EXPRESSION_NODE:
          //ast_print(ast->literal_expression_node.literal_type );
          switch (ast->literal_expression_node.literal_type){
              case INT_LITERAL:
                  printf(" %d ", ast->literal_expression_node.literal_value.int_val);
                  break;

              case FLOAT_LITERAL:
                  printf(" %f ", ast->literal_expression_node.literal_value.float_val);
                  break;

              case BOOL_LITERAL:
                  printf(" %s ", ast->literal_expression_node.literal_value.bool_val ? "true" : "false");
                  break;

              default:
                  //ERROR
                  printf("!!!!!STOP, unrecognized literal in LITERAL_EXPRESSION_NODE\n");
          }
          break;
    
      // variable
      case SINGULAR_VARIABLE: 
          printf(" %s ", ast->singular_variable.id);
          break;

      case ARRAY_VARIABLE:
          printf("( INDEX ");
          printf(" %s ", ast->array_variable.id);
          printf(" at_idx=%d", ast->array_variable.multiplicity);
          printf(")");
          break;
    
      // ast_print(arguments
      case BINARY_ARGUMENT:
          ast_print(ast->binary_arguments.arguments );
          ast_print(ast->binary_arguments.expression );
          break;

      case UNARY_ARGUMENT:
          ast_print(ast->unary_argument.expression );
          break;
    
      // ast_print(argument_opt
      case ARGUMENT_OPT:
          ast_print(ast->arguments_opt.arguments );
          break;
    
      // type
      case TYPE:
          printf(" type=%s ", type_str(ast->type.var_type, ast->type.multiplicity));
          //ast_print(ast->type.multiplicity );
          break;
    
      case UNKNOWN:
      default:
        printf("!!!!!STOP, unknown type!\n");
        break;
  }
}

char *bin_op_str(int op_code){
    switch (op_code){
        case PLUS_OP:
            return "+";
        case MINUS_OP:
            return "-";
        case MULT_OP:
            return "*";
        case DIV_OP:
            return "/";
        case POW_OP:
            return "^";
        case GEQ_OP:
            return ">=";
        case GT_OP:
            return ">";
        case LT_OP:
            return "<";
        case LEQ_OP:
            return "<=";
        case NEQ_OP:
            return "!=";
        case EQ_OP:
            return "=";
        case AND_OP:
            return "&&";
        case OR_OP:
            return "||";
    }
}

char *u_op_str(int op_code){
    switch(op_code){
        case UMINUS_OP:
            return "-";
        case UNEGATION_OP:
            return "!";
    }
}

char *type_str(int type_code, int multiplicity){
    switch(type_code){
        case INT_TYPE:
            return "INT";
        case IVEC_TYPE:
            switch(multiplicity){
                case 2:
                    return "IVEC2";
                case 3:
                    return "IVEC3";
                case 4:
                    return "IVEC4";
                default: 
                    return "IVEC?";
            }
        case BOOL_TYPE:
            return "BOOL";
        case BVEC_TYPE:
            switch(multiplicity){
                case 2:
                    return "BVEC2";
                case 3:
                    return "BVEC3";
                case 4:
                    return "BVEC4";
                default: 
                    return "BVEC?";
            }
        case FLOAT_TYPE:
            return "FLOAT";
        case VEC_TYPE:
            switch(multiplicity){
                case 2:
                    return "VEC2";
                case 3:
                    return "VEC3";
                case 4:
                    return "VEC4";
                default: 
                    return "VEC?";
            }
        default:
            return "UNKNOWN_TYPE";
    }
}

char *get_func(int func_code){
    switch(func_code){
        case 0:
            return "DP3";
        case 1:
            return "LIT";
        case 2:
            return "RSQ";
        default:
            return "UNKOWN_FUNC";
    }
}
