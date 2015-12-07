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


typedef struct output_reg_param{
    reg_t* output_reg;
    int output_reg_index;
    reg_t* output_reg_actual;
    int output_reg_actual_index;
    int output_redirect;
    char* output_s;
    char* output_actual_s;
} output_reg_param;

char* get_temp_name(registers_t* registers){
     // number of bytes we need is:
    // 7 for tempVar
    // idx/10 + 1 for integer
    // extra 1 for \0
    int idx = next_alloc(registers);
    char *result = (char *) malloc( 7 + idx/10 + 1 + 1);
    sprintf(result, "tempVar%d", idx);
    return result;
}
int parent_scope(reg_t* reg, int c_scope){
    if (reg->scope <c_scope){
        return 1;        
    }else{
        return 0;
    }
}
char *get_reg_id(reg_t* reg, int index){
	if (strcmp(reg->sb_id, "gl_FragColor") == 0) {
		return "result.color";

	} else if (strcmp(reg->sb_id, "gl_FragDepth") == 0) {
		return "result.depth";

	} else if (strcmp(reg->sb_id, "gl_FragCoord") == 0) {
		return "fragment.position";

	} else if (strcmp(reg->sb_id, "gl_TexCoord") == 0) {
		return "fragment.texcoord";

	} else if (strcmp(reg->sb_id, "gl_Color") == 0) {
		return "fragment.color";

	} else if (strcmp(reg->sb_id, "gl_Secondary") == 0) {
		return "fragment.color.secondary";

	} else if (strcmp(reg->sb_id, "gl_FogFragCoord") == 0) {
		return "fragment.fogcoord";

	} else if (strcmp(reg->sb_id, "gl_Light_Half") == 0) {
		return "state.light[0].half";

	} else if (strcmp(reg->sb_id, "gl_Light_Ambient") == 0) {
		return "state.lightmodel.ambient";

	} else if (strcmp(reg->sb_id, "gl_Material_Shininess") == 0) {
		return "state.material.shininess";

	} else if (strcmp(reg->sb_id, "env1") == 0) {
		return "program.env[1]";

	} else if (strcmp(reg->sb_id, "env2") == 0) {
		return "program.env[2]";

	} else if (strcmp(reg->sb_id, "env3") == 0) {
		return "program.env[3]";

	} else {
            int sizen = strlen(reg->sb_id)+2;
            char* str = (char*) malloc(sizeof(char)*(sizen));
            switch (index){
                case -1:
                     snprintf(str, sizen, "%s", reg->sb_id);
                     break;
                case 0:
                     snprintf(str, sizen, "%s.x", reg->sb_id);
                     break;
                case 1:
                    snprintf(str, sizen, "%s.y", reg->sb_id);
                    break;
                case 2:
                    snprintf(str, sizen, "%s.z", reg->sb_id);
                    break;
                case 3:
                    snprintf(str, sizen, "%s.w", reg->sb_id);
                    break;
                default:
                    assert(0);
                    }
            return str;
	}
	return "";
}
output_reg_param init_output_procedure(gen_param* param){
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
        if (param->ifelse.active ==1 && parent_scope(output_param.output_reg_actual, scope_level) == 1){
           output_param.output_redirect = 1;
           output_param.output_reg = allocate(registers, get_temp_name(registers), scope_level);
           output_param.output_reg_index = -1;
        }else{
           output_param.output_redirect = 0;
            output_param.output_reg = output_param.output_reg_actual;
            output_param.output_reg_index = output_param.output_reg_actual_index;
        }
        output_param.output_s = get_reg_id(output_param.output_reg, output_param.output_reg_index);
        output_param.output_actual_s = get_reg_id(output_param.output_reg_actual, output_param.output_reg_actual_index);
    return output_param;

}

void finish_output_procedure (output_reg_param* out_p, gen_param* param){
            if (out_p->output_redirect ==1){
                print("CMP %s, %s, %s, %s", get_reg_id(param->ifelse.reg, -1), get_reg_id(out_p->output_reg_actual, -1), get_reg_id(out_p->output_reg, -1), get_reg_id(out_p->output_reg_actual, out_p->output_reg_actual_index));
            }
}



typedef struct type{
    int type;
    int mul;
}type;

type get_type(node *ast){
    type ret;
    ret.type = 0;
    ret.mul = 0;
    if (ast == NULL)
        return ret;

    int kind = ast->kind;
   switch(kind){
       case TYPE:
          ret.type = ast->type.var_type;
          ret.mul = ast->type.multiplicity;
          return ret;
        default:
         return ret;
    } 
}



void init_gen_return(gen_return* ret){
    ret->reg = NULL;
    ret->index = -1;
    ret->next = NULL; 
}


