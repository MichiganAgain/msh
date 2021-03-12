#ifndef HASHMAP_H
#define HASHMAP_H

#include "hashMapLinkedList.h"
#include "list.h"

#define HM_DEFAULT_BUCKETS 4

typedef int (*indexFunction)(void* key);

struct hm_hashMap {
    int bucketNum;
    struct hmll_linkedList** buckets;
    indexFunction index;
} hm_hashMap;

// Create hash map struct and fill set member function pointers to arguments
struct hm_hashMap* hm_initialise(indexFunction, compareKeysFunction, freeKeyFunction, freeValueFunction, outputFunction);
// Insert key and value into hash map
void hm_insert(struct hm_hashMap* hashMap, void* key, void* value);
// Find and return value based off key in hash map
void* hm_find(struct hm_hashMap* hashMap, void* key);
// Remove key and value from hash map
void hm_remove(struct hm_hashMap* hashMap, void* key);
// Output hash map based off user function pointer provided
void hm_output(struct hm_hashMap* hashMap);
// Free all data used by hash map
void hm_free(struct hm_hashMap* hashMap);

struct list_list* hm_list(struct hm_hashMap* hashMap);

#endif
