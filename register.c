#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <string.h>
#include "register.h"

void free_registers(registers_t *register_stack){
    assert(register_stack != NULL);
    _recursive_free(register_stack->head);
    free(register_stack);
}

registers_t *register_stack(char *header){
    registers_t *new_reg = (registers_t *) malloc(sizeof(registers_t));
    assert(new_reg != NULL);

    new_reg->head = NULL;
    new_reg->size = 0;
    new_reg->header = header;
    return new_reg;
}

int next_alloc(registers_t *reg_stack){
    return reg_stack->size;
}

void dealloc(registers_t *register_stack, int reg_number){
    // deallocates registers up to register number reg_number (inclusive)
    assert(register_stack != NULL);
    reg_t *node = register_stack->head;
    while (node && node->reg_number != reg_number){
        node = node->next;
    }

    assert(node != NULL);

    reg_t *old_head = register_stack->head;
    register_stack->head = node->next;
    register_stack->size = reg_number;

    node->next = NULL;
    _recursive_free(old_head);
}

void _recursive_free(reg_t *node){
    if (node == NULL){
        // do nothing if node is NULL
        return;
    }

    _recursive_free(node->next);
    free(node);
}

reg_t *find_node(registers_t *register_stack, char *id){
    reg_t *head = register_stack->head;
    while (head && strcmp(id, head->id) != 0){
        head = head->next;
    }
    return head;
}

int find(registers_t *register_stack, char *id){
    reg_t *head = find_node(register_stack, id);
    if (head){
        return head->reg_number;
    }
    return -1;
}

int allocate(registers_t *register_stack, char *id){
    reg_t *new_reg = _allocate();
    new_reg->id = id;
    new_reg->reg_number = register_stack->size;
    register_stack->size++;
    reg_t* next_node = register_stack->head;
    register_stack->head = new_reg;
    new_reg->next = next_node;
    return new_reg->reg_number;
}


reg_t* _allocate(){
    reg_t* new_reg = (reg_t *) malloc(sizeof(reg_t));
    assert(new_reg != NULL);
    
    new_reg->id = NULL; 
    new_reg->reg_number = -1;
    return new_reg;
}

void print_registers(registers_t* reg_stack){
    reg_t *node = reg_stack->head;
    while (node){
        printf("%s|", node->id);
        node = node->next;
    }
    puts("");
}

int reg_test(){
    registers_t *new_regs = register_stack("mystack");
    int i, reg_number;
    reg_number = allocate(new_regs, "R100");
    reg_number = allocate(new_regs, "R99");
    reg_number = allocate(new_regs, "R98");
    for (i = 0; i < 20; i++){
        reg_number = allocate(new_regs, "R1");
        printf("Register number is %d\n", reg_number);
        reg_number = allocate(new_regs, "R2");
        printf("Register number is %d\n", reg_number);
    }
    print_registers(new_regs);
    reg_number = find(new_regs, "R98");
    printf("Register number found for %s is %d\n", "R98", reg_number);
    reg_number = find(new_regs, "R99");
    printf("Register number found for %s is %d\n", "R99", reg_number);
    reg_number = find(new_regs, "R100");
    printf("Register number found for %s is %d\n", "R100", reg_number);
    for (i = 10; i >= 0; i--){
        dealloc(new_regs, i);
        int size = new_regs->size;
        printf("Register dealloced%d, size remaining is %d\n", i, size);
        print_registers(new_regs);
    }
    free_registers(new_regs);
}

char *get_name_by_reg_num(registers_t *reg_stack, int id){
    reg_t *node = reg_stack->head;
    while (node && node->reg_number != id){
        node = node->next;
    }

    // Not found ---> this is bad
    assert(node != NULL);
    return node->id;
}