gen_return genCode(node *ast, gen_param param ){
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

    //test_print();
    //return 0;

    gen_return ret;
    init_gen_return(&ret);
    switch (kind){
        case PROGRAM:
            {
            genCode(ast->program.scope, param);
            break;
            }
        case SCOPE:
            {
            scope_level += 1;
            genCode(ast->scope.declarations, param);
            genCode(ast->scope.statements, param);
            scope_level -= 1;
            return ret;
            }
        case DECLARATIONS:
            {
            genCode(ast->declarations.declarations, param);
            genCode(ast->declarations.declaration, param);
            return ret;
            }
        case STATEMENTS:
            {
            genCode(ast->statements.statements, param);
            genCode(ast->statements.statement, param);
            return ret;
            }
        case DECLARATION:
            {
            // allocate register
            reg_t* reg = allocate(registers, ast->declaration.id, scope_level);
            print("TEMP %s\n", get_reg_id(reg, -1));
            return ret;
            }
        case INITIALIZED_DECLARATION:
            {
            // allocate register
            reg_t* this_reg = allocate(registers, ast->initialized_declaration.id, scope_level);
            print("TEMP %s\n", get_reg_id(this_reg, -1));

            param.target_reg.active = 1;
            param.target_reg.reg = this_reg;
            param.target_reg.index = -1;
            genCode(ast->initialized_declaration.expression, param);

            return ret;
            }
        case CONST_DECLARATION:
            {
            param.const_declaration.active = 1;
            param.const_declaration.sb_id = ast->const_declaration.id;

            genCode(ast->const_declaration.expression, param);

            return ret;
            }
        case ASSIGNMENT_STATEMENT:
            {
            gen_return var_reg = genCode(ast->assignment_statement.variable, param); 

            // to assgin, reg must exists
            assert(var_reg.reg != NULL);

            param.target_reg.active = 1;
            param.target_reg.reg = var_reg.reg;
            param.target_reg.index =  var_reg.index;
            genCode(ast->assignment_statement.expression, param);
            return ret;
            }
        case IF_ELSE_STATEMENT:
            {
            gen_return exp_result_reg = genCode(ast->if_else_statement.if_condition, param);
            param.ifelse.active = 1;
            param.ifelse.reg = exp_result_reg.reg;
            genCode(ast->if_else_statement.statement, param);
            // invert the if condition boolean
            print ("MUL %s, MINUS_1, %s\n", get_reg_id(exp_result_reg.reg, -1), get_reg_id(exp_result_reg.reg, -1));
            genCode(ast->if_else_statement.else_statement, param);

            return ret;
            }
        case IF_STATEMENT:
            {
            gen_return exp_result_reg = genCode(ast->if_else_statement.if_condition, param);
            param.ifelse.active = 1;
            param.ifelse.reg = exp_result_reg.reg;
            genCode(ast->if_else_statement.statement, param);

            return ret;
            }
        case SCOPE_STATEMENT:
            {
            ///irst note the next scope
            int next_reg = next_alloc(registers);
            genCode(ast->scope_statement.scope, param);

            // deallocate all registers in the scope
            //dealloc(registers, next_reg);
            return ret;
            }
        case SEMICOLEN_STATEMENT:
            // nothing to do here
            return ret;
        case TYPE_EXPRESSION_NODE:
            {
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
            genCode(ast->type_expression_node.arguments_opt, param);
            //get the data type, if multiplicity is less than 4, we need to mannully assign the uninitialized components with 0
            type t = get_type(ast->type_expression_node.type);
            int i;
            for (i = t.mul; i < 4; i++){
                param.target_reg.index = i;
                output_reg_param out_p = init_output_procedure(&param);
                print("MOV %s, %s\n", out_p.output_s, "ZERO");
                finish_output_procedure(&out_p, &param);
            }
            return ret;
            }
        case FUNC_EXPRESSION_NODE:
            {
            int func_name = ast->func_expression_node.func_name;
            output_reg_param out_p = init_output_procedure(&param); 
            //should expect a output reg here. reset the target reg
            if (func_name == DP3 ){//dp3
                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression_node.arguments_opt, param);
                reg_t* arg1 = ret.next->reg;
                reg_t* arg2 = ret.next->next->reg; 
                print("DP3 %s, %s.xyz, %s.xyz\n", get_reg_id(out_p.output_reg, out_p.output_reg_index), get_reg_id(arg1, -1), get_reg_id(arg2, -1));

            } else if (func_name == DP4){
                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression_node.arguments_opt, param);
                reg_t* arg1 = ret.next->reg;
                reg_t* arg2 = ret.next->next->reg; 
                print("DP4 %s, %s, %s\n", get_reg_id(out_p.output_reg, out_p.output_reg_index), get_reg_id(arg1, -1 ), get_reg_id(arg2, -1));
            }else if (func_name == RSQ ){
                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression_node.arguments_opt, param);
                reg_t* arg1 = ret.next->reg; 
                print("RSQ %s, %s\n",  get_reg_id(out_p.output_reg, out_p.output_reg_index), get_reg_id(arg1, ret.next->index ));
            }else if (func_name == LIT){

                param.get_list_gen_return.active = 1;
                gen_return ret = genCode(ast->func_expression_node.arguments_opt, param);
                reg_t* arg1 = ret.next->reg; 
                print("LIT %s, %s\n", get_reg_id(out_p.output_reg, out_p.output_reg_index), get_reg_id(arg1, ret.next->index));

            }else{
                assert(0);
            }
            finish_output_procedure(&out_p, &param);
            ret.reg =out_p.output_reg_actual; 
            ret.index = out_p.output_reg_actual_index;
            return ret;
            }
        case ARGUMENT_OPT:
             return genCode(ast->arguments_opt.arguments, param);    
        case BINARY_ARGUMENT:
             {
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
             }
        case UNARY_ARGUMENT:
             {
             //expect get_list_gen_return is true
             assert(param.get_list_gen_return.active ==1);
             param.get_list_gen_return.active = 0;
             gen_return exp_ret = genCode(ast->binary_arguments.expression, param);

             gen_return* ret_node = (gen_return*) malloc(sizeof(gen_return));
             *ret_node = exp_ret;
             ret.next = ret_node;
             return ret;     
             }
        case BINARY_EXPRESSION_NODE:    
             {
             //expect a output here, may have a target register
             output_reg_param out_p = init_output_procedure(&param); 
             gen_return left = genCode(ast->binary_expression_node.l_val, param);
             gen_return right = genCode(ast->binary_expression_node.r_val, param);
             char* left_s = get_reg_id(left.reg, left.index);
             char* right_s = get_reg_id(right.reg, right.index);
             switch(ast->binary_expression_node.op){
                  case PLUS_OP:
                      print("ADD %s, %s, %s,\n", out_p.output_s, left_s, right_s);
                      break;
                  case MINUS_OP:
                      print("SUB %s, %s, %s\n", out_p.output_s, left_s, right_s);
                      break;
                  case AND_OP:
                      print("MIN %s, %s, %s\n", out_p.output_s, left_s, right_s);
                      break;
                  case OR_OP:
                      print("MAX %s, %s, %s\n", out_p.output_s, left_s, right_s);
                      break;
                  case EQ_OP:
                      {
                      //if access index is not -1, then temp registers should have access index of 0
                      //eq is NXOR gate, or (A And B) or (~A or ~B)
                      int tempreg_index = (left.index==-1)? -1 : 0;
                      char* temp1 = get_reg_id(allocate(registers, get_temp_name(registers), scope_level), tempreg_index);
                      char* temp2 = get_reg_id(allocate(registers, get_temp_name(registers), scope_level), tempreg_index);
                      print("MUL %s, MINUS_1, %s\n", left_s, temp1);
                      print("MUL %s, MINUS_1, %s\n", right_s, temp2); //binary invert
                      print("MIN %s, %s, %s\n", temp2, temp1, temp2); //binary AND
                      print("MIN %s, %s, %s\n", temp1, left_s, right_s); //binary AND
                      print("MAX %s, %s, %s\n", out_p.output_s, temp1, temp2); //binary OR
                      break;
                      }
                  case NEQ_OP:
                      {
                      //if access index is not -1, then temp registers should have access index of 0
                      //neq is XOR gate, or (A OR B) ANd (~A OR ~B)
                      int tempreg_index = (left.index==-1)? -1 : 0;
                      char* temp1 = get_reg_id(allocate(registers, get_temp_name(registers), scope_level), tempreg_index);
                      char* temp2 = get_reg_id(allocate(registers, get_temp_name(registers), scope_level), tempreg_index);
                      print("MUL %s, MINUS_1, %s\n", left_s, temp1);
                      print("MUL %s, MINUS_1, %s\n", right_s, temp2); //binary invert
                      print("MAX %s, %s, %s\n", temp2, temp1, temp2); //binary OR
                      print("MAX %s, %s, %s\n", temp1, left_s, right_s); //binary OR
                      print("MIN %s, %s, %s\n", out_p.output_s, temp1, temp2); //binary AND
                      break;
                      }
                  case DIV_OP:
                      {
                      //only supports 
                      //left mul ( rcq right)
                      char* temp1 = get_reg_id(allocate(registers, get_temp_name(registers), scope_level), -1);
                      print("RCP %s, %s\n", temp1, get_reg_id(right.reg, (right.index == -1)?0:right.index));
                      print("MUL %s, %s, %s\n", out_p.output_s, temp1, left_s);
                      break;
                      }
                  case POW_OP:
                      print("POW %s, %s, %s\n", out_p.output_s, get_reg_id(left.reg, (left.index == -1)?0:left.index), get_reg_id(right.reg, (right.index == -1)?0:right.index));
                      break;
                  case LT_OP:
                      {
                        // a<b
                        // a<b? 0.5:-0.5
                        print("SLT %s, %s, %s\n", out_p.output_s, left_s, right_s);
                        print("SUB %s, %s, %s\n", out_p.output_s, out_p.output_s, "HALF");
                        break;
                      }
                  case LEQ_OP:
                      {
                      // b>=a ? 0.5: -0.5
                        print("SGE %s, %s, %s\n", out_p.output_s, left_s, right_s);
                        print("SUB %s, %s, %s\n", out_p.output_s, out_p.output_s, "HALF");
                        break;
                      }
                  case GT_OP:
                      {
                    // b<a ? 0.5:-0.5
                        print("SLT %s, %s, %s\n", out_p.output_s, right_s, left_s);
                        print("SUB %s, %s, %s\n", out_p.output_s, out_p.output_s, "HALF");
                        break;
                      }
                  case GEQ_OP:
                    // a>=b ? 0.5:-0.5
                        print("SGE %s, %s, %s\n", out_p.output_s, left_s, right_s);
                        print("SUB %s, %s, %s\n", out_p.output_s, out_p.output_s, "HALF");
                        break;
                  case MULT_OP:
                        print("MUL %s, %s, %s\n", out_p.output_s, left_s, right_s);
                        break;
                }
            finish_output_procedure(&out_p, &param);
            ret.reg = out_p.output_reg_actual;
            ret.index = out_p.output_reg_actual_index;
            return ret;
             }
        case UNARY_EXPRESSION_NODE:    
            {
                
             output_reg_param out_p = init_output_procedure(&param); 
             gen_return exp = genCode(ast->unary_expression_node.expression, param);
             char* exp_s = get_reg_id(exp.reg, exp.index);
             print("MUL %s, MINUS_1, %s\n", out_p.output_s, exp_s);
             finish_output_procedure(&out_p, &param);
             ret.reg = out_p.output_reg_actual;
             ret.index = out_p.output_reg_actual_index;
             return ret;
            }
        case PAREN_EXPRESSION_NODE:    
            return genCode(ast->paren_expression_node.expression, param);
        case VARIABLE_EXPRESSION_NODE:    
            {
                return genCode(ast->variable_expression_node.variable, param);
            }
        case SINGULAR_VARIABLE :    
            {
                reg_t* reg = find(registers, ast->singular_variable.id);
                ret.reg = reg;
                ret.index = -1;
                return ret;
            }
        case ARRAY_VARIABLE:    
            {
                reg_t* reg = find(registers, ast->singular_variable.id);
                ret.reg = reg;
                ret.index = ast->array_variable.multiplicity;
                return ret;
            }
        case LITERAL_EXPRESSION_NODE:
            {
                float value = 0;
                switch(ast->literal_expression_node.literal_type){
                    case INT_LITERAL:
                        value = ast->literal_expression_node.literal_value.int_val;
                        break;
                    case FLOAT_LITERAL:
                        value = ast->literal_expression_node.literal_value.float_val;
                        break;
                    case BOOL_LITERAL:
                        value = (ast->literal_expression_node.literal_value.bool_val == 1) ? 0.5 : -0.5;
                        break;
                    default:
                        assert(0);
                }
                char* const_name;

                if (param.const_declaration.active==1){
                    const_name = param.const_declaration.sb_id;
                }else{
                    const_name = get_temp_name(registers);
                }
                reg_t* reg = allocate(registers, const_name , scope_level);
                char* reg_s = get_reg_id(reg, -1);
                print("PARAM %s = %f\n", reg_s , value);
                if (param.target_reg.active ==0||param.const_declaration.active ==1){
                    ret.reg = reg;
                    ret.index = -1;
                    return ret;
                }
                output_reg_param out_p = init_output_procedure(&param); 
                print("MOV %s, %s\n", out_p.output_s,reg_s); 
                finish_output_procedure(&out_p, &param);
                ret.reg = out_p.output_reg_actual;
                ret.index = out_p.output_reg_actual_index;
                return ret;
            }
        default:
            //should not enter here
            assert(0);
    }

    return ret;
}


