#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "bitmap.h"

int main(int argc, char** argv)
{
    // Input files
    char* images[] = {
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
        "C:\\Users\\frede\\Repositories\\headers\\test_input\\1bpp-323x240.bmp"
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