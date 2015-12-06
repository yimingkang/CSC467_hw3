#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "codegen.h"
#include "instruction.h"
#include "register.h"

// typechecking
#define typecheck(type,x) \
({      type __dummy; \
        typeof(x) __dummy2; \
        (void)(&__dummy == &__dummy2); \
        1; \
})

int scope_level;

// extern vars init
FILE *output_file = NULL;
registers_t *registers = NULL;

/* Registers
 *  Create a register stack: registers_t *register_stack()
 *  Allocate a register:     int allocate(registers_t *, char *id)
 *  Deallocate a register:   void dealloc(registers_t *, int)
 *  Free registers           void free_registers(registers_t *)
 */

typedef struct gen_params{
    struct ifelse{
        int active;
        reg_t* reg;
    };

    struct target_reg{
        int active;
        reg_t* reg;
        int access_index;
    };
    
    struct const_declaration{
        int active;
        char* sb_id;
    };

    struct get_list_gen_return{
        int active;
    };

    struct argument_list{
       int active;
       int index;
    }; 


}gen_params;

typedef struct output_reg_param{
    reg_t* output_reg;
    int output_reg_index;
    reg_t* output_reg_actual;
    int output_reg_actual_index;
    int output_redirect;
    char* output_s;
    char* output_actual_s;
} output_reg_param;

output_reg_param init_output_procedure(gen_params* param){
    output_reg_param output_param;
        if (param->target_reg.active ==0){
            output_param.output_reg_actual = allocate(registers, get_temp_name(registers), scope_level);
            output_param.output_reg_actual_index = -1;
        }else{
            output_param.output_reg_actual = param->target_reg.reg;
            output_param.output_reg_actual_index = param->target_reg.index;
            param->target_reg.active = 0;
        }
        //may have a if else condition
        if (param->ifelse.active ==1 && parent_scope(output_reg_actual, scope_level) == 1){
           output_param.output_redirect = 1;
           output_param.output_reg = allocate(registers, get_temp_name(registers), scope_level);
           output_param.output_reg_index = -1;
        }else{
           output_param.output_redirect = 0;
            output_param.output_reg = output_reg_actual;
            output_param.output_reg_index = output_reg_actual_index;
        }
        output_param.output_s = get_reg_id(output_param.output_reg, output_param.output_reg_index);
        output_param.output_actual_s = get_reg_id(output_param.output_reg_actual, output_param.output_reg_actual_index);
    return output_param;

}

void finish_output_procedure (output_reg_param* out_p, gen_params* param){
            if (out_p.output_redirect ==1){
                print("CMP %s, %s, %s, %s", get_reg_id(param.ifelse.reg, -1), get_reg_id(out_p.output_reg_actual, -1), get_reg_id(out_p.output_reg, -1), get_reg_id(out_p.output_reg_actual, out_p.output_reg_actual_index));
            }
}


typedef struct gen_return {
    reg_t* reg;
    int access_index; //-1 represent operation on all components
    gen_return* next;
}gen_return;

void init_gen_return(gen_return* ret){
    ret->reg = NULL;
    ret->access_index = -1;
    ret->gen_return = NULL; 
}

