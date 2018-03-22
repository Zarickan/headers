#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "types.h"
#include "stack.h"
#include "memory.h"
#include "bitmap.h"

#define COLUMNS 8
#define ROWS    COLUMNS * 2
#define XY(x, y) (y * ROWS + x)

#define SIMD_WIDTH 8
#define SL(x, y) (y * (ROWS / SIMD_WIDTH) + x)

#define SIMD

int main(int argc, char** argv)
{
    union {
        f32    V[COLUMNS * ROWS];
        f32_4x P[COLUMNS * ROWS / 4 + 1];
        f32_8x S[COLUMNS * ROWS / 8 + 1];
    } A;
    
    A.S[0 ] = f32_8x_set(5, 8, 2, 4, 1, 4, 5, 3);
    A.S[2 ] = f32_8x_set(6, 9, 7, 3, 2, 1, 6, 4);
    A.S[4 ] = f32_8x_set(1, 9, 8, 3, 9, 3, 3, 7);
    A.S[6 ] = f32_8x_set(4, 2, 2, 7, 8, 7, 6, 2);
    A.S[8 ] = f32_8x_set(5, 7, 6, 2, 2, 4, 2, 6);
    A.S[10] = f32_8x_set(1, 4, 2, 3, 1, 5, 7, 4);
    A.S[12] = f32_8x_set(2, 9, 0, 8, 3, 2, 3, 7);
    A.S[14] = f32_8x_set(5, 1, 3, 5, 6, 1, 2, 9);
    
    A.S[1 ] = f32_8x_set(1, 0, 0, 0, 0, 0, 0, 0);
    A.S[3 ] = f32_8x_set(0, 1, 0, 0, 0, 0, 0, 0);
    A.S[5 ] = f32_8x_set(0, 0, 1, 0, 0, 0, 0, 0);
    A.S[7 ] = f32_8x_set(0, 0, 0, 1, 0, 0, 0, 0);
    A.S[9 ] = f32_8x_set(0, 0, 0, 0, 1, 0, 0, 0);
    A.S[11] = f32_8x_set(0, 0, 0, 0, 0, 1, 0, 0);
    A.S[13] = f32_8x_set(0, 0, 0, 0, 0, 0, 1, 0);
    A.S[15] = f32_8x_set(0, 0, 0, 0, 0, 0, 0, 1);
    
    for (s16 a = 0; a < COLUMNS; a++)
    {
#ifdef SIMD
        // Make pivot
        f32 k = A.V[XY(a, a)];
        for (u16 i = 0; i < ROWS / SIMD_WIDTH; i++)
            A.S[SL(i, a)] = f32_8x_div(A.S[SL(i, a)], f32_8x_set_all(k));
        k = A.V[XY(a, a)];
        
        // Eliminate below
        for (s16 col = a + 1; col < COLUMNS; col++) {
            f32 c = A.V[XY(a, col)] / k;
            
            for (u16 i = 0; i < ROWS / SIMD_WIDTH; i++) {
                f32_8x t = f32_8x_mul(A.S[SL(i, a)], f32_8x_set_all(c));
                A.S[SL(i, col)] = f32_8x_sub(A.S[SL(i, col)], t);
            }
        }
        
        // Eliminate above
        for (s16 col = a - 1; col >= 0; col--) {
            f32 c = A.V[XY(a, col)] / k;
            
            for (u16 i = 0; i < ROWS / SIMD_WIDTH; i++) {
                f32_8x t = f32_8x_mul(A.S[SL(i, a)], f32_8x_set_all(c));
                A.S[SL(i, col)] = f32_8x_sub(A.S[SL(i, col)], t);
            }
        }
#else
        // Make pivot
        f32 k = A.V[XY(a, a)];
        for (s16 row = 0; row < ROWS; row++) {
            A.V[XY(row, a)] /= k;
        }
        k = A.V[XY(a, a)];
        
        // Eliminate below
        for (s16 col = a + 1; col < COLUMNS; col++) {
            f32 c = A.V[XY(a, col)] / k;
            
            for (s16 row = a; row < ROWS; row++) {
                f32 t = A.V[XY(row, a)] * c;
                A.V[XY(row, col)] -= t;
            }
        }
        
        // Eliminate above
        for (s16 col = a - 1; col >= 0; col--) {
            f32 c = A.V[XY(a, col)] / k;
            
            for (s16 row = a; row < ROWS; row++) {
                f32 t = A.V[XY(row, a)] * c;
                A.V[XY(row, col)] -= t;
            }
        }
#endif
    }
    
    printf("Inverse:\n");
    for (u32 column = 0; column < COLUMNS; column++) {
        for (u32 row = ROWS / 2; row < ROWS; row++) {
            printf("%6.2f ", A.V[XY(row, column)]);
        }
        printf("\n");
    }
    
    exit(0);
}