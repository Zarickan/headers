#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

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
typedef int8_t  s08;
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

// v2, +-*/% operations
static inline void
v2_add(v2* result, v2 left, v2 right) {
    result->x = left.x + right.x;
    result->y = left.y + right.y;
}
static inline void
v2_sub(v2* result, v2 left, v2 right) {
    result->x = left.x + right.x;
    result->y = left.y + right.y;
}
static inline void
v2_mul(v2* result, v2 left, v2 right) {
    result->x = left.x * right.x;
    result->y = left.y * right.y;
}
static inline void
v2_div(v2* result, v2 left, v2 right) {
    result->x = left.x / right.x;
    result->y = left.y / right.y;
}

// v2 functions
static inline f32
v2_magnitude(v2 vector) {
    return 1.0 / sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}
static inline void
v2_normalize(v2 vector) {
    f32 magnitude = v2_magnitude(vector);
    vector.x *= magnitude;
    vector.y *= magnitude;
}

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

typedef struct f32_4x {
    union {
        __m128 P;
        f32    E[4];
        u32    U[4];
    };
} f32_4x;

#define MM_CVT_U32_F32

inline static f32_4x
f32_set(f32 f0, f32 f1, f32 f2, f32 f3) {
    f32_4x result;
    result.P = _mm_set_ps(f3, f2, f1, f0);
    
    return result;
}

inline static f32_4x
f32_set_all(f32 all) {
    f32_4x result;
    result.P = _mm_set1_ps(all);
    
    return result;
}

inline static f32_4x
f32_zero() {
    f32_4x result;
    result.P = _mm_setzero_ps();
    
    return result;
}

inline static f32_4x
f32_add(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_add_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_sub(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_sub_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_mul(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_mul_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_div(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_div_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_xor(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_xor_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_or(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_or_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_and(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_and_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_lss(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmplt_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_leq(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmple_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_gtr(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmpge_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_eql(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmpeq_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_new(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmpneq_ps(left.P, right.P);
    
    return result;
}

#endif // TYPES_H