#ifndef BUILTINS_H
#define BUILTINS_H

#include "../datastructures/HashMap/hashMap.h"

typedef struct builtin {
    char* command;
    void (*exeFunc)();
} builtin;

struct builtin* createShellBuiltin(char* command, void (*exeFunc)());
int index_builtin(struct hm_map* map, void* d);
bool compare_builtin(void* nd, void* d);
void free_builtin(void* d);
void output_builtin(void* d);

// builtin functions
void builtin_cd();


#endif
