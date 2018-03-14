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

#include <math.h>

#define IMAGES 27

s16 max(s16 a, s16 b) {
    return a > b ? a : b;
}
s16 min(s16 a, s16 b) {
    return a < b ? a : b;
}

void hsv_to_rgb(f32 h, f32 s, f32 v, u08* r, u08* g, u08* b) {
    f32 C = s * v;
    f32 X = 1; //C * (1 - abs((((s32) h) / 60) % 2 - 1));
    f32 m = v - C;
    
    f32 Rm, Gm, Bm = 0;
    
    //*
    if (0 <= h && h < 60) {
        Rm = C;
        Gm = X * (h / 60);
        Bm = 0;
    }
    else if (60 <= h && h < 120) {
        Rm = 1 - X * ((h - 60) / 60);
        Gm = C;
        Bm = 0;
    }
    else if (120 <= h && h < 180) {
        Rm = 0;
        Gm = C;
        Bm = X * ((h - 120) / 60);
    }
    else if (180 <= h && h < 240) {
        Rm = 0;
        Gm = 1 - X * ((h - 180) / 60);
        Bm = C;
    }
    else if (240 <= h && h < 300) {
        Rm = X * ((h - 240) / 60);
        Gm = 0;
        Bm = C;
    }
    else {
        Rm = C;
        Gm = 0;
        Bm = 1 - X * ((h - 300) / 60);
    }
    //*/
    /*
    if (0 <= h && h < 60) {
        Rm = C;
        Gm = X;//X * (h / 60);
        Bm = 0;
    }
    else if (60 <= h && h < 120) {
        Rm = X;//1 - X * ((h - 60) / 60);
        Gm = C;
        Bm = 0;
    }
    else if (120 <= h && h < 180) {
        Rm = 0;
        Gm = C;
        Bm = X;//X * ((h - 120) / 60);
    }
    else if (180 <= h && h < 240) {
        Rm = 0;
        Gm = X;//1 - X * ((h - 180) / 60);
        Bm = C;
    }
    else if (240 <= h && h < 300) {
        Rm = X; //X * ((h - 240) / 60);
        Gm = 0;
        Bm = C;
    }
    else {
        Rm = C;
        Gm = 0;
        Bm = X; //1 - X * ((h - 300) / 60);
    }
    //*/
    
    f32 R, G, B;
    R = (Rm + m) * 255;
    G = (Gm + m) * 255;
    B = (Bm + m) * 255;
    
    *r = (u08) R;
    *g = (u08) G;
    *b = (u08) B;
}

int main(int argc, char** argv)
{
    Bitmap image;
    bitmap_create_v5(&image, 256, 256);
    
    for(u64 y = 0; y < image.Info->Height; y++) {
        for(u64 x = 0; x < image.Info->Width; x++) {
            u64 index = y * image.Info->Height + x;
            
            /*
            f32 coeff = 1;
            s16 red   = max(coeff * 255 - ((255-x) + (255 - y)), 0) + max(coeff * 255 - ((x) + (y)), 0);
            s16 green = max(coeff * 255 - ((255-x) +        y) , 0) + max(coeff * 255 - ((x) + (y)), 0);
            s16 blue  = max(coeff * 255 - (      x + (255  -y)), 0);
            s16 alpha = max(coeff * 255 - ((x) + (y)), 0) + max(coeff * 255 - ((255 - x) + (y)), 0) + max(coeff * 255 - ((x) + (255 - y)), 0) + max(coeff * 255 - ((255 - x) + (255 - y)), 0);
            
            image.Data.Rgba[index].Red = (u08) min(max(red, 0), 255);
            image.Data.Rgba[index].Green = (u08) min(max(green, 0), 255);
            image.Data.Rgba[index].Blue = (u08) min(max(blue, 0), 255);
            image.Data.Rgba[index].Alpha = alpha;
            */
            
            f32 r = 148;
            f32 centerx, centery;
            centerx = centery = 128;
            
            f32 d1 = centerx - x, d2 = centery - y;
            f32 dist = d1*d1 + d2*d2;
            f32 pd = 1 - (dist / (r * r));
            
            f32 v = atan2(x - centerx, y - centery);
            if (v < 0) v += M_PI * 2;
            f32 p = v / (M_PI * 2);
            f32 hue = (u16) ((f32) 360 * p);
            
            if(dist < r*r) {
                //image.Data.Rgba[index].Red = (u08) (((f32) 255) * pd * (p));
                RgbQuad* pixel = &image.Data.Rgba[index];
                hsv_to_rgb(hue, 1.00, 1.00, &pixel->Red, &pixel->Green, &pixel->Blue);
                image.Data.Rgba[index].Alpha = 255 - 235 * pd;
            }
            else {
                
                RgbQuad* pixel = &image.Data.Rgba[index];
                hsv_to_rgb(hue, 1.00, 1.00, &pixel->Red, &pixel->Green, &pixel->Blue);
                image.Data.Rgba[index].Alpha = 255;
            }
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