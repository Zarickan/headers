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
cursor_read_header(FILE* file, CursorHeader* header) {
    fread(header, sizeof(CursorHeader), 1, file);
}
static inline void
cursor_read_entries(FILE* file, CursorHeader* header, CursorEntry* entries) {
    entries = malloc(sizeof(CursorEntry) * header->imageCount);
    
    for(u16 i = 0; i < header->imageCount; i++) {
        fread(&entries[i], sizeof(CursorEntry), 1, file);
    }
}

#endif // CURSOR_H