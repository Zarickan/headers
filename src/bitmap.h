#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "types.h"
#include "stack.h"

#ifndef BITMAP_H
#define BITMAP_H

// Helpers
#define DPI(pixels_per_meter) ((u32) 39.37008 * (pixels_per_meter))
#define MAPRGBBYTE(v, wordSize) (u08) ((v) / (pow(2, (wordSize)) - 1) * 255)

// Bitmap versions
#define BITMAP_VUNKNOWN 0xFFFFFFFF // -1
#define BITMAP_VCORE    0x00
#define BITMAP_V0       BITMAP_VCORE
#define BITMAP_V1       0x01
#define BITMAP_V2       0x02
#define BITMAP_V3       0x03
#define BITMAP_V4       0x04
#define BITMAP_V5       0x05

// Bitmap compression modes
#define BI_RGB            0x00
#define BI_RLE8           0x01
#define BI_RLE4           0x02
#define BI_BITFIELDS      0x03
#define BI_JPEG           0x04
#define BI_PNG            0x05
#define BI_ALPHABITFIELDS 0x06
#define BI_CMYK           0x0B
#define BI_CYKRLE8        0x0C
#define BI_CYKRLE4        0x0D

// NOTE (Frederi): Since these represent data on disc we do not want them packed by the compiler
#pragma pack(push,1)
// RGB Pixel structs
typedef union RgbDouble {
    u16 Value;
    struct {
        u16 Blue : 5;
        u16 Green : 5;
        u16 Red : 5;
        u16 Alpha : 1;
    };
} RgbDouble;

typedef struct RgbTriple {
    u08 Blue;
    u08 Green;
    u08 Red;
} RgbTriple;

typedef union RgbQuad {
    u32 Value;
    struct {
        u08 Blue;
        u08 Green;
        u08 Red;
        u08 Alpha;
    };
} RgbQuad;

// CIEXYZ color profiles
typedef u32 FXPT2DOT30;
typedef struct CIEXYZ {
    FXPT2DOT30 X;
    FXPT2DOT30 Y;
    FXPT2DOT30 Z;
} CIEXYZ;

typedef struct CIETriple {
    CIEXYZ Red;
    CIEXYZ Green;
    CIEXYZ Blue;
} CIETriple;

// Bitmap file header
typedef struct BitmapHeader {
    union {
        u16 Id;
        char Header[2];
    };
    u32 Size;
    u16 Reserved[2];
    u32 Offset;
} BitmapHeader;

// Bitmap image headers
typedef struct BitmapCoreHeader {
    u32 Size;
    s16 Width;
    s16 Height;
    u16 Planes;
    u16 BitCount;
} BitmapCoreHeader;

typedef struct BitmapInfoHeader {
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitCount;
    u32 Compression;
    u32 SizeImage;
    u32 XPelsPerMeter;
    u32 YPelsPerMeter;
    u32 UsedColors;
    u32 ImportantColors;
} BitmapInfoHeader;
typedef BitmapInfoHeader BitmapInfoV1Header;

typedef struct BitmapInfoV2Header {
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitCount;
    u32 Compression;
    u32 SizeImage;
    u32 XPelsPerMeter;
    u32 YPelsPerMeter;
    u32 UsedColors;
    u32 ImportantColors;
    
    // V2
    u32 RedMask;
    u32 GreenMask;
    u32 BlueMask;
} BitmapInfoV2Header;

typedef struct BitmapInfoV3Header {
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitCount;
    u32 Compression;
    u32 SizeImage;
    u32 XPelsPerMeter;
    u32 YPelsPerMeter;
    u32 UsedColors;
    u32 ImportantColors;
    
    // V2
    u32 RedMask;
    u32 GreenMask;
    u32 BlueMask;
    
    // V3
    u32 AlphaMask;
} BitmapInfoV3Header;

typedef struct BitmapInfoV4Header {
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitCount;
    u32 Compression;
    u32 SizeImage;
    u32 XPelsPerMeter;
    u32 YPelsPerMeter;
    u32 UsedColors;
    u32 ImportantColors;
    
    // V2
    u32 RedMask;
    u32 GreenMask;
    u32 BlueMask;
    
    // V3
    u32 AlphaMask;
    
    // V4
    u32 ColorSpaceType;
    CIETriple Endpoints;
    u32 GammaRed;
    u32 GammaGreen;
    u32 GammaBlue;
} BitmapInfoV4Header;

typedef struct BitmapInfoV5Header {
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitCount;
    u32 Compression;
    u32 SizeImage;
    u32 XPelsPerMeter;
    u32 YPelsPerMeter;
    u32 UsedColors;
    u32 ImportantColors;
    
    // V2
    u32 RedMask;
    u32 GreenMask;
    u32 BlueMask;
    
    // V3
    u32 AlphaMask;
    
    // V4
    u32 ColorSpaceType;
    CIETriple Endpoints;
    u32 GammaRed;
    u32 GammaGreen;
    u32 GammaBlue;
    
    // V5
    u32 Intent;
    u32 ProfileData;
    u32 ProfileSize;
    u32 Reserved;
} BitmapInfoV5Header;

#pragma pack(pop)

// Bitmap image geader union
typedef union BitmapInfo {
    BitmapCoreHeader Core;
    BitmapInfoV1Header V1;
    BitmapInfoV2Header V2;
    BitmapInfoV3Header V3;
    BitmapInfoV4Header V4;
    BitmapInfoV5Header V5;
} BitmapInfo;

// Bitmap struct
typedef struct Bitmap {
    BitmapHeader Header;
    BitmapInfo Info;
    
    union Data {
        u08*       Bytes;
        RgbTriple* Rgb;
        RgbQuad*   Rgba;
    } Data;
} Bitmap;

static inline void
bitmap_create_core(Bitmap* bitmap, s16 width, s16 height) {
    bitmap->Info.Core.Width = width;
    bitmap->Info.Core.Height = height;
    bitmap->Info.Core.Planes = 1;
    bitmap->Info.Core.BitCount = 24;
    
    u32 size = width * height * sizeof(RgbTriple);
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + bitmap->Info.Core.Size;
    bitmap->Header.Size = bitmap->Header.Offset + size;
    
    bitmap->Data.Rgb = malloc(size);
}

