/* Use a tree to represent scopes
 *
 */
#include <stdlib.h>
#include <stdio.h>

enum {
    RESULT,
    ATTRIBUTE,
    UNIFORM,
};

typedef struct declaration{
    char *name;
    int type;
    declaration_t *next;
}declaration_t;

typedef struct tree_node{
    declaration_t *head;
    int n_children;
    tree_node_t *first_child;
    tree_node_t *parent;
} tree_node_t;

tree_node_t *create_tree_node(){
    tree_node_t *node = (tree_node_t *) malloc(sizeof(tree_node_t));
    if (!node){
        printf("Unable to allocate tree node\n");
        exit(-1);
    }
    node->n_children = 0;
    node->first_child = NULL;
    node->parent = NULL;
    node->head = NULL;
    return node;
}

declaration_t *create_declaration(char *name, int type){
    declaration_t *dec = (declaration_t *) malloc(sizeof(declaration_t));
    if (!dec){
        printf("Unable to allocate tree node\n");
        exit(-1);
    }
    dec->name = name;
    dec->type = type;
    dec->next = NULL;
    return dec;
}

void add_declaration(tree_node_t *tree_node, char *name, int type){
    if (!tree_node){
        printf("Tree is NULL\n");
        exit(-1);
    }
    declaration_t *dec = create_declaration(name, type);
    declaration_t *last = tree_node->head;
    dec->next = last;
    tree_node->head = dec;
}

tree_node_t * add_scope(tree_node_t *tree_node, char *name, int type){
    tree_node_t *new_tree_node = create_tree_node();
    add_declaration(new_tree_node, name, type);
    new_tree_node->parent = tree_node;
    tree_node_t *first = tree_node->first_child;
}

