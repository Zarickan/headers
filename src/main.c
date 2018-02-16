#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "list.h"
#include "linkedlist.h"
#include "heaps.h"
#include "memory.h"
#include "formats/cursor.h"

int main(int argc, char** argv)
{
    printf("before file\n");
    FILE* file = fopen("file.cur", "r+");
    printf("after file\n");
    
    // Read header
    CursorHeader header;
    fread(&header, sizeof(CursorHeader), 1, file);
    printf("Cursor header:\n");
    printf("  reserved:   0x%#u\n", (u16) (header.reserved[0] + header.reserved[1]));
    printf("  imageType:  0x%#u\n", header.imageType);
    printf("  imageCount: 0x%#u\n", header.imageCount);
    
    // Read entries
    CursorEntry* entries = malloc(sizeof(CursorEntry) * header.imageCount);
    for(u16 i = 0; i < header.imageCount; i++) {
        fread(&entries[i], sizeof(CursorEntry), 1, file);
        
        CursorEntry entry = entries[i];
        printf("Cursor entry:\n");
        printf("  width:        0x%#u\n", entry.width);
        printf("  height:       0x%#u\n", entry.height);
        printf("  paletteSize:  0x%#u\n", entry.paletteSize);
        printf("  reserved:     0x%#u\n", entry.reserved);
        printf("  hotspotLeft:  0x%#u\n", entry.hotspotLeft);
        printf("  hotspotRight: 0x%#u\n", entry.hotspotRight);
        printf("  size:         0x%#u\n", entry.size);
        printf("  offset:       0x%#u\n", entry.offset);
    }
    
    // Extract individual image data
    for(u16 i = 0; i < header.imageCount; i++) {
        CursorEntry* entry = &entries[i];
        void* imageData = malloc(entry->size);
        
        fseek(file, entry->offset, SEEK_SET);
        fread(imageData, 1, entry->size, file);
        
        char outname[128];
        sprintf(outname, "output-%ix%i.bmp", entry->width, entry->height);
        
        FILE* outputFile = fopen(outname, "wb");
        fwrite(imageData, 1, entry->size, outputFile);
        fclose(outputFile);
    }
    
    fclose(file);
    free(entries);
    
    return 0;
}