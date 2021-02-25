#include <stdlib.h>
#include <stdbool.h>

#include "linkedList.h"

struct ll_node* ll_createNode(void* data, struct ll_node* next) {
	struct ll_node* temp = (struct ll_node*) malloc(sizeof(struct ll_node));
	temp->data = data;
	temp->next = next;

	return temp;
}

void ll_addNode(struct ll_node** head, void* data) {
	struct ll_node* newNode = ll_createNode(data, *head);
	*head = newNode;
}

void ll_freeList(struct ll_node* head) {
	struct ll_node* temp;
	while (head) {
		temp = head->next;
		free(head->data);
		free(head);
		head = temp;
	}
}

void ll_output(struct ll_node* head, void (*outFunc)(void*)) {
	struct ll_node* temp = head;
	while (temp) {
		outFunc(temp->data);
		temp = temp->next;
	}
}

void ll_removeElement(struct ll_node* head, bool (*rmFunc)(void* nodeData, void* comp), void* comp) {
	struct ll_node* prev = NULL;
	struct ll_node* temp;
	while (head) {
		if (rmFunc(head->data, comp)) {
			if (prev == NULL) {
				temp = head->next;
				free(head->data);
				free(head);	
			}
			else {
				prev->next = head->next;
				free(head->data);
				free(head);
				head = prev->next;
			}
			
		}
		else {
			prev = head;
			head = head->next;
		}
	}
}
