/*
	This linked list is heavily modified for use by the hash map, and is not really
	meant to be used as a standalone linked list.  Hence why it is in the HashMap directory.
*/

#include <stdio.h>
#include <stdlib.h>

#include "hashMapLinkedList.h"


/*
	Create and initialise linked list struct and set function pointer values
*/
struct hmll_linkedList* hmll_initialise(compareKeysFunction compareKeys, freeKeyFunction freeKey, freeValueFunction freeValue, outputFunction output) {
    struct hmll_linkedList* linkedList = (struct hmll_linkedList*) malloc(sizeof(struct hmll_linkedList));
    linkedList->head = linkedList->tail = NULL;
    linkedList->compareKeys = compareKeys;
    linkedList->freeKey = freeKey;
    linkedList->freeValue = freeValue;
    linkedList->output = output;

    return linkedList;
}

/*
	Insert key and value into linked list
*/
void hmll_insert(struct hmll_linkedList* list, void* key, void* value) {
	struct hmll_node* node = (struct hmll_node*) malloc(sizeof(struct hmll_node));

    node->value = value;
    node->key = key;
    node->value = value;

    if (list->head == NULL) {
        list->head = list->tail = node;
        node->prev = NULL;
		node->next = NULL;
    }
    else {
        node->prev = list->tail;
		node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }
}

/*
	Find key node and return value it points to
*/
void* hmll_find(struct hmll_linkedList* list, void* key) {
    struct hmll_node* tempNode = list->head;
    while (tempNode) {
        if (list->compareKeys(tempNode->key, key))
            return tempNode->value;
        tempNode = tempNode->next;
    }

    return NULL;
}

/*
	Remove key and value node from linked list
*/
void hmll_remove(struct hmll_linkedList* list, void* key) {
    struct hmll_node* tempNode = list->head;
    while (tempNode) {
        if (list->compareKeys(tempNode->key, key)) {
            if (tempNode == list->tail) list->tail = tempNode->prev;
            if (tempNode == list->head) list->head = tempNode->next;
            if (tempNode->prev) tempNode->prev->next = tempNode->next;
            if (tempNode->next) tempNode->next->prev = tempNode->prev;
            if (list->freeKey) list->freeKey(tempNode->key);
            if (list->freeValue) list->freeValue(tempNode->value);
            free(tempNode);
            return;
        }
        tempNode = tempNode->next;
    }
}

/*
	Output linked list from user provided function pointer
*/
void hmll_output(struct hmll_linkedList* list) {
    struct hmll_node* tempNode = list->head;
    while (tempNode) {
        list->output(tempNode->key, tempNode->value);
        tempNode = tempNode->next;
    }
}

/*
	Free all linked list and key / value pairs if user provides specific free function pointers
*/
void hmll_free(struct hmll_linkedList* list) {
    struct hmll_node* tempNode;
    while (list->head) {
        tempNode = list->head->next;
        if (list->freeKey) list->freeKey(list->head->key);
        if (list->freeValue) list->freeValue(list->head->value);
        free(list->head);
        list->head = tempNode;
    }
    free(list);
}


struct list_list* hmll_list(struct hmll_linkedList* linkedList) {
	struct list_list* list = list_initialise(NULL);
	struct hmll_node* temp = linkedList->head;
	while (temp) {
		struct hmll_pair* pair = (struct hmll_pair*) malloc(sizeof(hmll_pair));
		pair->key = temp->key;
		pair->value = temp->value;
		list_append(list, pair);

		temp = temp->next;
	}

	return list;
}

void hmll_freePair(void* data) {
	free(data);
}
