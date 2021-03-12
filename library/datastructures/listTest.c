#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"


typedef struct pair {
	void* key;
	void* value;
} pair;

void pairFree(void* data) {
	free(data);
}


int main() {
	list* l1 = list_init(NULL);
	list_append(l1, "This ");
	list_append(l1, "is ");
	list_append(l1, "a ");
	list_append(l1, "sentence ");
	list_append(l1, "that ");
	list_append(l1, "I ");
	list_append(l1, "made.  ");

	list* l2 = list_init(NULL);
	list_append(l2, "22This ");
	list_append(l2, "22is ");
	list_append(l2, "22a ");
	list_append(l2, "22sentence ");
	list_append(l2, "22that ");
	list_append(l2, "22I ");
	list_append(l2, "22made.\n");

	list_appendList(l1, l2);

	for (int i = 0; i < l1->currentFreeIndex; i++) {
		char* value = l1->data[i];
		printf("%s", value);
	}

	list_free(l1);
	list_free(l2);

	pair* p1 = (pair*) malloc(sizeof(pair));
	p1->key = "KEY";
	p1->value = "VALUE";
	pair* p2 = (pair*) malloc(sizeof(pair));
	p2->key = "KEY 2";
	p2->value = "VALUE 2";
	pair* p3 = (pair*) malloc(sizeof(pair));
	p3->key = "KEY 3";
	p3->value = "VALUE 3";

	list* l3 = list_init(pairFree);
	list_append(l3, p1);
	list_append(l3, p2);
	list_append(l3, p3);

	for (int i = 0; i < l3->currentFreeIndex; i++) {
		pair* temp = l3->data[i];
		char* key = temp->key;
		char* value = temp->value;
		printf("Key: %s\tValue: %s\n", key, value);

	}

	list_free(l3);

	return 0;
}
