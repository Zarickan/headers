#include <stdio.h>
#include "types.h"
#include "stack.h"

int main(int argc, char** argv)
{
    Stack stack;
    stack_init(&stack, 4);
    
    v3 vec1, vec2, vec3, vec4, vec5;
    
    vec1.x = 10;
    vec1.y = 20;
    vec1.z = 30;
    
    vec2.x = 1;
    vec2.y = 2;
    vec2.z = 3;
    
    vec3.x = 0.123f;
    vec3.y = 0.321f;
    vec3.z = 0.111f;
    
    vec5.x = 1.234f;
    vec5.y = 2.234f;
    vec5.z = 3.234f;
    
    stack_push(&stack, &vec1);
    stack_push(&stack, &vec2);
    stack_push(&stack, &vec3);
    stack_push(&stack, &vec4);
    stack_push(&stack, &vec5);
    
    for(u8 i = 0; i < 4; i++) {
        v3* el = (v3*) stack_pop(&stack);
        printf("Element %i from top is: {%f, %f, %f} \n", i, el->x, el->y, el->z);
    }
    
    v3* peek = (v3*) stack_peek(&stack);
    printf("Peeking at {%f, %f, %f}\n", peek->x, peek->y, peek->z);
    
    printf("Hello world!\n");
    return 0;
}