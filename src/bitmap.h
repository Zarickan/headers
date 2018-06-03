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
#define MAP(v, wordSize) (u08) ((v) / (pow(2, (wordSize)) - 1) * 255)

// Bitmap versions
#define BITMAP_VUNKNOWN 0xFF
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

#pragma pack(push,1)
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

typedef struct BitmapHeader {
    union {
        u16 Id;
        char Header[2];
    };
    u32 Size;
    u16 Reserved[2];
    u32 Offset;
} BitmapHeader;

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

typedef union BitmapInfo {
    BitmapCoreHeader Core;
    BitmapInfoV1Header V1;
    BitmapInfoV2Header V2;
    BitmapInfoV3Header V3;
    BitmapInfoV4Header V4;
    BitmapInfoV5Header V5;
} BitmapInfo;

typedef struct Bitmap {
    BitmapHeader Header;
    BitmapInfo Info;
    
    union Data {
        u08*       Bytes;
        RgbTriple* Rgb;
        RgbQuad*   Rgba;
    } Data;
} Bitmap;

static inline u08
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

static inline u64
bitmap_colorcount(BitmapInfo* info) {
    // TODO: Support for CORE bitmaps?
    return (u64) pow(2, info->V1.BitCount * info->V1.Planes);
}

static inline s32
bitmap_readinfo(BitmapHeader* header, BitmapInfo* info, FILE* file) {
    u64 readHeader = fread(header, sizeof(BitmapHeader), 1, file);
    u64 readCore = fread(info, sizeof(BitmapCoreHeader), 1, file);
    
    if (!readHeader || !readCore)
        return 0;
    
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

#define bitmap_display_metadata(info) (bitmap_log_metadata((info), stdout))
static inline void
bitmap_log_metadata(BitmapInfo* info, FILE* file) {
    s32 version = bitmap_version(info);
    u64 colorCount = bitmap_colorcount(info);
    if (version == BITMAP_VUNKNOWN) return;
    
    fprintf(file, "  BitmapInfoHeader:\n");
    fprintf(file, "  Version:        %i\n", version);
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

RgbQuad palette_black = { .Value = 0x00000000 };
static inline RgbQuad*
palette_color(RgbQuad* palette, u32 paletteSize, u32 color) {
    if (color + 1 > paletteSize) return &palette_black;
    return palette + color;
}

static inline u08*
bitmap_load(FILE* file, s32* width, s32* height) {
    BitmapHeader header;
    BitmapInfo info;
    if (!bitmap_readinfo(&header, &info, file))
        return NULL;
    
    // TODO: Temporary
    printf("Header:\n");
    printf("  Header:     %c%c (0x%2x)\n", header.Header[0], header.Header[1], header.Id);
    printf("  Size:       0x%8x\n", header.Size);
    printf("  Reserved 0: 0x%4x\n", header.Reserved[0]);
    printf("  Reserved 1: 0x%4x\n", header.Reserved[1]);
    printf("  Offset:     0x%8x\n", header.Offset);
    
    bitmap_display_metadata(&info);
    
    // NOTE: Core bitmaps have a different info header, so we handle them seperately
    u32 version = bitmap_version(&info);
    if (version == BITMAP_VCORE) {
        //return bitmap_load_core(file, width, height);
        // TODO: Load core bitmaps
        return NULL;
    }
    
    // NOTE: File size is used to check offsets
    s64 currentPos = ftell(file);
    fseek(file, 0l, SEEK_END);
    s64 fileSize = ftell(file) - currentPos;
    fseek(file, currentPos, SEEK_SET);
    if (fileSize == 0) return NULL;
    
    // NOTE: For unknown version assume v1
    if (version == BITMAP_VUNKNOWN)
        info.V1.Size = sizeof(BitmapInfoHeader), version = 1;
    
    // NOTE: Assuming negative width is an error, we take the absolute width
    if (info.V1.Width < 0)
        info.V1.Width = ABS(info.V1.Width);
    
    // NOTE: Invalid (0) height and/or width
    if (!info.V1.Width || !info.V1.Height)
        return NULL;
    
    *width = info.V1.Width;
    *height = ABS(info.V1.Height);
    
    // NOTE: Check the height and width are valid (the number of pixels needed should not be larger than what can be stored in  a u32, since u32 stors the SizeImage field)
    // NOTE: This is a lower bound for the amount of bytes needed, since it does not account for padding.
    u64 pixelsNeeded = ((u64) *width) * ((u64) *height) * sizeof(RgbQuad);
    if (pixelsNeeded > 0xFFFFFFFF)
        return NULL;
    
    // NOTE: Bad bitcount, calculate it from Width and SizeImage according to formula
    if (!(info.V1.BitCount ==  1 || info.V1.BitCount ==  2 || info.V1.BitCount ==  4 || info.V1.BitCount ==  8 || info.V1.BitCount == 16 || info.V1.BitCount == 24 || info.V1.BitCount == 32)) {
        info.V1.BitCount = (u16) ((32 * ((info.V1.SizeImage / (info.V1.Height * 4)) - (31 / 32))) / info.V1.Width);
    }
    
    
    // NOTE: Correct invalid RLE4/8 and BitCount values
    // If the bitcount does not match the compression mode correct the compression mode so it does
    if (info.V1.Compression == BI_RLE4 && info.V1.BitCount == 8)
        info.V1.Compression = BI_RLE8;
    if (info.V1.Compression == BI_RLE8 && info.V1.BitCount == 4)
        info.V1.Compression = BI_RLE4;
    
    // NOTE: Rows are fit to a DWORD (4 byte) boundary (32 is BPP of output, 31 is bpp - 1 bit)
    u32 rowLength = (u32) (floor((info.V1.BitCount * info.V1.Width + 31.0) / 32.0) * 4);
    u32 rowOffset = (u32) (rowLength - ceil(info.V1.Width * info.V1.BitCount / 8.0));
    u32 dataSize = rowLength * ABS(info.V1.Height);
    
    // NOTE: We cannot calculate the raw size of any image compressed with RLE and have to rely on the size specified in the info
    if (info.V1.Compression == BI_RLE4 || info.V1.Compression == BI_RLE8)
        dataSize = info.V1.SizeImage;
    
    // NOTE: If row is negative image is in the opposite direction, so reverse this point
    u64 resultSize = info.V1.Width * ABS(info.V1.Height);
    u08* resultData = malloc(resultSize * sizeof(RgbQuad));
    RgbQuad* resultRgb = (RgbQuad*) resultData;
    
    // NOTE: Set all pixels in output to black
    for (u64 i = 0; i < resultSize; i++)
        resultRgb[i].Value = 0xFF000000;
    
    if (!*width || !*height)
        return NULL;
    
    u08* inputData = malloc(dataSize);
    u08* pixelData = inputData;
    
    // NOTE: Compression methods using a paletteusing a palette
    if ((info.V1.Compression == BI_RGB && info.V1.BitCount < 16) || info.V1.Compression == BI_RLE4 || info.V1.Compression == BI_RLE8) {
        info.V1.Planes = 1; // NOTE: Planes should always be 1
        u32 maxColorCount = power(2, info.V1.BitCount * info.V1.Planes);
        u32 colorCount = MIN(info.V1.UsedColors, maxColorCount);
        
        // NOTE: If no palette, return data as is (transparent image of the given size)
        // TODO: Use a default palette?
        if (!colorCount)
            return NULL;
        
        RgbQuad* colors = malloc(sizeof(RgbQuad) * colorCount);
        u64 colorsRead = fread(colors, sizeof(RgbQuad), colorCount, file);
        if (colorsRead != colorCount)
            return NULL;
        
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
                    resultRgb->Red   = MAP(pixel.Red, 5);
                    resultRgb->Green = MAP(pixel.Green, 5);
                    resultRgb->Blue  = MAP(pixel.Blue, 5);
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
                
                resultRgb->Red   = MAP(red, redBits);
                resultRgb->Green = MAP(green, greenBits);
                resultRgb->Blue  = MAP(blue, blueBits);
                resultRgb->Alpha = (u08) (hasAlpha ? MAP(alpha, alphaBits) : 0xFF);
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

static inline RgbQuad
centroid(RgbQuad* quads, u32 size) {
    f32 red, green, blue;
    red = green = blue = 0;
    
    for (u32 i = 0; i < size; i++) {
        red += quads[i].Red;
        green += quads[i].Green;
        blue += quads[i].Blue;
    }
    
    RgbQuad result;
    result.Red = (u08) (red / size);
    result.Green = (u08) (green / size);
    result.Blue = (u08) (blue / size);
    result.Alpha = 0xFF;
    
    return result;
}

static inline f32
distance(RgbQuad* from, RgbQuad* to) {
    s32 red, green, blue;
    red = abs(from->Red - to->Red);
    green = abs(from->Green - to->Green);
    blue = abs(from->Blue - to->Blue);
    
    return (f32) sqrt(pow(red, 2) + pow(green, 2) + pow(blue, 2));
}

static inline f32
distance_manhattan(RgbQuad* from, RgbQuad* to) {
    f32 distR = from->Red - to->Red;
    f32 distG = from->Green - to->Green;
    f32 distB = from->Blue - to->Blue;
    
    return fabsf(distR) + fabsf(distG) + fabsf(distB);
}

static inline RgbQuad*
kmeans_cluster(RgbQuad* data, u32 width, u32 height, u32 clusterCount) {
    RgbQuad* centroids = malloc(sizeof(RgbQuad) * clusterCount);
    
    Stack* clusters = malloc(sizeof(Stack) * clusterCount);
    for (u32 i = 0; i < clusterCount; i++) {
        stack_init(clusters + i, 16);
    }
    
    // NOTE: Place initial centroids, starting with a grayscale palette of clusterCount colors
    u08 jmp = (u08) (256 / clusterCount);
    RgbQuad* currentCentroid = centroids;
    for (u32 i = 0; i < clusterCount; i++) {
        currentCentroid->Red = (u08) (jmp * i);
        currentCentroid->Green = (u08) (jmp * i);
        currentCentroid->Blue = (u08) (jmp * i);
        currentCentroid-> Alpha = 0xFF;
        currentCentroid++;
    }
    
    // NOTE: Do n iterations of k-means
    for (u32 n = 0; n < 1; n++) {
        for (s32 i = 0; i < clusterCount; i++)
            stack_clear(clusters + i);
        
        for (u32 y = 0; y < height; y++) {
            for (u32 x = 0; x < width; x++) {
                u32 i = width * y + x;
                RgbQuad* pixel = data + i;
                
                u32 minIndex = 0;
                f32 minDist = 256 * 256; // Larger than furthest distance in the colorspace
                for (u32 cent = 0; cent < clusterCount; cent++) {
                    RgbQuad* centroid = centroids + cent;
                    f32 dist = distance_manhattan(pixel, centroid);
                    
                    if (dist < minDist) {
                        minDist = dist;
                        minIndex = cent;
                    }
                }
                
                Stack* cluster = clusters + minIndex;
                stack_push(cluster, pixel);
            }
        }
        
        // NOTE: Recompute centroids from new clusters
        for (s32 i = 0; i < clusterCount; i++) {
            Stack* cluster = clusters + i;
            RgbQuad* clusterData = *((RgbQuad**) cluster->buffer);
            centroids[i] = centroid(clusterData, cluster->size);
        }
    }
    
    return centroids;
}

static inline RgbQuad*
palette_closets(RgbQuad* color, RgbQuad* palette, u32 paletteSize) {
    u32 minIndex = 0;
    f64 minSoFar = 1000;
    
    for (u32 i = 0; i < paletteSize; i++) {
        f64 dist = distance_manhattan(color, palette + i);
        
        if (dist < minSoFar) {
            minSoFar = dist;
            minIndex = i;
        }
    }
    
    return palette + minIndex;
}

static inline void
dither_floydsteinberg(RgbQuad* data, u32 width, u32 height, RgbQuad* palette, u32 paletteSize) {
    for (s64 y = 0; y < height; y++) {
        for (s64 x = 0; x < width; x++) {
            s64 i = width * y + x;
            
            RgbQuad* color = data + i;
            RgbQuad* palColor = palette_closets(color, palette, paletteSize);
            
            f32 errorR, errorG, errorB;
            errorR = ((color->Red - palColor->Red) / 255.0f);
            errorG = ((color->Green - palColor->Green) / 255.0f);
            errorB = ((color->Blue - palColor->Blue) / 255.0f);
            *color = *palColor;
            
            // NOTE: Push error to pixels we have not yet dithered
            RgbQuad* next;
            f32 pushR, pushG, pushB;
            if (x + 1 < width) {
                next = data + INDEX2D(x + 1, y, width);
                pushR = (f32) (next->Red + (errorR * 7.0 / 16.0) * 255.0);
                pushG = (f32) (next->Green + (errorG * 7.0 / 16.0) * 255.0);
                pushB = (f32) (next->Blue + (errorB * 7.0 / 16.0) * 255.0);
                
                next->Red = (u08) MIN(round(pushR), 255);
                next->Green = (u08) MIN(round(pushG), 255);
                next->Blue = (u08) MIN(round(pushB), 255);
            }
            if (x - 1 >= 0 && y + 1 < height) {
                next = data + INDEX2D(x - 1, y + 1, width);
                pushR = (f32) (next->Red + (errorR * 3.0 / 16.0) * 255.0);
                pushG = (f32) (next->Green + (errorG * 3.0 / 16.0) * 255.0);
                pushB = (f32) (next->Blue + (errorB * 3.0 / 16.0) * 255.0);
                
                next->Red = (u08) MIN(round(pushR), 255);
                next->Green = (u08) MIN(round(pushG), 255);
                next->Blue = (u08) MIN(round(pushB), 255);
            }
            if (y + 1 < height) {
                next = data + INDEX2D(x, y + 1, width);
                pushR = (f32) (next->Red + (errorR * 5.0 / 16.0) * 255.0);
                pushG = (f32) (next->Green + (errorG * 5.0 / 16.0) * 255.0);
                pushB = (f32) (next->Blue + (errorB * 5.0 / 16.0) * 255.0);
                
                next->Red = (u08) MIN(round(pushR), 255);
                next->Green = (u08) MIN(round(pushG), 255);
                next->Blue = (u08) MIN(round(pushB), 255);
            }
            if (x + 1 < width && y + 1 < height) {
                next = data + INDEX2D(x + 1, y + 1, width);
                pushR = (f32) (next->Red + (errorR * 1.0 / 16.0) * 255.0);
                pushG = (f32) (next->Green + (errorG * 1.0 / 16.0) * 255.0);
                pushB = (f32) (next->Blue + (errorB * 1.0 / 16.0) * 255.0);
                
                next->Red = (u08) MIN(round(pushR), 255);
                next->Green = (u08) MIN(round(pushG), 255);
                next->Blue = (u08) MIN(round(pushB), 255);
            }
        }
    }
}

#endif // BITMAP_H