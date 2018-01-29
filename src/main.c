#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "list.h"
#include "linkedlist.h"
#include "heaps.h"

int main(int argc, char** argv)
{
    const u32 size = 10;
    f32 array[] = { 4, 3, 7, 3, 8, 2, 9, 10, 5, 1 };
    
    printf("[\n");
    for(u32 i = 0; i < size; i++) {
        printf("%f, \n", array[i]);
    }
    printf("]\n");
    
    //build_max_heap(array, size);
    
    printf("[\n");
    for(u32 i = 0; i < size; i++) {
        printf("%f, \n", array[i]);
    }
    printf("]\n");
    
    heap_sort(array, size);
    
    printf("[\n");
    for(u32 i = 0; i < size; i++) {
        printf("%f, \n", array[i]);
    }
    printf("]\n");
    
    printf("Hello world!\n");
    return 0;
}