#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#ifndef TYPES_H
#define TYPES_H

typedef size_t size;
typedef intptr_t intptr;
typedef uintptr_t uintptr;

// Integers
typedef uint8_t  u8;
typedef uint8_t  u08;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

// Floating point
typedef float  real32;
typedef double real64;
typedef real32 f32;
typedef real64 f64;
typedef real32 r32;
typedef real64 r64;

// Vectors
// (as done in Handmade Hero)
typedef union v2
{
    struct
    {
        f32 x, y;
    };
    struct
    {
        f32 u, v;
    };
} v2;
typedef union v3
{
    struct
    {
        f32 x, y, z;
    };
    struct
    {
        f32 u, v, __;
    };
    struct
    {
        f32 r, g, b;
    };
    struct
    {
        v2 xy;
        real32 Ignored0_;
    };
    struct
    {
        real32 Ignored1_;
        v2 yz;
    };
    struct
    {
        v2 uv;
        real32 Ignored2_;
    };
    
    f32 E[3];
} v3;

#endif // TYPES_H