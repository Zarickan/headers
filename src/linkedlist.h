#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types.h"

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct ListNode {
    void* value;
    void* prev;
    void* next;
} ListNode;

inline void
listnode_init(ListNode* node, void* value) {
    node->prev = NULL;
    node->next = NULL;
    node->value = value;
}

inline ListNode*
listnode_create(void* value) {
    ListNode* node = (ListNode*) malloc(sizeof(ListNode));
    node_init(node, value);
    
    return node;
}

typedef struct LinkedList {
    u32 size;
    ListNode* head;
    ListNode* tail;
} LinkedList;

inline void
linkedlist_init(LinkedList* list) {
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

inline LinkedList*
linkedlist_create() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    linkedlist_init(list);
    
    return list;
}

inline void
linkedlist_add(LinkedList* list, ListNode* node) {
    if(list->size == 0) {
        node->next = NULL;
        node->prev = NULL;
        
        list->head = node;
        list->tail = node;
    }
    else if (list->size == 1) {
        node->next = NULL;
        node->prev = list->head;
        
        list->head->next = node;
        list->head->prev = NULL;
        
        list->tail = node;
    }
    else {
        node->prev = list->tail;
        node->next = NULL;
        
        list->tail->next = node;
        list->tail = node;
    }
    
    list->size++;
}

inline void
linkedlist_remove(LinkedList* list, ListNode* node) {
    if(list->tail == node) {
        list->tail = node->prev;
        ((ListNode*) node->prev)->next = node->next;
    }
    else if(list->head == node) {
        list->head = node->next;
        ((ListNode*) node->next)->prev = node->prev;
    }
    else {
        ((ListNode*) node->prev)->next = node->next;
        ((ListNode*) node->next)->prev = node->prev;
    }
    
    list->size--;
}

#endif //LINKEDLIST_H