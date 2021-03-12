#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

struct list_list* list_init(void (*freeItem)(void* data)) {
	struct list_list* temp = (struct list_list*) malloc(sizeof(struct list_list));
	temp->currentBufferSize = LIST_BUFFER_SIZE;
	temp->currentFreeIndex = 0;
	temp->data = (void**) malloc(sizeof(void*) * temp->currentBufferSize);
	temp->freeItem = freeItem;

	return temp;
}

void list_append(struct list_list* l, void* data) {
	if (l->currentFreeIndex >= l->currentBufferSize) {
		l->currentBufferSize *= 2;
		l->data = (void**) realloc(l->data, sizeof(void*) * l->currentBufferSize);
	}
	l->data[l->currentFreeIndex++] = data;
}

void list_appendList(struct list_list* l1, struct list_list* l2) {
	if (l1->currentFreeIndex + l2->currentFreeIndex >= l1->currentBufferSize) {
		l1->currentBufferSize = l1->currentFreeIndex + l2->currentFreeIndex;
		l1->data = (void**) realloc(l1->data, sizeof(void*) * l1->currentBufferSize);
	}
	memcpy(&(l1->data[l1->currentFreeIndex]), l2->data, sizeof(void*) * l2->currentFreeIndex);
	l1->currentFreeIndex += l2->currentFreeIndex;
}

void list_free(struct list_list* l) {
	if (l->freeItem) {
		for (int i = 0; i < l->currentFreeIndex; i++) {
			if (l->data[i]) l->freeItem(l->data[i]);
		}
	}
	free(l->data);
	free(l);
}
