#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "list.h"
#include "linkedlist.h"
#include "heaps.h"
#include "memory.h"
#include "formats/cursor.h"
#include "formats/ico.h"

int main(int argc, char** argv)
{
    FILE* file = fopen("file.cur", "r+");
    
    IcoHeader header;
    ico_read_header(&header, file);
    
    printf("Cursor header:\n");
    printf("  reserved:   0x%x\n", (u16) (header.reserved[0] + header.reserved[1]));
    printf("  imageType:  0x%x\n", header.imageType);
    printf("  imageCount: 0x%x\n", header.imageCount);
    
    IcoEntry* entries = ico_read_entries(&header, file);
    for(u16 i = 0; i < header.imageCount; i++) {
        IcoEntry entry = entries[i];
        printf("Cursor entry:\n");
        printf("  width:        0x%x\n", entry.width);
        printf("  height:       0x%x\n", entry.height);
        printf("  paletteSize:  0x%x\n", entry.paletteSize);
        printf("  reserved:     0x%x\n", entry.reserved);
        printf("  colorplanes:  0x%x\n", entry.colorPlanes);
        printf("  bitsPerPixel: 0x%x\n", entry.bitsPerPixel);
        printf("  size:         0x%x\n", entry.size);
        printf("  offset:       0x%x\n", entry.offset);
    }
    
    fclose(file);
    free(entries);
    
    exit(0);
}