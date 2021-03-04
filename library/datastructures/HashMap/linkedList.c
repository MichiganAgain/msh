#include <stdio.h>
#include <stdlib.h>

#include "linkedList.h"

struct hmll_linkedList* hmll_initialise(compareKeysFunction compareKeys, freeKeyFunction freeKey, freeValueFunction freeValue, outputFunction output) {
    struct hmll_linkedList* linkedList = (struct hmll_linkedList*) malloc(sizeof(struct hmll_linkedList));
    linkedList->head = linkedList->tail = NULL;
    linkedList->compareKeys = compareKeys;
    linkedList->freeKey = freeKey;
    linkedList->freeValue = freeValue;
    linkedList->output = output;

    return linkedList;
}

void hmll_insert(struct hmll_linkedList* list, void* key, void* value) {
    struct hmll_valueNode* valueNode = (struct hmll_valueNode*) malloc(sizeof(struct hmll_valueNode));
    struct hmll_keyNode* keyNode = (struct hmll_keyNode*) malloc(sizeof(struct hmll_keyNode));

    valueNode->value = value;

    keyNode->key = key;
    keyNode->value = valueNode;
    keyNode->next = NULL;

    if (list->head == NULL) {
        list->head = list->tail = keyNode;
        keyNode->prev = NULL;
    }
    else {
        keyNode->prev = list->tail;
        list->tail->next = keyNode;
        list->tail = keyNode;
    }
}

void* hmll_find(struct hmll_linkedList* list, void* key) {
    struct hmll_keyNode* tempKeyNode = list->head;
    while (tempKeyNode) {
        if (list->compareKeys(tempKeyNode->key, key))
            return tempKeyNode->value->value;
        tempKeyNode = tempKeyNode->next;
    }

    return NULL;
}

void hmll_remove(struct hmll_linkedList* list, void* key) {
    struct hmll_keyNode* tempKeyNode = list->head;
    while (tempKeyNode) {
        if (list->compareKeys(tempKeyNode->key, key)) {
            if (tempKeyNode == list->tail) list->tail = list->tail->prev;
            if (tempKeyNode == list->head) list->head = NULL;
            if (tempKeyNode->prev) tempKeyNode->prev->next = tempKeyNode->next;
            if (tempKeyNode->next) tempKeyNode->next->prev = tempKeyNode->prev;
            if (list->freeKey) list->freeKey(tempKeyNode->key);
            if (list->freeValue) list->freeValue(tempKeyNode->value->value);
            free(tempKeyNode->value);
            free(tempKeyNode);
            return;
        }
        tempKeyNode = tempKeyNode->next;
    }
}

void hmll_output(struct hmll_linkedList* list) {
    struct hmll_keyNode* tempKeyNode = list->head;
    while (tempKeyNode) {
        list->output(tempKeyNode->key, tempKeyNode->value->value);
        tempKeyNode = tempKeyNode->next;
    }
}

void hmll_free(struct hmll_linkedList* list) {
    struct hmll_keyNode* tempKeyNode;
    while (list->head) {
        tempKeyNode = list->head->next;
        if (list->freeKey) list->freeKey(list->head->key);
        if (list->freeValue) list->freeValue(list->head->value->value);
        free(list->head->value);
        free(list->head);
        list->head = tempKeyNode;
    }
    free(list);
}
