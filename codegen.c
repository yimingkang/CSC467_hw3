#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "codegen.h"
#include "register.h"

FILE *output_file = NULL; 
int scope_level;

/* Registers
 *  Create a register stack: registers_t *register_stack()
 *  Allocate a register:     int allocate(registers_t *, char *id)
 *  Deallocate a register:   void dealloc(registers_t *, int)
 *  Free registers           void free_registers(registers_t *)
 */

registers_t *registers = NULL;

int genCode(node *ast){
    assert(ast != NULL);

    #ifndef DEBUG
    if (!output_file){
        // do all the init work here
        output_file = fopen("file.txt", "w");
        scope_level = 0;
        assert(output_file != NULL);
        // keep track of id -> register mapping
    }
    #endif 

    if (registers == NULL){
        // Get a new register
        registers = register_stack();
    }

    int kind = ast->kind;
    int scope;
    int left, right;
    int reg_number, next_reg;

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
            print("TEMP %s\n", ast->declaration.id);
            break;
        case INITIALIZED_DECLARATION:
            // allocate register
            allocate(registers, ast->initialized_declaration.id);
            print("TEMP %s\n", ast->initialized_declaration.id);

            next_reg = next_alloc(registers);
            // get the tmp results first
            reg_number = genCode(ast->initialized_declaration.expression);
            dealloc(registers, next_reg);
            print("MOV %s\n", ast->declaration.id);
            break;
        case
        case
        case
        case
        case
    }

    return kind;
}

void print(const char *fmt, ...){
    va_list arg;

    va_start (arg, fmt);
    #ifdef DEBUG
    vfprintf (stdout, fmt, arg);
    #else
    vfprintf (output_file, fmt, arg);
    #endif
    va_end (arg);
}



/*
int main(){
    print("This is a test %d\n", 0);
}
*/
