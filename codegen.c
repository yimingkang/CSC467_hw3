#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "codegen.h"
#include "register.h"

// typechecking
#define typecheck(type,x) \
({      type __dummy; \
        typeof(x) __dummy2; \
        (void)(&__dummy == &__dummy2); \
        1; \
})

FILE *output_file = NULL; 
int scope_level;

/* Registers
 *  Create a register stack: registers_t *register_stack()
 *  Allocate a register:     int allocate(registers_t *, char *id)
 *  Deallocate a register:   void dealloc(registers_t *, int)
 *  Free registers           void free_registers(registers_t *)
 */

registers_t *registers = NULL;



void MOV(int src, int dst){
    // given src, dest
    char *src_id = get_name_by_reg_num(registers, src);
    char *dst_id = get_name_by_reg_num(registers, dst);

    print("MOV %s, %s\n", src_id, dst_id);
}

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
    int next_reg, reg_number, this_reg, src_reg;

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
            this_reg = allocate(registers, ast->initialized_declaration.id);
            print("TEMP %s\n", ast->initialized_declaration.id);

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
            print("TEMP %s\n", ast->const_declaration.id);

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
        case
        case
        case
        case
        case
        case
        case
        case
        case
        case
        case
        case
        case
        case
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
