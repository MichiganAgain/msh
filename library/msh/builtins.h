#ifndef BUILTINS_H
#define BUILTINS_H

#include "../datastructures/HashMap/hashMap.h"


// Functions provided for hash map / linked list
int index_builtin(void* key);
bool compare_builtin(void* k1, void* k2);
void output_builtin(void* key, void* value);


// builtin functions
void builtin_cd();
void builtin_alias();

#endif
