#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"

int scope_level = 0;
int scope_base = 0;

enum {
    INT,
    FLOAT,
    BOOL,
    IVEC2,
    IVEC3,
    IVEC4,
    VEC2,
    VEC3,
    VEC4,
    BVEC2,
    BVEC3,
    BVEC4,
};
int get_type_by_id(char *id, int scope){
    return 0;
}

int check_existance(char *name, int scope){
    // TODO: finish this function
    return -1;
}

int check_args(node *starting_node){
    if (starting_node == NULL){
        return 0;
    }
    if (starting_node->kind == BINARY_ARGUMENT){
        return 1 + check_args(starting_node->binary_arguments.arguments);
    }
    else{
        return 1;
    }
}


int semantic_check( node *ast) {
    if(ast==NULL){
        //printf("ERROR empty tree\n");
        return 0;
    }

    int kind, expr_type, nargs, func_code, var_type;
    int right_exp, left_exp, op_type, rtype, ltype;
    int multiplicity, arg_type, condition_type, type;
    int declared;
    kind = ast->kind;
    printf("KIND->%d\n", kind);

  switch(kind){
    // program
    case PROGRAM:
        return semantic_check(ast->program.scope );
  
    // scope
    case SCOPE:
        scope_level++;
        left_exp = semantic_check(ast->scope.declarations );
        right_exp = semantic_check(ast->scope.statements );
        scope_level--;
        if (left_exp == -1 || right_exp == -1){
            return -1;
        }
        return 0;
  
    // declarations
    case DECLARATIONS: // includes null
        left_exp = semantic_check(ast->declarations.declarations );
        right_exp = semantic_check(ast->declarations.declaration );
        if (left_exp == -1 || right_exp == -1){
            return -1;
        }
        return 0;
  
    // statements:
    case STATEMENTS: // includes null
        left_exp = semantic_check(ast->statements.statements );
        right_exp = semantic_check(ast->statements.statement );
        if (left_exp == -1 || right_exp == -1){
            return -1;
        }
        return 0;
  
    // declaration
    case DECLARATION:
        declared = check_existance(ast->declaration.id, scope_level);
        if (declared != -1){
            // -1 means not yet defined
            printf("ERROR: Cannot declare variable (already_defined?)\n");
            return -1;
        }
        return semantic_check(ast->declaration.type);

    case INITIALIZED_DECLARATION:
        left_exp = semantic_check(ast->initialized_declaration.type);
        declared = check_existance(ast->initialized_declaration.id, scope_level);
        if (declared != -1){
            // -1 means not yet defined
            printf("ERROR: Cannot declare variable (already_defined?)\n");
            return -1;
        }
        right_exp = semantic_check(ast->initialized_declaration.expression);
        if (left_exp == -1 || right_exp == -1)
            return -1;

        if (left_exp != right_exp){
            printf("ERROR: initialization type and provided type mismatch\n");
            return -1;
        }
        return 0;

    case CONST_DECLARATION:
        left_exp = semantic_check(ast->const_declaration.type );
        right_exp = semantic_check(ast->const_declaration.expression );
        if (left_exp == -1 || right_exp == -1)
            return -1;
        if (left_exp != right_exp){
            printf("ERROR: Const initialization type mismatch\n");
            return -1;
        }
        declared = check_existance(ast->const_declaration.id, scope_level);
        if (declared != -1){
            printf("ERROR: Cannot declared variable (already_defined)\n");
            return -1;
        }
        // TODO: check const assignment
        return 0;
  
    // statement
    case ASSIGNMENT_STATEMENT:
        left_exp = semantic_check(ast->assignment_statement.variable );
        right_exp = semantic_check(ast->assignment_statement.expression );
        if (left_exp == -1 or right_exp == -1)
            return -1;
        if (left_exp != right_exp){
            printf("ERROR: Assignment left/right values type mismatch\n");
            return -1;
        }
        // TODO: check const, fail when assigned
        return 0;

    case IF_ELSE_STATEMENT:
        condition_type = semantic_check(ast->if_else_statement.if_condition );
        if (condition_type != BOOL_TYPE){
            printf("ERROR: IF condition must be of BOOL type\n");
            return -1;
        }
        left_exp = semantic_check(ast->if_else_statement.statement );
        right_exp = semantic_check(ast->if_else_statement.else_statement );
        if (left_exp == -1 || right_exp == -1)
            return -1;
        return 0;

    case IF_STATEMENT:
        condition_type = semantic_check(ast->if_statement.if_condition );
        if (condition_type != BOOL_TYPE){
            printf("ERROR: IF condition must be of BOOL type\n");
            return -1;
        }
        left_exp = semantic_check(ast->if_statement.statement );
        if (left_exp == -1)
            return -1;
        return 0;

    case SCOPE_STATEMENT:
        left_exp = semantic_check(ast->scope_statement.scope );
        if (left_exp == -1)
            return -1;
        return 0;

    case SEMICOLEN_STATEMENT:
        // signifies end of statement
        return 0;
  
    // Expression
    case FUNC_EXPRESSION_NODE:
        // TODO: Check number of arguments and EACH of the argumetn
        
        arg_type = semantic_check(ast->func_expression_node.arguments_opt );
        if (arg_type == -1)
            return -1;

        func_code = ast->func_expression_node.func_name;
        switch (func_code){
            case 0:
                // dp3
                if (arg_type == VEC4 || arg_type == VEC3){
                    return FLOAT;
                }
                else if (arg_type == IVEC4 || arg_type == IVEC3){
                    return INT;
                }
                else{
                    printf("ERROR: FUNC has incorrect arg type\n");
                    return -1;
                }
            case 1:
                // lit
                if (arg_type == VEC4){
                    return VEC4;
                }else{
                    printf("ERROR: FUNC has incorrect arg type\n");
                    return -1;
                }
            case 2:
                // rsq
                if (arg_type == FLOAT || arg_type == INT){
                    return INT;
                }else{
                    return -1;
                }
        }

    case TYPE_EXPRESSION_NODE:
        // constructor call
        // TODO: Complete the function check_args
        // TODO: Must also check if ALL args are of the smae time
        arg_type = semantic_check(ast->type_expression_node.type );
        expr_type = semantic_check(ast->type_expression_node.arguments_opt);
        nargs = -1;
        if (arg_type != expr_type){
            printf("ERROR: Constructor argument type mismatch\n");
        }

        switch (arg_type){
            case INT:
                nargs = check_args(ast->type_expression_node.arguments_opt);
                if (nargs == 1)
                    return INT;
                else
                    return -1;
            case FLOAT:
                nargs = check_args(ast->type_expression_node.arguments_opt);
                if (nargs == 1)
                    return FLOAT;
                else
                    return -1;
            case BOOL:
                nargs = check_args(ast->type_expression_node.arguments_opt);
                if (nargs == 1)
                    return BOOL;
                else
                    return -1;
            case BVEC2:
            case BVEC3:
            case BVEC4:
                nargs = check_args(ast->type_expression_node.arguments_opt);
                if (arg_type == BVEC2 && nargs == 2)
                    return BVEC2;
                if (arg_type == BVEC3 && nargs == 3)
                    return BVEC3;
                if (arg_type == BVEC4 && nargs == 4)
                    return BVEC4;
                return -1;

            case IVEC2:
            case IVEC3:
            case IVEC4:
                nargs = check_args(ast->type_expression_node.arguments_opt);
                if (arg_type == IVEC2 && nargs == 2)
                    return IVEC2;
                if (arg_type == IVEC3 && nargs == 3)
                    return IVEC3;
                if (arg_type == IVEC4 && nargs == 4)
                    return IVEC4;
                return -1;

            case VEC2:
            case VEC3:
            case VEC4:
                nargs = check_args(ast->type_expression_node.arguments_opt);
                if (arg_type == VEC2 && nargs == 2)
                    return VEC2;
                if (arg_type == VEC3 && nargs == 3)
                    return VEC3;
                if (arg_type == VEC4 && nargs == 4)
                    return VEC4;
                return -1;
            default: return -1;
        }
        return -1;

    case BINARY_EXPRESSION_NODE:
        ltype = semantic_check(ast->binary_expression_node.l_val );
        rtype = semantic_check(ast->binary_expression_node.r_val );
        op_type = ast->binary_expression_node.op;
        switch (op_type){
            case PLUS_OP:
            case MINUS_OP:
                // arithmetic +/-
                if (ltype == INT && rtype == INT)
                    return INT;
                if (ltype == FLOAT && rtype == FLOAT)
                    return FLOAT;
                if (ltype == BVEC2 && rtype == BVEC2)
                    return BVEC2;
                if (ltype == BVEC3 && rtype == BVEC3)
                    return BVEC3;
                if (ltype == BVEC4 && rtype == BVEC4)
                    return BVEC4;
                if (ltype == IVEC2 && rtype == IVEC2)
                    return IVEC2;
                if (ltype == IVEC3 && rtype == IVEC3)
                    return IVEC3;
                if (ltype == IVEC4 && rtype == IVEC4)
                    return IVEC4;
                if (ltype == VEC2 && rtype == VEC2)
                    return VEC2;
                if (ltype == VEC3 && rtype == VEC3)
                    return VEC3;
                if (ltype == VEC4 && rtype == VEC4)
                    return VEC4;
                printf("ERROR: Arithmetic +/-: type mismatch\n");
                return -1;
            case MULT_OP:
                if (ltype == INT && (rtype == INT || rtype == IVEC2 || rtype == IVEC3 || rtype == IVEC4))
                    return rtype;
                if (ltype == FLOAT && (rtype == FLOAT || rtype == VEC2 || rtype == VEC3 || rtype == VEC4))
                    return rtype;
                if (ltype == BOOL && (rtype == BOOL || rtype == BVEC2 || rtype == BVEC3 || rtype == BVEC4))
                    return rtype;
                if (ltype == rtype)
                    return rtype;
                printf("ERROR: Arithmetic multiply: type mismatch\n");
                return -1;
            case AND_OP:
            case OR_OP:
                if (ltype == BOOL && rtype == BOOL)
                    return ltype;
                if (ltype == BVEC2 && rtype == BVEC2)
                    return ltype;
                if (ltype == BVEC3 && rtype == BVEC3)
                    return ltype;
                if (ltype == BVEC4 && rtype == BVEC4)
                    return ltype;
                printf("ERROR: Logical AND/OR mismatch\n");
                return -1;
            case GEQ_OP:
            case GT_OP:
            case LT_OP:
            case LEQ_OP:
                if (ltype == INT && rtype == INT)
                    return ltype;
                if (ltype == FLOAT && rtype == FLOAT)
                    return ltype;
                printf("ERROR: Comparison mismatch\n");
                return -1;
            case EQ_OP:
            case NEQ_OP:
                if (ltype == INT && rtype == INT)
                    return ltype;
                if (ltype == FLOAT && rtype == FLOAT)
                    return ltype;
                if (ltype == IVEC2 && rtype == IVEC2)
                    return ltype;
                if (ltype == IVEC3 && rtype == IVEC3)
                    return ltype;
                if (ltype == IVEC4 && rtype == IVEC4)
                    return ltype;
                if (ltype == VEC2 && rtype == VEC2)
                    return ltype;
                if (ltype == VEC3 && rtype == VEC3)
                    return ltype;
                if (ltype == VEC4 && rtype == VEC4)
                    return ltype;
                printf("ERROR: EQ/NEQ type mismatch\n");
                return -1;
        }
        printf("ERROR: UNKNOWN ERROR\n");
        return -1;

    case UNARY_EXPRESSION_NODE:
        op_type = ast->unary_expression_node.op;
        rtype = semantic_check(ast->unary_expression_node.expression );
        switch(op_type){
            case UMINUS_OP:
                if (rtype == FLOAT || rtype == INT)
                    return rtype;
                if (rtype == IVEC2 || rtype == IVEC3 || rtype == IVEC4)
                    return rtype;
                if (rtype == VEC2 || rtype == VEC3 || rtype == VEC4)
                    return rtype;
                printf("ERROR: Unary minus type mismatch\n");
                return -1;
            case UNEGATION_OP:
                if (rtype == BOOL)
                    return rtype;
                if (rtype == BVEC2 || rtype == BVEC3 || rtype == BVEC4)
                    return rtype;
                printf("ERROR: Unary negation type mismatch\n");
                return -1;
        }
        printf("ERROR: UNKNOWN ERROR\n");
        return -1;

    case PAREN_EXPRESSION_NODE:
        return semantic_check(ast->paren_expression_node.expression);

    case VARIABLE_EXPRESSION_NODE:
        // get the type of variable
        return semantic_check(ast->variable_expression_node.variable );

    case LITERAL_EXPRESSION_NODE:
        switch (ast->literal_expression_node.literal_type){
            case INT_LITERAL:
                return INT;

            case FLOAT_LITERAL:
                return FLOAT;

            case BOOL_LITERAL:
                return BOOL;

            default:
                printf("ERROR: Unrecognized literal type\n");
                return -1;
        }
        printf("ERROR: UNKNOWN ERROR\n");
        return -1;
  
    // variable
    case SINGULAR_VARIABLE: 
        // TODO: complete function get_type_by_id
        return get_type_by_id(ast->singular_variable.id, scope_level);

    case ARRAY_VARIABLE:
        var_type = get_type_by_id(ast->array_variable.id, scope_level);
        multiplicity = ast->array_variable.multiplicity;
        switch (var_type){
            case BOOL:
            case INT:
            case FLOAT:
                printf("ERROR: Cannot access index of non-array type\n");
                return -1;

            // FLOAT type
            case VEC2:
                if (multiplicity < 2){
                    return FLOAT;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;
            case VEC3:
                if (multiplicity < 3){
                    return FLOAT;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;
            case VEC4:
                if (multiplicity < 4){
                    return FLOAT;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;

            // INT type     
            case IVEC2:
                if (multiplicity < 2){
                    return INT;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;
            case IVEC3:
                if (multiplicity < 3){
                    return INT;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;
            case IVEC4:
                if (multiplicity < 4){
                    return INT;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;

            // BOOL type     
            case BVEC2:
                if (multiplicity < 2){
                    return BOOL;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;
            case BVEC3:
                if (multiplicity < 3){
                    return BOOL;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;
            case BVEC4:
                if (multiplicity < 4){
                    return BOOL;    
                }
                printf("ERROR: Array access out-of-bound\n");
                return -1;
        }
        printf("ERROR: UNKNOWN ERROR\n");
        return -1;
  
    // semantic_check(arguments
    case BINARY_ARGUMENT:
        // Cannot extract type info, therefore return 0
        left_exp = semantic_check(ast->binary_arguments.arguments );
        right_exp = semantic_check(ast->binary_arguments.expression );
        if (left_exp == -1 || right_exp == -1)
            return -1;
        return left_exp;

    case UNARY_ARGUMENT:
        // Cannot extract type info, therefore return 0
        left_exp = semantic_check(ast->unary_argument.expression );
        if (left_exp == -1)
            return -1;
        return left_exp;
  
    // argument_opt
    case ARGUMENT_OPT:
        // Cannot extract type info, therefore return 0
        left_exp = semantic_check(ast->arguments_opt.arguments );
        if (left_exp == -1)
            return -1;
        return left_exp;
  
    // type
    case TYPE:
        type = ast->type.var_type;
        multiplicity = ast->type.multiplicity;
        switch (type){
            case INT_TYPE:
                if (multiplicity == 0)
                    return INT;
                if (multiplicity == 1)
                    return IVEC2;
                if (multiplicity == 2)
                    return IVEC3;
                if (multiplicity == 3)
                    return IVEC4;
                printf("ERROR: Unrecognized INT type\n");
                return -1;

            case FLOAT_TYPE:
                if (multiplicity == 0)
                    return FLOAT;
                if (multiplicity == 1)
                    return VEC2;
                if (multiplicity == 2)
                    return VEC3;
                if (multiplicity == 3)
                    return VEC4;
                printf("ERROR: Unrecognized FLOAT type\n");
                return -1;

            case BOOL_TYPE:
                if (multiplicity == 0)
                    return BOOL;
                if (multiplicity == 1)
                    return BVEC2;
                if (multiplicity == 2)
                    return BVEC3;
                if (multiplicity == 3)
                    return BVEC4;
                printf("ERROR: Unrecognized BOOL type\n");
                return -1;
        }
        printf("ERROR: UNKNOWN ERROR\n");
        return -1;
  
        case UNKNOWN:
        default:
            printf("ERROR: UNKNOWN ERROR\n");
            return -1;
    }
    return 0; // failed checks
}
