#ifndef LIST_H
#define LIST_H

#define LIST_BUFFER_SIZE 1

typedef struct list_list {
	int currentBufferSize;
	int currentFreeIndex;
	void** data;
	void (*freeItem)(void* data);
} list;

struct list_list* list_init(void (*freeItem)(void* data));
void list_append(struct list_list* l, void* data);
void list_appendList(struct list_list* l1, struct list_list* l2);
void list_free(struct list_list* l);

#endif
