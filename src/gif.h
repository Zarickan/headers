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
            u08 MapSize   : 3;
            u08 Sort      : 1;
            u08 Bpp       : 3;
            u08 GlobalMap : 1;
        };
    } Info; // TODO: Better name?
    u08 Background;
    u08 ColorMap;
} ScreenDescriptor;

typedef struct ImageDescriptor {
    u16 Left;
    u16 Top;
    u16 Width;
    u16 Height;
    
    union {
        u08 Value;
        struct {
            u08 MapSize   : 3;
            u08 Reserved1 : 1;
            u08 Reserved2 : 1;
            u08 Sort      : 1;
            u08 Interlace : 1;
            u08 LocalMap  : 1;
        };
    } Info;
} ImageDescriptor;


typedef struct LzwBlock {
    u08 MinimumCodeSize;
    u08 CodeSize;
    
    u08 Length;
    u08 *Data;
} LzwBlock;

#pragma pack(pop)

// LZW Decoding

typedef struct LzwTable {
    u16 Code;
    
    u16 Length;
    u08 *Data;
    
    struct LzwTable *Previous;
    struct LzwTable *Next;
} LzwTable;


static inline LzwTable *
lzw_table_find(LzwTable *table, u16 code) {
    LzwTable *current = table;
    while(current != NULL) {
        if(current->Code < code) {
            current = current->Next;
        }
        else if(current->Code > code){
            current = current->Previous;
        }
        else {
            return current;
        }
    }
    
    return NULL;
}

static inline LzwTable *
lzw_table_insert_next(LzwTable *table, u16 dataLength, u08* data) {
    LzwTable *tail = table;
    while(tail->Next != NULL) tail = tail->Next;
    
    LzwTable *newLzwTable = malloc(sizeof(LzwTable));
    
    newLzwTable->Code = tail->Code + 1;
    newLzwTable->Length = dataLength;
    newLzwTable->Data = data;
    
    newLzwTable->Previous = tail;
    newLzwTable->Next = NULL;
    
    return newLzwTable;
}

static inline LzwTable *
lzw_table_make(u16 colorCount) {
    u16 codeCount = colorCount + 2;
    LzwTable *codes = malloc(sizeof(LzwTable) * codeCount);
    
    LzwTable *previous = NULL;
    for(u32 i = 0; i < codeCount; ++i) {
        LzwTable *current = codes + i;
        
        current->Code = i;
        if(i < colorCount) {
            current->Length = 1;
            current->Data = malloc(sizeof(u08));
            current->Data[0] = i;
        } else {
            current->Length = 0;
        }
        
        current->Previous = previous;
        current->Next = NULL;
        
        if(previous != NULL) {
            previous->Next = current;
        }
        
        previous = current;
    }
    
    return previous;
}

static inline void
lzw_table_print_value(LzwTable *code) {
    printf("%3x: ", code->Code);
    for(u16 i = 0; i < code->Length; ++i) {
        printf("%2x ", code->Data[i]);
    }
    printf("\n");
}

