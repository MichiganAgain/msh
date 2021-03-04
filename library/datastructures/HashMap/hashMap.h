#ifndef HASHMAP_H
#define HASHMAP_H

#include "linkedList.h"

#define HM_DEFAULT_BUCKETS 32

typedef int (*indexFunction)(void* key);

struct hm_hashMap {
    int bucketNum;
    struct hmll_linkedList** buckets;
    indexFunction index;
} hm_hashMap;

struct hm_hashMap* hm_initialise(indexFunction, compareKeysFunction, freeKeyFunction, freeValueFunction, outputFunction);
void hm_insert(struct hm_hashMap* hashMap, void* key, void* value);
void* hm_find(struct hm_hashMap* hashMap, void* key);
void hm_remove(struct hm_hashMap* hashMap, void* key);
void hm_output(struct hm_hashMap* hashMap);
void hm_free(struct hm_hashMap* hashMap);

#endif
