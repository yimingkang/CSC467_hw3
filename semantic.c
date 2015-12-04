
#include "semantic.h"

int semantic_check( node *ast) {
  
    if (!ast){
        return 0;
    }

    int kind = ast->kind;
    int scope_depth = 0;
    int val;

    switch(kind){
        case PROGRAM:
            scope_depth++; 
            val = semantic_check(ast->program.scope);
            scope_depth--; 
            return val;

        case SCOPE:
            if (semantic_check(ast->scope.declarations) == -1)
                return -1;
            return semantic_check(ast->scope.declarations);
  
        case NODE_LIST:
            if (semantic_check(ast->node_list.payload) == -1)
                return -1;
            return semantic_check(ast->node_list.next_node);
  
        case DECLARATION:
            break;
  
        case STATEMENT_ASSIGN:
            break;
  
        case STATEMENT_IF:
            break;
      
        case TYPE_INT:
        case TYPE_IVEC:
        case TYPE_BOOL:
        case TYPE_BVEC:
        case TYPE_FLOAT:
        case TYPE_VEC:
            break;
  
        case EXPRESSION_CONSTRUCTOR:
            break;
  
        case EXPRESSION_FUNC:
            break;
  
        case EXPRESSION_UNARY_A:
        case EXPRESSION_UNARY_L:
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
            break;
  
        case EXPRESSION_BOOL_C:
            break;
  
        case EXPRESSION_INT_C:
            break;
        case EXPRESSION_FLOAT_C:
            break;
  
        case EXPRESSION_VARIABLE:
            break;
  
        default:
            // unrecognized
            return 0;
            break;
    }
    return 0; // failed checks
}
