#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "linkedList.h"
#include "hashMap.h"


struct hm_map* hm_initialise(int (*indexFunc)(struct hm_map* map, void* data), bool (*compFunc)(void* nodeData, void* comp), void (*outFunc)(void* data), void (*freeFunc)(void* data)) {
	struct hm_map* temp = (struct hm_map*) malloc(sizeof(struct hm_map));
	temp->bucketNum = HM_DEFAULT_BUCKETS;
	temp->buckets = (struct ll_linkedList**) malloc(sizeof(struct ll_linkedList*) * temp->bucketNum);
	temp->indexFunc = indexFunc;

	for (int i = 0; i < temp->bucketNum; i++) {
		temp->buckets[i] = ll_initialise(compFunc, outFunc, freeFunc);
	}

	return temp;
}

void hm_insert(struct hm_map* map, void* data) {
	struct ll_node* temp;
	int index = map->indexFunc(map, data);
	if ((temp = ll_findNode(map->buckets[index], data)) != NULL) ll_replaceNode(map->buckets[index], temp, data);
	else ll_addNode(map->buckets[index], data);
}

bool hm_findNode(struct hm_map* map, void* data) {
	int index = map->indexFunc(map, data);
	return ll_findNode(map->buckets[index], data);
}

void hm_removeNode(struct hm_map* map, void* data) {
	int index = map->indexFunc(map, data);
	ll_removeNode(map->buckets[index], data);
}

void hm_output(struct hm_map* map) {
	for (int i = 0; i < map->bucketNum; i++) {
		printf("Bucket %d: ", i);
		ll_output(map->buckets[i]);
		printf("\n");
	}
}

void hm_free(struct hm_map* map) {
	for (int i = 0; i < map->bucketNum; i++) ll_free(map->buckets[i]);
	free(map->buckets);
	free(map);
}

