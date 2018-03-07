#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "types.h"

#ifndef ICO_H
#define ICO_H

#define TYPE_ICO 1
#define TYPE_CUR 2

#pragma pack(push, 1)
typedef struct IcoHeader {
    u08 reserved[2];
    
    u16 imageType;
    u16 imageCount;
} IcoHeader;

typedef struct IcoEntry {
    u08 width;
    u08 height;
    
    u08 paletteSize;
    
    u08 reserved;
    
    u16 colorPlanes;
    u16 bitsPerPixel;
    
    u32 size;
    u32 offset;
} IcoEntry;
#pragma pack(pop)

static inline void
ico_read_header(IcoHeader* header, FILE* file) {
    if (header == NULL) {
        header = malloc(sizeof(IcoHeader));
    }
    
    fread(header, sizeof(IcoHeader), 1, file);
    printf("Reserved: %u %u\n", header->reserved[0], header->reserved[1]);
    assert(header->reserved[0] == 0 &&  header->reserved[1] == 0);
    assert(header->imageType == 1 || header->imageType == 2);
}

static inline IcoEntry*
ico_read_entries(IcoHeader* header, FILE* file) {
    IcoEntry* entries = malloc(sizeof(IcoEntry) * header->imageCount);
    fread(entries, sizeof(IcoEntry), header->imageCount, file);
    for (u08 i = 0; i < header->imageCount; i++) {
        assert(entries[i].reserved == 0);
        
        if (header->imageType == TYPE_ICO)
            assert(entries[i].colorPlanes < 2);
    }
    
    return entries;
}

static inline IcoEntry*
ico_read(IcoHeader* header, FILE* file) {
    ico_read_header(header, file);
    return ico_read_entries(header, file);
}

static inline void
ico_write_header(IcoHeader* header, FILE* file) {
    assert(header->reserved[0] == header->reserved[1] == 0);
    fwrite(header, sizeof(IcoHeader), 1, file);
}

static inline void
ico_write_entries(IcoHeader* header, IcoEntry* entries, FILE* file) {
    for (u16 i = 0; i < header->imageCount; i++) {
        IcoEntry* entry = &entries[i];
        
        assert(entry->reserved == 0);
        fwrite(entry, sizeof(IcoEntry), 1, file);
    }
}

static inline void
ico_write(IcoHeader* header, IcoEntry* entries, FILE* file) {
    ico_write_header(header, file);
    ico_write_entries(header, entries, file);
}

#endif // ICO_H