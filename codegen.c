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


int genCode(node *ast){
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

    switch (kind){
        case PROGRAM:
            genCode(ast->program.scope);
            break;
        case SCOPE:
            scope_level += 1;
            left = genCode(ast->scope.declarations);
            right = genCode(ast->scope.statements);
            if (left != -1 && right != -1){
                return 0;
            }
            return -1;
        case DECLARATIONS:
            left = genCode(ast->declarations.declarations);
            right = genCode(ast->declarations.declaration);
            if (left != -1 && right != -1){
                return 0;
            }
            return -1;
        case STATEMENTS:
            left = genCode(ast->statements.statements);
            right = genCode(ast->statements.statement);
            if (left != -1 && right != -1){
                 return 0;
            }
            return -1;
        case DECLARATION:
            // allocate register
            allocate(registers, ast->declaration.id);
            print("PARAM %s\n", ast->declaration.id);
            break;
        case INITIALIZED_DECLARATION:
            // allocate register
            this_reg = allocate(registers, ast->initialized_declaration.id);
            print("PARAM %s\n", ast->initialized_declaration.id);

            next_reg = next_alloc(registers);
            // get the tmp results first
            src_reg = genCode(ast->initialized_declaration.expression);

            // mov
            MOV(src_reg, this_reg);
            
            // deallocate the tmps
            dealloc(registers, next_reg);
            break;
        case CONST_DECLARATION:
            // allocate register
            this_reg = allocate(registers, ast->const_declaration.id);
            print("PARAM %s\n", ast->const_declaration.id);

            next_reg = next_alloc(registers);
            // get the tmp results first
            src_reg = genCode(ast->const_declaration.expression);

            // mov
            MOV(src_reg, this_reg);
            
            // deallocate the tmps
            dealloc(registers, next_reg);
            break;
        case ASSIGNMENT_STATEMENT:
            
            this_reg = genCode(ast->assignment_statement.variable); 

            // to assgin, reg must exists
            assert(this_reg != -1);
            next_reg = next_alloc(registers);

            // get the tmp results first
            src_reg = genCode(ast->assignment_statement.expression);
            // move result into this_reg
            MOV(src_reg, this_reg);

            // deallocate the tmps
            dealloc(registers, next_reg);
            break;
        case IF_ELSE_STATEMENT:
            // TODO: this is tricky, do later
            break;
        case IF_STATEMENT:
            // TODO: this is tricky, do later
            break;
        case SCOPE_STATEMENT:
            // first note the next scope
            next_reg = next_alloc(registers);
            genCode(ast->scope_statement.scope);

            // deallocate all registers in the scope
            dealloc(registers, next_reg);
            break;
        case SEMICOLEN_STATEMENT:
            // nothing to do here
            return -1;
        case TYPE_EXPRESSION_NODE:
            // type(arguments_opt)
            next_reg = next_alloc(registers);

            // allocate a register 
            this_reg_name = tempVar(next_reg);
            this_reg = allocate(registers, this_reg_name);
            print("TEMP %s = {", this_reg_name);
            
            // recursively print all the arguments
            genCode(ast->type_expression_node.arguments_opt);
            print("};\n");
            break;
        case FUNC_EXPRESSION_NODE:
            func_name = ast->func_expression_node.func_name;
            if (func_name == 0){

            } else if (func_name == 1){

            }else if (func_name == 2){

            }else{
                // bad
                assert(0);
            }

            // no return value
            return -1;
        case BINARY_EXPRESSION_NODE:

        case :    
        case :    
        case :    
        case :    
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
