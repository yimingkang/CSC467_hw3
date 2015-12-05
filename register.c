/*
 * Register allocation, search and deallocation
 *
 */
#include <string.h>

typedef struct reg{
    char *id;
    int reg_number;
    struct reg* next;
}reg_t;

typedef registers{
   size_t size;
   struct reg* head;
}registers_t;

void free_registers(registers_t *register_stack){
    assert(register_stack != NULL);
    _recursive_free(register_stack->head);
    free(register_stack);
}

registers_t *register_stack(){
    registers_t *new_reg = malloc(sizeof(registers_t));
    assert(new_reg != NULL);

    new_reg->head = NULL;
    new_reg->size = 0;
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

int find(registers_t *register_stack, char *id){
    int reg_number = -1;
    reg_t *head = register_stack->head;
    while (head && strcmp(id, head->id) != 0){
        head = head->next;
    }
    if (head){
        reg_number = head->reg_number;
    }
    return reg_number;
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
    reg_t* new_reg = malloc(sizeof(reg_t));
    assert(new_reg != NULL);
    
    new_reg->id = "";
    new_reg->reg_number = -1;
    return new_reg;
}

int main(){
    registers_t *new_regs = register_stack();
    int i, reg_number;
    for (i = 0; i < 50; i++){
        reg_number = allocate(new_reg, "MY REG 1");
        printf("Register number is %d\n", reg_number);
        reg_number = allocate(new_reg, "MY REG 2");
        printf("Register number is %d\n", reg_number);
    }
    for (i = 0; i < 49; i++){
        reg_number = find(new_reg, "MY REG 1");
        printf("Register number found at %d\n", reg_number);
    }
    free_registers(new_reg);
}
