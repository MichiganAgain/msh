#include <stdio.h>
#include "list.h"

int main() {
	struct list_list* list1 = list_initialise(NULL);
	list_append(list1, "This ");
	list_append(list1, "is ");
	list_append(list1, "a ");
	list_append(list1, "Sentence");
	list_append(list1, ".  ");

	struct list_list* list2 = list_initialise(NULL);
	list_append(list2, "Cow ");
	list_append(list2, "Doing ");
	list_append(list2, "a ");
	list_append(list2, "flip\n");

	struct list_list* list3 = list_initialise(NULL);
	list_appendList(list3, list1);
	list_appendList(list3, list2);

	for (int i = 0; i < list1->currentFreeIndex; i++) printf("List item: %s\n", list1->list[i]);
	printf("\n");
	for (int i = 0; i < list2->currentFreeIndex; i++) printf("List item: %s\n", list2->list[i]);
	printf("\n");
	for (int i = 0; i < list3->currentFreeIndex; i++) printf("%s", list3->list[i]);

	list_free(list1);
	list_free(list2);
	return 0;
}
