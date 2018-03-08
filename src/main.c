#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "stack.h"
#include "list.h"
#include "linkedlist.h"
#include "heaps.h"
#include "memory.h"
#include "cur.h"
#include "ico.h"
#include "bitmap.h"

#define IMAGES 27

int main(int argc, char** argv)
{
    char images[IMAGES][40] = {
        { "rgb32bfdef.bmp" },
        { "rgb32bf.bmp" },
        { "rgb32.bmp" },
        { "rgb24pal.bmp"}, 
        { "rgb24.bmp" },
        { "rgb16bfdef.bmp"}, 
        { "rgb16.bmp" },
        { "rgb16-565pal.bmp"}, 
        { "rgb16-565.bmp" },
        { "pal8w126.bmp" },
        { "pal8w125.bmp" },
        { "pal8w124.bmp" },
        { "pal8v5.bmp" },
        { "pal8v4.bmp" },
        { "pal8topdown.bmp" },
        { "pal8rle.bmp" },
        { "pal8os2.bmp" },
        { "pal8nonsquare.bmp"}, 
        { "pal8gs.bmp" },
        { "pal8.bmp" },
        { "pal8-0.bmp" },
        { "pal4rle.bmp" },
        { "pal4gs.bmp" },
        { "pal4.bmp" },
        { "pal1wb.bmp" },
        { "pal1bg.bmp" },
        { "pal1.bmp" }
    }; 
    
    for (u08 i = 0; i < IMAGES; i++) {
        FILE* file = fopen("pal1.bmp", "rb");
        if (file == NULL || ferror(file)) {
            printf("Error reading file %s\n", images[i]);
            continue;
        }
        
        BitmapInfoHeader header;
        bitmap_read_info(&header, file);
        
        printf("Metadata for %s\n", images[i]);
        display_bitmapinfo(&header);
        printf("\n---------------------------\n");
    }
    
    exit(0);
}