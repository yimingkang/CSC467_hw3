#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_node{
    char *name;
    int data_type;
    int is_const;
    int type;
    struct list_node *next;
} list_node_t;

typedef struct {
    list_node_t *head;
    list_node_t *tail;
}linked_list_t;

linked_list_t *linked_list(){
    linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

list_node_t *new_node(char *name, int data_type, int is_const, int type){
    list_node_t *new_node = (list_node_t *) malloc(sizeof(list_node_t));
    new_node->name = name;
    new_node->data_type = data_type;
    new_node->is_const = is_const;
    new_node->type = type;
    new_node->next = NULL;
    return new_node;
}

void insert(linked_list_t *list, char *name, int data_type, int type, int is_const){
    if (!list){
        exit(-1);
    }
    list_node_t *node = new_node(name, data_type, is_const, type);
    if (list->head == NULL){
        list->head = node;
        list->tail = node;
        node->next = NULL;
        return;
    }else{
        list->tail->next = node;
        list->tail = node;
        node->next = NULL;
    }
}

void delete_from(linked_list_t *list, list_node_t *node){
    if(list){
        list->tail = node;
        if (list->tail == NULL){
            list->head = NULL;
        }
    }
    if (node){
        node->next = NULL;
    }
}


list_node_t *search(linked_list_t*list, char *name){
    if (!list){
        return NULL;
    }
    list_node_t *ptr = list->head;
    while (ptr){
        if(!strcmp(name, ptr->name)){
            // found a match
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

