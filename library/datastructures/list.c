#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

struct list_list* list_initialise(void (*freeItem)(void*)) {
	struct list_list* temp = (struct list_list*) malloc(sizeof(struct list_list));
	temp->currentBufferSize = LIST_BUFFER_SIZE;
	temp->currentFreeIndex = 0;
	temp->list = (void**) malloc(sizeof(void*) * temp->currentBufferSize);
	temp->freeItem = freeItem;

	return temp;
}

void list_append(struct list_list* l1, void* data) {
	if (l1->currentFreeIndex == l1->currentBufferSize) {
		l1->currentBufferSize *= 2;
		l1->list = realloc(l1->list, l1->currentBufferSize);
	}
	l1->list[l1->currentFreeIndex++] = data;
}

void list_appendList(struct list_list* l1, struct list_list* l2) {
	if (l1->currentFreeIndex + l2->currentFreeIndex >= l1->currentBufferSize) {
		l1->currentBufferSize = l1->currentBufferSize + l2->currentFreeIndex + 1;
		l1->list = realloc(l1->list, l1->currentBufferSize);
	}
	memcpy(&l1->list[l1->currentFreeIndex], l2->list, l2->currentFreeIndex * sizeof(void*));
	l1->currentFreeIndex += l2->currentFreeIndex;
}

void list_free(struct list_list* l) {
	if (l->freeItem) {
		for (int i = 0; i < l->currentFreeIndex; i++) {
			printf("LIST_FREE BEFORE l->FREEITEM i = %d\tlist = %p\n", i, l);
			l->freeItem(l->list[i]);
			printf("LIST_FREE AFTER l->FREEITEM i = %d\n", i);
		}
	}
	free(l->list);
	free(l);
}
