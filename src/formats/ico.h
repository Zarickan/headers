#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "../types.h"

#ifndef ICO_H
#define ICO_H

#define ICONTYPE_ICON   1
#define ICONTYPE_CURSOR 2

#pragma pack(push, 1)
typedef struct IconHeader {
    u08 Reserved[2];
    u16 Type;
    u16 ImageCount;
} IconHeader;

typedef struct IconEntry {
    u08 Width;
    u08 Height;
    u08 PaletteSize;
    u08 Reserved;
    u16 ColorPlanes;
    u16 BitsPerPixel;
    u32 Size;
    u32 Offset;
} IconEntry;

typedef struct IconImage {
    u08  Width;
    u08  Height;
    u08  PaletteSize;
    u16  ColorPlanes;
    u16  BitsPerPixel;
    u32  Size;
    u08* Data;
} IconImage;
#pragma pack(pop)

typedef struct Icon {
    u16 ImageCount;
    IconImage* Images;
} Icon;

// Create a new icon witht the specified ammount of images allocated
static inline Icon
icon_create(u16 images) {
    Icon icon = { images  };
    icon.Images = malloc(sizeof(IconImage) * images);
    
    return icon;
}

// Intializes an icon and allocates the specified ammounts of images
static inline void
icon_init(Icon* icon, u16 images) {
    icon->ImageCount = images;
    icon->Images = malloc(sizeof(IconImage) * images);
}

// Frees an icon ands its images (and their data)
static inline void
icon_free(Icon* icon) {
    for(u16 i = 0; i < icon->ImageCount; i++) {
        free(icon->Images[i].Data);
    }
    
    free(icon->Images);
}

// Initializes and reads an icon from the given file
static inline void
icon_read(FILE* file, Icon* icon) {
    IconHeader header;
    fread(&header, sizeof(IconHeader), 1, file);
    icon->ImageCount = header.ImageCount;
    icon->Images = malloc(sizeof(IconImage) * header.ImageCount);
    
    IconEntry* entries = malloc(sizeof(IconEntry) * icon->ImageCount);
    fread(entries, sizeof(IconEntry), icon->ImageCount, file);
    
    for(u16 i = 0; i < icon->ImageCount; i++) {
        IconEntry  entry =  entries[i];
        IconImage* image = &icon->Images[i];
        
        image->Width = entry.Width;
        image->Height = entry.Height;
        image->PaletteSize = entry.PaletteSize;
        image->ColorPlanes = entry.ColorPlanes;
        image->BitsPerPixel = entry.BitsPerPixel;
        image->Size = entry.Size;
        
        image->Data = malloc(entry.Size);
        fseek(file, entry.Offset, SEEK_SET);
        fread(image->Data, sizeof(u08), entry.Size, file);
    }
    
    free(entries);
}

// Writes an icon to the given file
static inline void
icon_write(FILE* file, Icon* icon) {
    IconHeader header = {{0, 0}, ICONTYPE_ICON, icon->ImageCount};
    fwrite(&header, sizeof(IconHeader), 1, file);
    
    u64 offset = sizeof(IconHeader) + sizeof(IconEntry) * icon->ImageCount;
    for(u16 i = 0; i < icon->ImageCount; i++) {
        IconImage image = icon->Images[i];
        IconEntry entry = {
            image.Width,
            image.Height,
            image.PaletteSize,
            0,
            image.ColorPlanes,
            image.BitsPerPixel,
            image.Size,
            offset
        };
        
        fwrite(&entry, sizeof(IconEntry), 1, file);
        offset += image.Size;
    }
    
    for(u16 i = 0; i < icon->ImageCount; i++) {
        IconImage image = icon->Images[i];
        fwrite(image.Data, sizeof(u08), image.Size, file);
    }
}

static inline void
ico_read_header(IconHeader* header, FILE* file) {
    if(header == NULL) {
        header = malloc(sizeof(IconHeader));
    }
    
    fread(header, sizeof(IconHeader), 1, file);
}

static inline IconEntry*
ico_read_entries(IconHeader* header, FILE* file) {
    IconEntry* entries = malloc(sizeof(IconEntry) * header->ImageCount);
    
    for(u16 i = 0; i < header->ImageCount; i++) {
        IconEntry* entry = &entries[i];
        fread(entry, sizeof(IconEntry), 1, file);
    }
    
    return entries;
}

static inline IconEntry*
ico_read(IconHeader* header, FILE* file) {
    ico_read_header(header, file);
    return ico_read_entries(header, file);
}

static inline void
ico_write_header(IconHeader* header, FILE* file) {
    fwrite(header, sizeof(IconHeader), 1, file);
}

static inline void
ico_write_entries(IconHeader* header, IconEntry* entries, FILE* file) {
    for(u16 i = 0; i < header->ImageCount; i++) {
        IconEntry* entry = &entries[i];
        fwrite(entry, sizeof(IconEntry), 1, file);
    }
}

static inline void
ico_write(IconHeader* header, IconEntry* entries, FILE* file) {
    ico_write_header(header, file);
    ico_write_entries(header, entries, file);
}

#endif // ICO_H