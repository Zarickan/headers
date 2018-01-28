#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "list.h"

int main(int argc, char** argv)
{
    List list;
    list_init(&list, 4);
    
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
    
    vec4.x = 2.222222f;
    vec4.y = 2.222222f;
    vec4.z = 2.222222f;
    
    vec5.x = 1.234f;
    vec5.y = 2.234f;
    vec5.z = 3.234f;
    
    list_add(&list, &vec1);
    list_add(&list, &vec2);
    list_add(&list, &vec3);
    list_add(&list, &vec4);
    list_add(&list, &vec5);
    
    printf("List contains:\n");
    for(u32 i = 0; i < list.size; i++) {
        v3* el = (v3*) list.elements[i];
        printf("Element %i is: {%f, %f, %f} \n", i, el->x, el->y, el->z);
    }
    
    v3* removed = (v3*) list_remove(&list, 2);
    printf("Removed {%f, %f, %f} at index 2\n\n", removed->x, removed->y, removed->z);
    
    printf("Size %i, capacity %i\n", list.size, list.capacity);
    
    printf("List now contains:\n");
    for(u32 c = 0; c < list.size; c++) {
        v3* el = (v3*) list.elements[c];
        printf("Element %i is: {%f, %f, %f} \n", c, el->x, el->y, el->z);
    }
    
    printf("Hello world!\n");
    return 0;
}