static inline void
bitmap_create_v1(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoHeader* info = malloc(sizeof(BitmapInfoHeader));
    bitmap->Info.V1.Size = sizeof(BitmapInfoHeader);
    bitmap->Info.V1.Width = width;
    bitmap->Info.V1.Height = height;
    bitmap->Info.V1.Planes = 1;
    bitmap->Info.V1.BitCount = 32;
    bitmap->Info.V1.Compression = BI_RGB;
    bitmap->Info.V1.SizeImage = sizeof(RgbQuad) * height * width;
    bitmap->Info.V1.XPelsPerMeter = DPI(72);
    bitmap->Info.V1.YPelsPerMeter = DPI(72);
    bitmap->Info.V1.UsedColors = 0x00;
    bitmap->Info.V1.ImportantColors = 0x00;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + bitmap->Info.V1.Size;
    bitmap->Header.Size = bitmap->Header.Offset + bitmap->Info.V1.SizeImage;
    
    bitmap->Data.Rgba = malloc(bitmap->Info.V1.SizeImage);
}

static inline void
bitmap_create_v2(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoV2Header* info = malloc(sizeof(BitmapInfoV2Header));
    bitmap->Info.V2.Size = sizeof(BitmapInfoV2Header);
    bitmap->Info.V2.Width = width;
    bitmap->Info.V2.Height = height;
    bitmap->Info.V2.Planes = 1;
    bitmap->Info.V2.BitCount = 32;
    bitmap->Info.V2.Compression = BI_BITFIELDS;
    bitmap->Info.V2.SizeImage = sizeof(RgbQuad) * height * width;
    bitmap->Info.V2.XPelsPerMeter = DPI(72);
    bitmap->Info.V2.YPelsPerMeter = DPI(72);
    bitmap->Info.V2.UsedColors = 0x00;
    bitmap->Info.V2.ImportantColors = 0x00;
    bitmap->Info.V2.RedMask   = 0x00FF0000;
    bitmap->Info.V2.GreenMask = 0x0000FF00;
    bitmap->Info.V2.BlueMask  = 0x000000FF;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + bitmap->Info.V2.Size;
    bitmap->Header.Size = bitmap->Header.Offset + bitmap->Info.V2.SizeImage;
    
    bitmap->Data.Rgba = malloc(bitmap->Info.V2.SizeImage);
}

static inline void
bitmap_create_v3(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoV3Header* info = malloc(sizeof(BitmapInfoV3Header));
    bitmap->Info.V3.Size = sizeof(BitmapInfoV3Header);
    bitmap->Info.V3.Width = width;
    bitmap->Info.V3.Height = height;
    bitmap->Info.V3.Planes = 1;
    bitmap->Info.V3.BitCount = 32;
    bitmap->Info.V3.Compression = BI_BITFIELDS;
    bitmap->Info.V3.SizeImage = sizeof(RgbQuad) * height * width;
    bitmap->Info.V3.XPelsPerMeter = DPI(72);
    bitmap->Info.V3.YPelsPerMeter = DPI(72);
    bitmap->Info.V3.UsedColors = 0x00;
    bitmap->Info.V3.ImportantColors = 0x00;
    bitmap->Info.V3.RedMask   = 0x00FF0000;
    bitmap->Info.V3.GreenMask = 0x0000FF00;
    bitmap->Info.V3.BlueMask  = 0x000000FF;
    bitmap->Info.V3.AlphaMask = 0xFF000000;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + bitmap->Info.V3.Size;
    bitmap->Header.Size = bitmap->Header.Offset + bitmap->Info.V3.SizeImage;
    
    bitmap->Data.Rgba = malloc(bitmap->Info.V3.SizeImage);
}

static inline void
bitmap_create_v4(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoV4Header* info = malloc(sizeof(BitmapInfoV4Header));
    bitmap->Info.V4.Size = sizeof(BitmapInfoV4Header);
    bitmap->Info.V4.Width = width;
    bitmap->Info.V4.Height = height;
    bitmap->Info.V4.Planes = 1;
    bitmap->Info.V4.BitCount = 32;
    bitmap->Info.V4.Compression = BI_BITFIELDS;
    bitmap->Info.V4.SizeImage = sizeof(RgbQuad) * height * width;
    bitmap->Info.V4.XPelsPerMeter = DPI(72);
    bitmap->Info.V4.YPelsPerMeter = DPI(72);
    bitmap->Info.V4.UsedColors = 0x00;
    bitmap->Info.V4.ImportantColors = 0x00;
    bitmap->Info.V4.RedMask   = 0x00FF0000;
    bitmap->Info.V4.GreenMask = 0x0000FF00;
    bitmap->Info.V4.BlueMask  = 0x000000FF;
    bitmap->Info.V4.AlphaMask = 0xFF000000;
    bitmap->Info.V4.ColorSpaceType = 0x73524742;
    memset(&bitmap->Info.V4.Endpoints, 0, sizeof(CIETriple)); // NOTE: Ignored for the use ColorSpaceType (see specification)
    bitmap->Info.V4.GammaRed   = 0;
    bitmap->Info.V4.GammaGreen = 0;
    bitmap->Info.V4.GammaBlue  = 0;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + bitmap->Info.V4.Size;
    bitmap->Header.Size = bitmap->Header.Offset + bitmap->Info.V4.SizeImage;
    
    bitmap->Data.Rgba = malloc(bitmap->Info.V4.SizeImage);
}

static inline void
bitmap_create_v5(Bitmap* bitmap, s32 width, s32 height) {
    u32 rowSize = (u32) (floor((32.0 * width + 31.0) / 32.0) * 4.0);
    u32 dataSize = rowSize * height;
    
    BitmapInfoV5Header* info = malloc(sizeof(BitmapInfoV5Header));
    bitmap->Info.V5.Size = sizeof(BitmapInfoV5Header);
    bitmap->Info.V5.Width = width;
    bitmap->Info.V5.Height = height;
    bitmap->Info.V5.Planes = 1;
    bitmap->Info.V5.BitCount = 32;
    bitmap->Info.V5.Compression = BI_BITFIELDS;
    bitmap->Info.V5.SizeImage = dataSize;
    bitmap->Info.V5.XPelsPerMeter = DPI(72);
    bitmap->Info.V5.YPelsPerMeter = DPI(72);
    bitmap->Info.V5.UsedColors = 0x00;
    bitmap->Info.V5.ImportantColors = 0x00;
    bitmap->Info.V5.RedMask   = 0x00FF0000;
    bitmap->Info.V5.GreenMask = 0x0000FF00;
    bitmap->Info.V5.BlueMask  = 0x000000FF;
    bitmap->Info.V5.AlphaMask = 0xFF000000;
    bitmap->Info.V5.ColorSpaceType = 0x73524742;
    memset(&bitmap->Info.V5.Endpoints, 0, sizeof(CIETriple)); // NOTE: Ignored for the use ColorSpaceType (see specification)
    bitmap->Info.V5.GammaRed   = 0;
    bitmap->Info.V5.GammaGreen = 0;
    bitmap->Info.V5.GammaBlue  = 0;
    bitmap->Info.V5.Intent = 4;
    bitmap->Info.V5.ProfileData = 0;
    bitmap->Info.V5.ProfileSize = 0;
    bitmap->Info.V5.Reserved = 0;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + bitmap->Info.V5.Size;
    bitmap->Header.Size = bitmap->Header.Offset + bitmap->Info.V5.SizeImage;
    
    bitmap->Data.Bytes = malloc(bitmap->Info.V5.SizeImage);
}

