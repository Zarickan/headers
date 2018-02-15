#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "list.h"
#include "linkedlist.h"
#include "heaps.h"
#include "memory.h"

int main(int argc, char** argv)
{
    MemoryBlock* block = block_prealloc(1024*1024*1024);
    f32* blockArray = block_alloc(block, sizeof(f32) * 64);
    
    for(u8 i = 0; i < 64; i++) {
        blockArray[i] = i * 10.364819f;
    }
    
    
    printf("Array is at %lu with:\n", (intptr) blockArray);
    for(u8 i = 0; i < 64; i++) {
        printf("%lu has %f\n", (intptr) &(blockArray[i]), blockArray[i]);
    }
    
    printf("Hello world!\n");
    
    return 0;
}