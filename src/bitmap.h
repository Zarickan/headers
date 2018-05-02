#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "types.h"

#ifndef BITMAP_H
#define BITMAP_H

// Bitmap versions
#define BITMAP_VUNKNOWN 0xFF
#define BITMAP_VCORE    0x00
#define BITMAP_V0       0x00
#define BITMAP_V1       0x01
#define BITMAP_V2       0x02
#define BITMAP_V3       0x03
#define BITMAP_V4       0x04
#define BITMAP_V5       0x05

// Compression mods, use BI_RGB for raw RGB data with no compression
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

// DPI Values
#define DPI(pixels_per_meter) ((u32) 39.37008 * (pixels_per_meter))
#define DPI72   DPI(72)
#define DPI96   DPI(96)
#define DPI150  DPI(150)
#define DPI300  DPI(300)
#define DPI2540 DPI(2540)
#define DPI4000 DPI(4000)

#pragma pack(push,1)

typedef struct RgbTriple {
    u08 Blue;
    u08 Green;
    u08 Red;
} RgbTriple;

typedef struct RgbQuad {
    u08 Blue;
    u08 Green;
    u08 Red;
    u08 Alpha;
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
    u16 Id;
    u32 Size;
    u16 Reserved[2];
    u32 Offset;
}  BitmapHeader;

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
    u32       ColorSpaceType;
    CIETriple Endpoints;
    u32       GammaRed;
    u32       GammaGreen;
    u32       GammaBlue;
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
    u32       ColorSpaceType;
    CIETriple Endpoints;
    u32       GammaRed;
    u32       GammaGreen;
    u32       GammaBlue;
    
    // V5
    u32 Intent;
    u32 ProfileData;
    u32 ProfileSize;
    u32 Reserved;
} BitmapInfoV5Header;

#pragma pack(pop)

typedef struct Bitmap {
    BitmapHeader      Header;
    BitmapInfoHeader* Info;
    
    union Data {
        u08*       Bytes;
        RgbTriple* Rgb;
        RgbQuad*   Rgba;
    } Data;
} Bitmap;

#define bitmap_version(info) bitmap_get_version((BitmapCoreHeader*) (info))
#define bitmap_colorcount(info) bitmap_get_colorcount((BitmapCoreHeader*) (info))
#define bitmap_read_info(header, info, file) bitmap_read_info_from_file(header, (BitmapCoreHeader*) (info), file)

