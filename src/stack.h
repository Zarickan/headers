#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types.h"

#ifndef STACK_H
#define STACK_H

typedef struct Stack {
    u32 size;
    u32 capacity;
    
    void** buffer;
} Stack;

static inline void
stack_init(Stack* stack, u32 size) {
    stack->buffer = (void*) malloc(sizeof(void*) * size);
    
    stack->size = 0;
    stack->capacity = size;
}

static inline Stack*
stack_create(u32 size) { 
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack_init(stack, size);
    
    return stack;
}

static inline void
stack_push(Stack* stack, void* element) {
    if(stack->size > stack->capacity - 1) {
        assert(stack->capacity * 2 <= UINT32_MAX);
        
        u32 capacity = stack->capacity * 2;
        stack->buffer = realloc(stack->buffer, sizeof(void*) * capacity);
        stack->capacity = capacity;
    }
    
    stack->buffer[stack->size] = element;
    stack->size++;
}


static inline void*
stack_pop(Stack* stack) {
    assert(stack->size >= 1);
    
    stack->size--;
    void* element = stack->buffer[stack->size];
    stack->buffer[stack->size] = NULL;
    
    u32 capacity = stack->capacity / 2;
    if(stack->size < (capacity / 2) && capacity > 0) {
        stack->buffer = realloc(stack->buffer, sizeof(void*) * capacity);
        stack->capacity = capacity;
    }
    
    return element;
}

static inline void*
stack_peek(Stack* stack) {
    return stack->buffer[stack->size - 1];
}

#endif // STACK_H