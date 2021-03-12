#include <stdio.h>
#include <stdlib.h>

#include "hashMap.h"
#include "list.h"

/*
	Create and initialise the hashmap data structure with function pointers for
	indexing, comparing, freeing and outputting data.  Then initialise the buckets
	with pointers to a linked list data structure specific for each specific bucket
*/
struct hm_hashMap* hm_initialise(indexFunction index, compareKeysFunction compareKeys, freeKeyFunction freeKey, freeValueFunction freeValue, outputFunction output) {
    struct hm_hashMap* hashMap = (struct hm_hashMap*) malloc(sizeof(hm_hashMap));
    hashMap->bucketNum = HM_DEFAULT_BUCKETS;
    hashMap->buckets = (struct hmll_linkedList**) malloc(sizeof(struct hmll_linkedList*) * hashMap->bucketNum);
    hashMap->index = index;

    for (int i = 0; i < hashMap->bucketNum; i++)
        hashMap->buckets[i] = hmll_initialise(compareKeys, freeKey, freeValue, output);

    return hashMap;
}

/*
	Insert into the hash map by calculating the bucket using the user provided index
	function and using the linked list function to insert it.
*/
void hm_insert(struct hm_hashMap* hashMap, void* key, void* value) {
    int index = hashMap->index(key) % hashMap->bucketNum;
    hmll_remove(hashMap->buckets[index], key);
    hmll_insert(hashMap->buckets[index], key, value);
}

/*
	Find a key in the hash map and return the data that it points to
	using the linked list functions.
*/
void* hm_find(struct hm_hashMap* hashMap, void* key) {
    int index = hashMap->index(key) % hashMap->bucketNum;
    return hmll_find(hashMap->buckets[index], key);
}

/*
	Remove a key and value from the hash map.  Again, using the linked list functions
*/
void hm_remove(struct hm_hashMap* hashMap, void* key) {
    int index = hashMap->index(key) % hashMap->bucketNum;
    hmll_remove(hashMap->buckets[index], key);
}

/*
	Output the hash map depending on the user implemented output functions
*/
void hm_output(struct hm_hashMap* hashMap) {
    for (int i = 0; i < hashMap->bucketNum; i++) {
        printf("Bucket: %d\n", i);
        hmll_output(hashMap->buckets[i]);
    }
}

/*
	Free all memory used by the hash map, as well as freeing the key and value
	data if a free function pointer was provided for them (implemented in the linked list).
*/
void hm_free(struct hm_hashMap* hashMap) {
    for (int i = 0; i < hashMap->bucketNum; i++)
        hmll_free(hashMap->buckets[i]);

    free(hashMap);
}

struct list_list* hm_list(struct hm_hashMap* hashMap) {
	struct list_list* list = list_initialise(hmll_freePair);
	for (int i = 0; i < hashMap->bucketNum; i++) {
		printf("1) in hm_list\n");
		struct list_list* bucketList = hmll_list(hashMap->buckets[i]);
		printf("2) in hm_list\n");
		list_appendList(list, bucketList);
		printf("3) in hm_list\n");
		list_free(bucketList);
		printf("4) in hm_list\n");
	}

	return list;
}
