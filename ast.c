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

    if(ast==NULL)
        return;

    int kind;

    kind = ast->kind;


    switch(kind){
      // program
      case PROGRAM:
          ast_free(ast->program.scope);
          free(ast);
          break;
    
      // scope
      case SCOPE:
          ast_free(ast->scope.declarations);
          ast_free(ast->scope.statements);
          free(ast);
          break;
    
      // declarations
      case DECLARATIONS: // includes null
          ast_free(ast->declarations.declarations);
          ast_free(ast->declarations.declaration);
          free(ast);
          break;
    
      // statements:
      case STATEMENTS: // includes null
          ast_free(ast->statements.statements );
          ast_free(ast->statements.statement );
          free(ast);
          break;
    
      // declaration
      case DECLARATION:
          ast_free(ast->declaration.type );
          free(ast);
          break;

      case INITIALIZED_DECLARATION:
          ast_free(ast->initialized_declaration.type );
          ast_free(ast->initialized_declaration.expression );
          free(ast);
          break;

      case CONST_DECLARATION:
          ast_free(ast->const_declaration.type );
          ast_free(ast->const_declaration.expression );
          free(ast);
          break;
    
      // statement
      case ASSIGNMENT_STATEMENT:
          ast_free(ast->assignment_statement.variable );
          ast_free(ast->assignment_statement.expression );
          free(ast);
          break;

      case IF_ELSE_STATEMENT:
          ast_free(ast->if_else_statement.if_condition );
          ast_free(ast->if_else_statement.statement );
          ast_free(ast->if_else_statement.else_statement );
          free(ast);
          break;

      case IF_STATEMENT:
          ast_free(ast->if_statement.if_condition );
          ast_free(ast->if_statement.statement );
          free(ast);
          break;

      case SCOPE_STATEMENT:
          ast_free(ast->scope_statement.scope );
          free(ast);
          break;

      case SEMICOLEN_STATEMENT:
          free(ast);
          break;
    
      // Expression
      case FUNC_EXPRESSION_NODE:
          ast_free(ast->func_expression_node.arguments_opt );
          free(ast);
          break;

      case TYPE_EXPRESSION_NODE:
          ast_free(ast->type_expression_node.type );
          ast_free(ast->type_expression_node.arguments_opt );
          free(ast);
          break;

      case BINARY_EXPRESSION_NODE:
          ast_free(ast->binary_expression_node.l_val);
          ast_free(ast->binary_expression_node.r_val);
          free(ast);
          break;

      case UNARY_EXPRESSION_NODE:
          ast_free(ast->unary_expression_node.expression );
          free(ast);
          break;

      case PAREN_EXPRESSION_NODE:
          ast_free(ast->paren_expression_node.expression );
          free(ast);
          break;

      case VARIABLE_EXPRESSION_NODE:
          ast_free(ast->variable_expression_node.variable );
          free(ast);
          break;

      case LITERAL_EXPRESSION_NODE:
          free(ast);
          break;
    
      // variable
      case SINGULAR_VARIABLE: 
          free(ast);
          break;

      case ARRAY_VARIABLE:
          free(ast);
          break;
          
    
      // ast_free(arguments
      case BINARY_ARGUMENT:
          ast_free(ast->binary_arguments.expression);
          ast_free(ast->binary_arguments.arguments);
          free(ast);
          break;

      case UNARY_ARGUMENT:
          ast_free(ast->unary_argument.expression );
          free(ast);
          break;
    
      // ast_free(argument_opt
      case ARGUMENT_OPT:
          ast_free(ast->arguments_opt.arguments );
          free(ast);
          break;
    
      // type
      case TYPE:
          free(ast);
          break;
    
      case UNKNOWN:
      default:
          free(ast);
        break;
  }
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
       //printf("    ");
    }

    switch(kind){
      // program
      case PROGRAM:
          ast_print(ast->program.scope);
          break;
    
      // scope
      case SCOPE:
          printf("( SCOPE \n");
          print_level++;
          ast_print(ast->scope.declarations);
          ast_print(ast->scope.statements);
          print_level--;
          printf(")\n");
          break;
    
      // declarations
      case DECLARATIONS: // includes null
          printf("( DECLARATIONS \n");
          ast_print(ast->declarations.declarations);
          ast_print(ast->declarations.declaration);
          printf(")\n");
          break;
    
      // statements:
      case STATEMENTS: // includes null
          printf("( STATEMENTS \n");
          ast_print(ast->statements.statements );
          ast_print(ast->statements.statement );
          printf(")\n");
          break;
    
      // declaration
      case DECLARATION:
          printf("( DECLARATION \n");
          printf(" ID=%s ", ast->declaration.id);
          ast_print(ast->declaration.type );
          printf(")\n");
          break;

      case INITIALIZED_DECLARATION:
          printf("( DECLARATION \n");
          printf(" ID=%s ", ast->declaration.id);
          ast_print(ast->initialized_declaration.type );
          ast_print(ast->initialized_declaration.expression );
          printf(")\n");
          break;

      case CONST_DECLARATION:
          printf("( DECLARATION \n");
          printf(" ID=%s ", ast->declaration.id);
          ast_print(ast->const_declaration.type );
          ast_print(ast->const_declaration.expression );
          printf(")\n");
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
          int func_code = ast->func_expression_node.func_name;
          int over_1 = 0;
          int over_2 = 0;
          int over_3 = 0;
          int over_4 = 0;
            switch(func_code){
                case 0:
                    //dp3
                    over_1 = arg_check(ast->func_expression_node.arguments_opt, (int)VEC_TYPE, 3, 2, sb_table, scope); // mult = 4, num of args = 2;
                    over_2 = arg_check(ast->func_expression_node.arguments_opt, (int)VEC_TYPE, 2, 2, sb_table, scope); //mult = 3, num of args = 2;
                    over_3 = arg_check(ast->func_expression_node.arguments_opt, (int)IVEC_TYPE, 3, 2, sb_table, scope); // mult = 4, num of args = 2;
                    over_4 = arg_check(ast->func_expression_node.arguments_opt, (int)IVEC_TYPE, 2, 2, sb_table, scope); //mult = 3, num of args = 2;
                    if (over_1 ==0 && over_2 ==0 && over_3==0 && over_4==0){
                        printf("no overload for dp3 found\n");
                    }

                    ast_check_res res;
                    if (over_1==1 || over_2==1){
                        res.type.type_code = (int) FLOAT_TYPE;
                        res.type.multiplicity = 0;
                    }else if (over_3 ==1 || over_4 ==1){
                        res.type.type_code = (int) INT_TYPE;
                        res.type.multiplicity = 0;
                    }else{
                        res.type.type_code = (int) INT_TYPE;
                        res.type.multiplicity = 0;
                    }
                    return res;

                case 1:
                    over_1 = arg_check(ast->func_expression_node.arguments_opt, (int)VEC_TYPE, 3, 2, sb_table, scope); // mult = 4, num of args = 2;
                    if(over_1 ==0){
                        printf("no overload for lit\n");
                    }
                    ast_check_res res;
                    res.type.type_code = (int) VEC_TYPE;
                    res.type.multiplicity = 3;
                    return res; //lit
                case 2:
                    over_1 = arg_check(ast->func_expression_node.arguments_opt, (int)INT_TYPE, 0, 1, sb_table, scope); // mult = 0, num of args = 1;
                    over_2 = arg_check(ast->func_expression_node.arguments_opt, (int)FLOAT_TYPE, 0, 1, sb_table, scope); // mult = 0, num of args = 1;
                    if(over_1 ==0&&over_2==0){
                        printf("no overload for lit\n");
                    }
                    ast_check_res res;
                    res.type.type_code = (int) FLOAT_TYPE;
                    res.type.multiplicity = 0;
                    return res; //lit
                default:
                    printf ("unknow function \n");
                    ast_check_res res;
                    res.type.type_code = (int) ANY_TYPE;
                    res.type.multiplicity = 0;
                    return res; 
            }
          break;

      case TYPE_EXPRESSION_NODE:
          ast_check_res type = ast_check(ast->type_expression_node.type, sb_table, scope );
          int r = arg_check(ast->type_expression_node.arguments_opt, type.type.type_code, type.type.multiplicity, sb_table, scope);
          if (r ==0){
              printf("no overload for constructor of %s\n", type_str(type.type.type_code, type.type.multiplicity));
          }
          ast_check_res res;
          res.type.type_code = type.type.type_code;
          res.type.multiplicity = type.type.multiplicity;
          return res;
          break;

      case BINARY_EXPRESSION_NODE:
          ast_check_res l = ast_check(ast->binary_expression_node.l_val, sb_table, scope);
          printf(" %s ", bin_op_str(ast->binary_expression_node.op));
          as_check_res r = ast_check(ast->binary_expression_node.r_val, sb_table, scope);
          switch(ast->binary_expression_node.op){
              case PLUS_OP:
              case MINUS_OP:
              case AND_OP:
              case OR_OP:
              case EQ_OP:
              case NEQ_OP:
                 ast_check_res res;
                 if (l.type.type_code != r.type.type_code || l.type.multiplicity != r.type.multiplicity){
                     res.type.type_code = ANY_TYPE;
                     res.type.multiplicity = 0;
                     printf("operation cannot be done on mixed type\n");
                     return res;
                 }
                 res.type.type_code = l.type.type_code;
                 res.type.multiplicity = l.type.multiplicity;
                 return res;
              case DIV_OP:
              case POW_OP:
              case LT_OP:
              case LEQ_OP:
              case GT_OP:
              case GEQ_OP:
                 ast_check_res res;
                 if (l.type.multiplicity>0 || r.type.multiplicity>0){
                     res.type.type_code = ANY_TYPE;
                     res.type.multiplicity = 0;
                     printf("operation cannot be done on vectors\n");
                     return res;
                 }
                 if (l.type.type_code != r.type.type_code){
                     res.type.type_code = ANY_TYPE;
                     res.type.multiplicity = 0;
                     printf("operation cannot be done on mixed type\n");
                     return res;
                 }
                 res.type.type_code = l.type.type_code;
                 res.type.multiplicity = l.type.multiplicity;
                 return res;
                case MULT_OP:
                 int l_s_type = to_s_type(l.type.type_code);
                 int r_s_type = to_s_type(r.type.type_code);
                 int l_m = l.type.multiplicity;
                 int r_m = r.type.multiplicity;
                 ast_check_res res;
                 res.type.multiplicity = (l_m>r_m) ? l_m:r_m;
                 if (l_m !=0 && r_m !=0 && l_m != r_m){
                     printf("vector length does not match \n");
                     res.type.multiplicity = 0;
                 }
                 res.type.type_code = to_v_type(l_s_type, res.type.multiplicity);
                 if (l_s_type != r_s_type){
                     printf("base type does not match \n");
                     res.type.type_code = ANY_TYPE;
                 }
                 return res;
                 
          }
          break;

      case UNARY_EXPRESSION_NODE:
          printf(" %s ", u_op_str(ast->unary_expression_node.op));
          ast_check_res exp= ast_check(ast->unary_expression_node.expression, sb_table, scope);
          ast_check_res res;
          res.type.type_code = exp.type.type_code;
          res.type.multiplicity = exp.type.multiplicity;
          return res;
          break;

      case PAREN_EXPRESSION_NODE:
          return ast_check(ast->paren_expression_node.expression, sb_table, scope);
          break;

      case VARIABLE_EXPRESSION_NODE:
          return ast_check(ast->variable_expression_node.variable, sb_table, scope);
          break;

      case LITERAL_EXPRESSION_NODE:
          //ast_print(ast->literal_expression_node.literal_type );

          ast_check_res res;
          res.type.type_code = ast->literal_expression_node.literal_type;
          res.type.multiplicity = 0;
          return res;
          break;
    
      // variable
      case SINGULAR_VARIABLE: 
          ast_check_res var = find_no_type(sb_table, ast->singular_variable.id, scope);
          ast_check_res res;
          if (var){
                res.type.type_code = var.type.type_code;
                res.type.multiplicity =0;
                return res;
         }else {
             printf("variable %s is undefined\n", ast->singular_variable.id);
            res.type.type_code = ANY_TYPE;
            res.type.multiplicity = 0 ;
            return res;
         }
          break;

      case ARRAY_VARIABLE:
          ast_check_res var = find_no_type(sb_table, ast->array_variable.id, scope);
          ast_check_res res;
          if (ast->array_variable.multiplicity <0 || var.type.multiplicity > ast->array_variable.multiplicity){
              printf("array index out of bound \n");
        }
          if (var){
                res.type.type_code = var.type.type_code;
                res.type.multiplicity =var.type.multiplicity;
                return res;
         }else {
             printf("variable %s is undefined\n", ast->array_variable.id);
            res.type.type_code = ANY_TYPE;
            res.type.multiplicity = 0 ;
            return res;
         }
          break;
    
      // ast_print(arguments
      case BINARY_ARGUMENT:
          return NULL; //not handled here
          break;

      case UNARY_ARGUMENT:
          return NULL;
          break;

    
      // ast_print(argument_opt
      case ARGUMENT_OPT:
          return NULL;
          break;
    
      // type
      case TYPE:
          ast_check_res res;
          res.type.type_code = ast->type.var_type;
          res.type.multiplicity = ast->type.multiplicity;
          return res;
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
                case 1:
                    return "IVEC2";
                case 2:
                    return "IVEC3";
                case 3:
                    return "IVEC4";
                default: 
                    return "IVEC?";
            }
        case BOOL_TYPE:
            return "BOOL";
        case BVEC_TYPE:
            switch(multiplicity){
                case 1:
                    return "BVEC2";
                case 2:
                    return "BVEC3";
                case 3:
                    return "BVEC4";
                default: 
                    return "BVEC?";
            }
        case FLOAT_TYPE:
            return "FLOAT";
        case VEC_TYPE:
            switch(multiplicity){
                case 1:
                    return "VEC2";
                case 2:
                    return "VEC3";
                case 3:
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

struct {
    union{
        symbol* sb_table; // returned by declaration/declarations 
        struct {  //returned by expression/variable/type
            int type_code;
            int multiplicity;
        } type;
    };
}ast_check_res;

int arg_check(node * ast, int type_code, int mult, symbol* sb_table, int scope){
    if (ast==NULL)
        return;
    int kind = ast->kind;
    switch(kind){

      case BINARY_ARGUMENT:
          int r = arg_check(ast->binary_arguments.arguments, type_code,mult, sb_table, scope);
          ast_check_res exp = ast_check(ast->unary_argument.expression, sb_table, scope);
          if (exp.type.type_code == type_code && exp.type.multiplicity == mult){
              return 1|r;
          } else{
              return 0|r;
        }
          break;

      case UNARY_ARGUMENT:
          ast_check_res exp = ast_check(ast->unary_argument.expression, sb_table, scope);
          if (exp.type.type_code == type_code && exp.type.multiplicity == mult){
              return 1;
          } else{
              return 0;
        }
          break;
    
      // ast_print(argument_opt
      case ARGUMENT_OPT:
          return arg_check(ast->arguments_opt.arguments, type_code, mult, sb_table,scope);
          break;
    }

}

int find(symbol* sb_table, int type_code, int mult, char* id, int scope){
    symbol* head = sb_table;
    while ( head !=NULL){
        if (head->scope !=scope )
            return 0;
        if (type_code == head->type_code && mult == head->mult && strcmp(id, head->id)==0){
            return 1;
    }
        head = head->prev;
    }
    return 0;
}
ast_check_res find_no_type(symbol* sb_table, char* id, int scope){
    symbol* head = sb_table;
    while ( head !=NULL){
        if (strcmp(id, head->id)==0){
           ast_check_res res;
          res.type.type_code = head->type_code;
         res.type.multiplicity = head->mult; 
    }
        head = head->prev;
    }
    return NULL;
    

}

symbol*  add_to_table(symbol* sb_table,int type_code,int mult, char* id, int scope, SYMBOL_ATTRIBUTE  attr){
    symbol* new_s = malloc(sizeof(symbol));
    new_s ->type_code = type_code;
    new_s->id = id;
    new_s->symbol_attribute = (int) attr;
    new_s->prev = sb_table;
    return new_s;
}
void ast_check(node * ast, symbol* sb_table,int scope){
    
    if(ast==NULL)
        return;

    int kind;

    kind = ast->kind;

    switch(kind){
      // program
      case PROGRAM:
          return ast_check->(ast->program.scope, sb_table, scope);
          break;
    
      // scope
      case SCOPE:
          ast_check_res res = ast_check(ast->scope.declarations, sb_table, scope+1);
          symbol* new_table = res.sb_table;
          ast_print(ast->scope.statements, new_table, scope+1);
          //remove_sb_table(new_table, sb_table);
          return NULL;
          break;
    
      // declarations
      case DECLARATIONS: // includes null
          if (ast->declarations.declarations!=NULL){
                ast_check_res res = ast_check(ast->declarations.declarations, sb_table, scope);//depth first search
                sb_table = res.sb_table;
          }
          ast_check(ast->declarations.declaration, sb_table, scope);
          break;
    
      // statements:
      case STATEMENTS: // includes null
          printf("( STATEMENTS \n");
          ast_print(ast->statements.statements );
          ast_print(ast->statements.statement );
          printf(")\n");
          break;
    
      // declaration
      case DECLARATION:
          ast_check_res res = ast_check(ast->declaration.type,sb_table, scope);
          int typecode = res.type.type_code;
          int mult = res.type.multiplicity;
          if (find (sb_table, typecode, ast->declaration.id, scope)){
                printf("duplicated declaration of %s\n",ast->declaration.id);
          }else{
                sb_table = add_to_table(sb_table, typecode,mult, ast->declaration.id,scope, NONE);
          }
          ast_check_res res;
          res.sb_table = sb_table;
          return ast_check_res;
          break;

      case INITIALIZED_DECLARATION:
          ast_check_res res = ast_check(ast->declaration.type,sb_table, scope);
          int typecode = res.type.type_code;
          int mult = res.type.multiplicity;
          if (find (sb_table, typecode, ast->declaration.id, scope)){
                printf("duplicated declaration of %s\n",ast->declaration.id);
          }else{
                ast_check_res exp_res = ast_check(ast->declaration.expression, sb_table, scope);
                int exp_type = exp_res.type.type_code;
                int exp_mult = exp_res.type.muliplicity;
                if (exp_type != typecode || exp_mult !=mult){
                    printf ("expression type %s cannot be assigned to %s\n", type_str(exp_type,exp_mult),type_str(typecode,mult));
                }
                sb_table = add_to_table(sb_table, typecode,mult, ast->declaration.id,scope, NONE);
          }
          ast_check_res res;
          res.sb_table = sb_table;
          return ast_check_res;
          break;

      case CONST_DECLARATION:
          ast_check_res res = ast_check(ast->declaration.type,sb_table, scope);
          int typecode = res.type.type_code;
          int mult = res.type.multiplicity;
          if (find (typecode, mult, ast->declaration.id, scope)){
                printf("duplicated declaration of %s\n",ast->declaration.id);
          }else{
                ast_check_res exp_res = ast_check(ast->declaration.expression, sb_table, scope);
                int exp_type = exp_res.type.type_code;
                int exp_mult = exp_res.type.muliplicity;
                if (exp_type != typecode || exp_mult !=mult){
                    printf ("expression type %s cannot be assigned to %s\n", type_str(exp_type,exp_mult),type_str(typecode,mult));
                }
                sb_table = add_to_table(sb_table, typecode,mult, ast->declaration.id,scope, UNIFORM);
          }
          ast_check_res res;
          res.sb_table = sb_table;
          return ast_check_res;
          break;
    
      // statement
      case ASSIGNMENT_STATEMENT:
          ast_check_res var = ast_check(ast->assignment_statement.variable,sb_table, scope);
          ast_check_res exp = ast_check(ast->assignment_statement.expression,sb_table,scope);
          if (var.type.type_code != exp.type.type_code || var.type.multiplicity != exp.type.multiplicity){
              printf("expression type %s cannot be assigned to %s\n", type_str(exp.type.type_code, var.type.multiplicity));
          }
          return NULL;
          break;

      case IF_ELSE_STATEMENT:
          ast_check_res exp = ast_check(ast->if_else_statement.if_condition,sb_table,scope );
          if (exp.type.type_code !=(int) BOOL_TYPE){
              printf("if condition must evaluate to boolean\n");
          }
          ast_check(ast->if_else_statement.statement, sb_table, scope );
          ast_check(ast->if_else_statement.else_statement, sb_table, scope );
          return NULL;
          break;

      case IF_STATEMENT:
          ast_check_res exp = ast_check(ast->if_else_statement.if_condition,sb_table,scope );
          if (exp.type.type_code !=(int) BOOL_TYPE){
              printf("if condition must evaluate to boolean\n");
          }
          ast_check(ast->if_else_statement.statement, sb_table, scope );
          return NULL;
          break;

      case SCOPE_STATEMENT:
          ast_check(ast->scope_statement.scope,sb_table, scope);
          return NULL;
          break;

      case SEMICOLEN_STATEMENT:
          //not used
          return NULL;
          break;
    
      // Expression
      case FUNC_EXPRESSION_NODE:
          printf("( CALL %s ", get_func(ast->func_expression_node.func_name));
          ast_print(ast->func_expression_node.arguments_opt );
          printf(")\n");
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
