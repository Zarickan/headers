#include <stdio.h>
#include "types.h"
#include "stack.h"
#include "list.h"
#include "linkedlist.h"

int main(int argc, char** argv)
{
    LinkedList list;
    linkedlist_init(&list);
    
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
    
    ListNode n1, n2, n3, n4, n5;
    listnode_init(&n1, &vec1);
    listnode_init(&n2, &vec2);
    listnode_init(&n3, &vec3);
    listnode_init(&n4, &vec4);
    listnode_init(&n5, &vec5);
    
    linkedlist_add(&list, &n1);
    linkedlist_add(&list, &n2);
    linkedlist_add(&list, &n3);
    linkedlist_add(&list, &n4);
    linkedlist_add(&list, &n5);
    
    printf("Size %i\n", list.size);
    printf("List contains:\n");
    ListNode* node = list.head; 
    while(node != NULL) {
        v3* el = (v3*) node->value;
        printf("Element is: {%f, %f, %f} \n", el->x, el->y, el->z);
        
        node = (ListNode*) node->next;
    }
    
    linkedlist_remove(&list, &n4);
    
    printf("Size %i\n", list.size);
    printf("List contains:\n");
    node = list.head; 
    while(node != NULL) {
        v3* el = (v3*) node->value;
        printf("Element is: {%f, %f, %f} \n", el->x, el->y, el->z);
        
        node = (ListNode*) node->next;
    }
    
    printf("Hello world!\n");
    return 0;
}