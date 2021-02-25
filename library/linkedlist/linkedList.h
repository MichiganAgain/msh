#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/*
	The following functions will need to be manually provided:
		-	Output function, takes void* to data and casts
		-	Remove function, takes void* data and compare value
*/

#include <stdbool.h>


typedef struct ll_node {
	void* data;
	struct ll_node* next;
} ll_node;


struct ll_node* ll_createNode(void* data, struct ll_node* next);
void ll_addNode(struct ll_node** head, void* data);
void ll_freeList(struct ll_node* head);
void ll_output(struct ll_node* head, void (*outFunc)(void*));
void ll_removeElement(struct ll_node* head, bool (*rmFunc)(void* nodeData, void* comp), void* comp);

#endif

