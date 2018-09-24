#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "bitmap.h"
#include "gif.h"

void
gif_test(void) {
    char* images[] = {
        "gif_valid/sunflower.gif",
        "gif_valid/small2.gif",
        "gif_valid/morph.gif",
        "gif_valid/newton.gif",
        "gif_valid/oil.gif",
        "gif_valid/fullcolor.gif",
        "gif_valid/quilt.gif",
        "gif_valid/globe.gif",
    };
    
    char outputFile[500];
    u16 imageCount = sizeof(images) / sizeof(char*);
    for (u16 i = 0; i < imageCount; i++) {
        strcpy(outputFile, "");
        strcat(outputFile, "gif_output/");
        strcat(outputFile,  strstr(images[i], "/") + 1);
        strcat(outputFile, ".bmp");
        
        printf("\nImage: %s\n", images[i]);
        
        // I/O Files
        FILE* input = fopen(images[i], "rb");
        if (input == NULL) continue;
        
        // Load the pixel data from the bitmap
        s32 width, height;
        u08* data = gif_load(input, &width, &height);
        
        FILE* output = fopen(outputFile, "wb");
        
        // Save the bitmap to disk
        if (data != NULL)
            bitmap_save(output, width, height, data);
        
        fclose(input);
        fclose(output);
        free(data);
    }
    
    exit(0);
}

