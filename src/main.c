#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "bitmap.h"

int main(int argc, char** argv)
{
    /*
    // I/O Files
    FILE* input = fopen("test_input/lenna.bmp", "rb");
    
    // Load the pixel data from the bitmap
    s32 width, height;
    u08* data = bitmap_load(input, &width, &height);
    RgbQuad* rgbData = (RgbQuad*) data;
    
    FILE* output = fopen("test.bmp", "wb");
    
    RgbQuad* palette = kmeans_cluster(rgbData, width, height, 256);
    //u32* palette = malloc(sizeof(u32) * 2);
    //palette[0] = 0xFF000000;
    //palette[1] = 0xFFFFFFFF;
    
    dither_floydsteinberg(rgbData, width, height, (RgbQuad*) palette, 2);
    
    // Save the bitmap to disk
    bitmap_save(output, width, height, data);
    
    free(palette);
    
    fclose(input);
    fclose(output);
    free(data);
    
    exit(0);
    */
    
    // Input files
    char* images[] = {
        //"test_input/pal8rle.bmp",
        //*
        //"test_input/pixeldata-rle8-toomuch.bmp",
        "test_input/rletopdown.bmp",
        "test_input/rle4-no-end-of-bitmap-marker.bmp",
        "test_input/rle8-height-negative.bmp",
        "test_input/rle8-no-end-of-bitmap-marker.bmp",
        "test_input/rle4-height-negative.bmp",
        "test_input/pal4rletrns.bmp",
        "test_input/pal8rlecut.bmp",
        "test_input/pal8rletrns.bmp",
        "test_input/pal4rlecut.bmp",
        "test_input/rgb32bfdef.bmp",
        "test_input/rgb32fakealpha.bmp",
        "test_input/rgb32h52.bmp",
        "test_input/rgb32-xbgr.bmp",
        "test_input/rgba16-1924.bmp",
        "test_input/rgba16-4444.bmp",
        "test_input/rgba16-5551.bmp",
        "test_input/rgba32.bmp",
        "test_input/rgba32-61754.bmp",
        "test_input/rgba32-81284.bmp",
        "test_input/rgba32-1010102.bmp",
        "test_input/rgba32abf.bmp",
        "test_input/rgba32h56.bmp",
        "test_input/rle4-absolute-320x240.bmp",
        "test_input/rle4-alternate-320x240.bmp",
        "test_input/rle4-delta-320x240.bmp",
        "test_input/rle4-encoded-320x240.bmp",
        "test_input/rle8-64000x1.bmp",
        "test_input/rle8-absolute-320x240.bmp",
        "test_input/rle8-blank-160x120.bmp",
        "test_input/rle8-delta-320x240.bmp",
        "test_input/rle8-encoded-320x240.bmp",
        "test_input/spaces in  filename.bmp",
        "test_input/test.bmp",
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
        "test_input/8bpp-colorsimportant-large.bmp",
        "test_input/8bpp-colorsimportant-two.bmp",
        "test_input/8bpp-colorsused-zero.bmp",
        "test_input/8bpp-pixels-not-in-palette.bmp",
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
        "test_input/badbitcount.bmp",
        "test_input/badbitssize.bmp",
        "test_input/baddens1.bmp",
        "test_input/baddens2.bmp",
        "test_input/badfilesize.bmp",
        "test_input/badheadersize.bmp",
        "test_input/badplanes.bmp",
        "test_input/badwidth.bmp",
        "test_input/filesize-bad.bmp",
        "test_input/filesize-zero.bmp",
        "test_input/lenna.bmp",
        "test_input/misleadingextension.jpg",
        "test_input/nofileextension",
        "test_input/offbits-zero.bmp",
        "test_input/pal1.bmp",
        "test_input/pal1bg.bmp",
        "test_input/pal1p1.bmp",
        "test_input/pal1wb.bmp",
        "test_input/pal2.bmp",
        "test_input/pal2color.bmp",
        "test_input/pal4.bmp",
        "test_input/pal4gs.bmp",
        "test_input/pal4rle.bmp",
        "test_input/pal8.bmp",
        "test_input/pal8-0.bmp",
        "test_input/pal8badindex.bmp",
        "test_input/pal8gs.bmp",
        "test_input/pal8nonsquare.bmp",
        "test_input/pal8offs.bmp",
        //"test_input/pal8os2.bmp",
        "test_input/pal8oversizepal.bmp",
        "test_input/pal8rle.bmp",
        "test_input/pal8topdown.bmp",
        "test_input/pal8v4.bmp",
        "test_input/pal8v5.bmp",
        "test_input/pal8w124.bmp",
        "test_input/pal8w125.bmp",
        "test_input/pal8w126.bmp",
        "test_input/palette-too-big.bmp",
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
        "test_input/rgb16.bmp",
        "test_input/rgb16-231.bmp",
        "test_input/rgb16-565.bmp",
        "test_input/rgb16-565pal.bmp",
        "test_input/rgb16-880.bmp",
        "test_input/rgb16-3103.bmp",
        "test_input/rgb16bfdef.bmp",
        "test_input/rgb16faketrns.bmp",
        "test_input/rgb24.bmp",
        "test_input/rgb24largepal.bmp",
        "test_input/rgb24lprof.bmp",
        "test_input/rgb24pal.bmp",
        "test_input/rgb24prof.bmp",
        "test_input/rgb24prof2.bmp",
        "test_input/rgb32.bmp",
        "test_input/rgb32-7187.bmp",
        "test_input/rgb32-111110.bmp",
        "test_input/rgb32bf.bmp",
        //*/
    };
    
    char outputFile[500];
    u16 imageCount = sizeof(images) / sizeof(char*);
    for (u16 i = 0; i < imageCount; i++) {
        strcpy(outputFile, "");
        strcat(outputFile, "test_output/");
        strcat(outputFile,  strstr(images[i], "/") + 1);
        
        printf("Image: %s\n", images[i]);
        
        // I/O Files
        FILE* input = fopen(images[i], "rb");
        
        // Load the pixel data from the bitmap
        s32 width, height;
        u08* data = bitmap_load(input, &width, &height);
        if (data == NULL) continue;
        
        FILE* output = fopen(outputFile, "wb");
        
        // Save the bitmap to disk
        bitmap_save(output, width, height, data);
        
        fclose(input);
        fclose(output);
        free(data);
    }
    
    exit(0);
}