static inline void
bitmap_free(Bitmap* bitmap) {
    free(bitmap->Data.Bytes);
    free(bitmap);
}

/*
* Gets the version of the bitmap represented by the BitmapInfo header.
* (This is based solely on the Size field of the header)
*/
static inline s32
bitmap_version(BitmapInfo* info) {
    switch(info->V1.Size) {
        case sizeof(BitmapCoreHeader):
        return BITMAP_V0;
        
        case sizeof(BitmapInfoHeader):
        return BITMAP_V1;
        
        case sizeof(BitmapInfoV2Header):
        return BITMAP_V2;
        
        case sizeof(BitmapInfoV3Header):
        return BITMAP_V3;
        
        case sizeof(BitmapInfoV4Header):
        return BITMAP_V4;
        
        case sizeof(BitmapInfoV5Header):
        return BITMAP_V5;
        
        default:
        return BITMAP_VUNKNOWN;
    }
}
static inline char*
bitmap_version_string(BitmapInfo* info) {
    switch(info->V1.Size) {
        case sizeof(BitmapCoreHeader):
        return "Core";
        
        case sizeof(BitmapInfoHeader):
        return "version 1";
        
        case sizeof(BitmapInfoV2Header):
        return "Version 2";
        
        case sizeof(BitmapInfoV3Header):
        return "Version 3";
        
        case sizeof(BitmapInfoV4Header):
        return "Version 4";
        
        case sizeof(BitmapInfoV5Header):
        return "Version 5";
        
        default:
        return "Unknown version";
    }
}

/*
* Calculates the number of colors used in the given bitmap.
*/
static inline u64
bitmap_colorcount(BitmapInfo* info) {
    if(bitmap_version(info) == BITMAP_VCORE)
        return (u64) pow(2, info->Core.BitCount * info->Core.Planes);
    
    return (u64) pow(2, info->V1.BitCount * info->V1.Planes);
}

/*
* Reads bitmap file and image headers (metadata) from the given file.
* Returns true if the data was successfully read.
*/
static inline s32
bitmap_read_metadata(BitmapHeader* header, BitmapInfo* info, FILE* file) {
    u64 readHeader = fread(header, sizeof(BitmapHeader), 1, file);
    u64 readCore = fread(info, sizeof(BitmapCoreHeader), 1, file);
    
    if (!readHeader || !readCore)
        return 0;
    
    // NOTE: If info size is invalid, assume V1 info
    // (If the size is invalid the version will be VUNKNOWN)
    if (bitmap_version(info) == BITMAP_VUNKNOWN) {
        printf("Warning: Invalid info header size, assuming V1\n");
        info->V1.Size = sizeof(BitmapInfoV1Header);
    }
    
    
    // NOTE: Read remaining fields depending on the bitmap version
    u32 v0InfoSize = sizeof(BitmapCoreHeader);
    if (info->V1.Size > v0InfoSize) {
        u08* infoPtr = (u08*) info;
        u32  infoSize = info->V1.Size - sizeof(BitmapCoreHeader);
        assert(infoSize > 0);
        
        u64 readExtra = fread(infoPtr + v0InfoSize, sizeof(u08), infoSize, file);
        if (readExtra != infoSize)
            return 0;
    }
    
    return 1;
}

