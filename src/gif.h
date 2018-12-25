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

const u32 powers[17] = {
    0x000001 << 0,
    0x000001 << 1,
    0x000001 << 2,
    0x000001 << 3,
    0x000001 << 4,
    0x000001 << 5,
    0x000001 << 6,
    0x000001 << 7,
    0x000001 << 8,
    0x000001 << 9,
    0x000001 << 10,
    0x000001 << 11,
    0x000001 << 12,
    0x000001 << 13,
    0x000001 << 14,
    0x000001 << 15,
    0x000001 << 16,
};

static inline u32
readbits(u08** data, u08 bits, u08* bitPosition) {
    u32 result = 0;
    
    for (s16 i = 0; i < bits; i++) {
        result |= (powers[*bitPosition] & **data) >> *bitPosition;
        result <<= 1;
        
        if (*bitPosition + 1 > 7) {
            *bitPosition = 0;
            (*data)++;
        }
        else {
            (*bitPosition)++;
        }
    }
    
    return result;
}

#define CODECOUNT 4096
static inline void
gif_decompress(u08* data, s16  length, s16 codeSize, RgbQuad* output) {
    u08* start = data;
    u08 currentBit = 0;
}

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
    
    printf("  Global color map:\n");
    printf("    Colors: %i\n", colorCount);
    /*
    for(u16 c = 0; c < colorCount; c++) {
        Rgb* color = globalColorMap + c;
        printf("    %3i: %2x%2x%2x\n", c, color->Red, color->Green, color->Blue);
    }
    */
    
    // NOTE: Resulting image data
    *width = screen.Width;
    *height = screen.Height;
    u64 resultSize = *width * *height;
    // TODO: Use something else than RgbQuad, or move RgbQuad out of bitmap.h
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
        
        do {
            fseek(file, blockLength, SEEK_CUR);
            fread(&blockLength, sizeof(u08), 1, file);
        } while (blockLength);
        
    } while (seperator == '!');
    
    // NOTE: Read image descriptors until end of file
    do {
        s32 seperatorRead = fread(&seperator, sizeof(char), 1, file);
        if (!seperatorRead) return NULL;
        fseek(file, -1, SEEK_CUR);
        if (seperator != ',') continue;
        
        s64 location = ftell(file);
        printf("----Location 0x%llx\n", location);
        
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
        /*
        for(u16 c = 0; c < localColorCount; c++) {
            Rgb* color = localColorMap + c;
            printf("    %3i: %2x%2x%2x\n", c, color->Red, color->Green, color->Blue);
        }
        */
        
        // NOTE: Allocate raw pixel data (where we store the decompressed data)
        u08* pixelData = malloc(image.Width * image.Height);
        
        // NOTE: Read raster data
        u08 minimumCodeSize, codeSize;
        if (!fread(&minimumCodeSize, sizeof(u08), 1, file)) return NULL;
        codeSize = minimumCodeSize;
        printf("Minimum code size: %i\n", minimumCodeSize);
        printf("Code size: %i\n", codeSize);
        
        u08 lzwLength;
        fread(&lzwLength, sizeof(u08), 1, file);
        
        printf("Size: %u\n", image.Width * image.Height);
        printf("Len : %u\n", lzwLength);
        
        
        //gif_decompress(data, lzwLength, minimumCodeSize, resultRgb);
        u08 buffer[256];
        do {
            fread(&buffer, sizeof(u08), lzwLength, file);
            fread(&lzwLength, sizeof(u08), 1, file);
        } while (lzwLength);
        
        // Turn palette indexes into raw color data in the output
        u32 dataIndex = 0;
        for(u32 top = image.Top; top < image.Top + image.Height; top++) {
            for(u32 left = image.Left; left < image.Left + image.Width; left++) {
                u32 index = top * screen.Width + left;
                Rgb* color = globalColorMap + pixelData[dataIndex];
                
                resultRgb[index].Red = color->Red;
                resultRgb[index].Green = color->Green;
                resultRgb[index].Blue = color->Blue;
                resultRgb[index].Alpha = 0xFF;
                
                dataIndex++;
            }
        }
        
        free(pixelData);
        free(localColorMap);
    } while (seperator == 0x3B);
    
    free(globalColorMap);
    
    return resultData;
}

static inline void
gif_save(FILE* file, s32 width, s32 height, const u08* data) {
}

#endif // GIF_H