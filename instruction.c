#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "instruction.h"
#include "register.h"

// INSTRUCTION

/*
 Instruction Inputs Output Description
 ----------- ------ ------ --------------------------------
 ABS v v absolute value
 ADD v,v v add
 CMP v,v,v v compare
 DP3 v,v ssss 3-component dot product
 LIT v v compute light coefficients
 MAD v,v,v v multiply and add
 MOV v v move
 MUL v,v v multiply
 POW s,s ssss exponentiate
 RSQ s ssss reciprocal square root
 SGE v,v v set on greater than or equal
 SLT v,v v set on less than
 SUB v,v v subtract
*/

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

void CMP(int src1, int src2, int src3, int dst){

    char *s1 = get_name_by_reg_num(registers, src1);
    char *s2 = get_name_by_reg_num(registers, src2);
    char *s3 = get_name_by_reg_num(registers, src3);
    char *dest = get_name_by_reg_num(registers, dst);

    print("CMP %s, %s, %s, %s;\n", s1, s2, s3, dest);
}

void MOV(int src, int dst){
    // given src, dest
    char *src_id = get_name_by_reg_num(registers, src);
    char *dst_id = get_name_by_reg_num(registers, dst);

    print("MOV %s, %s;\n", src_id, dst_id);
}

void ADD(int src, int dst){
    // given src, dest
    char *src_id = get_name_by_reg_num(registers, src);
    char *dst_id = get_name_by_reg_num(registers, dst);

    print("ADD %s, %s;\n", src_id, dst_id);
}

void test_print(){
    print("****THIS IS A TEST****\n");
    print("FIlE is allocated at address -> 0x%p\n", output_file);
}
