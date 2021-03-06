/*
	This linked list is heavily modified for use by the hash map, and is not really
	meant to be used as a standalone linked list.  Hence why it is in the HashMap directory.
*/

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


// Initialise linked list structure
struct hmll_linkedList* hmll_initialise(compareKeysFunction, freeKeyFunction, freeValueFunction, outputFunction);
// Insert key and value into linked list
void hmll_insert(struct hmll_linkedList* list, void* key, void* value);
// Find a key in the linked list and return the value pointed to by the key struct
void* hmll_find(struct hmll_linkedList* list, void* key);
// Remove a key and value from the linked list
void hmll_remove(struct hmll_linkedList* list, void* key);
// Output the linked list using the user provided output function pointer
void hmll_output(struct hmll_linkedList* list);
// Free the list, also uses any user provided free functions if provided
void hmll_free(struct hmll_linkedList* list);

#endif
