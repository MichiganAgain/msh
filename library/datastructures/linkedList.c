#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "linkedList.h"


struct ll_linkedList* ll_initialise(bool (*compFunc)(void* nodeData, void* comp), void (*outFunc)(void* data), void (*freeFunc)(void* data)) {
	struct ll_linkedList* temp = (struct ll_linkedList*) malloc(sizeof(struct ll_linkedList));
	temp->head = temp->tail = NULL;
	temp->compFunc = compFunc;
	temp->outFunc = outFunc;
	temp->freeFunc = freeFunc;

	return temp;
}

void ll_addNode(struct ll_linkedList* list, void* data) {
	struct ll_node* temp = (struct ll_node*) malloc(sizeof(struct ll_node));
	temp->data = data;
	temp->prev = temp->next = NULL;

	if (list->head == NULL && list->tail == NULL) list->head = list->tail = temp;
	else {
		temp->prev = list->tail;
		list->tail->next = temp;
		list->tail = temp;
	}
}

ll_node* ll_findNode(struct ll_linkedList* list, void* comp) {
	struct ll_node* temp = list->head;
	while (temp) {
		if (list->compFunc(temp->data, comp)) return temp;
		temp = temp->next;
	}

	return NULL;
}

void ll_free(struct ll_linkedList* list) {
	struct ll_node* temp;
	while (list->head) {
		temp = list->head->next;
		if (list->freeFunc) list->freeFunc(list->head->data);
		free(list->head->data);
		free(list->head);
		list->head = temp;
	}
	free(list);
}

void ll_output(struct ll_linkedList* list) {
	if (list->outFunc == NULL) {
		printf("There is no output function defined for this list type\n");
		return;
	}

	struct ll_node* temp = list->head;
	while (temp) {
		list->outFunc(temp->data);
		temp = temp->next;
	}
}

void ll_removeNode(struct ll_linkedList* list, void* comp) {
	struct ll_node* curr = list->head;
	struct ll_node* temp;
	while (curr) {
		if (list->compFunc(curr->data, comp)) {
			if (curr->prev != NULL) curr->prev->next = curr->next;
			if (curr->next != NULL) curr->next->prev = curr->prev;
			if (list->head == list->tail) list->head = list->tail = NULL;

			temp = curr->next;
			if (list->freeFunc) list->freeFunc(curr->data);
			free(curr->data);
			free(curr);
			curr = temp;
		}
		else curr = curr->next;
	}
}

void ll_replaceNode(struct ll_linkedList* list, struct ll_node* node, void* data) {
	if (list->freeFunc) list->freeFunc(node->data);
	node->data = data;
}

