#include <stdlib.h>
#include <stdio.h>
#include "types.h"

#ifndef MAP_H
#define MAP_H

#define MAP_SIZE 65536

typedef struct Bucket {
    void *Value;
    void *Next;
    
    u08 HasValue;
} Bucket;

typedef struct Map {
    Bucket Value[MAP_SIZE];
    u16 (*hash)(void*);
} Map;

// make / free
static inline Map*
map_make(u16 (*hash)(void*)) {
    Map *map = malloc(sizeof(Map));
    map->hash = hash;
    
    return map;
}
static inline void
map_free(Map *map) {
    free(map);
}

// add / remove
static inline s32
map_add(Map *map, void *value) {
    u16 key = map->hash(value);
    
    // If the map has no bcuket already, add a new one
    if(!map->Value[key].HasValue) {
        Bucket valueBucket;
        valueBucket.Value = value;
        valueBucket.Next = NULL;
        valueBucket.HasValue = 1;
        
        map->Value[key] = valueBucket;
        return 0;
    }
    
    
    // Create a new bucket for the value
    Bucket *valueBucket = malloc(sizeof(Bucket));
    valueBucket->Value = value;
    valueBucket->Next = NULL;
    valueBucket->HasValue = 1;
    
    // Find the last bucket in the cahin
    Bucket *bucket = map->Value + key;
    while(bucket->Next != NULL) {
        bucket = bucket->Next;
        
        if(bucket->Value == value)
            return 1;
    }
    
    bucket->Next = valueBucket;
    return 0;
}
static inline void*
map_remove(Map *map, void *value) {
    u16 key = map->hash(value);
    
    Bucket *bucket = map->Value + key;
    Bucket *previous = NULL;
    if(bucket == NULL) return NULL;
    
    // Look through all buckets until we find the one that contains the
    // value we are given, keep track of the previous bucket.
    while(bucket->Value != value) {
        if(bucket->Next == NULL)
            return NULL;
        
        previous = bucket;
        bucket = previous->Next;
    }
    
    // Remove the bucket from the cahin, if it is the first bucket
    // in the chain the next one after it will become the first one.
    if(previous != NULL)
        previous->Next = bucket->Next;
    else
        map->Value[key] = *((Bucket*) bucket->Next);
    
    return bucket;
}

static inline void
map_print(Map *map) {
    printf("Map of size %lu:\n", MAP_SIZE);
    
    for(s32 i = 0; i < MAP_SIZE; i++) {
        Bucket *bucket = map->Value + i;
        if(!bucket->HasValue) continue;
        printf("  Bucket %4x:\n", i);
        
        while(bucket != NULL && bucket->HasValue) {
            u08 *value = bucket->Value;
            
            printf("    %c (%x)\n", *value, *value);
            bucket = bucket->Next;
        }
    }
}

#endif // MAP_H