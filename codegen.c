#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "ast.h"

#define DEBUG

FILE *output_file = NULL; 

void gen_code(node *ast){
    #ifndef DEBUG
    if (!output_file){
        // do all the init work here
        output_file = fopen("file.txt", "w");
        if (!output_file){
            exit(-1);
        }
        // keep track of id -> register mapping
    }
    #endif 
    int kind = ast->kind;


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


int main(){
    print("This is a test %d\n", 0);
}