#define bitmap_display_metadata(header, info) (bitmap_log_metadata((header), (info), stdout))
/*
* Writes bitmap metdata (as text) to the file.
*/
static inline void
bitmap_log_metadata(BitmapHeader* header,  BitmapInfo* info, FILE* file) {
    s32 version = bitmap_version(info);
    char* versionString = bitmap_version_string(info);
    
    u64 colorCount = bitmap_colorcount(info);
    
    fprintf(file, "BitmapHeader:\n");
    fprintf(file, "  Header:     %c%c (0x%2x)\n", header->Header[0], header->Header[1], header->Id);
    fprintf(file, "  Size:       0x%8x\n", header->Size);
    fprintf(file, "  Reserved 0: 0x%4x\n", header->Reserved[0]);
    fprintf(file, "  Reserved 1: 0x%4x\n", header->Reserved[1]);
    fprintf(file, "  Offset:     0x%8x\n", header->Offset);
    
    fprintf(file, "  BitmapInfoHeader:\n");
    fprintf(file, "  Version:        %i (%s)\n", version, versionString);
    fprintf(file, "  Color count:    %llu\n", colorCount);
    fprintf(file, "  Uses palette:   %s\n", info->V1.UsedColors > 0 ? "true" : "false");
    fprintf(file, "    Size:            0x%x\n", info->V1.Size);
    fprintf(file, "    Width:           0x%x\n", info->V1.Width);
    fprintf(file, "    Height:          0x%x\n", info->V1.Height);
    fprintf(file, "    Planes:          0x%2x\n", info->V1.Planes);
    fprintf(file, "    BitCount:        0x%2x\n", info->V1.BitCount);
    fprintf(file, "    Compression:     0x%2x\n", info->V1.Compression);
    fprintf(file, "    SizeImage:       0x%x\n", info->V1.SizeImage);
    fprintf(file, "    XPelsPerMeter:   0x%x\n", info->V1.XPelsPerMeter);
    fprintf(file, "    YPelsPerMeter:   0x%x\n", info->V1.YPelsPerMeter);
    fprintf(file, "    UsedColors:      0x%2x\n", info->V1.UsedColors);
    fprintf(file, "    ImportantColors: 0x%2x\n", info->V1.ImportantColors);
    
    if (version == BITMAP_VUNKNOWN) return;
    
    if (version >= BITMAP_V2) {
        fprintf(file, "    RedMask:         0x%8x\n", info->V2.RedMask);
        fprintf(file, "    GreenMask:       0x%8x\n", info->V2.GreenMask);
        fprintf(file, "    BlueMask:        0x%8x\n", info->V2.BlueMask);
    }
    if (version >= BITMAP_V3) {
        fprintf(file, "    AlphaMask:       0x%8x\n", info->V3.AlphaMask);
    }
    if (version >= BITMAP_V4) {
        fprintf(file, "    ColorSpaceType:  0x%8x\n", info->V4.ColorSpaceType);
        fprintf(file, "    CIE Endpoinst:\n");
        fprintf(file, "      RedEndPointX:    %i\n", info->V4.Endpoints.Red.X);
        fprintf(file, "      RedEndPointY:    %i\n", info->V4.Endpoints.Red.Y);
        fprintf(file, "      RedEndPointZ:    %i\n", info->V4.Endpoints.Red.Z);
        fprintf(file, "      GreenEndPointX:  %i\n", info->V4.Endpoints.Green.X);
        fprintf(file, "      GreenEndPointY:  %i\n", info->V4.Endpoints.Green.Y);
        fprintf(file, "      GreenEndPointZ:  %i\n", info->V4.Endpoints.Green.Z);
        fprintf(file, "      BlueEndPointX:   %i\n", info->V4.Endpoints.Blue.X);
        fprintf(file, "      BlueEndPointY:   %i\n", info->V4.Endpoints.Blue.Y);
        fprintf(file, "      BlueEndPointZ:   %i\n", info->V4.Endpoints.Blue.Z);
        fprintf(file, "    GammaRed:        0x%8x\n", info->V4.GammaRed);
        fprintf(file, "    GammaBlue:       0x%8x\n", info->V4.GammaBlue);
        fprintf(file, "    GammaGreen:      0x%8x\n", info->V4.GammaGreen);
    }
    if (version >= BITMAP_V5) {
        fprintf(file, "    Intent:          0x%8x\n", info->V5.Intent);
        fprintf(file, "    ProfileData:     0x%8x\n", info->V5.ProfileData);
        fprintf(file, "    ProfileSize:     0x%8x\n", info->V5.ProfileSize);
        fprintf(file, "    Reserved:        0x%8x\n", info->V5.Reserved);
    }
}

static inline void
bitmap_write(Bitmap* bitmap, FILE* file) {
    assert(bitmap->Info.V1.Size != 0);
    fwrite(&bitmap->Header, sizeof(BitmapHeader), 1, file);
    fwrite(&bitmap->Info.V1, bitmap->Info.V1.Size, 1, file);
    
    if (bitmap_version(&bitmap->Info) == BITMAP_VCORE) {
        u32 size = (u32) (bitmap->Info.V1.Width * bitmap->Info.V1.Height * (bitmap->Info.V1.BitCount / 8));
        
        fwrite(bitmap->Data.Bytes, sizeof(u08), size, file);
        return;
    }
    
    fwrite(bitmap->Data.Bytes, sizeof(u08), bitmap->Info.V1.SizeImage, file);
}

// Palette helpers
RgbQuad palette_black = { .Value = 0x00000000 };
static inline RgbQuad*
palette_color(RgbQuad* palette, u32 paletteSize, u32 color) {
    if (color + 1 > paletteSize) return &palette_black;
    return palette + color;
}
RgbTriple palette_black_core = { .Red = 0x00, .Green = 0x00, .Blue = 0x00 };
static inline RgbTriple*
palette_color_core(RgbTriple* palette, u32 paletteSize, u32 color) {
    if (color + 1 > paletteSize) return &palette_black_core;
    return palette + color;
}

