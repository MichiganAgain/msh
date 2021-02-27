#ifndef HM_LINKED_LIST_H
#define HM_LINKED_LIST_H

#include <stdbool.h>

typedef bool (*compareKeysFunction)(void* k1, void* k2);
typedef void (*freeKeyFunction)(void* key);     // Gives opportunity to free key if it was allocated on heap
typedef void (*freeValueFunction)(void* value); // Gives opportunity to free value if it was allocated on heap
typedef void (*outputFunction)(void* key, void* value);

struct hmll_valueNode {
    void* value;
} hmll_valueNode;

struct hmll_keyNode {
    void* key;
    struct hmll_valueNode* value;
    struct hmll_keyNode *prev, *next;
} hmll_keyNode;

struct hmll_linkedList {
    struct hmll_keyNode *head, *tail;
    compareKeysFunction compareKeys;
    freeKeyFunction freeKey;
    freeValueFunction freeValue;
    outputFunction output;
} hmll_linkedList;


struct hmll_linkedList* hmll_initialise(compareKeysFunction, freeKeyFunction, freeValueFunction, outputFunction);
void hmll_insert(struct hmll_linkedList* list, void* key, void* value);
void* hmll_find(struct hmll_linkedList* list, void* key);
void hmll_remove(struct hmll_linkedList* list, void* key);
void hmll_output(struct hmll_linkedList* list);
void hmll_free(struct hmll_linkedList* list);

#endif
