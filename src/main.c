#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "types.h"
#include "stack.h"
#include "memory.h"
#include "bitmap.h"

int main(int argc, char** argv)
{
    // I/O Files
    FILE* input = fopen("input.bmp", "rb");
    FILE* output = fopen("output.bmp", "wb");
    
    // Load the pixel data from the bitmap
    u32 width, height;
    u08* data = bitmap_load(input, &width, &height);
    
    // Save the bitmap to disk
    //bitmap_save(output, width, height);
    
    // Free the bitmap
    
    exit(0);
}