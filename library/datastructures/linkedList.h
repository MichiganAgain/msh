#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/*
    ALL DATA MUST BE ALLOCATED ON HEAD NOT STACK
        -   Else when removing the node it will try and free a value that is on the stack
        -   It will cause undefined behaviour if you do not do this which very much ====== bad

	The following functions will need to be manually provided:
        -   Compare function, takes node data* in list and data* you are looking for
		-	Free function, takes pointer to data and frees data pointed to by it, not the actual pointer itself
            -   i.e, free(apple_struct* as), you won't do free(as), rather free(as->heapData)
        -   OPTIONAL
            -   Output function, takes void* to data and casts
*/

#include <stdbool.h>


typedef struct ll_node {
	void* data;
    struct ll_node* prev;
	struct ll_node* next;
} ll_node;

typedef struct ll_linkedList {
    struct ll_node* head;
    struct ll_node* tail;
    bool (*compFunc)(void* nodeData, void* comp);
    void (*outFunc)(void* data);
    void (*freeFunc)(void* data);   // free only data *in* data, not data.  If nothing, set as NULL
} ll_linkedList;


struct ll_linkedList* ll_initialise(bool (*compFunc)(void* nodeData, void* comp), void (*outFunc)(void* data), void (*freeFunc)(void* data));
void ll_addNode(struct ll_linkedList* list, void* data);
struct ll_node* ll_findNode(struct ll_linkedList* list, void* comp);
void ll_free(struct ll_linkedList* list);
void ll_output(struct ll_linkedList* list);
void ll_removeNode(struct ll_linkedList* list, void* comp);
void ll_replaceNode(struct ll_linkedList* list, struct ll_node* node, void* data);


#endif

