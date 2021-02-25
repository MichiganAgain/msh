#ifndef ENVIRONMENT_VARIABLES_H
#define ENVIRONMENT_VARIABLES_H

#include <stdbool.h>

#include "../datastructures/hashMap.h"

typedef struct environmentVariable {
    char* name;
    char* value;
} environmentVariable;

// strings for creating environment variable MUST be heap allocated so they can be modified
struct environmentVariable* createEnvironmentVariable(char* name, char* value);
int index_environmentVariable(struct hm_map* map, void* d);
bool compare_environmentVariable(void* nd, void* d);
void free_environmentVariable(void* d);
void output_environmentVariable(void* d);

#endif
