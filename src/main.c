#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "bitmap.h"

int main(int argc, char** argv)
{
    // Input files
    char* images[] = {
        "test_input/555-1x1.bmp",
        "test_input/555-320x240.bmp",
        "test_input/555-321x240.bmp",
        "test_input/565-1x1.bmp",
        "test_input/565-320x240.bmp",
        "test_input/565-320x240-topdown.bmp",
        "test_input/565-321x240.bmp",
        "test_input/565-321x240-topdown.bmp",
        "test_input/565-322x240.bmp",
        "test_input/565-322x240-topdown.bmp",
        "test_input/8bpp-topdown-320x240.bmp",
        "test_input/24bpp-1x1.bmp",
        "test_input/24bpp-320x240.bmp",
        "test_input/24bpp-321x240.bmp",
        "test_input/24bpp-322x240.bmp",
        "test_input/24bpp-323x240.bmp",
        "test_input/24bpp-imagesize-zero.bmp",
        "test_input/24bpp-topdown-320x240.bmp",
        "test_input/32bpp-1x1.bmp",
        "test_input/32bpp-320x240.bmp",
        "test_input/32bpp-888-optimalpalette-320x240.bmp",
        "test_input/32bpp-101110-320x240.bmp",
        "test_input/32bpp-optimalpalette-320x240.bmp",
        "test_input/32bpp-topdown-320x240.bmp",
        "test_input/misleadingextension.jpg",
        "test_input/nofileextension",
        "test_input/spaces in  filename.bmp",
        "test_input/1bpp-1x1.bmp",
        "test_input/1bpp-320x240.bmp",
        "test_input/1bpp-320x240-color.bmp",
        "test_input/1bpp-320x240-overlappingcolor.bmp",
        "test_input/1bpp-321x240.bmp",
        "test_input/1bpp-322x240.bmp",
        "test_input/1bpp-323x240.bmp",
        "test_input/1bpp-324x240.bmp",
        "test_input/1bpp-325x240.bmp",
        "test_input/1bpp-326x240.bmp",
        "test_input/1bpp-327x240.bmp",
        "test_input/1bpp-328x240.bmp",
        "test_input/1bpp-329x240.bmp",
        "test_input/1bpp-330x240.bmp",
        "test_input/1bpp-331x240.bmp",
        "test_input/1bpp-332x240.bmp",
        "test_input/1bpp-333x240.bmp",
        "test_input/1bpp-334x240.bmp",
        "test_input/1bpp-335x240.bmp",
        "test_input/1bpp-topdown-320x240.bmp",
        "test_input/4bpp-1x1.bmp",
        "test_input/4bpp-320x240.bmp",
        "test_input/4bpp-321x240.bmp",
        "test_input/4bpp-322x240.bmp",
        "test_input/4bpp-323x240.bmp",
        "test_input/4bpp-324x240.bmp",
        "test_input/4bpp-325x240.bmp",
        "test_input/4bpp-326x240.bmp",
        "test_input/4bpp-327x240.bmp",
        "test_input/4bpp-topdown-320x240.bmp",
        "test_input/8bpp-1x1.bmp",
        "test_input/8bpp-1x64000.bmp",
        "test_input/8bpp-320x240.bmp",
        "test_input/8bpp-321x240.bmp",
        "test_input/8bpp-322x240.bmp",
        "test_input/8bpp-323x240.bmp",
        "test_input/8bpp-colorsimportant-two.bmp",
        "test_input/8bpp-colorsused-zero.bmp",
        "test_input/pels-per-meter-x-large.bmp",
        "test_input/pels-per-meter-x-negative.bmp",
        "test_input/pels-per-meter-x-zero.bmp",
        "test_input/pels-per-meter-y-large.bmp",
        "test_input/pels-per-meter-y-negative.bmp",
        "test_input/pels-per-meter-y-zero.bmp",
        "test_input/pixeldata-toomuch.bmp",
        "test_input/planes-large.bmp",
        "test_input/planes-zero.bmp",
        "test_input/reserved1-bad.bmp",
        "test_input/reserved2-bad.bmp",
        "test_input/8bpp-pixels-not-in-palette.bmp",
        "test_input/filesize-bad.bmp",
        "test_input/filesize-zero.bmp"
    };
    
    char outputFile[500];
    u16 imageCount = sizeof(images) / sizeof(char*);
    for (u16 i = 0; i < imageCount; i++) {
        strcpy(outputFile, "");
        strcat(outputFile, "test_output/");
        strcat(outputFile,  strstr(images[i], "/") + 1);
        //strcat(outputFile, "_output.bmp");
        
        printf("Image: %s\n", images[i]);
        
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