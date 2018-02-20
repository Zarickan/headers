#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "../types.h"

#ifndef ICO_H
#define ICO_H

typedef struct IcoHeader {
    u08 reserved[2];
    
    u16 imageType;
    u16 imageCount;
} IcoHeader;

typedef struct IcoEntry {
    u08 width;
    u08 height;
    
    u08 paletteSize;
    
    u08 reserved; // NOTE: Should always be 0
    
    u16 colorPlanes;
    u16 bitsPerPixel;
    
    u32 size;
    u32 offset;
} IcoEntry;

static inline void
ico_read_header(IcoHeader* header, FILE* file) {
    if(header == NULL) {
        header = malloc(sizeof(IcoHeader));
    }
    
    fread(header, sizeof(IcoHeader), 1, file);
}

static inline IcoEntry*
ico_read_entries(IcoHeader* header, FILE* file) {
    IcoEntry* entries = malloc(sizeof(IcoEntry) * header->imageCount);
    
    for(u16 i = 0; i < header->imageCount; i++) {
        IcoEntry* entry = &entries[i];
        fread(entry, sizeof(IcoEntry), 1, file);
    }
    
    for(u16 i = 0; i < header->imageCount; i++) {
        IcoEntry entry = entries[i];
        void* imageData = malloc(entry.size);
        
        fseek(file, entry.offset, SEEK_SET);
        fread(imageData, entry.size, 1, file);
    }
    
    return entries;
}

#endif // ICO_H