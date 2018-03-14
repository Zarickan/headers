#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "stack.h"
#include "memory.h"
#include "bitmap.h"

s16
max(s16 a, s16 b) {
    return a > b ? a : b;
}

s16
min(s16 a, s16 b) {
    return a < b ? a : b;
}

void hsv_to_rgb(f32 h, f32 s, f32 v, u08* r, u08* g, u08* b) {
    f32 C = s * v;
    f32 X = 1;
    f32 m = v - C;
    
    f32 Rm, Gm, Bm = 0;
    
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
    
    f32 R, G, B;
    R = (Rm + m) * 255;
    G = (Gm + m) * 255;
    B = (Bm + m) * 255;
    
    *r = (u08) R;
    *g = (u08) G;
    *b = (u08) B;
}

#define PI 3.1415926535

int main(int argc, char** argv)
{
    Bitmap image;
    bitmap_create_v5(&image, 256, 256);
    
    for (u64 y = 0; y < image.Info->Height; y++) {
        for (u64 x = 0; x < image.Info->Width; x++) {
            u64 index = y * image.Info->Height + x;
            
            f32 r = 148;
            f32 centerx, centery;
            centerx = centery = 128;
            
            f32 d1 = centerx - x, d2 = centery - y;
            f32 dist = d1*d1 + d2*d2;
            f32 pd = 1 - (dist / (r * r));
            
            f32 v = atan2(x - centerx, y - centery);
            if (v < 0)
                v += PI * 2;
            
            f32 p = v / (PI * 2);
            f32 hue = (u16) ((f32) 360 * p);
            
            if (dist < r*r) {
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
}