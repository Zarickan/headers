#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types.h"

#ifndef HEAPS_H
#define HEAPS_H

static inline u32
parent(u32 index) {
    return (index + 1) / 2 - 1;
}

static inline u32
right(u32 index) {
    return 2 * (index + 1);
}

static inline u32
left(u32 index) {
    return 2 * (index + 1) - 1;
}

static inline void
exchange(f32* array, u32 src, u32 dst) {
    f32 temp = array[src];
    array[src] = array[dst];
    array[dst] = temp;
}

static inline void
max_heapify(f32* array, u32 size, u32 index) {
    u32 l = left(index);
    u32 r = right(index);
    
    u32 largest = index;
    if(l <= size && array[l] > array[index]) {
        largest = l;
    }
    if(r <= size && array[r] > array[largest]) {
        largest = r;
    }
    
    if(largest != index) {
        exchange(array, index, largest);
        max_heapify(array, size, largest);
    }
}

static inline void
build_max_heap(f32* array, u32 size) {
    for(u32 i = (size / 2); i --> 0;) {
        max_heapify(array, size, i);
    }
}

static inline void
heap_sort(f32* array, u32 size) {
    build_max_heap(array, size);
    
    for(u32 i = size - 1; i --> 1;) {
        exchange(array, 0, i);
        max_heapify(array, size - (size - 1 -i), 0);
    }
}

#endif // HEAPS_H