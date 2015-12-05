#ifndef __REGISTERS_H
#define __REGISTERS_H

typedef struct reg{
    char *id;
    int reg_number;
    struct reg* next;
}reg_t;

typedef struct registers{
    char *header;
    size_t size;
    struct reg* head;
}registers_t;

void free_registers(registers_t *);
registers_t *register_stack();
void dealloc(registers_t *, int);
void _recursive_free(reg_t *);
int find(registers_t *, char *);
int allocate(registers_t *, char *);
reg_t* _allocate();
reg_t *find_node(registers_t *, char *);

#endif