void
bitmap_test(void) {
    
    char* images[] = {
        // Corrupt (technically corrupt, but we should manage)
        "test_corrupt/1bpp-no-palette.bmp",
        "test_corrupt/1bpp-pixeldata-cropped.bmp",
        "test_corrupt/24bpp-pixeldata-cropped.bmp",
        "test_corrupt/32bpp-pixeldata-cropped.bmp",
        "test_corrupt/4bpp-no-palette.bmp",
        "test_corrupt/4bpp-pixeldata-cropped.bmp",
        "test_corrupt/555-pixeldata-cropped.bmp",
        "test_corrupt/8bpp-colorsimportant-large.bmp",
        "test_corrupt/8bpp-colorsimportant-negative.bmp",
        "test_corrupt/8bpp-colorsused-large.bmp",
        "test_corrupt/8bpp-colorsused-negative.bmp",
        "test_corrupt/8bpp-no-palette.bmp",
        "test_corrupt/8bpp-pixeldata-cropped.bmp",
        "test_corrupt/badbitcount.bmp",
        "test_corrupt/badbitssize.bmp",
        "test_corrupt/baddens1.bmp",
        "test_corrupt/baddens2.bmp",
        "test_corrupt/badfilesize.bmp",
        "test_corrupt/badheadersize.bmp",
        "test_corrupt/badpalettesize.bmp",
        "test_corrupt/badplanes.bmp",
        "test_corrupt/badrle.bmp",
        "test_corrupt/badrle4.bmp",
        "test_corrupt/badrle4bis.bmp",
        "test_corrupt/badrle4ter.bmp",
        "test_corrupt/badrlebis.bmp",
        "test_corrupt/badrleter.bmp",
        "test_corrupt/badwidth.bmp",
        "test_corrupt/bitdepth-large.bmp",
        "test_corrupt/bitdepth-odd.bmp",
        "test_corrupt/bitdepth-zero.bmp",
        "test_corrupt/colormasks-cropped.bmp",
        "test_corrupt/colormasks-missing.bmp",
        "test_corrupt/compression-bad-rle4-for-8bpp.bmp",
        "test_corrupt/compression-bad-rle8-for-4bpp.bmp",
        "test_corrupt/compression-unknown.bmp",
        "test_corrupt/emptyfile.bmp",
        "test_corrupt/fileinfoheader-cropped.bmp",
        "test_corrupt/height-zero.bmp",
        "test_corrupt/infoheader-cropped.bmp",
        "test_corrupt/infoheader-missing.bmp",
        "test_corrupt/infoheadersize-small.bmp",
        "test_corrupt/infoheadersize-zero.bmp",
        "test_corrupt/magicnumber-bad.bmp",
        "test_corrupt/magicnumber-cropped.bmp",
        "test_corrupt/offbits-large.bmp",
        "test_corrupt/offbits-negative.bmp",
        "test_corrupt/offbits-zero.bmp",
        "test_corrupt/pal8badindex.bmp",
        "test_corrupt/palette-cropped.bmp",
        "test_corrupt/palette-missing.bmp",
        "test_corrupt/palette-too-big.bmp",
        "test_corrupt/pixeldata-missing.bmp",
        "test_corrupt/reallybig.bmp",
        "test_corrupt/rgb16-880.bmp",
        "test_corrupt/rle4-absolute-cropped.bmp",
        "test_corrupt/rle4-delta-cropped.bmp",
        "test_corrupt/rle4-no-end-of-line-marker.bmp",
        "test_corrupt/rle4-runlength-cropped.bmp",
        "test_corrupt/rle8-absolute-cropped.bmp",
        "test_corrupt/rle8-delta-cropped.bmp",
        "test_corrupt/rle8-deltaleavesimage.bmp",
        "test_corrupt/rle8-no-end-of-line-marker.bmp",
        "test_corrupt/rle8-runlength-cropped.bmp",
        "test_corrupt/rletopdown.bmp",
        "test_corrupt/shortfile.bmp",
        "test_corrupt/width-negative.bmp",
        "test_corrupt/width-times-height-overflow.bmp",
        "test_corrupt/width-zero.bmp",
        //"test_corrupt/infoheadersize-large.bmp", // Crashes
        // Questionable (we should be able to read all these)
        "test_questionable/8bpp-pixels-not-in-palette.bmp",
        "test_questionable/filesize-bad.bmp",
        "test_questionable/filesize-zero.bmp",
        "test_questionable/pal2.bmp",
        "test_questionable/pal2color.bmp",
        "test_questionable/pal4rlecut.bmp",
        "test_questionable/pal4rletrns.bmp",
        "test_questionable/pal8offs.bmp",
        "test_questionable/pal8oversizepal.bmp",
        "test_questionable/pal8rlecut.bmp",
        "test_questionable/pal8rletrns.bmp",
        "test_questionable/pels-per-meter-x-large.bmp",
        "test_questionable/pels-per-meter-x-negative.bmp",
        "test_questionable/pels-per-meter-x-zero.bmp",
        "test_questionable/pels-per-meter-y-large.bmp",
        "test_questionable/pels-per-meter-y-negative.bmp",
        "test_questionable/pels-per-meter-y-zero.bmp",
        "test_questionable/pixeldata-rle8-toomuch.bmp",
        "test_questionable/pixeldata-toomuch.bmp",
        "test_questionable/planes-large.bmp",
        "test_questionable/planes-zero.bmp",
        "test_questionable/reserved1-bad.bmp",
        "test_questionable/reserved2-bad.bmp",
        "test_questionable/rgb16-231.bmp",
        "test_questionable/rgb16-3103.bmp",
        "test_questionable/rgb16faketrns.bmp",
        "test_questionable/rgb24largepal.bmp",
        "test_questionable/rgb24lprof.bmp",
        "test_questionable/rgb24prof.bmp",
        "test_questionable/rgb24prof2.bmp",
        "test_questionable/rgb32-111110.bmp",
        "test_questionable/rgb32-7187.bmp",
        "test_questionable/rgb32-xbgr.bmp",
        "test_questionable/rgb32fakealpha.bmp",
        "test_questionable/rgb32h52.bmp",
        "test_questionable/rgba16-1924.bmp",
        "test_questionable/rgba16-4444.bmp",
        "test_questionable/rgba16-5551.bmp",
        "test_questionable/rgba32-1010102.bmp",
        "test_questionable/rgba32-61754.bmp",
        "test_questionable/rgba32-81284.bmp",
        "test_questionable/rgba32.bmp",
        "test_questionable/rgba32abf.bmp",
        "test_questionable/rgba32h56.bmp",
        "test_questionable/rle4-height-negative.bmp",
        "test_questionable/rle4-no-end-of-bitmap-marker.bmp",
        "test_questionable/rle8-height-negative.bmp",
        "test_questionable/rle8-no-end-of-bitmap-marker.bmp",
        // Valid images
        "test_valid/1bpp-1x1.bmp",
        "test_valid/1bpp-320x240-color.bmp",
        "test_valid/1bpp-320x240-overlappingcolor.bmp",
        "test_valid/1bpp-320x240.bmp",
        "test_valid/1bpp-321x240.bmp",
        "test_valid/1bpp-322x240.bmp",
        "test_valid/1bpp-323x240.bmp",
        "test_valid/1bpp-324x240.bmp",
        "test_valid/1bpp-325x240.bmp",
        "test_valid/1bpp-326x240.bmp",
        "test_valid/1bpp-327x240.bmp",
        "test_valid/1bpp-328x240.bmp",
        "test_valid/1bpp-329x240.bmp",
        "test_valid/1bpp-330x240.bmp",
        "test_valid/1bpp-331x240.bmp",
        "test_valid/1bpp-332x240.bmp",
        "test_valid/1bpp-333x240.bmp",
        "test_valid/1bpp-334x240.bmp",
        "test_valid/1bpp-335x240.bmp",
        "test_valid/1bpp-topdown-320x240.bmp",
        "test_valid/24bpp-1x1.bmp",
        "test_valid/24bpp-320x240.bmp",
        "test_valid/24bpp-321x240.bmp",
        "test_valid/24bpp-322x240.bmp",
        "test_valid/24bpp-323x240.bmp",
        "test_valid/24bpp-imagesize-zero.bmp",
        "test_valid/24bpp-topdown-320x240.bmp",
        "test_valid/32bpp-101110-320x240.bmp",
        "test_valid/32bpp-1x1.bmp",
        "test_valid/32bpp-320x240.bmp",
        "test_valid/32bpp-888-optimalpalette-320x240.bmp",
        "test_valid/32bpp-optimalpalette-320x240.bmp",
        "test_valid/32bpp-topdown-320x240.bmp",
        "test_valid/4bpp-1x1.bmp",
        "test_valid/4bpp-320x240.bmp",
        "test_valid/4bpp-321x240.bmp",
        "test_valid/4bpp-322x240.bmp",
        "test_valid/4bpp-323x240.bmp",
        "test_valid/4bpp-324x240.bmp",
        "test_valid/4bpp-325x240.bmp",
        "test_valid/4bpp-326x240.bmp",
        "test_valid/4bpp-327x240.bmp",
        "test_valid/4bpp-topdown-320x240.bmp",
        "test_valid/555-1x1.bmp",
        "test_valid/555-320x240.bmp",
        "test_valid/555-321x240.bmp",
        "test_valid/565-1x1.bmp",
        "test_valid/565-320x240-topdown.bmp",
        "test_valid/565-320x240.bmp",
        "test_valid/565-321x240-topdown.bmp",
        "test_valid/565-321x240.bmp",
        "test_valid/565-322x240-topdown.bmp",
        "test_valid/565-322x240.bmp",
        "test_valid/8bpp-1x1.bmp",
        "test_valid/8bpp-1x64000.bmp",
        "test_valid/8bpp-320x240.bmp",
        "test_valid/8bpp-321x240.bmp",
        "test_valid/8bpp-322x240.bmp",
        "test_valid/8bpp-323x240.bmp",
        "test_valid/8bpp-colorsimportant-two.bmp",
        "test_valid/8bpp-colorsused-zero.bmp",
        "test_valid/8bpp-topdown-320x240.bmp",
        "test_valid/misleadingextension.jpg",
        "test_valid/nofileextension",
        "test_valid/pal1.bmp",
        "test_valid/pal1bg.bmp",
        "test_valid/pal1wb.bmp",
        "test_valid/pal4.bmp",
        "test_valid/pal4gs.bmp",
        "test_valid/pal4rle.bmp",
        "test_valid/pal8-0.bmp",
        "test_valid/pal8.bmp",
        "test_valid/pal8gs.bmp",
        "test_valid/pal8nonsquare.bmp",
        "test_valid/pal8rle.bmp",
        "test_valid/pal8topdown.bmp",
        "test_valid/pal8v4.bmp",
        "test_valid/pal8v5.bmp",
        "test_valid/pal8w124.bmp",
        "test_valid/pal8w125.bmp",
        "test_valid/pal8w126.bmp",
        "test_valid/rgb16-565.bmp",
        "test_valid/rgb16-565pal.bmp",
        "test_valid/rgb16.bmp",
        "test_valid/rgb16bfdef.bmp",
        "test_valid/rgb24.bmp",
        "test_valid/rgb24pal.bmp",
        "test_valid/rgb32.bmp",
        "test_valid/rgb32bf.bmp",
        "test_valid/rgb32bfdef.bmp",
        "test_valid/rle4-absolute-320x240.bmp",
        "test_valid/rle4-alternate-320x240.bmp",
        "test_valid/rle4-delta-320x240.bmp",
        "test_valid/rle4-encoded-320x240.bmp",
        "test_valid/rle8-64000x1.bmp",
        "test_valid/rle8-absolute-320x240.bmp",
        "test_valid/rle8-blank-160x120.bmp",
        "test_valid/rle8-delta-320x240.bmp",
        "test_valid/rle8-encoded-320x240.bmp",
        "test_valid/spaces in  filename.bmp"
    };
    
    
    FILE* invalid = fopen("invalid.bmp", "rb");
    s32 iwidth, iheight;
    u08* invalid_template = bitmap_load(invalid, &iwidth, &iheight);
    
    char outputFile[500];
    u16 imageCount = sizeof(images) / sizeof(char*);
    for (u16 i = 0; i < imageCount; i++) {
        strcpy(outputFile, "");
        strcat(outputFile, "test_output/");
        strcat(outputFile,  strstr(images[i], "/") + 1);
        
        printf("\nImage: %s\n", images[i]);
        
        // I/O Files
        FILE* input = fopen(images[i], "rb");
        
        // Load the pixel data from the bitmap
        s32 width, height;
        u08* data = bitmap_load(input, &width, &height);
        
        FILE* output = fopen(outputFile, "wb");
        
        // Save the bitmap to disk
        if (data == NULL)
            bitmap_save(output, iwidth, iheight, invalid_template);
        else
            bitmap_save(output, width, height, data);
        
        fclose(input);
        fclose(output);
        free(data);
    }
    
    exit(0);
}

int
main(int argc, char** argv) {
    //gif_test();
    bitmap_test();
    
    exit(0);
}