static inline u08*
bitmap_load_core(FILE* file, s64 fileSize, BitmapHeader* header, BitmapCoreHeader* core, s32* width, s32* height) {
    // NOTE: Assuming negative width is an error, we take the absolute width
    if (core->Width < 0)
        core->Width = ABS(core->Width);
    
    // NOTE: Invalid (0) height and/or width
    if (!core->Width || !core->Height)
        return NULL;
    
    *width = core->Width;
    *height = ABS(core->Height);
    
    // NOTE: Check the height and width are valid (the number of pixels needed should not be larger than what can be stored in  a u32, since u32 stores the SizeImage field)
    // NOTE: This is a lower bound for the amount of bytes needed, since it does not account for padding.
    u64 pixelsNeeded = ((u64) *width) * ((u64) *height) * sizeof(RgbTriple);
    if (pixelsNeeded > 0xFFFFFFFF)
        return NULL;
    
    // NOTE: Check that the bitcount is valid, technically Core only supports 1,4,8 and 24 bpp images, though we might as well support 16 bpp RGB and 2 bpp palette as well
    if (!(core->BitCount ==  1 || core->BitCount ==  2 || core->BitCount ==  4 || core->BitCount ==  8 || core->BitCount == 16 || core->BitCount == 24)) {
        return NULL;
    }
    
    // NOTE: Rows are fit to a DWORD (4 byte) boundary (32 is BPP of output, 31 is bpp - 1 bit)
    u32 rowLength = (u32) (floor((core->BitCount * core->Width + 31.0) / 32.0) * 4);
    u32 rowOffset = (u32) (rowLength - ceil(core->Width * core->BitCount / 8.0));
    u32 dataSize = rowLength * ABS(core->Height);
    
    // NOTE: If row is negative image is in the opposite direction, so reverse this point
    u64 resultSize = core->Width * ABS(core->Height);
    u08* resultData = malloc(resultSize * sizeof(RgbQuad));
    RgbQuad* resultRgb = (RgbQuad*) resultData;
    
    // NOTE: Set all pixels in output to black
    for (u64 i = 0; i < resultSize; i++)
        resultRgb[i].Value = 0xFF000000;
    
    if (!*width || !*height)
        return NULL;
    
    u08* inputData = malloc(dataSize);
    u08* pixelData = inputData;
    
    // NOTE: Compression methods using a palette
    if (core->BitCount < 16) {
        core->Planes = 1; // NOTE: Planes should always be 1
        u32 colorCount = power(2, core->BitCount * core->Planes);
        
        // NOTE: If no palette, return data as is (transparent image of the given size)
        // TODO: Use a default palette?
        if (!colorCount)
            return NULL;
        
        RgbTriple* colors = malloc(sizeof(RgbTriple) * colorCount);
        u64 colorsRead = fread(colors, sizeof(RgbTriple), colorCount, file);
        if (colorsRead != colorCount)
            return NULL;
        
        // NOTE: Jump to the offset only if non-zero
        if (header->Offset && header->Offset < fileSize)
            fseek(file, header->Offset, SEEK_SET);
        u64 read = fread(pixelData, sizeof(u08), dataSize, file);
        
        // NOTE: Number of columns in the image (width in bytes)
        s32 columns = rowLength - rowOffset;
        
        // NOTE: Number of "spare" bits at the end of the last byte in each row
        s32 pixelMask = power(2, core->BitCount) - 1;
        s32 remainingBits = core->Width * core->BitCount % 8;
        remainingBits = remainingBits == 0 ? 8 : remainingBits;
        
        // NOTE: For each row, column (bytes) then for each pixel in the bytes (if bpp < 8)
        for (s32 row = 0; row < ABS(core->Height) && pixelData - inputData < read; row++) {
            for (s32 column = 0; column < columns && pixelData - inputData < read; column++) {
                // NOTE: Amount of colors / pixels in each byte (differs for the last byte depending on pixel width)
                // Extra shift is the extra shift used for the last byte (since data is stored little endian)
                s32 colorsInByte = (column + 1 == columns ? remainingBits : 8) / core->BitCount;
                s32 extraShift  = (column + 1 == columns ? 8 / core->BitCount - colorsInByte : 0);
                
                for(s32 bit = colorsInByte - 1; bit >= 0; bit--) {
                    s32 mask = (pixelMask << (bit + extraShift) * core->BitCount);
                    s32 shift = (bit + extraShift) * core->BitCount;
                    s32 pixelValue = (*pixelData & mask) >> shift;
                    RgbTriple color = *palette_color_core(colors, colorCount, pixelValue);
                    
                    // NOTE: In color tables alpha is inverted (for some odd reason?)
                    resultRgb->Red = color.Red;
                    resultRgb->Green = color.Green;
                    resultRgb->Blue = color.Blue;
                    resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                    resultRgb++;
                }
                pixelData++;
            }
            pixelData += rowOffset;
        }
        
        free(colors);
    }
    else {
        // NOTE: Jump to the offset only if non-zero
        if (header->Offset && header->Offset < fileSize)
            fseek(file, header->Offset, SEEK_SET);
        u64 read = fread(pixelData, sizeof(u08), dataSize, file);
        
        s32 columns = (rowLength - rowOffset) / (core->BitCount / 8);
        for (s32 row = 0; row < ABS(core->Height) && pixelData - inputData < read; row++) {
            for (s32 column = 0; column < columns && pixelData - inputData < read; column++) {
                if (core->BitCount == 16) {
                    RgbDouble pixel = *((RgbDouble*) pixelData);
                    resultRgb->Red   = MAPRGBBYTE(pixel.Red, 5);
                    resultRgb->Green = MAPRGBBYTE(pixel.Green, 5);
                    resultRgb->Blue  = MAPRGBBYTE(pixel.Blue, 5);
                    resultRgb->Alpha = (u08) (pixel.Alpha ? 0x00 : 0xFF);
                    // TODO: Currently supports alpha for bitmaps of versions that do _NOT_ support alpha,
                    // primarily v1 and v2 bitmaps. Should this feature be kept?
                    // Or only "enabled" when the version supports alpha/transparency?
                    
                    pixelData += sizeof(RgbDouble);
                }
                
                else if(core->BitCount  == 24) {
                    RgbTriple pixel = *((RgbTriple*) pixelData);
                    resultRgb->Red   = pixel.Red;
                    resultRgb->Green = pixel.Green;
                    resultRgb->Blue  = pixel.Blue;
                    resultRgb->Alpha = 0xFF;
                    
                    pixelData += sizeof(RgbTriple);
                }
                else if (core->BitCount == 32) {
                    RgbQuad pixel = *((RgbQuad*) pixelData);
                    resultRgb->Red   = pixel.Red;
                    resultRgb->Green = pixel.Green;
                    resultRgb->Blue  = pixel.Blue;
                    resultRgb->Alpha = (u08) (0xFF - pixel.Alpha);
                    // NOTE: Alpha is inverted, like with palette images
                    
                    pixelData += sizeof(RgbQuad);
                }
                
                resultRgb++;
            }
            pixelData += rowOffset;
        }
    }
    
    // NOTE: Invert the bitmap if top-down
    if (core->Height < 0) {
        u32* dataDWords = (u32*) resultData;
        for (s32 row = 0; row < (*height) / 2; row++) {
            for (s32 column = 0; column < (*width); column++) {
                s32 srcWord = (*width) * row + column;
                s32 tgtWord = (*width) * ((*height) - row - 1) + column;
                
                dataDWords[srcWord] ^= dataDWords[tgtWord];
                dataDWords[tgtWord] ^= dataDWords[srcWord];
                dataDWords[srcWord] ^= dataDWords[tgtWord];
            }
        }
    }
    
    if (inputData)
        free(inputData);
    
    return resultData;
}

