#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "bitmap.h"
#include "gif.h"

#pragma warning(disable:4996)

void
gif_test(void) {
    char* images[] = {
        //"gif/valid/sample.gif",
        //"gif/valid/small.gif",
        //"gif/valid/small2.gif",
        //"gif/valid/uncompressed.gif",
		"gif/valid/sunflower.gif",
		/*
"gif/valid/morph.gif",
"gif/valid/newton.gif",
"gif/valid/oil.gif",
"gif/valid/fullcolor.gif",
"gif/valid/quilt.gif",
"gif/valid/globe.gif",
*/
    };
    
    char outputFile[500];
    u16 imageCount = sizeof(images) / sizeof(char*);
    for (u16 i = 0; i < imageCount; i++) {
        strncpy(outputFile, "", 500);
        strncat(outputFile, "gif/output/", 500);
        strncat(outputFile,  strstr(strstr(images[i], "/") + 1, "/") + 1, 500);
        strncat(outputFile, ".bmp", 500);
        
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
        "bitmap/corrupt/infoheadersize-large.bmp",
        "bitmap/corrupt/1bpp-no-palette.bmp",
        "bitmap/corrupt/1bpp-pixeldata-cropped.bmp",
        "bitmap/corrupt/24bpp-pixeldata-cropped.bmp",
        "bitmap/corrupt/32bpp-pixeldata-cropped.bmp",
        "bitmap/corrupt/4bpp-no-palette.bmp",
        "bitmap/corrupt/4bpp-pixeldata-cropped.bmp",
        "bitmap/corrupt/555-pixeldata-cropped.bmp",
        "bitmap/corrupt/8bpp-colorsimportant-large.bmp",
        "bitmap/corrupt/8bpp-colorsimportant-negative.bmp",
        "bitmap/corrupt/8bpp-colorsused-large.bmp",
        "bitmap/corrupt/8bpp-colorsused-negative.bmp",
        "bitmap/corrupt/8bpp-no-palette.bmp",
        "bitmap/corrupt/8bpp-pixeldata-cropped.bmp",
        "bitmap/corrupt/badbitcount.bmp",
        "bitmap/corrupt/badbitssize.bmp",
        "bitmap/corrupt/baddens1.bmp",
        "bitmap/corrupt/baddens2.bmp",
        "bitmap/corrupt/badfilesize.bmp",
        "bitmap/corrupt/badheadersize.bmp",
        "bitmap/corrupt/badpalettesize.bmp",
        "bitmap/corrupt/badplanes.bmp",
        "bitmap/corrupt/badrle.bmp",
        "bitmap/corrupt/badrle4.bmp",
        "bitmap/corrupt/badrle4bis.bmp",
        "bitmap/corrupt/badrle4ter.bmp",
        "bitmap/corrupt/badrlebis.bmp",
        "bitmap/corrupt/badrleter.bmp",
        "bitmap/corrupt/badwidth.bmp",
        "bitmap/corrupt/bitdepth-large.bmp",
        "bitmap/corrupt/bitdepth-odd.bmp",
        "bitmap/corrupt/bitdepth-zero.bmp",
        "bitmap/corrupt/colormasks-cropped.bmp",
        "bitmap/corrupt/colormasks-missing.bmp",
        "bitmap/corrupt/compression-bad-rle4-for-8bpp.bmp",
        "bitmap/corrupt/compression-bad-rle8-for-4bpp.bmp",
        "bitmap/corrupt/compression-unknown.bmp",
        "bitmap/corrupt/emptyfile.bmp",
        "bitmap/corrupt/fileinfoheader-cropped.bmp",
        "bitmap/corrupt/height-zero.bmp",
        "bitmap/corrupt/infoheader-cropped.bmp",
        "bitmap/corrupt/infoheader-missing.bmp",
        "bitmap/corrupt/infoheadersize-small.bmp",
        "bitmap/corrupt/infoheadersize-zero.bmp",
        "bitmap/corrupt/magicnumber-bad.bmp",
        "bitmap/corrupt/magicnumber-cropped.bmp",
        "bitmap/corrupt/offbits-large.bmp",
        "bitmap/corrupt/offbits-negative.bmp",
        "bitmap/corrupt/offbits-zero.bmp",
        "bitmap/corrupt/pal8badindex.bmp",
        "bitmap/corrupt/palette-cropped.bmp",
        "bitmap/corrupt/palette-missing.bmp",
        "bitmap/corrupt/palette-too-big.bmp",
        "bitmap/corrupt/pixeldata-missing.bmp",
        "bitmap/corrupt/reallybig.bmp",
        "bitmap/corrupt/rgb16-880.bmp",
        "bitmap/corrupt/rle4-absolute-cropped.bmp",
        "bitmap/corrupt/rle4-delta-cropped.bmp",
        "bitmap/corrupt/rle4-no-end-of-line-marker.bmp",
        "bitmap/corrupt/rle4-runlength-cropped.bmp",
        "bitmap/corrupt/rle8-absolute-cropped.bmp",
        "bitmap/corrupt/rle8-delta-cropped.bmp",
        "bitmap/corrupt/rle8-deltaleavesimage.bmp",
        "bitmap/corrupt/rle8-no-end-of-line-marker.bmp",
        "bitmap/corrupt/rle8-runlength-cropped.bmp",
        "bitmap/corrupt/rletopdown.bmp",
        "bitmap/corrupt/shortfile.bmp",
        "bitmap/corrupt/width-negative.bmp",
        "bitmap/corrupt/width-times-height-overflow.bmp",
        "bitmap/corrupt/width-zero.bmp",
        //"bitmap/corrupt/infoheadersize-large.bmp", // Crashes
        // Questionable (we should be able to read all these)
        "bitmap/questionable/8bpp-pixels-not-in-palette.bmp",
        "bitmap/questionable/filesize-bad.bmp",
        "bitmap/questionable/filesize-zero.bmp",
        "bitmap/questionable/pal2.bmp",
        "bitmap/questionable/pal2color.bmp",
        "bitmap/questionable/pal4rlecut.bmp",
        "bitmap/questionable/pal4rletrns.bmp",
        "bitmap/questionable/pal8offs.bmp",
        "bitmap/questionable/pal8oversizepal.bmp",
        "bitmap/questionable/pal8rlecut.bmp",
        "bitmap/questionable/pal8rletrns.bmp",
        "bitmap/questionable/pels-per-meter-x-large.bmp",
        "bitmap/questionable/pels-per-meter-x-negative.bmp",
        "bitmap/questionable/pels-per-meter-x-zero.bmp",
        "bitmap/questionable/pels-per-meter-y-large.bmp",
        "bitmap/questionable/pels-per-meter-y-negative.bmp",
        "bitmap/questionable/pels-per-meter-y-zero.bmp",
        "bitmap/questionable/pixeldata-rle8-toomuch.bmp",
        "bitmap/questionable/pixeldata-toomuch.bmp",
        "bitmap/questionable/planes-large.bmp",
        "bitmap/questionable/planes-zero.bmp",
        "bitmap/questionable/reserved1-bad.bmp",
        "bitmap/questionable/reserved2-bad.bmp",
        "bitmap/questionable/rgb16-231.bmp",
        "bitmap/questionable/rgb16-3103.bmp",
        "bitmap/questionable/rgb16faketrns.bmp",
        "bitmap/questionable/rgb24largepal.bmp",
        "bitmap/questionable/rgb24lprof.bmp",
        "bitmap/questionable/rgb24prof.bmp",
        "bitmap/questionable/rgb24prof2.bmp",
        "bitmap/questionable/rgb32-111110.bmp",
        "bitmap/questionable/rgb32-7187.bmp",
        "bitmap/questionable/rgb32-xbgr.bmp",
        "bitmap/questionable/rgb32fakealpha.bmp",
        "bitmap/questionable/rgb32h52.bmp",
        "bitmap/questionable/rgba16-1924.bmp",
        "bitmap/questionable/rgba16-4444.bmp",
        "bitmap/questionable/rgba16-5551.bmp",
        "bitmap/questionable/rgba32-1010102.bmp",
        "bitmap/questionable/rgba32-61754.bmp",
        "bitmap/questionable/rgba32-81284.bmp",
        "bitmap/questionable/rgba32.bmp",
        "bitmap/questionable/rgba32abf.bmp",
        "bitmap/questionable/rgba32h56.bmp",
        "bitmap/questionable/rle4-height-negative.bmp",
        "bitmap/questionable/rle4-no-end-of-bitmap-marker.bmp",
        "bitmap/questionable/rle8-height-negative.bmp",
        "bitmap/questionable/rle8-no-end-of-bitmap-marker.bmp",
        // Valid images
        "bitmap/valid/1bpp-1x1.bmp",
        "bitmap/valid/1bpp-320x240-color.bmp",
        "bitmap/valid/1bpp-320x240-overlappingcolor.bmp",
        "bitmap/valid/1bpp-320x240.bmp",
        "bitmap/valid/1bpp-321x240.bmp",
        "bitmap/valid/1bpp-322x240.bmp",
        "bitmap/valid/1bpp-323x240.bmp",
        "bitmap/valid/1bpp-324x240.bmp",
        "bitmap/valid/1bpp-325x240.bmp",
        "bitmap/valid/1bpp-326x240.bmp",
        "bitmap/valid/1bpp-327x240.bmp",
        "bitmap/valid/1bpp-328x240.bmp",
        "bitmap/valid/1bpp-329x240.bmp",
        "bitmap/valid/1bpp-330x240.bmp",
        "bitmap/valid/1bpp-331x240.bmp",
        "bitmap/valid/1bpp-332x240.bmp",
        "bitmap/valid/1bpp-333x240.bmp",
        "bitmap/valid/1bpp-334x240.bmp",
        "bitmap/valid/1bpp-335x240.bmp",
        "bitmap/valid/1bpp-topdown-320x240.bmp",
        "bitmap/valid/24bpp-1x1.bmp",
        "bitmap/valid/24bpp-320x240.bmp",
        "bitmap/valid/24bpp-321x240.bmp",
        "bitmap/valid/24bpp-322x240.bmp",
        "bitmap/valid/24bpp-323x240.bmp",
        "bitmap/valid/24bpp-imagesize-zero.bmp",
        "bitmap/valid/24bpp-topdown-320x240.bmp",
        "bitmap/valid/32bpp-101110-320x240.bmp",
        "bitmap/valid/32bpp-1x1.bmp",
        "bitmap/valid/32bpp-320x240.bmp",
        "bitmap/valid/32bpp-888-optimalpalette-320x240.bmp",
        "bitmap/valid/32bpp-optimalpalette-320x240.bmp",
        "bitmap/valid/32bpp-topdown-320x240.bmp",
        "bitmap/valid/4bpp-1x1.bmp",
        "bitmap/valid/4bpp-320x240.bmp",
        "bitmap/valid/4bpp-321x240.bmp",
        "bitmap/valid/4bpp-322x240.bmp",
        "bitmap/valid/4bpp-323x240.bmp",
        "bitmap/valid/4bpp-324x240.bmp",
        "bitmap/valid/4bpp-325x240.bmp",
        "bitmap/valid/4bpp-326x240.bmp",
        "bitmap/valid/4bpp-327x240.bmp",
        "bitmap/valid/4bpp-topdown-320x240.bmp",
        "bitmap/valid/555-1x1.bmp",
        "bitmap/valid/555-320x240.bmp",
        "bitmap/valid/555-321x240.bmp",
        "bitmap/valid/565-1x1.bmp",
        "bitmap/valid/565-320x240-topdown.bmp",
        "bitmap/valid/565-320x240.bmp",
        "bitmap/valid/565-321x240-topdown.bmp",
        "bitmap/valid/565-321x240.bmp",
        "bitmap/valid/565-322x240-topdown.bmp",
        "bitmap/valid/565-322x240.bmp",
        "bitmap/valid/8bpp-1x1.bmp",
        "bitmap/valid/8bpp-1x64000.bmp",
        "bitmap/valid/8bpp-320x240.bmp",
        "bitmap/valid/8bpp-321x240.bmp",
        "bitmap/valid/8bpp-322x240.bmp",
        "bitmap/valid/8bpp-323x240.bmp",
        "bitmap/valid/8bpp-colorsimportant-two.bmp",
        "bitmap/valid/8bpp-colorsused-zero.bmp",
        "bitmap/valid/8bpp-topdown-320x240.bmp",
        "bitmap/valid/misleadingextension.jpg",
        "bitmap/valid/nofileextension",
        "bitmap/valid/pal1.bmp",
        "bitmap/valid/pal1bg.bmp",
        "bitmap/valid/pal1wb.bmp",
        "bitmap/valid/pal4.bmp",
        "bitmap/valid/pal4gs.bmp",
        "bitmap/valid/pal4rle.bmp",
        "bitmap/valid/pal8-0.bmp",
        "bitmap/valid/pal8.bmp",
        "bitmap/valid/pal8gs.bmp",
        "bitmap/valid/pal8nonsquare.bmp",
        "bitmap/valid/pal8rle.bmp",
        "bitmap/valid/pal8topdown.bmp",
        "bitmap/valid/pal8v4.bmp",
        "bitmap/valid/pal8v5.bmp",
        "bitmap/valid/pal8w124.bmp",
        "bitmap/valid/pal8w125.bmp",
        "bitmap/valid/pal8w126.bmp",
        "bitmap/valid/rgb16-565.bmp",
        "bitmap/valid/rgb16-565pal.bmp",
        "bitmap/valid/rgb16.bmp",
        "bitmap/valid/rgb16bfdef.bmp",
        "bitmap/valid/rgb24.bmp",
        "bitmap/valid/rgb24pal.bmp",
        "bitmap/valid/rgb32.bmp",
        "bitmap/valid/rgb32bf.bmp",
        "bitmap/valid/rgb32bfdef.bmp",
        "bitmap/valid/rle4-absolute-320x240.bmp",
        "bitmap/valid/rle4-alternate-320x240.bmp",
        "bitmap/valid/rle4-delta-320x240.bmp",
        "bitmap/valid/rle4-encoded-320x240.bmp",
        "bitmap/valid/rle8-64000x1.bmp",
        "bitmap/valid/rle8-absolute-320x240.bmp",
        "bitmap/valid/rle8-blank-160x120.bmp",
        "bitmap/valid/rle8-delta-320x240.bmp",
        "bitmap/valid/rle8-encoded-320x240.bmp",
        "bitmap/valid/spaces in  filename.bmp"
    };
    
    FILE* invalid = fopen("bitmap/invalid.bmp", "rb");
    s32 iwidth, iheight;
    u08* invalid_template = bitmap_load(invalid, &iwidth, &iheight);
    
    char outputFile[500];
    u16 imageCount = sizeof(images) / sizeof(char*);
    for (u16 i = 0; i < imageCount; i++) {
        strcpy(outputFile, "");
        strcat(outputFile, "bitmap/output/");
        strcat(outputFile,  strstr(strstr(images[i], "/") + 1, "/") + 1);
        
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
    gif_test();
    //bitmap_test();
    
    exit(0);
}