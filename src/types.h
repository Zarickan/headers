#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>

#ifndef TYPES_H
#define TYPES_H

#define MMX
#define SSE
#define AVX

typedef size_t size;
typedef intptr_t intptr;
typedef uintptr_t uintptr;

// Math
#define MIN(left, right) ((left) <= (right) ? (left) : (right))
#define MAX(left, right) ((left) >= (left) ? (right) : (right))
#define ABS(value) ((value) < 0 ? -(value) : (value))
#define CLAMP(value, mn, mx) min(max((value), (mn)), (max))
#define INDEX2D(x, y, w) ((y) * (w) + (x))

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
    return (f32) (1.0 / sqrt(pow(vector.x, 2) + pow(vector.y, 2)));
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

#ifdef MMX

typedef struct f32_2x {
    union {
        __m64 P;
        f32   E[2];
        u32   U[2];
    };
} f32_2x;

#endif // MMX

#ifdef SSE

typedef struct f32_4x {
    union {
        __m128 P;
        f32    E[4];
        u32    U[4];
#ifdef MMX
        f32_2x M[2];
#endif
    };
} f32_4x;

inline static f32_4x
f32_4x_set(f32 f0, f32 f1, f32 f2, f32 f3) {
    f32_4x result;
    result.P = _mm_set_ps(f3, f2, f1, f0);
    
    return result;
}

inline static f32_4x
f32_4x_set_all(f32 all) {
    f32_4x result;
    result.P = _mm_set1_ps(all);
    
    return result;
}

inline static f32_4x
f32_4x_zero() {
    f32_4x result;
    result.P = _mm_setzero_ps();
    
    return result;
}

inline static f32_4x
f32_4x_add(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_add_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_sub(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_sub_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_mul(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_mul_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_div(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_div_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_xor(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_xor_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_or(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_or_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_and(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_and_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_lss(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmplt_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_leq(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmple_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_gtr(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmpgt_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_geq(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmpge_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_eql(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmpeq_ps(left.P, right.P);
    
    return result;
}

inline static f32_4x
f32_4x_neq(f32_4x left, f32_4x right) {
    f32_4x result;
    result.P = _mm_cmpneq_ps(left.P, right.P);
    
    return result;
}

#endif // SSE

#ifdef AVX

typedef struct f32_8x {
    union {
        __m256 P;
        f32    E[8];
        u32    U[8];
#ifdef MMX
        f32_2x M[4];
#endif
#ifdef SSE
        f32_4x F[2];
#endif
    };
} f32_8x;

inline static f32_8x
f32_8x_set(f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7) {
    f32_8x result;
    result.P = _mm256_set_ps(f7, f6, f5, f4, f3, f2, f1, f0);
    
    return result;
}

inline static f32_8x
f32_8x_set_all(f32 all) {
    f32_8x result;
    result.P = _mm256_set1_ps(all);
    
    return result;
}

inline static f32_8x
f32_8x_zero() {
    f32_8x result;
    result.P = _mm256_setzero_ps();
    
    return result;
}

inline static f32_8x
f32_8x_add(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_add_ps(left.P, right.P);
    
    return result;
}

inline static f32_8x
f32_8x_sub(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_sub_ps(left.P, right.P);
    
    return result;
}

inline static f32_8x
f32_8x_mul(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_mul_ps(left.P, right.P);
    
    return result;
}

inline static f32_8x
f32_8x_div(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_div_ps(left.P, right.P);
    
    return result;
}

inline static f32_8x
f32_8x_xor(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_xor_ps(left.P, right.P);
    
    return result;
}

inline static f32_8x
f32_8x_or(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_or_ps(left.P, right.P);
    
    return result;
}

inline static f32_8x
f32_8x_and(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_and_ps(left.P, right.P);
    
    return result;
}

inline static f32_8x
f32_8x_lss(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_cmp_ps(left.P, right.P, _CMP_LT_OQ);
    
    return result;
}

inline static f32_8x
f32_8x_leq(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_cmp_ps(left.P, right.P, _CMP_LE_OQ);
    
    return result;
}

inline static f32_8x
f32_8x_gtr(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_cmp_ps(left.P, right.P, _CMP_GT_OQ);
    
    return result;
}

inline static f32_8x
f32_8x_geq(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_cmp_ps(left.P, right.P, _CMP_GE_OQ);
    
    return result;
}

inline static f32_8x
f32_8x_eql(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_cmp_ps(left.P, right.P, _CMP_EQ_OQ);
    
    return result;
}

inline static f32_8x
f32_8x_neq(f32_8x left, f32_8x right) {
    f32_8x result;
    result.P = _mm256_cmp_ps(left.P, right.P, _CMP_NEQ_OQ);
    
    return result;
}

#endif // AVX2

// Math functions
static inline u32
power(u32 base, u32 power) {
    if (power == 0) return 1;
    
    u32 result = base;;
    for (u32 n = 1; n < power; n++)
        result *= base;
    
    return result;
}

static inline u32
setbits(u32 number)
{
    number -= number >> 1 & 0x55555555;
    number = (number & 0x33333333) + (number >> 2 & 0x33333333);
    return (((number + (number >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

#endif // TYPES_H