static inline u08*
bitmap_load(FILE* file, s32* width, s32* height) {
    BitmapHeader header;
    BitmapInfo info;
    if (!bitmap_read_metadata(&header, &info, file)) {
        printf("Error: Unable to read bitmap info header\n");
        return NULL;
    }
    
    //bitmap_display_metadata(&header, &info);
    
    // NOTE: File size is used to check offsets
    s64 currentPos = ftell(file);
    fseek(file, 0l, SEEK_END);
    s64 fileSize = ftell(file) - currentPos;
    fseek(file, currentPos, SEEK_SET);
    if (fileSize == 0) {
        printf("Error: No pixel data in file\n");
        return NULL;
    }
    
    // NOTE: Core bitmaps have a different info header, so we handle them seperately
    s32 version = bitmap_version(&info);
    if (version == BITMAP_VCORE) {
        return bitmap_load_core(file, fileSize, &header, &info.Core, width, height);
    }
    
    // NOTE: Check for proper magic number in header
    if(header.Id != 0x4D42) {
        printf("Warning: Invalid magic number sepcified in file header\n");
    }
    
    // NOTE: Check if reserved fields are set properly
    if (header.Reserved[0] != 0x00 || header.Reserved[1] != 0x00) {
        printf("Warning: Reserved bytes in header non zero\n");
    }
    
    // NOTE: For unknown version assume v1
    if (version == BITMAP_VUNKNOWN) {
        printf("Warning: Unknown version specified in info header, assuming Version 1\n");
        info.V1.Size = sizeof(BitmapInfoHeader), version = 1;
    }
    
    // NOTE: Assuming negative width is an error, we take the absolute width
    if (info.V1.Width < 0) {
        printf("Warning: Width is negative, assuming width should be positive\n");
        info.V1.Width = ABS(info.V1.Width);
    }
    
    // NOTE: Invalid (0) height and/or width
    if (!info.V1.Width || !info.V1.Height) {
        printf("Error: Invalid bitmap, width and/or height is set to 0\n");
        return NULL;
    }
    
    *width = info.V1.Width;
    *height = ABS(info.V1.Height);
    
    // NOTE: Check the height and width are valid (the number of pixels needed should not be larger than what can be stored in  a u32, since u32 stores the SizeImage field)
    // NOTE: This is a lower bound for the amount of bytes needed, since it does not account for padding.
    u64 pixelsNeeded = ((u64) *width) * ((u64) *height) * sizeof(RgbQuad);
    if (pixelsNeeded > 0xFFFFFFFF) {
        printf("Error: Invalid bitmap, width * height overflows 32 bits\n");
        return NULL;
    }
    
    // NOTE: Bad bitcount, calculate it from Width and SizeImage according to formula
    if (!(info.V1.BitCount ==  1 || info.V1.BitCount ==  2 || info.V1.BitCount ==  4 || info.V1.BitCount ==  8 || info.V1.BitCount == 16 || info.V1.BitCount == 24 || info.V1.BitCount == 32)) {
        printf("Warning: Invalid Bitcount specified in info, calculating from size, width and height\n");
        info.V1.BitCount = (u16) ((32 * ((info.V1.SizeImage / (info.V1.Height * 4)) - (31 / 32))) / info.V1.Width);
    }
    
    // NOTE: Correct invalid RLE4/8 and BitCount values
    // If the bitcount does not match the compression mode correct the compression mode so it does
    if (info.V1.Compression == BI_RLE4 && info.V1.BitCount == 8) {
        printf("Warning: BitCount 8 specified for RLE4, assuming RLE8\n");
        info.V1.Compression = BI_RLE8;
    }
    if (info.V1.Compression == BI_RLE8 && info.V1.BitCount == 4) {
        printf("Warning: BitCount 4 specified for RLE8, assuming RLE4\n");
        info.V1.Compression = BI_RLE4;
    }
    
    // NOTE: Rows are fit to a DWORD (4 byte) boundary (32 is BPP of output, 31 is bpp - 1 bit)
    u32 rowLength = (u32) (floor((info.V1.BitCount * info.V1.Width + 31.0) / 32.0) * 4);
    u32 rowOffset = (u32) (rowLength - ceil(info.V1.Width * info.V1.BitCount / 8.0));
    u32 dataSize = rowLength * ABS(info.V1.Height);
    
    // NOTE: We cannot calculate the raw size of any image compressed with RLE and have to rely on the size specified in the info
    if (info.V1.Compression == BI_RLE4 || info.V1.Compression == BI_RLE8)
        dataSize = info.V1.SizeImage;
    else if (dataSize != info.V1.SizeImage) {
        // NOTE: Datasize does not match Size, correct this
        printf("Warning: SizeImage does not match calculated size (%u != %u)\n", info.V1.SizeImage, dataSize);
        info.V1.SizeImage = dataSize;
    }
    
    // NOTE: If row is negative image is in the opposite direction, so reverse this point
    u64 resultSize = info.V1.Width * ABS(info.V1.Height);
    u08* resultData = malloc(resultSize * sizeof(RgbQuad));
    RgbQuad* resultRgb = (RgbQuad*) resultData;
    
    // NOTE: Set all pixels in output to black
    for (u64 i = 0; i < resultSize; i++)
        resultRgb[i].Value = 0xFF000000;
    
    u08* inputData = malloc(dataSize);
    u08* pixelData = inputData;
    
    // NOTE: Compression methods using a palette
    if ((info.V1.Compression == BI_RGB && info.V1.BitCount < 16) || info.V1.Compression == BI_RLE4 || info.V1.Compression == BI_RLE8) {
        info.V1.Planes = 1; // NOTE: Planes should always be 1
        u32 maxColorCount = power(2, info.V1.BitCount * info.V1.Planes);
        u32 colorCount = MIN(info.V1.UsedColors, maxColorCount);
        
        // NOTE: If no palette, return data as is (transparent image of the given size)
        // TODO: Use a default palette?
        if (!colorCount) {
            printf("Warning: No colors specified for palette (UsedColors == 0), assuming maximum palette size\n");
            colorCount = maxColorCount;
        }
        
        RgbQuad* colors = malloc(sizeof(RgbQuad) * colorCount);
        u64 colorsRead = fread(colors, sizeof(RgbQuad), colorCount, file);
        if (colorsRead != colorCount) {
            printf("Error: Unable to read palette from file\n");
            return NULL;
        }
        
        // NOTE: Jump to the offset only if non-zero
        if (header.Offset && header.Offset < fileSize)
            fseek(file, header.Offset, SEEK_SET);
        u64 read = fread(pixelData, sizeof(u08), dataSize, file);
        
        // NOTE: No compression, using palette
        if (info.V1.Compression == BI_RGB) {
            s32 pixelMask = power(2, info.V1.BitCount) - 1;
            
            // NOTE: Number of columns in the image (width in bytes)
            s32 columns = rowLength - rowOffset;
            
            // NOTE: Number of "spare" bits at the end of the last byte in each row
            s32 remainingBits = info.V1.Width * info.V1.BitCount % 8;
            remainingBits = remainingBits == 0 ? 8 : remainingBits;
            
            // NOTE: For each row, column (bytes) then for each pixel in the bytes (if bpp < 8)
            for (s32 row = 0; row < ABS(info.V1.Height) && pixelData - inputData < read; row++) {
                for (s32 column = 0; column < columns && pixelData - inputData < read; column++) {
                    // NOTE: Amount of colors / pixels in each byte (differs for the last byte depending on pixel width)
                    // Extra shift is the extra shift used for the last byte (since data is stored little endian)
                    s32 colorsInByte = (column + 1 == columns ? remainingBits : 8) / info.V1.BitCount;
                    s32 extraShift  = (column + 1 == columns ? 8 / info.V1.BitCount - colorsInByte : 0);
                    
                    for(s32 bit = colorsInByte - 1; bit >= 0; bit--) {
                        s32 mask = (pixelMask << (bit + extraShift) * info.V1.BitCount);
                        s32 shift = (bit + extraShift) * info.V1.BitCount;
                        s32 pixelValue = (*pixelData & mask) >> shift;
                        RgbQuad color = *palette_color(colors, colorCount, pixelValue);
                        
                        // NOTE: In color tables alpha is inverted (for some odd reason?)
                        *resultRgb = color;
                        resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                        resultRgb++;
                    }
                    pixelData++;
                }
                pixelData += rowOffset;
            }
        }
        // NOTE: RLE4
        else if (info.V1.Compression == BI_RLE4) {
            RgbQuad* rgbStart = resultRgb;
            
            s32 x = 0, y = 0;
            s08 absoluteMode = 0;
            for (u32 pos = 0; (pixelData - inputData) < dataSize && (resultRgb - rgbStart + 2) <= resultSize && pixelData - inputData < read; pos++) {
                u08 word = *pixelData++;
                u08 value = *pixelData++;
                
                // NOTE: Control
                if (word == 0x00) {
                    switch (value) {
                        case 0x00: { // End of line
                            y++;
                            x = 0;
                            resultRgb = rgbStart + INDEX2D(x, y, *width);
                            continue;
                        }
                        case 0x01: {// End of file
                            // Setting this makes the for loop exit
                            pos = dataSize;
                            continue;
                        }
                        case 0x02: { // Delta
                            x += *pixelData++;
                            y += *pixelData++;
                            resultRgb = rgbStart + INDEX2D(x, y, *width);
                            continue;
                        }
                        default: { // Absolute marker
                            absoluteMode = 1;
                            break;
                        }
                    }
                }
                
                if(!absoluteMode) {
                    u32 upper = (u32) word / 2 + word % 2;
                    for (u08 count = 0; count < upper; count++) {
                        *resultRgb = *palette_color(colors, colorCount, value & 0x0F);
                        resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                        resultRgb++;
                        x++;
                        
                        if (count == upper - 1 && word % 2 == 1)
                            continue;
                        
                        *resultRgb = *palette_color(colors, colorCount, (value & 0xF0) >> 4);
                        resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                        resultRgb++;
                        x++;
                    }
                }
                else {
                    u32 upper = (u32) (value / 2 + value % 2);
                    for (u08 count = 0; count < upper; count++) { // RLE4 has two per pixel
                        u08 runData = *pixelData++;
                        
                        *resultRgb = *palette_color(colors, colorCount, (runData & 0xF0) >> 4);
                        resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                        resultRgb++;
                        x++;
                        
                        if (count == upper - 1 && value % 2 == 1)
                            continue;
                        
                        *resultRgb = *palette_color(colors, colorCount, runData & 0x0F);
                        resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                        resultRgb++;
                        x++;
                    }
                    
                    absoluteMode = 0;
                    pixelData += (value / 2 + value % 2) % 2;
                }
            }
        }
        // NOTE: RLE8
        else if (info.V1.Compression == BI_RLE8) {
            RgbQuad* rgbStart = resultRgb;
            
            s32 x = 0, y = 0;
            s08 absoluteMode = 0;
            for (u32 pos = 0; (pixelData - inputData) < dataSize && (resultRgb - rgbStart + 2) <= resultSize && pixelData - inputData < read; pos++) {
                u08 word = *pixelData++;
                u08 value = *pixelData++;
                
                // NOTE: Control
                if (word == 0x00) {
                    switch (value) {
                        case 0x00: {// End of line
                            y++;
                            x = 0;
                            resultRgb = rgbStart + INDEX2D(x, y, *width);
                            continue;
                        }
                        case 0x01: { // End of file
                            // Setting this makes the for loop exit
                            pos = dataSize;
                            continue;
                        }
                        case 0x02: { // Delta
                            x += *pixelData++;
                            y += *pixelData++;
                            resultRgb = rgbStart + INDEX2D(x, y, *width);
                            continue;
                        }
                        default: { // Absolute marker
                            absoluteMode = 1;
                            break;
                        }
                    }
                }
                
                if(!absoluteMode) {
                    for (u08 count = 0; count < word; count++) {
                        *resultRgb = *palette_color(colors, colorCount, value);
                        resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                        resultRgb++;
                        x++;
                    }
                }
                else {
                    for (u08 count = 0; count < value; count++) {
                        *resultRgb = *palette_color(colors, colorCount, *pixelData++);
                        resultRgb->Alpha = (u08) (0xFF - resultRgb->Alpha);
                        resultRgb++;
                        x++;
                    }
                    
                    absoluteMode = 0;
                    pixelData += value % 2;
                }
            }
        }
        
        free(colors);
    }
    // NOTE: BI_RGB
    else if (info.V1.Compression == BI_RGB) {
        // NOTE: Jump to the offset only if non-zero
        if (header.Offset && header.Offset < fileSize)
            fseek(file, header.Offset, SEEK_SET);
        u64 read = fread(pixelData, sizeof(u08), dataSize, file);
        
        s32 columns = (rowLength - rowOffset) / (info.V1.BitCount / 8);
        for (s32 row = 0; row < ABS(info.V1.Height) && pixelData - inputData < read; row++) {
            for (s32 column = 0; column < columns && pixelData - inputData < read; column++) {
                if (info.V1.BitCount == 16) {
                    RgbDouble pixel = *((RgbDouble*) pixelData);
                    resultRgb->Red   = MAPRGBBYTE(pixel.Red, 5);
                    resultRgb->Green = MAPRGBBYTE(pixel.Green, 5);
                    resultRgb->Blue  = MAPRGBBYTE(pixel.Blue, 5);
                    resultRgb->Alpha = (u08) (pixel.Alpha ? 0x00 : 0xFF);
                    // TODO: Currently supports alpha for bitmaps of versions that do _NOT_ support alpha,
                    // primarily v1 and v2 bitmaps. Should this feature be kept?
                    // Or only "enabled" when the version supports alpha/transparency?
                    
                    pixelData += sizeof(RgbDouble);
                }
                
                else if(info.V1.BitCount  == 24) {
                    RgbTriple pixel = *((RgbTriple*) pixelData);
                    resultRgb->Red   = pixel.Red;
                    resultRgb->Green = pixel.Green;
                    resultRgb->Blue  = pixel.Blue;
                    resultRgb->Alpha = 0xFF;
                    
                    pixelData += sizeof(RgbTriple);
                }
                else if (info.V1.BitCount == 32) {
                    RgbQuad pixel = *((RgbQuad*) pixelData);
                    resultRgb->Red   = pixel.Red;
                    resultRgb->Green = pixel.Green;
                    resultRgb->Blue  = pixel.Blue;
                    resultRgb->Alpha = (u08) (0xFF - pixel.Alpha);
                    // NOTE: Alpha is inverted, like with palette images
                    
                    pixelData += sizeof(RgbQuad);
                }
                
                resultRgb++;
            }
            pixelData += rowOffset;
        }
    }
    // NOTE: BitFields
    else if (info.V1.Compression == BI_BITFIELDS || info.V1.Compression == BI_ALPHABITFIELDS) {
        s32 hasAlpha = !(version <= 2 && info.V1.Compression != BI_ALPHABITFIELDS);
        
        // NOTE: For versions > 2 we only use alpha if the bitfield specifies any values of alpha, IE. has any bits set
        if (version  >= 3)
            hasAlpha = hasAlpha && info.V3.AlphaMask;
        
        // NOTE: While v1 bitmaps have no fields for RGB bitmasks they come right after the info header,
        // for convenience we read them into the fields of the v2 header.
        if (version == 1) {
            u64 read = fread(&info.V2.RedMask, sizeof(u32), 3, file);
            if (read != 3) return NULL;
        }
        if (version == 1 && info.V1.Compression == BI_ALPHABITFIELDS) {
            u64 read = fread(&info.V3.AlphaMask, sizeof(u32), 1, file);
            if (read != 1) return NULL;
        }
        
        if (header.Offset && header.Offset < fileSize)
            fseek(file, header.Offset, SEEK_SET);
        u64 read = fread(pixelData, sizeof(u08), dataSize, file);
        
        // NOTE: Amount to shift the result to properly align the first bit
        u32 redShift, greenShift, blueShift, alphaShift = 0;
        for (redShift = 0; redShift < 32 && !(info.V2.RedMask & power(2, redShift)); redShift++);
        for (greenShift = 0; greenShift < 32 && !(info.V2.GreenMask & power(2, greenShift)); greenShift++);
        for (blueShift = 0; blueShift < 32 && !(info.V2.BlueMask & power(2, blueShift)); blueShift++);
        
        // NOTE: Bpp for the individual channels
        s32 redBits = setbits(info.V2.RedMask);
        s32 greenBits = setbits(info.V2.GreenMask);
        s32 blueBits = setbits(info.V2.BlueMask);
        s32 alphaBits = 0;
        
        // NOTE: These are only used if alpha is used
        if (hasAlpha) {
            for (alphaShift = 0; alphaShift < 32 && !(info.V3.AlphaMask & power(2, alphaShift)); alphaShift++);
            alphaBits = setbits(info.V3.AlphaMask);
        }
        
        s32 columns = (rowLength - rowOffset) / (info.V1.BitCount / 8);
        for (s32 row = 0; row < ABS(info.V1.Height) && pixelData - inputData < read; row++) {
            for (s32 column = 0; column < columns && pixelData - inputData < read; column++) {
                u32 pixelValue = 0x00;
                
                s32 totalBytes = info.V1.BitCount / 8;
                for (s08 bytes = 0; bytes < totalBytes; bytes++) {
                    pixelValue |= ((*pixelData) << bytes * 8);
                    pixelData++;
                }
                
                u32 red = (pixelValue & info.V2.RedMask) >> redShift;
                u32 green = (pixelValue & info.V2.GreenMask) >> greenShift;
                u32 blue = (pixelValue & info.V2.BlueMask) >> blueShift;
                u32 alpha = !hasAlpha ? 0 : ((pixelValue & info.V3.AlphaMask) >> alphaShift);
                
                resultRgb->Red   = MAPRGBBYTE(red, redBits);
                resultRgb->Green = MAPRGBBYTE(green, greenBits);
                resultRgb->Blue  = MAPRGBBYTE(blue, blueBits);
                resultRgb->Alpha = (u08) (hasAlpha ? MAPRGBBYTE(alpha, alphaBits) : 0xFF);
                resultRgb++;
            }
            pixelData += rowOffset;
        }
    }
    
    // NOTE: Invert the bitmap if top-down
    if (info.V1.Height < 0) {
        u32* dataDWords = (u32*) resultData;
        for (s32 row = 0; row < (*height) / 2; row++) {
            for (s32 column = 0; column < (*width); column++) {
                s32 srcWord = (*width) * row + column;
                s32 tgtWord = (*width) * ((*height) - row - 1) + column;
                
                dataDWords[srcWord] ^= dataDWords[tgtWord];
                dataDWords[tgtWord] ^= dataDWords[srcWord];
                dataDWords[srcWord] ^= dataDWords[tgtWord];
            }
        }
    }
    
    if (inputData)
        free(inputData);
    
    return resultData;
}

static inline void
bitmap_save(FILE* file, s32 width, s32 height, const u08* data) {
    Bitmap bitmap;
    bitmap_create_v5(&bitmap, width, height);
    
    s32 bitmapHeight = bitmap.Info.V1.Height == 0 ? 1 : bitmap.Info.V1.Height;
    u32 rowLength = (bitmap.Info.V1.SizeImage / sizeof(RgbQuad)) / bitmapHeight;
    u32 rowOffset = rowLength - bitmap.Info.V1.Width;
    
    u08* outputData = bitmap.Data.Bytes;
    for (s32 row = 0; row < bitmap.Info.V1.Height; row++) {
        for (s32 column = 0; column < bitmap.Info.V1.Width; column++) {
            RgbQuad* outputPixel = (RgbQuad*) outputData;
            *outputPixel = *((RgbQuad*) data);
            
            data += sizeof(RgbQuad);
            outputData += sizeof(RgbQuad);
        }
        outputData += rowOffset;
    }
    
    bitmap_write(&bitmap, file);
    free(bitmap.Data.Bytes);
}

#endif // BITMAP_H