const u32 PowersOfTwo[17] = {
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

static inline u08
bits_in_code(u16 code) {
    u08 counter = 0;
    while(code != 0) {
        code >>= 1;
        ++counter;
    }
    
    return counter;
}

static inline u16
readbits(u08 **data, u08 bits, u08 *bitPosition) {
    u16 result = 0;
    
    for (s16 i = 0; i < bits; i++) {
        u08 bitMask = PowersOfTwo[*bitPosition];
        u08 value = **data & bitMask;
        
        if(value)
            result |= PowersOfTwo[i];
        
        
        if (*bitPosition + 1 > 7) {
            *bitPosition = 0;
            (*data)++;
        }
        else
            (*bitPosition)++;
    }
    
    return result;
}

static inline s32
read_lzw_block(FILE *file, LzwBlock *block) {
    u08 bytesRead = fread(&block->MinimumCodeSize, sizeof(u08), 1, file);
    if(!bytesRead) return 0;
    block->CodeSize = block->MinimumCodeSize;
    
    bytesRead = fread(&block->Length, sizeof(u08), 1, file);
    if(!bytesRead) return 0;
    
    u08* data = malloc(sizeof(u08) * block->Length);
    bytesRead = fread(data, sizeof(u08), block->Length, file);
    if(bytesRead != block->Length) return 0;
    block->Data = data;
    
    return 1;
}

static inline void
decode_lzw_block(LzwBlock *block, Rgb *palette, u16 paletteSize, RgbQuad **output) {
    LzwTable *table = lzw_table_make(paletteSize);
    block->CodeSize = bits_in_code(table->Code);
    u16 clearLzwTable = paletteSize, endLzwTable = paletteSize + 1;
    
    u08 *data = block->Data;
    u08 bitPosition = 0;
    
    LzwTable *localCode = NULL;
    for(;;) {
        u16 value = readbits(&data, block->CodeSize, &bitPosition);
        LzwTable *current = lzw_table_find(table, value);
        
        // NOTE: The code does not exist in the table
        if(current == NULL) {
            u16 dataSize = 1 + localCode->Length;
            u08* data = malloc(sizeof(u08) * dataSize);
            memcpy(data, localCode->Data, localCode->Length);
            data[dataSize - 1] = localCode->Data[0];
            
            localCode = table = lzw_table_insert_next(table, dataSize, data);
            block->CodeSize = MAX(block->MinimumCodeSize, bits_in_code(table->Code));
            
            for(u16 i = 0; i < table->Length; ++i) {
                Rgb tableEntry = palette[table->Data[i]];
                RgbQuad rgb = {
                    .Red = tableEntry.Red,
                    .Green = tableEntry.Green,
                    .Blue = tableEntry.Blue,
                    .Alpha = 0xFF
                };
                
                **output = rgb;
                ++(*output);
            }
        }
        // NOTE: LzwTable doest exist in table
        else {
            if(current->Code == endLzwTable) break;
            if(current->Code == clearLzwTable) {
                table = lzw_table_make(paletteSize);
                block->CodeSize = bits_in_code(table->Code);
                
                localCode = NULL;
                continue;
            }
            
            for(u16 i = 0; i < current->Length; ++i) {
                Rgb currentEntry = palette[current->Data[i]];
                RgbQuad rgb = {
                    .Red = currentEntry.Red,
                    .Green = currentEntry.Green,
                    .Blue = currentEntry.Blue,
                    .Alpha = 0xFF
                };
                
                **output = rgb;
                ++(*output);
            }
            
            // Non-special codes
            if(localCode != NULL) {
                u16 dataSize = 1 + localCode->Length;
                u08* data = malloc(sizeof(u08) * dataSize);
                memcpy(data, localCode->Data, localCode->Length);
                data[dataSize - 1] = current->Data[0];
                
                table = lzw_table_insert_next(table, dataSize, data);
                block->CodeSize = MAX(block->MinimumCodeSize, bits_in_code(table->Code));
            }
            
            localCode = current;
        }
    }
    
    // Free memory used by the table
    for(;;) {
        free(localCode->Data);
        
        if(localCode->Previous == NULL) break;
        localCode = localCode->Previous;
    }
    free(localCode);
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
    Rgb *globalColorMap = NULL;
    u16 globalColorMapSize = 0;
    
    if(screen.Info.GlobalMap) {
        // TODO: Use the BPP to figure out how much data to read (this won't work if BPP is not 8)
        u08 bpp = screen.Info.Bpp + 1;
        globalColorMapSize = pow(2, screen.Info.MapSize + 1);
        
        globalColorMap = malloc(sizeof(Rgb) * globalColorMapSize);
        s32 colorsRead = fread(globalColorMap, sizeof(Rgb), globalColorMapSize, file);
        if (colorsRead != globalColorMapSize) return NULL;
        
        printf("  Global color map:\n");
        printf("    Colors: %i\n", globalColorMapSize);
        for(u16 c = 0; c < globalColorMapSize; c++) {
            Rgb* color = globalColorMap + c;
            //printf("    %3i: %2x%2x%2x\n", c, color->Red, color->Green, color->Blue);
        }
    }
    else
        printf("    No Global color map\n");
    
    // NOTE: Resulting image data
    *width = screen.Width; 
    *height = screen.Height;
    u64 resultSize = screen.Width * screen.Height;
    
    // TODO: Use something else than RgbQuad, or move RgbQuad out of bitmap.h
    u08* resultData = malloc(resultSize * sizeof(RgbQuad));
    RgbQuad* resultRgb = (RgbQuad*) resultData;
    
    // NOTE: Set all colors in the output to the background color
    RgbQuad* result = resultRgb;
    for (s16 x = 0; x < screen.Width; x++) {
        for (s16 y = 0; y < screen.Height; y++) {
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
        // Read the separator, skip if the separator != !
        fread(&seperator, sizeof(char), 1, file);
        if(seperator != '!') {
            // NOTE: If the separator is != '!' then it should be 0x00
            //       (no more extensions follow)
            //       Hence we do not need to seek back 1 byte
            continue;
        }
        
        u08 functionCode;
        u08 blockLength;
        fread(&functionCode, sizeof(u08), 1, file);
        fread(&blockLength, sizeof(u08), 1, file);
        fseek(file, blockLength, SEEK_CUR);
        
        printf("  Extension descriptor:\n");
        printf("    Function code:    0x%2x\n", functionCode);
        printf("    Extension length: %u\n", blockLength);
    } while (seperator == '!');
    
    // NOTE: Read image descriptors until end of file
    do {
        // Read the separator, skip if the separator != ,
        // If it is, we seek back so we can read the full descriptor
        fread(&seperator, sizeof(char), 1, file);
        if(seperator != ',') {
            continue;
        }
        
        ImageDescriptor image;
        s32 numberRead = fread(&image, sizeof(ImageDescriptor), 1, file);
        if (!numberRead) return NULL;
        printf("  Image Descriptor:\n");
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
        
        // NOTE: The local color map is only present if the info byte is != 0
        Rgb *colorMap = NULL;
        if (image.Info.Value != 0x00) {
            u16 colorCount = pow(2, image.Info.MapSize + 1) * image.Info.LocalMap;
            colorMap = malloc(sizeof(Rgb) * colorCount);
            
            numberRead = fread(colorMap, sizeof(Rgb), colorCount, file);
            if (numberRead != colorCount) return NULL;
        }
        else
            printf("    No local color map\n");
        
        // NOTE: Read LZW data
        // TODO: Check return value
        LzwBlock lzw;
        read_lzw_block(file, &lzw);
        printf("    LzwBlock:\n");
        printf("      MinimumCodeSize: %i\n", lzw.MinimumCodeSize);
        printf("      CodeSize:        %u\n", lzw.CodeSize);
        printf("      Length:          %u\n", lzw.Length);
        
        decode_lzw_block(&lzw, globalColorMap, globalColorMapSize, &resultRgb);
        
        // TODO: Handle when more LZW blocks follow
        // NOTE: The final byte after the LZW data should be 0x00
        u08 eoi;
        fread(&eoi, sizeof(u08), 1, file);
        printf("  EOI: %x\n", eoi);
        
        // TODO: Free what we allocate
    } while (seperator == 0x3B);
    
    u08 final = 0;
    fread(&final, sizeof(u08), 1, file);
    printf("EOF %x\n", final);
    
    return resultData;
}

static inline void
gif_save(FILE* file, s32 width, s32 height, const u08* data) {
}

#endif // GIF_H