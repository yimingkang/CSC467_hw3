#ifndef __REGISTER_H
#define __REGISTER_H
// REGISTERS
typedef struct reg{
    char *sb_id; //represents symbol id
    int reg_number; 
    struct reg* next;
}reg_t;

typedef struct registers{ 
    char *header;
    size_t size;
    struct reg* head;
}registers_t;

int next_alloc(registers_t *);
void free_registers(registers_t *);
registers_t *register_stack(char *);
void dealloc(registers_t *, int);
void _recursive_free(reg_t *);
int find(registers_t *, char *);
int allocate(registers_t *, char *);
reg_t* _allocate();
reg_t *find_node(registers_t *, char *);
char *get_name_by_reg_num(registers_t*, int);


extern registers_t *registers;
extern FILE *output_file; 
#endif
