#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "types.h"

#ifndef HEAPS_H
#define HEAPS_H

inline u32
parent(u32 index) {
    return (index + 1) / 2 - 1;
}

inline u32
right(u32 index) {
    return 2 * (index + 1);
}

inline u32
left(u32 index) {
    return 2 * (index + 1) - 1;
}

inline void
exchange(f32* array, u32 src, u32 dst) {
    f32 temp = array[src];
    array[src] = array[dst];
    array[dst] = temp;
}

inline void
max_Heapify(f32* array, u32 size, u32 index) {
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

inline void
build_max_heap(f32* array, u32 size) {
    for(u32 i = size / 2 - 1; i > 0; i--) {
        max_heapify(array, size, i);
    }
}

inline void
heapsort(f32* array, u32 size) {
    build_max_heap(array, size);
    
    for(u32 i = size - 1; i > 1; i--) {
        exchange(array, 0, i);
        max_heapify(array, size, 0);
    }
}

#endif // HEAPS_H