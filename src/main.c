#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "bitmap.h"

int main(int argc, char** argv)
{
    // Input files
    char* images[] = {
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\pixeldata-toomuch.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\planes-large.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\planes-zero.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\reserved1-bad.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\reserved2-bad.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\8bpp-pixels-not-in-palette.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\filesize-bad.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\filesize-zero.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\pels-per-meter-x-large.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\pels-per-meter-x-negative.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\pels-per-meter-x-zero.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\pels-per-meter-y-large.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\pels-per-meter-y-negative.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_corrupt\\pels-per-meter-y-zero.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-topdown-320x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-topdown-320x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-topdown-320x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-324x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-325x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-326x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-327x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-328x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-329x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-330x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-331x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-332x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-333x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-334x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-335x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-1x1.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-320x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-321x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-322x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-323x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-324x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-325x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-326x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\4bpp-327x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-1x1.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-1x64000.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-320x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-321x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-322x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-323x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-1x1.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-320x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-320x240-color.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-320x240-overlappingcolor.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-321x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-322x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-323x240.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-colorsimportant-two.bmp",
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\8bpp-colorsused-zero.bmp"
    };
    
    char outputFile[500];
    u16 imageCount = sizeof(images) / sizeof(char*);
    for (u16 i = 0; i < imageCount; i++) {
        strcpy(outputFile, "");
        strcat(outputFile, images[i]);
        strcat(outputFile, "_output.bmp");
        
        // I/O Files
        FILE* input = fopen(images[i], "rb");
        FILE* output = fopen(outputFile, "wb");
        
        // Load the pixel data from the bitmap
        s32 width, height;
        u08* data = bitmap_load(input, &width, &height);
        
        // Save the bitmap to disk
        bitmap_save(output, width, height, data);
        
        fclose(input);
        fclose(output);
        free(data);
    }
    
    exit(0);
}