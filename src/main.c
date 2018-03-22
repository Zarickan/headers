#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "types.h"
#include "stack.h"
#include "memory.h"
#include "bitmap.h"

#define COLUMNS 4
#define ROWS    COLUMNS * 2
#define XY(x, y) y * ROWS + x

int main(int argc, char** argv)
{
    union {
        f32    V[COLUMNS * ROWS];
        f32_4x P[8];
        f32_8x S[4];
    } A;
    
    A.S[0] = f32_8x_set(5, 8, 2, 4, 1, 0, 0, 0);
    A.S[1] = f32_8x_set(6, 9, 7, 3, 0, 1, 0, 0);
    A.S[2] = f32_8x_set(1, 9, 8, 3, 0, 0, 1, 0);
    A.S[3] = f32_8x_set(1, 4, 2, 7, 0, 0, 0, 1);
    
    printf("Before:\n");
    for (u32 column = 0; column < COLUMNS; column++) {
        for (u32 row = 0; row < ROWS / 2; row++) {
            printf("%6.2f ", A.V[XY(row, column)]);
        }
        printf(" | ");
        for (u32 row = ROWS / 2; row < ROWS; row++) {
            printf("%6.2f ", A.V[XY(row, column)]);
        }
        printf("\n");
    }
    
    clock_t begin = clock();
    for (s16 a = 0; a < COLUMNS; a++)
    {
        f32 k = A.V[XY(a, a)];
        A.S[a] = f32_8x_div(A.S[a], f32_8x_set_all(k));
        k = A.V[XY(a, a)];
        
        for (s16 col = a + 1; col < COLUMNS; col++) {
            f32 c = A.V[XY(a, col)] / k;
            f32_8x t = f32_8x_mul(A.S[a], f32_8x_set_all(c));
            A.S[col] = f32_8x_sub(A.S[col], t);
        }
        
        for (s16 col = a - 1; col >= 0; col--) {
            f32 c = A.V[XY(a, col)] / k;
            f32_8x t = f32_8x_mul(A.S[a], f32_8x_set_all(c));
            A.S[col] = f32_8x_sub(A.S[col], t);
        }
    }
    clock_t end = clock();
    
    printf("\n");
    printf("Time taken: %f clocks\n\n", (double)(end - begin) / CLOCKS_PER_SEC);
    
    printf("After:\n");
    for (u32 column = 0; column < COLUMNS; column++) {
        for (u32 row = 0; row < ROWS / 2; row++) {
            printf("%6.2f ", A.V[XY(row, column)]);
        }
        printf(" | ");
        for (u32 row = ROWS / 2; row < ROWS; row++) {
            printf("%6.2f ", A.V[XY(row, column)]);
        }
        printf("\n");
    }
    
    exit(0);
}