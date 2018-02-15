#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types.h"

#ifndef MEMORY_H
#define MEMORY_H

typedef struct MemoryBlock {
    u64 size;
    u64 position;
    
    void* memory;
} MemoryBlock;

static inline MemoryBlock*
block_prealloc(u64 size) {
    MemoryBlock* block = malloc(sizeof(MemoryBlock));
    block->memory = malloc(size);
    block->size = size;
    block->position = 0;
    
    return block;
}

static inline void*
block_alloc(MemoryBlock* block, u64 size) {
    assert(block->position + size < block->size);
    
    block->position += size;
    return block->memory + (block->position - size);
}

static inline void
block_free(MemoryBlock* block) {
    free(block->memory);
    free(block);
}

#endif // MEMORY_H

