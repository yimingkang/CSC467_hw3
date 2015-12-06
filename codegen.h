// include guard
#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "ast.h"

int genCode(node *ast);
void print(const char *, ...);
char *tempVar(int);

#endif
