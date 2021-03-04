#include <stdio.h>
#include <stdlib.h>

#include "hashMap.h"

struct hm_hashMap* hm_initialise(indexFunction index, compareKeysFunction compareKeys, freeKeyFunction freeKey, freeValueFunction freeValue, outputFunction output) {
    struct hm_hashMap* hashMap = (struct hm_hashMap*) malloc(sizeof(hm_hashMap));
    hashMap->bucketNum = HM_DEFAULT_BUCKETS;
    hashMap->buckets = (struct hmll_linkedList**) malloc(sizeof(struct hmll_linkedList*) * hashMap->bucketNum);
    hashMap->index = index;

    for (int i = 0; i < hashMap->bucketNum; i++)
        hashMap->buckets[i] = hmll_initialise(compareKeys, freeKey, freeValue, output);

    return hashMap;
}

void hm_insert(struct hm_hashMap* hashMap, void* key, void* value) {
    int index = hashMap->index(key) % hashMap->bucketNum;
    hmll_remove(hashMap->buckets[index], key);
    hmll_insert(hashMap->buckets[index], key, value);
}

void* hm_find(struct hm_hashMap* hashMap, void* key) {
    int index = hashMap->index(key) % hashMap->bucketNum;
    return hmll_find(hashMap->buckets[index], key);
}

void hm_remove(struct hm_hashMap* hashMap, void* key) {
    int index = hashMap->index(key) % hashMap->bucketNum;
    hmll_remove(hashMap->buckets[index], key);
}

void hm_output(struct hm_hashMap* hashMap) {
    for (int i = 0; i < hashMap->bucketNum; i++) {
        printf("Bucket: %d\n", i);
        hmll_output(hashMap->buckets[i]);
    }
}

void hm_free(struct hm_hashMap* hashMap) {
    for (int i = 0; i < hashMap->bucketNum; i++)
        hmll_free(hashMap->buckets[i]);

    free(hashMap);
}