gen_returns genCode(node *ast, gen_params param ){
    assert(ast != NULL);

    if (registers == NULL){
        registers = register_stack("test");
    }


    #ifndef DEBUG
    if (!output_file){
        // do all the init work here
        output_file = fopen("file.txt", "w");
        scope_level = 0;
        assert(output_file != NULL);
        // keep track of id -> register mapping
    }
    #endif 

    int kind = ast->kind;
    int scope;
    int left, right;
    int next_reg, reg_number, this_reg, src_reg, func_name;
    char *this_reg_name;

    //test_print();
    //return 0;

    gen_return ret;
    init_gen_return(&ret);
    switch (kind){
        case PROGRAM:
            genCode(ast->program.scope, NULL);
            break;
        case SCOPE:
            scope_level += 1;
            genCode(ast->scope.declarations, param);
            genCode(ast->scope.statements, param);
            scope_level -= 1;
            return ret;
        case DECLARATIONS:
            genCode(ast->declarations.declarations, param);
            genCode(ast->declarations.declaration, param);
            return ret;
        case STATEMENTS:
            genCode(ast->statements.statements, param);
            genCode(ast->statements.statement, param);
            return ret;
        case DECLARATION:
            // allocate register
            reg = allocate(registers, ast->declaration.id, scope_level);
            print("TEMP %s\n", get_reg_id(reg));
            return ret;
        case INITIALIZED_DECLARATION:
            // allocate register
            this_reg = allocate(registers, ast->initialized_declaration.id, scope_level);
            print("TEMP %s\n", get_reg_id(this_reg));

            param.target_reg.active = 1;
            param.target_reg.reg = this_reg;
            param.target_reg.access_index = -1;
            genCode(ast->initialized_declaration.expression, param);

            return ret;
        case CONST_DECLARATION:
            param.const_declaration.active = 1;
            param.const_declaration.sb_id = ast->const_declaration.id;

            src_reg = genCode(ast->const_declaration.expression, param);

            return ret;
        case ASSIGNMENT_STATEMENT:
            
            var_reg = genCode(ast->assignment_statement.variable); 

            // to assgin, reg must exists
            assert(var_reg.reg != NULL);

            param.target_reg.active = 1;
            param.target_reg.reg = var_reg.reg;
            param.target_reg.access_index = = var_get.index;
            src_reg = genCode(ast->assignment_statement.expression, param);
            return ret;
        case IF_ELSE_STATEMENT:
            reg_t* exp_result_reg = genCode(ast->if_else_statement.if_condition, param);
            param.ifelse.active = 1;
            param.ifelse.reg = exp_result_reg;
            genCode(ast->if_else_statement.statement, param);
            // invert the if condition boolean
            print ("MUL MINUS_1, %s, %s\n", get_reg_id(exp_result_reg));
            genCode(ast->if_else_statement.else_statement, param);

            return ret;
        case IF_STATEMENT:
            reg_t* exp_result_reg = genCode(ast->if_else_statement.if_condition, param);
            param.ifelse.active = 1;
            param.ifelse.reg = exp_result_reg;
            genCode(ast->if_else_statement.statement, param);

            return ret;
        case SCOPE_STATEMENT:
            // first note the next scope
            next_reg = next_alloc(registers);
            genCode(ast->scope_statement.scope, param);

            // deallocate all registers in the scope
            dealloc(registers, next_reg);
            return ret;
        case SEMICOLEN_STATEMENT:
            // nothing to do here
            return ret;
        case TYPE_EXPRESSION_NODE:

            //should expect a target reg
            if (param.target_reg.active ==0){
                reg_t* temp = allocate(registers, get_temp_name(registers), scope_level);
                param.target_reg.active = 1;
                param.target_reg.reg = temp;
                param.target_reg.index = -1;
            }           
            param.argument_list.active =1;
            param.argument_list.index = 0;
            //continue search the sub nodes and write to target reg
            genCode(ast->type_expression_node.arguments_opt);
            return ret;
        case FUNC_EXPRESSION_NODE:
            func_name = ast->func_expression_node.func_name;
            output_reg_param out_p = init_output_procedure(&param); 
            //should expect a output reg here. reset the target reg
            if (func_name == DP3 ){//dp3
                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression.arguments_opt, param);
                reg_t* arg1 = ret.next->reg;
                reg_t* arg2 = ret.next->next->reg; 
                print("DP3 %s, %s, %s\n", get_reg_id(arg1, -1), get_reg_id(arg2, -1), get_reg_id(out_p.output_reg, out_p.output_reg_index));

            } else if (func_name == DP4){
                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression.arguments_opt, param);
                reg_t* arg1 = ret.next->reg;
                reg_t* arg2 = ret.next->next->reg; 
                print("DP4 %s, %s, %s\n", get_reg_id(arg1, -1 ), get_reg_id(arg2, -1), get_reg_id(out_p.output_reg, out_p.output_reg_index));
            }else if (func_name == RSQ ){
                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression.arguments_opt, param);
                reg_t* arg1 = ret.next->reg; 
                print("RSQ %s.x, %s.x\n", get_reg_id(arg1, -1), get_reg_id(out_p.output_reg, out._p.output_reg_index));
            }else if (func_name == LIT){

                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression.arguments_opt, param);
                reg_t* arg1 = ret.next->reg; 
                print("LIT %s, %s\n", get_reg_id(arg1, -1), get_reg_id(out_p.output_reg, out_p.output_reg_index));

            }else{
                assert(0);
            }
            finish_output_procedure(&out_p, &param);
            ret.reg =out_p.output_reg_actual; 
            ret.index = out_p.output_reg_actual_index;
            return ret;
        case ARGUMENTS_OPT:
             return genCode(ast->arguments, param);    
        case BINARY_ARGUMENTS:
             //expect get_list_gen_return is true
             assert(param.get_list_gen_return.active ==1);
             gen_return rets = genCode(ast->binary_arguments.arguments, param);
             param.get_list_gen_return.active = 0;
             gen_return exp_ret = genCode(ast->binary_arguments.expression, param);
             exp_ret.next = rets.next;
             gen_return* ret_node = (gen_return*) malloc(sizeof(gen_return));
             *ret_node = exp_ret;
             ret.next = ret_node;
             return ret;
        case UNARY_ARGUMENT:
             //expect get_list_gen_return is true
             assert(param.get_list_gen_return.active ==1);
             param.get_list_gen_return.active = 0;
             gen_return exp_ret = genCode(ast->binary_arguments.expression, param);

             gen_return* ret_node = (gen_return*) malloc(sizeof(gen_return));
             *ret_node = exp_ret;
             ret.next = ret_node;
             return ret;     
        case BINARY_EXPRESSION_NODE:    
             //expect a output here, may have a target register
             output_reg_param out_p = init_output_procedure(&param); 
             gen_return left = genCode(ast->binary_expression_node.l_val, param);
             gen_return right = genCode(ast->binary_expression_node.r_val, param);
             char* left_s = get_reg_id(left.reg, left.index);
             char* right_s = get_reg_id(right.reg, right.index);
             switch(ast->binary_expression_node.op){
                  case PLUS_OP:
                      print("ADD %s, %s, %s,\n", left_s, right_s, out_p.output_s);
                      break;
                  case MINUS_OP:
                      print("SUB %s, %s, %s,\n", left_s, right_s, out_p.output_s);
                      break;
                  case AND_OP:
                      print("MIN %s, %s, %s,\n", left_s, right_s, out_p.output_s);
                      break;
                  case OR_OP:
                      print("MAX %s, %s, %s,\n", left_s, right_s, out_p.output_s);
                      break;
                  case EQ_OP:
                      char* temp1 = get_reg_id(allocate(registers, get_temp_name(registers), scope_level), left.index);
                      char* temp2 = allocate(registers, get_temp_name(registers), scope_level), left_index);
                      char* temp3 = allocate(registers, get_temp_name(registers), scope_level);
                      print("MUL MINUS_1, %s, %s\n", left_s, temp1);
                      print("MUL MINUS_1, %s, %s\n", right_s, temp2);
                      print("MIN  %s, %s, %s\n", temp1, temp2, temp2);
                  case NEQ_OP:
                  case DIV_OP:
                  case POW_OP:
                  case LT_OP:
                  case LEQ_OP:
                  case GT_OP:
                  case GEQ_OP:
                  case MULT_OP:

        case :    
        case :    
        case :    
        case :    
        case :    
        case :    
        case :    
        case :    
    }

    return kind;
}

char *tempVar(int idx){
    // number of bytes we need is:
    // 7 for tempVar
    // idx/10 + 1 for integer
    // extra 1 for \0
    char *result = (char *) malloc( 7 + idx/10 + 1 + 1);
    sprintf(result, "tempVar%d", idx);
}
