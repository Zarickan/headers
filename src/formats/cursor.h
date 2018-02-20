#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "../types.h"

#ifndef CURSOR_H
#define CURSOR_H

typedef struct CursorHeader {
    u08 reserved[2];
    
    u16 imageType;
    u16 imageCount;
} CursorHeader;

typedef struct CursorEntry {
    u08 width;
    u08 height;
    
    u08 paletteSize;
    
    u08 reserved; // NOTE: Should always be 0
    
    u16 hotspotLeft;
    u16 hotspotRight;
    
    u32 size;
    u32 offset;
} CursorEntry;

static inline void
cur_read_header(CursorHeader* header, FILE* file) {
    if(header == NULL) {
        header = malloc(sizeof(CursorHeader));
    }
    
    fread(header, sizeof(CursorHeader), 1, file);
}

static inline CursorEntry*
cur_read_entries(CursorHeader* header, FILE* file) {
    CursorEntry* entries = malloc(sizeof(CursorEntry) * header->imageCount);
    
    for(u16 i = 0; i < header->imageCount; i++) {
        CursorEntry* entry = &entries[i];
        fread(entry, sizeof(CursorEntry), 1, file);
    }
    
    for(u16 i = 0; i < header->imageCount; i++) {
        CursorEntry entry = entries[i];
        void* imageData = malloc(entry.size);
        
        fseek(file, entry.offset, SEEK_SET);
        fread(imageData, entry.size, 1, file);
    }
    
    return entries;
}

#endif // CURSOR_H