static inline u08
bitmap_get_version(BitmapCoreHeader* infoHeader) {
    switch(infoHeader->Size) {
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
bitmap_get_colorcount(BitmapCoreHeader* header) {
    return (u64) pow(2, header->BitCount * header->Planes);
}

static inline void
bitmap_read_info_from_file(BitmapHeader* header, BitmapCoreHeader* info, FILE* file) {
    fread(header, sizeof(BitmapHeader), 1, file);
    fread(info, sizeof(BitmapCoreHeader), 1, file);
    
    // NOTE: Read remaining fields depending on the bitmap version
    u32 v0InfoSize = sizeof(BitmapCoreHeader);
    if (info->Size > v0InfoSize) {
        u08* infoPtr = (u08*) info;
        u32  infoSize = info->Size - sizeof(BitmapCoreHeader);
        assert(infoSize > 0);
        
        fread(infoPtr + v0InfoSize, sizeof(u08), infoSize, file);
    }
}

static inline void
bitmap_create_core(Bitmap* bitmap, s16 width, s16 height) {
    BitmapCoreHeader* info = malloc(sizeof(BitmapCoreHeader));
    info->Size = sizeof(BitmapCoreHeader);
    info->Width = width;
    info->Height = height;
    info->Planes = 1;
    info->BitCount = 24;
    
    u32 size = width * height * sizeof(RgbTriple);
    
    bitmap->Info = (BitmapInfoHeader*) info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + size;
    
    bitmap->Data.Rgb = malloc(size);
}

static inline void
bitmap_create_v1(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoHeader* info = malloc(sizeof(BitmapInfoHeader));
    info->Size = sizeof(BitmapInfoHeader);
    info->Width = width;
    info->Height = height;
    info->Planes = 1;
    info->BitCount = 32;
    info->Compression = BI_RGB;
    info->SizeImage = sizeof(RgbQuad) * height * width;
    info->XPelsPerMeter = DPI72;
    info->YPelsPerMeter = DPI72;
    info->UsedColors = 0x00;
    info->ImportantColors = 0x00;
    
    bitmap->Info = info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + info->SizeImage;
    
    bitmap->Data.Rgba = malloc(info->SizeImage);
}

static inline void
bitmap_create_v2(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoV2Header* info = malloc(sizeof(BitmapInfoV2Header));
    info->Size = sizeof(BitmapInfoV2Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 1;
    info->BitCount = 32;
    info->Compression = BI_BITFIELDS;
    info->SizeImage = sizeof(RgbQuad) * height * width;
    info->XPelsPerMeter = DPI72;
    info->YPelsPerMeter = DPI72;
    info->UsedColors = 0x00;
    info->ImportantColors = 0x00;
    info->RedMask   = 0x00FF0000;
    info->GreenMask = 0x0000FF00;
    info->BlueMask  = 0x000000FF;
    
    bitmap->Info = (BitmapInfoHeader*) info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + info->SizeImage;
    
    bitmap->Data.Rgba = malloc(info->SizeImage);
}

static inline void
bitmap_create_v3(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoV3Header* info = malloc(sizeof(BitmapInfoV3Header));
    info->Size = sizeof(BitmapInfoV3Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 1;
    info->BitCount = 32;
    info->Compression = BI_BITFIELDS;
    info->SizeImage = sizeof(RgbQuad) * height * width;
    info->XPelsPerMeter = DPI72;
    info->YPelsPerMeter = DPI72;
    info->UsedColors = 0x00;
    info->ImportantColors = 0x00;
    info->RedMask   = 0x00FF0000;
    info->GreenMask = 0x0000FF00;
    info->BlueMask  = 0x000000FF;
    info->AlphaMask = 0xFF000000;
    
    bitmap->Info = (BitmapInfoHeader*) info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + info->SizeImage;
    
    bitmap->Data.Rgba = malloc(info->SizeImage);
}

static inline void
bitmap_create_v4(Bitmap* bitmap, s32 width, s32 height) {
    BitmapInfoV4Header* info = malloc(sizeof(BitmapInfoV4Header));
    info->Size = sizeof(BitmapInfoV4Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 1;
    info->BitCount = 32;
    info->Compression = BI_BITFIELDS;
    info->SizeImage = sizeof(RgbQuad) * height * width;
    info->XPelsPerMeter = DPI72;
    info->YPelsPerMeter = DPI72;
    info->UsedColors = 0x00;
    info->ImportantColors = 0x00;
    info->RedMask   = 0x00FF0000;
    info->GreenMask = 0x0000FF00;
    info->BlueMask  = 0x000000FF;
    info->AlphaMask = 0xFF000000;
    info->ColorSpaceType = 0x206E6957;
    memset(&info->Endpoints, 0, sizeof(CIETriple)); // NOTE: Ignored for the use ColorSpaceType (see specification)
    info->GammaRed   = 0;
    info->GammaGreen = 0;
    info->GammaBlue  = 0;
    
    bitmap->Info = (BitmapInfoHeader*) info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + info->SizeImage;
    
    bitmap->Data.Rgba = malloc(info->SizeImage);
}

static inline void
bitmap_create_v5(Bitmap* bitmap, s32 width, s32 height) {
    u32 rowSize = (u32) (floor((32.0 * width + 31.0) / 32.0) * 4.0);
    u32 dataSize = rowSize * height;
    
    BitmapInfoV5Header* info = malloc(sizeof(BitmapInfoV5Header));
    info->Size = sizeof(BitmapInfoV5Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 1;
    info->BitCount = 32;
    info->Compression = BI_BITFIELDS;
    info->SizeImage = dataSize;
    info->XPelsPerMeter = DPI72;
    info->YPelsPerMeter = DPI72;
    info->UsedColors = 0x00;
    info->ImportantColors = 0x00;
    info->RedMask   = 0x00FF0000;
    info->GreenMask = 0x0000FF00;
    info->BlueMask  = 0x000000FF;
    info->AlphaMask = 0xFF000000;
    info->ColorSpaceType = 0x206E6957;
    memset(&info->Endpoints, 0, sizeof(CIETriple)); // NOTE: Ignored for the use ColorSpaceType (see specification)
    info->GammaRed   = 0;
    info->GammaGreen = 0;
    info->GammaBlue  = 0;
    info->Intent = 0;
    info->ProfileData = 0;
    info->ProfileSize = 0;
    info->Reserved = 0;
    
    bitmap->Info = (BitmapInfoHeader*) info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + info->SizeImage;
    
    bitmap->Data.Bytes = malloc(info->SizeImage);
}

static inline void
create_bitmap(Bitmap* bitmap, BitmapHeader* header, BitmapInfoHeader* bitmapInfo, u08* data) {
    // TODO: Creating different types of bitmaps, BM, BA etc.?
    header->Id = 0x4D42;
    header->Size = sizeof(BitmapHeader) + bitmapInfo->Size + bitmapInfo->SizeImage;
    header->Reserved[0] = 0;
    header->Reserved[1] = 0;
    header->Offset = sizeof(BitmapHeader) + bitmapInfo->Size;
    
    bitmap->Header = *header;
    bitmap->Info = bitmapInfo;
    bitmap->Data.Bytes = data;
}

static inline void
bitmap_free(Bitmap* bitmap) {
    free(bitmap->Info);
    free(bitmap->Data.Bytes);
    free(bitmap);
}

static inline void
write_bitmap_to_file(Bitmap* bitmap, FILE* file) {
    assert(bitmap->Info->Size != 0);
    fwrite(&bitmap->Header, sizeof(BitmapHeader), 1, file);
    fwrite(bitmap->Info, bitmap->Info->Size, 1, file);
    
    if (bitmap_version(bitmap->Info) == BITMAP_VCORE) {
        u32 size = (u32) (bitmap->Info->Width * bitmap->Info->Height * (bitmap->Info->BitCount / 8));
        
        fwrite(bitmap->Data.Bytes, sizeof(u08), size, file);
        return;
    }
    
    fwrite(bitmap->Data.Bytes, sizeof(u08), bitmap->Info->SizeImage, file);
}

static inline void
display_palette(BitmapInfoHeader* header) {
    //assert(header->UsedColors != 0x00);
    if(header->UsedColors == 0x00) return;
    
    // TODO: Handle BitmapCoreHeader using RgbTriple for colors instead of RgbQuad
    RgbQuad* palette = malloc(sizeof(RgbQuad) * header->UsedColors);
    
    for (u16 i = 0; i < header->UsedColors; i++) {
        RgbQuad rgb = palette[i];
        u32 color = *((u32*) palette + i);
        printf("%2x : %8x [%2x %2x %2x]\n",i , color, rgb.Red, rgb.Green, rgb.Blue);
    }
}

static inline void
display_bitmapinfo(BitmapInfoHeader* header) {
    printf("  BitmapInfoHeader:\n");
    printf("  Version:        %i\n", bitmap_get_version((BitmapCoreHeader*) header));
    printf("  Color count:    %llu\n", bitmap_get_colorcount((BitmapCoreHeader*) header));
    printf("  Uses palette:   %s\n", header->UsedColors > 0 ? "true" : "false");
    printf("    Size:            0x%x\n", header->Size);
    printf("    Width:           0x%x\n", header->Width);
    printf("    Height:          0x%x\n", header->Height);
    printf("    Planes:          0x%x\n", header->Planes);
    printf("    BitCount:        0x%x\n", header->BitCount);
    printf("    Compression:     0x%x\n", header->Compression);
    printf("    SizeImage:       0x%x\n", header->SizeImage);
    printf("    XPelsPerMeter:   0x%x\n", header->XPelsPerMeter);
    printf("    YPelsPerMeter:   0x%x\n", header->YPelsPerMeter);
    printf("    UsedColors:      0x%x\n", header->UsedColors);
    printf("    ImportantColors: 0x%x\n", header->ImportantColors);
}

static inline void
log_bitmapinfo(BitmapInfoHeader* header, FILE* file) {
    fprintf(file, "  BitmapInfoHeader:\n");
    fprintf(file, "  Version:        %i\n", bitmap_get_version((BitmapCoreHeader*) header));
    fprintf(file, "  Color count:    %llu\n", bitmap_get_colorcount((BitmapCoreHeader*) header));
    fprintf(file, "  Uses palette:   %s\n", header->UsedColors > 0 ? "true" : "false");
    fprintf(file, "    Size:            0x%x\n", header->Size);
    fprintf(file, "    Width:           0x%x\n", header->Width);
    fprintf(file, "    Height:          0x%x\n", header->Height);
    fprintf(file, "    Planes:          0x%x\n", header->Planes);
    fprintf(file, "    BitCount:        0x%x\n", header->BitCount);
    fprintf(file, "    Compression:     0x%x\n", header->Compression);
    fprintf(file, "    SizeImage:       0x%x\n", header->SizeImage);
    fprintf(file, "    XPelsPerMeter:   0x%x\n", header->XPelsPerMeter);
    fprintf(file, "    YPelsPerMeter:   0x%x\n", header->YPelsPerMeter);
    fprintf(file, "    UsedColors:      0x%x\n", header->UsedColors);
    fprintf(file, "    ImportantColors: 0x%x\n", header->ImportantColors);
}

u16
power(u16 base, u16 power) {
    if (power == 0) return 1;
    
    u16 result = base;;
    for (u16 n = 1; n < power; n++)
        result *= base;
    
    return result;
}

static inline u08*
bitmap_load_core(FILE* file, s32* width, s32* height) {
    BitmapHeader header;
    BitmapCoreHeader info;
    bitmap_read_info(&header, &info, file);
    
    *width = info.Width;
    *height = info.Height;
    
    u32 rowSize = (u32) (floor((info.BitCount * info.Width + 32.0) / 31.0) * 4);
    u32 rowOffset = (info.Width * info.BitCount / 8) - rowSize;
    u32 dataSize = rowSize * info.Height;
    
    u08* data = malloc(dataSize);
    fread(data, sizeof(u08), dataSize, file);
    
    u08* result = malloc(info.Width * info.Height * sizeof(RgbQuad));
    RgbQuad* rgb = (RgbQuad*) result;
    
    // NOTE: Core bitmaps are either (1, 4 or 8) bpp bitmaps using the colortable or 24 bpp BGR bitmaps
    assert(info.BitCount == 1 || info.BitCount == 4 || info.BitCount == 8 || info.BitCount == 24);
    if (info.BitCount == 24) {
        u08* pixelData = malloc(dataSize);
        fread(pixelData, sizeof(u08), dataSize, file);
        
        // TODO: Handle 24bpp core bitmaps
        for (s32 row = 0; row < info.Height; row++) {
            for (s32 column = 0; column < rowSize - rowOffset; column++) {
                memcpy(rgb, pixelData, sizeof(RgbTriple));
                rgb->Alpha = 0xFF;
                
                rgb++;
                pixelData += sizeof(RgbTriple);
            }
            
            pixelData += rowOffset;
        }
    }
    else {
        // TODO: Handle missing colorcount, IE set to 0?
        u32 colorCount = power(2, info.BitCount * info.Planes);
        RgbTriple* colors = malloc(sizeof(RgbTriple) * colorCount);
        fread(colors, sizeof(RgbQuad), colorCount, file);
        
        s32 pixelMask = power(2, info.BitCount) - 1;
        u08* pixelData = malloc(dataSize);
        fread(pixelData, sizeof(u08), dataSize, file);
        
        // NOTE: Number of columns in the image (width in bytes)
        s32 columns = rowSize - rowOffset;
        
        // NOTE: Number of "spare" bits at the end of the last byte in each row
        s32 bitsMod8 = info.Width * info.BitCount % 8;
        s32 remainingBits = bitsMod8 == 0 ? 8 : bitsMod8;
        
        // NOTE: For each row, column (bytes) then for each pixel in the bytes (if bpp < 8)
        for (s32 row = 0; row < info.Height; row++) {
            for (s32 column = 0; column < columns; column++) {
                // NOTE: Ammount of colors / pixels in each byte (differs for the last byte depending on pixel width)
                // Extrashift is the extra shift used for the last byte (since data is stored little endian)
                s32 colorsInByte = (column + 1 == columns ? remainingBits : 8) / info.BitCount;
                s32 extraShift  = (column + 1 == columns ? 8 / info.BitCount - colorsInByte : 0);
                
                for(s32 j = colorsInByte - 1; j >= 0; j--) {
                    s32 mask = (pixelMask << (j + extraShift) * info.BitCount);
                    s32 shift = (j + extraShift) * info.BitCount;
                    s32 pixel = (*pixelData & mask) >> shift;
                    
                    memcpy(rgb, colors + pixel, sizeof(RgbTriple));
                    rgb->Alpha = 0xFF;
                    rgb++;
                }
                
                pixelData++;
            }
            
            pixelData += rowOffset;
        }
    }
    
    return result;
}

static inline u08*
bitmap_load(FILE* file, s32* width, s32* height) {
    BitmapHeader header;
    union {
        BitmapCoreHeader core;
        BitmapInfoHeader v1;
        BitmapInfoV2Header v2;
        BitmapInfoV3Header v3;
        BitmapInfoV4Header v4;
        BitmapInfoV5Header v5;
    } info;
    bitmap_read_info(&header, &info.core, file);
    
    // NOTE: Core bitmaps have a different info header, so we handle them seperately
    u32 version = bitmap_version(&info.core);
    if (version == BITMAP_VCORE) {
        return bitmap_load_core(file, width, height);
    }
    
    *width = info.v1.Width;
    *height = ABS(info.v1.Height);
    
    // NOTE: Rows are fit to a DWORD (4 byte) boundary (32 is BPP of output, 31 is bpp - 1 bit)
    u32 rowSize = (u32) (floor((info.v1.BitCount * info.v1.Width + 31.0) / 32.0) * 4);
    u32 rowOffset = (u32) (rowSize - ceil(info.v1.Width * info.v1.BitCount / 8.0));
    u32 dataSize = rowSize * ABS(info.v1.Height);
    
    // NOTE: If row is negative image is in the oposite direction, so reverse this point
    u08* result = malloc(info.v1.Width * ABS(info.v1.Height) * sizeof(RgbQuad));
    RgbQuad* rgb = (RgbQuad*) result;
    
    if (info.v1.BitCount < 16) {
        u32 maxColorCount = power(2, info.v1.BitCount * info.v1.Planes);
        u32 colorCount = MIN(info.v1.UsedColors, maxColorCount);
        colorCount = colorCount == 0 ? maxColorCount : colorCount;
        
        RgbQuad* colors = malloc(sizeof(RgbQuad) * colorCount);
        fread(colors, sizeof(RgbQuad), colorCount, file);
        
        s32 pixelMask = power(2, info.v1.BitCount) - 1;
        u08* data = malloc(dataSize);
        u08* pixelData = data;
        fread(pixelData, sizeof(u08), dataSize, file);
        
        // NOTE: Number of columns in the image (width in bytes)
        s32 columns = rowSize - rowOffset;
        
        // NOTE: Number of "spare" bits at the end of the last byte in each row
        s32 remainingBits = info.v1.Width * info.v1.BitCount % 8;
        remainingBits = remainingBits == 0 ? 8 : remainingBits;
        
        // NOTE: For each row, column (bytes) then for each pixel in the bytes (if bpp < 8)
        for (s32 row = 0; row < ABS(info.v1.Height); row++) {
            for (s32 column = 0; column < columns; column++) {
                // NOTE: Ammount of colors / pixels in each byte (differs for the last byte depending on pixel width)
                // Extrashift is the extra shift used for the last byte (since data is stored little endian)
                s32 colorsInByte = (column + 1 == columns ? remainingBits : 8) / info.v1.BitCount;
                s32 extraShift  = (column + 1 == columns ? 8 / info.v1.BitCount - colorsInByte : 0);
                
                for(s32 j = colorsInByte - 1; j >= 0; j--) {
                    s32 mask = (pixelMask << (j + extraShift) * info.v1.BitCount);
                    s32 shift = (j + extraShift) * info.v1.BitCount;
                    s32 pixel = (*pixelData & mask) >> shift;
                    
                    if (pixel > colorCount)
                        pixel = 0;
                    
                    RgbQuad color = *(colors + pixel);
                    
                    // NOTE: In colortables alpha is inverted (for some odd reason?)
                    *rgb = color;
                    rgb->Alpha = 0xFF - rgb->Alpha;
                    rgb++;
                }
                
                pixelData++;
            }
            
            pixelData += rowOffset;
        }
        
        // NOTE: Swap the rows for images that are top-down
        // TODO: Optimization, can this be done while reading the data?
        if (info.v1.Height < 0) {
            u32* rawData = (u32*) result;
            for (s32 row = 0; row < (*height) / 2; row++) {
                for (s32 column = 0; column < (*width); column++) {
                    u32 src = (*width) * row + column;
                    u32 tgt = (*width) * ((*height) - row - 1) + column;
                    
                    rawData[src] ^= rawData[tgt];
                    rawData[tgt] ^= rawData[src];
                    rawData[src] ^= rawData[tgt];
                }
            }
        }
        
        free(data);
        free(colors);
    }
    // NOTE: 16bpp RGB images
    else if (info.v1.Compression == BI_RGB && info.v1.BitCount == 16) {
        assert(dataSize == info.v1.SizeImage);
        
        u16 redMask   = 0xF800;
        u16 greenMask = 0x07C0;
        u16 blueMask  = 0x003E;
        u16 alphaMask = 0x0000;
        u16 xMask     = 0x0001;
        
        // TODO: Extract
    }
    // NOTE: 24bpp RGB images
    else if (info.v1.Compression == BI_RGB && info.v1.BitCount == 24) {
        
        u32 redMask   = 0xFF0000;
        u32 greenMask = 0x00FF00;
        u32 blueMask  = 0x0000FF;
        u32 alphaMask = 0x000000;
        u32 xMask     = 0x000000;
        
        // TODO: Extract
    }
    // NOTE: 32bpp RGB images
    else if (info.v1.Compression == BI_RGB && info.v1.BitCount == 24) {
        
        u32 redMask   = 0xFF000000;
        u32 greenMask = 0x00FF0000;
        u32 blueMask  = 0x0000FF00;
        u32 alphaMask = 0x00000000;
        u32 xMask     = 0x000000FF;
        
        // TODO: Extract
    }
    
    return result;
}

static inline void
bitmap_save(FILE* file, s32 width, s32 height, u08* data) {
    Bitmap bitmap;
    bitmap_create_v1(&bitmap, width, height);
    
    u32 rowSize = (bitmap.Info->SizeImage / sizeof(RgbQuad)) / bitmap.Info->Height;
    u32 rowOffset = rowSize - bitmap.Info->Width;
    
    u08* output = bitmap.Data.Bytes;
    for (s32 row = 0; row < bitmap.Info->Height; row++) {
        for (s32 x = 0; x < bitmap.Info->Width; x++) {
            RgbQuad* rgb = (RgbQuad*) output;
            *rgb = *((RgbQuad*) data);
            
            data += sizeof(RgbQuad);
            output += sizeof(RgbQuad);
        }
        output += rowOffset;
    }
    
    write_bitmap_to_file(&bitmap, file);
    free(bitmap.Info);
    free(bitmap.Data.Bytes);
}

#endif // BITMAP_H