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
    
    IconHeader header;
    ico_read_header(&header, file);
    
    printf("Cursor header:\n");
    printf("  reserved:   0x%x\n", (u16) (header.Reserved[0] + header.Reserved[1]));
    printf("  imageType:  0x%x\n", header.Type);
    printf("  imageCount: 0x%x\n", header.ImageCount);
    
    IconEntry* entries = ico_read_entries(&header, file);
    for(u16 i = 0; i < header.ImageCount; i++) {
        IconEntry entry = entries[i];
        printf("Cursor entry:\n");
        printf("  width:        0x%x\n", entry.Width);
        printf("  height:       0x%x\n", entry.Height);
        printf("  paletteSize:  0x%x\n", entry.PaletteSize);
        printf("  reserved:     0x%x\n", entry.Reserved);
        printf("  colorplanes:  0x%x\n", entry.ColorPlanes);
        printf("  bitsPerPixel: 0x%x\n", entry.BitsPerPixel);
        printf("  size:         0x%x\n", entry.Size);
        printf("  offset:       0x%x\n", entry.Offset);
    }
    
    fclose(file);
    free(entries);
    
    FILE* in  = fopen("file.ico", "rb");
    FILE* out = fopen("newi.ico", "wb");
    
    Icon icon;
    icon_read (in,  &icon);
    icon_write(out, &icon);
    
    icon_free(&icon);
    fclose(in);
    fclose(out);
    
    exit(0);
}