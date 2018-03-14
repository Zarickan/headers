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

typedef union ColorF {
    __m128 P;
    struct {
        union {
            f32 R;
            f32 H;
        };
        union {
            f32 G;
            f32 S;
        };
        union {
            f32 B;
            f32 V;
        };
        
        f32 A;
    };
    
    f32 All[4];
} ColorF;

static inline ColorF
colorf_create(f32 r, f32 g, f32 b, f32 a) {
    ColorF result;
    result.P = _mm_set_ps(a, b, g, r);
    
    return result;
}

static inline void
colorf_set(ColorF* color, f32 r, f32 g, f32 b, f32 a) {
    color->P = _mm_set_ps(a, b, g, r);
}

static inline ColorF
colorf_add(ColorF* left, ColorF* right) {
    ColorF result;
    result.P = _mm_add_ps(left->P, right->P);
    
    return result;
}

static inline ColorF
colorf_sub(ColorF* left, ColorF* right) {
    ColorF result;
    result.P = _mm_sub_ps(left->P, right->P);
    
    return result;
}

static inline ColorF
colorf_mul(ColorF* left, ColorF* right) {
    ColorF result;
    result.P = _mm_mul_ps(left->P, right->P);
    
    return result;
}

static inline ColorF
colorf_div(ColorF* left, ColorF* right) {
    ColorF result;
    result.P = _mm_div_ps(left->P, right->P);
    
    return result;
}

static inline ColorF
hsv_to_rgb(ColorF* from) {
    ColorF result;
    
    f64 min, max, delta;
    
    min = from->R < from->G ? from->R : from->G;
    min = min  < from->B ? min  : from->B;
    
    max = from->R > from->G ? from->R : from->G;
    max = max  > from->B ? max  : from->B;
    
    result.V = max;
    delta = max - min;
    
    if (max <= 0.0) {
        colorf_set(&result, NAN, 0.0, result.V, result.A);
        return result;
    }
    
    result.S = (delta / max);
    
    if (from->R >= max) {
        result.H = (from->G - from->B) / delta;
    }
    else {
        if (from->G >= max)
            result.H = 2.0 + (from->B - from->R) / delta;
        else
            result.H = 4.0 + (from->R - from->G) / delta;
    }
    
    result.H *= 60.0;
    
    if (result.H < 0.0)
        result.H += 360.0;
    
    return result;
}

#endif // COLOR_H