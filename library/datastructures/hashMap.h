#ifndef HASHMAP_H
#define HASHMAP_H

/*
	The following functions will have to be provided manually:
		-	Index function to select bucket, int index(void* data)
        -   Compare function, takes node data* in list and data* you are looking for
        -   Free function, takes pointer to data and frees data pointed to by it, not the actual pointer itself
            -   i.e, free(apple_struct* as), you won't do free(as), rather free(as->heapData)
        -   OPTIONAL
            -   Output function, takes void* to data and casts
*/

#include <stdbool.h>

#include "linkedList.h"

#define HM_DEFAULT_BUCKETS 16


typedef struct hm_map {
	struct ll_linkedList** buckets;
	int bucketNum;
    int (*indexFunc)(struct hm_map* map, void* data);
} hm_map;


struct hm_map* hm_initialise(int (*indexFunc)(struct hm_map* map, void* data), bool (*compFunc)(void* nodeData, void* comp), void (*freeFunc)(void* data), void (*outFunc)(void* data));
void hm_insert(struct hm_map* map, void* data);
bool hm_findNode(struct hm_map* map, void* data);
void hm_removeNode(struct hm_map* map, void* data);
void hm_output(struct hm_map* map);
void hm_free(struct hm_map* hm);

#endif

