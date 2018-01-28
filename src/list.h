#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types.h"

#ifndef LIST_H
#define LIST_H

typedef struct List {
    u32 size;
    u32 capacity;
    
    void** elements;
} List;

inline void
list_init(List* list, u32 size) {
    list->elements = (void*) malloc(sizeof(void*) * size);
    
    list->size = 0;
    list->capacity = size;
}

inline List*
list_create(u32 size) {
    List* list = (List*) malloc(sizeof(List));
    list_init(list, size);
    
    return list;
}

inline void
list_add(List* list, void* element) {
    if(list->size > list->capacity - 1) {
        assert(list->capacity * 2 <= UINT32_MAX);
        
        u32 capacity = list->capacity * 2;
        list->elements = realloc(list->elements, sizeof(void*) * capacity);
        list->capacity = capacity;
    }
    
    list->elements[list->size] = element;
    list->size++;
}

inline void*
list_remove(List* list, u32 index) {
    assert(index < list->size);
    void* element = list->elements[index];
    list->elements[index] = NULL;
    
    for(u32 i = index + 1; i < list->size; i++) {
        list->elements[i - 1] = list->elements[i];
    }
    
    list->size--;
    if(list->size < (list->capacity / 4) && (list->capacity / 2) > 0) {
        list->elements = realloc(list->elements, sizeof(void*) * (list->capacity / 2));
        list->capacity = list->capacity / 2;
    }
    
    return element;
}

#endif // LIST_H