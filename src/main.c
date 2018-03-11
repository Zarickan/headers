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
    Bitmap image;
    bitmap_create_v5(&image, 256, 256);
    
    for(u64 y = 0; y < image.Info->Height; y++) {
        for(u64 x = 0; x < image.Info->Width; x++) {
            u64 index = y * image.Info->Height + x;
            image.Data.Rgb[index].Red = x;
            image.Data.Rgb[index].Green = y;
            image.Data.Rgb[index].Blue = 0;
            image.Data.Rgb[index].Alpha = x;
        }
    }
    
    FILE* outputImage = fopen("test.bmp", "wb");
    write_bitmap_to_file(&image, outputImage);
    fclose(outputImage);
    
    exit(0);
    
    char images[IMAGES][40] = {
        { "output/bmpsuite/g/rgb32bfdef.bmp" },
        { "output/bmpsuite/g/rgb32bf.bmp" },
        { "output/bmpsuite/g/rgb32.bmp" },
        { "output/bmpsuite/g/rgb24pal.bmp"}, 
        { "output/bmpsuite/g/rgb24.bmp" },
        { "output/bmpsuite/g/rgb16bfdef.bmp"}, 
        { "output/bmpsuite/g/rgb16.bmp" },
        { "output/bmpsuite/g/rgb16-565pal.bmp"}, 
        { "output/bmpsuite/g/rgb16-565.bmp" },
        { "output/bmpsuite/g/pal8w126.bmp" },
        { "output/bmpsuite/g/pal8w125.bmp" },
        { "output/bmpsuite/g/pal8w124.bmp" },
        { "output/bmpsuite/g/pal8v5.bmp" },
        { "output/bmpsuite/g/pal8v4.bmp" },
        { "output/bmpsuite/g/pal8topdown.bmp" },
        { "output/bmpsuite/g/pal8rle.bmp" },
        { "output/bmpsuite/g/pal8os2.bmp" },
        { "output/bmpsuite/g/pal8nonsquare.bmp"}, 
        { "output/bmpsuite/g/pal8gs.bmp" },
        { "output/bmpsuite/g/pal8.bmp" },
        { "output/bmpsuite/g/pal8-0.bmp" },
        { "output/bmpsuite/g/pal4rle.bmp" },
        { "output/bmpsuite/g/pal4gs.bmp" },
        { "output/bmpsuite/g/pal4.bmp" },
        { "output/bmpsuite/g/pal1wb.bmp" },
        { "output/bmpsuite/g/pal1bg.bmp" },
        { "output/bmpsuite/g/pal1.bmp" }
    };
    
    for (u08 i = 0; i < IMAGES; i++) {
        FILE* file = fopen(images[i], "rb");
        if (file == NULL || ferror(file)) {
            printf("Error reading file %s\n", images[i]);
            continue;
        }
        
        BitmapHeader header;
        BitmapInfoHeader info;
        bitmap_read_info(&header, &info, file);
        
        u08* data = malloc(info.SizeImage);
        fread(data, sizeof(u08), info.SizeImage, file);
        
        printf("Metadata for %s\n", images[i]);
        display_bitmapinfo(&info);
        
        if (info.UsedColors > 0) {
            printf("  :: Colors in palette:\n");
            display_palette(&info, data);
        }
        
        printf("\n---------------------------\n");
    }
    
    exit(0);
}