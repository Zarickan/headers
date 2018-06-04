#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "types.h"

#ifndef GIF_H
#define GIF_H

#pragma pack(push, 1)

// TODO: Move to color.h
typedef struct Rgb {
    u08 Red;
    u08 Green;
    u08 Blue;
} Rgb;

typedef struct GifHeader {
    char Signature[3];
    char Version[3];
} GifHeader;

typedef struct ScreenDescriptor {
    u16 Width;
    u16 Height;
    union {
        u08 Value;
        struct {
            u08 MapSize : 3;
            u08 Sort : 1;
            u08 Bpp : 3;
            u08 GlobalMap : 1;
        };
    } Info; // TODO: Better name?
    u08 Background;
    u08 ColorMap;
} ScreenDescriptor;

typedef struct ImageDescriptor {
    char Seperator;
    u16 Left;
    u16 Top;
    u16 Width;
    u16 Height;
    
    union {
        u08 Value;
        struct {
            u08 MapSize : 3;
            u08 Reserved1 : 1;
            u08 Reserved2 : 1;
            u08 Sort : 1;
            u08 Interlace : 1;
            u08 LocalMap : 1;
        };
    } Info;
} ImageDescriptor;

#pragma pack(pop)

static inline u08*
gif_load(FILE* file, s32* width, s32* height) {
    GifHeader header;
    s32 headerRead = fread(&header, sizeof(GifHeader), 1, file);
    if (!headerRead) return NULL;
    
    printf("GifHeader:\n");
    printf("  Signature: %3s\n", header.Signature);
    printf("  Version:   %3s\n", header.Version);
    
    ScreenDescriptor screen;
    s32 screenRead = fread(&screen, sizeof(ScreenDescriptor), 1, file);
    if (!screenRead) return NULL;
    
    printf("  Screen Descriptor:\n");
    printf("    Width:       0x%4x (%3u)\n", screen.Width, screen.Width);
    printf("    Height:      0x%4x (%3u)\n", screen.Height, screen.Height);
    printf("    Info:        0x%2x\n", screen.Info.Value);
    printf("      MapSize:   0x%1x (M)\n", screen.Info.MapSize);
    printf("      Sort:      0x%1x\n", screen.Info.Sort);
    printf("      Bpp:       0x%1x (cr)\n", screen.Info.Bpp);
    printf("      GlobalMap: 0x%1x (pixel)\n", screen.Info.GlobalMap);
    printf("    Background:  0x%2x\n", screen.Background);
    printf("    Map:         0x%2x\n", screen.ColorMap);
    
    // NOTE: Read the colormap
    u08 bpp = screen.Info.MapSize + 1;
    u16 colorCount = pow(2, bpp) * screen.Info.GlobalMap;
    Rgb* globalColorMap = malloc(sizeof(Rgb) * colorCount);
    s32 colorsRead = fread(globalColorMap, sizeof(Rgb), colorCount, file);
    if (colorsRead != colorCount) return NULL;
    
    printf("  Global color map:\n", colorCount, bpp);
    printf("    Colors: %i\n", colorCount);
    
    // NOTE: Resulting image data
    *width = screen.Width;
    *height = screen.Height;
    u64 resultSize = *width * *height;
    // TODO: Use something else than RgbQuad, or move RgbQuad oput of bitmap.h
    u08* resultData = malloc(resultSize * sizeof(RgbQuad));
    RgbQuad* resultRgb = (RgbQuad*) resultData;
    
    // NOTE: Set all colors in the output to the background color
    RgbQuad* result = resultRgb;
    for (s16 x = 0; x < *width; x++) {
        for (s16 y = 0; y < *height; y++) {
            // TODO: Refactor out function to get color from palette (to account for invalid indexes)
            Rgb* color = globalColorMap + screen.Background;
            result->Red = color->Red;
            result->Green = color->Green;
            result->Blue = color->Blue;
            result->Alpha = 0x00;
            result++;
        }
    }
    
    // NOTE: Read all extension descriptors until we encounter image descriptors
    // TODO: We currently ignore extension, properly read and use them instead
    char seperator;
    do {
        s32 seperatorRead = fread(&seperator, sizeof(char), 1, file);
        if (!seperatorRead) return NULL;
        fseek(file, -1, SEEK_CUR);
        
        if (seperator != '!') continue;
        
        u16 functionCode;
        fread(&functionCode, sizeof(u16), 1, file);
        printf("  Extension descriptor:\n");
        printf("    Function code: 0x%2x\n", functionCode);
        
        u08 blockLength;
        fread(&blockLength, sizeof(u08), 1, file);
        while (blockLength) {
            fseek(file, blockLength, SEEK_CUR); // NOTE: Skip past the data
            fread(&blockLength, sizeof(u08), 1, file);
        }
        
    } while (seperator == '!');
    
    // NOTE: Read image descriptors until end of file
    do {
        s32 seperatorRead = fread(&seperator, sizeof(char), 1, file);
        if (!seperatorRead) return NULL;
        fseek(file, -1, SEEK_CUR);
        
        ImageDescriptor image;
        s32 imageRead = fread(&image, sizeof(ImageDescriptor), 1, file);
        if (!imageRead) return NULL;
        
        printf("  Image Descriptor:\n");
        printf("    Seperator:   %c\n", image.Seperator);
        printf("    Left:        0x%2x (%3u)\n", image.Left, image.Left);
        printf("    Top:         0x%2x (%3u)\n", image.Top, image.Top);
        printf("    Width:       0x%2x (%3u)\n", image.Width, image.Width);
        printf("    Height:      0x%2x (%3u)\n", image.Height, image.Height);
        printf("    Info:        0x%2x\n", image.Info.Value);
        printf("      MapSize:   0x%2x (pixel)\n", image.Info.MapSize);
        printf("      Reserved1: 0x%2x\n", image.Info.Reserved1);
        printf("      Reserved2: 0x%2x\n", image.Info.Reserved2);
        printf("      Sort:      0x%2x\n", image.Info.Sort);
        printf("      Interlace: 0x%2x (I)\n", image.Info.Interlace);
        printf("      LocalMap:  0x%2x (M)\n", image.Info.LocalMap);
        
        // NOTE: Read local color map
        u08 localBpp = image.Info.MapSize + 1;
        u16 localColorCount = pow(2, localBpp) * image.Info.LocalMap;
        Rgb* localColorMap = malloc(sizeof(Rgb) * localColorCount);
        colorsRead = fread(localColorMap, sizeof(Rgb), localColorCount, file);
        if (colorsRead != localColorCount) return NULL;
        
        printf("  Local color map:\n");
        printf("    Colors: %i\n", localColorCount);
        
        // NOTE: Read raster data
        u08 minimumCodeSize;
        if (!fread(&minimumCodeSize, sizeof(u08), 1, file)) return NULL;
        u08 lzwLength;
        if (!fread(&lzwLength, sizeof(u08), 1, file)) return NULL;
        
        printf("Minimum code size: %i\n", minimumCodeSize);
        printf("LZW bytes: %i\n", lzwLength);
        
        fseek(file, lzwLength, SEEK_CUR);
        fseek(file, 1, SEEK_CUR);
        
    } while (seperator != 0x3B);
    
    return resultData;
}

static inline void
gif_save(FILE* file, s32 width, s32 height, const u08* data) {
}

#endif // GIF_H