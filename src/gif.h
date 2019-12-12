#ifndef GIF_H
#define GIF_H

#include <stdlib.h>
#include <stdio.h>
#include "types.h"

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


typedef struct lzw_block {
    u08 MinimumCodeSize;
    
    u08 *data;
	u08 dataSize;
} LzwBlock;

#pragma pack(pop)

// Helpers
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

static u08
bits_in_code(u16 code) {
	u08 shiftCount = 0;
	while (code != 0) {
		code >>= 1;
		++shiftCount;
	}

	return shiftCount;
}

static u16
read_bits(u08** data, const u08 bits, u08* bitPosition) {
	u16 result = 0;

	for (s16 i = 0; i < bits; i++) {
		const u08 bitMask = PowersOfTwo[*bitPosition];
		const u08 value = **data & bitMask;

		if (value)
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

// ------------------------------------------------------------------------------------------
// LZW Decoding
typedef u16 LzwCode;

typedef struct lzw_table {
	LzwCode code;

	u16 dataSize;
	u08 *data;

	struct lzw_table* previous;
	struct lzw_table* next;
} LzwTable;

typedef struct lzw_context
{
	struct lzw_table *table;

	u08 minimumCodeSize, currentCodeSize;
	LzwCode clearCode, endCode, nextCode;

	u16 paletteSize;

	u32 dataSize;
	u08 *data;
	u08 dataBitPosition;
} LzwContext;

static void
lzw_ctx_create(struct lzw_context *ctx, LzwBlock *block, u16 paletteSize) {
	ctx->table = NULL;

	ctx->data = block->data;
	ctx->dataSize = block->dataSize;
	ctx->dataBitPosition = 0;

	ctx->paletteSize = paletteSize;
	ctx->minimumCodeSize = block->MinimumCodeSize;
	ctx->currentCodeSize = block->MinimumCodeSize + 1;

	ctx->clearCode = PowersOfTwo[ctx->minimumCodeSize];
	ctx->endCode = ctx->clearCode + 1;
	ctx->nextCode = ctx->endCode + 1;
}

static void
lzw_ctx_free(struct lzw_context *ctx) {
	struct lzw_table *tail = ctx->table;
	while (tail->next != NULL) tail = tail->next;

	for (struct lzw_table *current = tail; current != NULL;) {
		if(current->data != NULL) {
			free(current->data);
		}

		tail = current;
		current = current->previous;

		free(tail);
	}
}

static void
lzw_ctx_create_table(struct lzw_context *ctx) {
	struct lzw_table *codes = malloc(sizeof(struct lzw_table) * ctx->paletteSize);

	struct lzw_table *previous = NULL;
	for (u32 i = 0; i < ctx->paletteSize; ++i) {
		codes[i].code = i;
		codes[i].dataSize = 1;

		codes[i].data = malloc(sizeof(u08));
		codes[i].data[0] = i;

		codes[i].previous = previous;
		if (previous != NULL) {
			previous->next = &codes[i];
		}
		codes[i].next = NULL;

		previous = &codes[i];
	}

	ctx->currentCodeSize = ctx->minimumCodeSize + 1;
	ctx->clearCode = PowersOfTwo[ctx->minimumCodeSize];
	ctx->endCode = ctx->clearCode + 1;
	ctx->nextCode = ctx->endCode + 1;

	ctx->table = previous;
}

static struct lzw_table *
lzw_ctx_find_code(struct lzw_context *ctx, const LzwCode code) {
	struct lzw_table *current = ctx->table;

	while (current != NULL) {
		if (current->code < code) {
			current = current->next;
		}
		else if (current->code > code) {
			current = current->previous;
		}
		else {
			return current;
		}
	}

	return NULL;
}

static void
lzw_ctx_insert_next_code(struct lzw_context *ctx, u08 *data, const u16 dataSize) {
	struct lzw_table *tail = ctx->table;
	while (tail->next != NULL) tail = tail->next;

	struct lzw_table *newNode = malloc(sizeof(struct lzw_table));
	newNode->code = ctx->nextCode++;
	if(ctx->nextCode == ctx->clearCode) {
		ctx->nextCode = ctx->endCode + 1;
	}
	
	newNode->dataSize = dataSize;
	newNode->data = data;

	newNode->previous = tail;
	newNode->next = NULL;

	const u16 newCodeSize = bits_in_code(ctx->nextCode);
	if(newCodeSize > ctx->currentCodeSize) {
		ctx->currentCodeSize = newCodeSize;
	}

	ctx->table = newNode;
}

static void
lzw_ctx_insert_unknown(struct lzw_context *ctx, struct lzw_table *local) {
	const u16 dataSize = local->dataSize + 1;
	u08 *data = malloc(sizeof(u08) * dataSize);

	memcpy(data, local->data, local->dataSize);
	data[dataSize - 1] = local->data[0];

	lzw_ctx_insert_next_code(ctx, data, dataSize);
}

static void
lzw_ctx_insert_known(struct lzw_context *ctx, struct lzw_table *local, struct lzw_table *current) {
	const u16 dataSize = local->dataSize + 1;
	u08 *data = malloc(sizeof(u08) * dataSize);

	memcpy(data, local->data, local->dataSize);
	data[dataSize - 1] = current->data[0];

	lzw_ctx_insert_next_code(ctx, data, dataSize);
}

static void 
print_node(struct lzw_table *node) {
	printf("%3x: ", node->code);
	for(u32 i = 0; i < node->dataSize; ++i) {
		printf("%2x ", node->data[i]);
	}
	printf("\n");
}

static void
lzw_ctx_write_node_to_buffer(struct lzw_table *node, Rgb *palette, u16 paletteSize, RgbQuad **output) {
	for (u16 i = 0; i < node->dataSize; ++i) {
		printf("Outputting: %x\n", node->data[i]);

		const Rgb paletteColor = palette[node->data[i]];
		const RgbQuad rgb = {
			.Blue = paletteColor.Blue,
			.Green = paletteColor.Green,
			.Red = paletteColor.Red,
			.Alpha = 0xFF
		};

		**output = rgb;
		++(*output);
	}
	
}

static void
lzw_block_decode(struct lzw_block *block, Rgb *palette, u16 paletteSize, RgbQuad **output) {
	struct lzw_context context;
	lzw_ctx_create(&context, block, paletteSize);

	s32 c = 0;
	for(struct lzw_table *local = NULL; context.data - block->data < block->dataSize;) {
		const LzwCode code = read_bits(&context.data, context.currentCodeSize, &context.dataBitPosition);
		printf("\nRead %3x (codesize %i) read %2x\n", code, context.currentCodeSize, ++c);

		// END / CLEAR
		if (code == context.endCode) {
			printf("END\n");
			break;
		}
		if(code == context.clearCode) {
			printf("CLEAR\n");
			lzw_ctx_create_table(&context);
			local = NULL;

			continue;
		}
		
		struct lzw_table *current = lzw_ctx_find_code(&context, code);

		// Unknown code
		if(current == NULL) {
			lzw_ctx_insert_unknown(&context, local);
			print_node(context.table);

			lzw_ctx_write_node_to_buffer(context.table, palette, paletteSize, output);

			local = context.table;
			continue;
		}

		// Known code
		if (local != NULL) {
			lzw_ctx_insert_known(&context, local, current);
			print_node(context.table);
		}

		local = current;
		lzw_ctx_write_node_to_buffer(local, palette, paletteSize, output);
	}

	//lzw_ctx_free(&context);
}

// ------------------------------------------------------------------------------------------

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
            //result->Red = color->Red;
            //result->Green = color->Green;
            //result->Blue = color->Blue;
			//result->Alpha = 0xFF;
			result->Red = 0xFF;
			result->Green = 0;
			result->Blue = 0xFF;
			result->Alpha = 0xFF;
            result++;
        }
    }
    
    // NOTE: Read all extension descriptors until we encounter image descriptors
    char seperator;
	do {
		// TODO: We currently ignore extension, properly read and use them instead
		fread(&seperator, sizeof(char), 1, file);
        if(seperator != '!') {
            // NOTE: If the separator is != '!' then it should be 0x00
            //       (no more extensions follow)
            //       Hence we do not need to seek back 1 byte
			fseek(file, -1, SEEK_CUR);
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

		// Read the separator, skip if the separator != !
		fread(&seperator, sizeof(char), 1, file);
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

		// Read chunks of lzw data
		u08 buffer[255];
		u08 minimumCodeSize, chunkSize ;
		fread(&minimumCodeSize, sizeof(u08), 1, file);
		printf("    LzwChunks:\n");
		printf("      MinimumCodeSize:          %u\n", minimumCodeSize);

		struct lzw_block block = { .MinimumCodeSize = minimumCodeSize };
		do {
			fread(&chunkSize, sizeof(u08), 1, file);
			printf("      Chunk:          %u\n", chunkSize);
			fread(&buffer, sizeof(u08), chunkSize, file);

			block.data = buffer;
			block.dataSize = chunkSize;
			lzw_block_decode(&block, globalColorMap, globalColorMapSize, &resultRgb);
		} while (chunkSize != 0);
		printf("  EOI: %x\n", chunkSize);
        
        // TODO: Free what we allocate
    } while (seperator == 0x3B);
    
    u08 final = 0;
    fread(&final, sizeof(u08), 1, file);
    printf("EOF: %x\n", final);
    
    return resultData;
}

static inline void
gif_save(FILE* file, s32 width, s32 height, const u08* data) {
}

#endif // GIF_H