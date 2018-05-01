#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "bitmap.h"

int main(int argc, char** argv)
{
    // I/O Files
    FILE* input = fopen("test.bmp", "rb");
    FILE* output = fopen("output.bmp", "wb");

    // Load the pixel data from the bitmap
    s32 width, height;
    u08* data = bitmap_load(input, &width, &height);

    // Save the bitmap to disk
    bitmap_save(output, width, height, data);

    // Free the bitmap
    exit(0);
}