// include guard
#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "ast.h"
#include "register.h"

typedef struct gen_param{
    struct ifelse_t{
        int active;
        reg_t* reg;
    }ifelse;

    struct target_reg_t{
        int active;
        reg_t* reg;
        int index;

    }target_reg;
    
    struct const_declaration_t{
        int active;
        char* sb_id;
    }const_declaration;

    struct get_list_gen_return_t{
        int active;
    }get_list_gen_return;

    struct argument_list_t{
       int active;
       int index;
    }argument_list; 


}gen_param;

typedef struct gen_return {
    reg_t* reg;
    int index; //-1 represent operation on all components
    gen_return* next;
}gen_return;

init_gen_param(gen_param* param){
    param->ifelse.active = 0;
    param->target_reg.active = 0;
    param->const_declaration.active = 0;
    param->get_list_gen_return.active = 0;
    param->argument_list.active = 0;
}

gen_return genCode(node *ast, gen_param param);
void print(const char *, ...);
char *tempVar(int);

#endif
