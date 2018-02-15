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

#endif // CURSOR_H