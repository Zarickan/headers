#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "types.h"

#ifndef BITMAP_H
#define BITMAP_H

#define BITMAP_VUNKNOWN 0xFF
#define BITMAP_V0       0x00
#define BITMAP_V1       0x01
#define BITMAP_V2       0x02
#define BITMAP_V3       0x03
#define BITMAP_V4       0x04
#define BITMAP_V5       0x05

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

#define DPI72 2835

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

#define bitmap_version(info) bitmap_get_version((BitmapCoreHeader*) info)
#define bitmap_colorcount(info) bitmap_get_colorcount((BitmapCoreHeader*) info)
#define bitmap_read_info(header, info, file) bitmap_read_info_from_file(header, (BitmapCoreHeader*) info, file)

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
    return pow(2, header->BitCount * header->Planes);
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
bitmap_create_core(Bitmap* bitmap, s32 height, s32 width) {
    BitmapCoreHeader* info = malloc(sizeof(BitmapCoreHeader));
    info->Size = sizeof(BitmapCoreHeader);
    info->Width = width;
    info->Height = height;
    info->Planes = 0;
    info->BitCount = 24;
    
    u32 size = width * height * sizeof(RgbTriple);
    
    bitmap->Info = (BitmapInfoHeader*) info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + size;
    
    bitmap->Data.Rgba = malloc(size);
}

static inline void
bitmap_create_v1(Bitmap* bitmap, s32 height, s32 width) {
    BitmapInfoHeader* info = malloc(sizeof(BitmapInfoHeader));
    info->Size = sizeof(BitmapInfoHeader);
    info->Width = width;
    info->Height = height;
    info->Planes = 0;
    info->BitCount = 32;
    info->Compression = BI_RGB;
    info->SizeImage = sizeof(RgbQuad) * height * width;
    info->XPelsPerMeter = DPI72;
    info->YPelsPerMeter = DPI72;
    info->UsedColors = 0x00;
    info->ImportantColors = 0x00;
    
    bitmap->Info = (BitmapInfoHeader*) info;
    
    bitmap->Header.Id = 0x4D42;
    bitmap->Header.Reserved[0] = 0;
    bitmap->Header.Reserved[1] = 0;
    bitmap->Header.Offset = sizeof(BitmapHeader) + info->Size;
    bitmap->Header.Size = bitmap->Header.Offset + info->SizeImage;
    
    bitmap->Data.Rgba = malloc(info->SizeImage);
}

static inline void
bitmap_create_v2(Bitmap* bitmap, s32 height, s32 width) {
    BitmapInfoV2Header* info = malloc(sizeof(BitmapInfoV2Header));
    info->Size = sizeof(BitmapInfoV2Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 0;
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
bitmap_create_v3(Bitmap* bitmap, s32 height, s32 width) {
    BitmapInfoV3Header* info = malloc(sizeof(BitmapInfoV3Header));
    info->Size = sizeof(BitmapInfoV3Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 0;
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
bitmap_create_v4(Bitmap* bitmap, s32 height, s32 width) {
    BitmapInfoV4Header* info = malloc(sizeof(BitmapInfoV4Header));
    info->Size = sizeof(BitmapInfoV4Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 0;
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
bitmap_create_v5(Bitmap* bitmap, s32 height, s32 width) {
    BitmapInfoV5Header* info = malloc(sizeof(BitmapInfoV5Header));
    info->Size = sizeof(BitmapInfoV5Header);
    info->Width = width;
    info->Height = height;
    info->Planes = 0;
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
    
    bitmap->Data.Rgba = malloc(info->SizeImage);
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
    fwrite(bitmap->Data.Bytes, sizeof(u08), bitmap->Info->SizeImage, file);
}

static inline void
display_palette(BitmapInfoHeader* header, u08* data) {
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

#endif // BITMAP_H