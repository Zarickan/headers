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

#endif // ICO_H