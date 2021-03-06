#ifndef ENVIRONMENT_VARIABLES_H
#define ENVIRONMENT_VARIABLES_H

#include <stdbool.h>


// Functions provided for hash map / linked list
int index_environmentVariable(void* key);
bool compare_environmentVariable(void* k1, void* k2);
void output_environmentVariable(void* key, void* value);


#endif
