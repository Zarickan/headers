#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types.h"

#ifndef STACK_H
#define STACK_H

typedef struct Stack {
    u32 size;
    u32 position;
    u32 capacity;
    
    void** buffer;
} Stack;

inline void
stack_init(Stack* stack, u32 size) {
    stack->buffer = (void*) malloc(sizeof(void*) * size);
    
    memset(stack, 0, sizeof(u32) * 2); // NOTE (Frederik): Sets first 2 fields to 0
    stack->capacity = size;
}

inline Stack*
stack_create(u32 size) { 
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack_init(stack, size);
    
    return stack;
}

inline void
stack_push(Stack* stack, void* element) {
    if(stack->position > stack->capacity - 1) {
        assert(stack->capacity * 2 <= UINT32_MAX);
        
        u32 capacity = stack->capacity * 2;
        stack->buffer = realloc(stack->buffer, sizeof(void*) * capacity);
        stack->capacity = capacity;
    }
    
    stack->buffer[stack->position] = element;
    stack->size++;
    stack->position++;
}


inline void*
stack_pop(Stack* stack) {
    assert(stack->position >= 1);
    
    stack->size--;
    stack->position--;
    void* element = stack->buffer[stack->position];
    stack->buffer[stack->position] = NULL;
    
    u32 capacity = stack->capacity / 2;
    if(stack->position < (capacity / 2) && capacity > 0) {
        stack->buffer = realloc(stack->buffer, sizeof(void*) * capacity);
        stack->capacity = capacity;
    }
    
    return element;
}

inline void*
stack_peek(Stack* stack) {
    return stack->buffer[stack->position - 1];
}

#endif // STACK_H