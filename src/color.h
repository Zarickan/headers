#include <stdlib.h>
#include <stdio.h>
#include "types.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

#ifndef COLOR_H
#define COLOR_H

typedef union Color {
    struct{
        union {
            u08 R;
            u08 H;
        };
        union {
            u08 G;
            u08 S;
        };
        union {
            u08 B;
            u08 V;
        };
        
        u08 A;
    };
    
    u32 All;
    u08 E[4];
} Color;

typedef union Color_4x {
    __m128i P;
    Color   A[4];
    u32     E[4];
} Color_4x;

static inline Color_4x
color_4x_set(Color c0, Color c1, Color c2, Color c3) {
    Color_4x result;
    result.P = _mm_set_epi32(c3.All, c2.All, c1.All, c0.All);
    
    return result;
}

static inline Color_4x
color_4x_add(Color_4x left, Color_4x right) {
    Color_4x result;
    result.P = _mm_adds_epu8(left.P, right.P);
    // NOTE: specifically for colors we want no overflow, hence we use "adds" over "add"
    
    return result;
}

static inline Color_4x
color_4x_sub(Color_4x left, Color_4x right) {
    Color_4x result;
    result.P = _mm_subs_epu8(left.P, right.P);
    // NOTE: specifically for colors we want no overflow, hence we use "adds" over "add"
    
    return result;
}

#endif // COLOR_H