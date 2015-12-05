#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

#define DEBUG

FILE *output_file = NULL; 
int scope_level;

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

    int kind = ast->kind;
    int scope;
    int left, right;

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
            break;
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
