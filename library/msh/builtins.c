#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"


struct builtin* createShellBuiltin(char* command, void (*exeFunc)()) {
    struct builtin* temp = (struct builtin*) malloc(sizeof(struct builtin));
    temp->command = command;
    temp->exeFunc = exeFunc;

    return temp;
}

int index_builtin(struct hm_map* map, void* keyData) {
    char* key = (char*) keyData;
    int total = 0;
    for (int i = 0; i < strlen(key); i++) total += data->command[i];
    return total % map->bucketNum;
}

bool compare_builtin(void* nk, void* k) {
    char* nodeKey = (char*) nk;
    char* key = (char*) k;

    return (!strcmp(nodeKey, key)) ? true: false;
}

void free_builtin(void* d) {
    builtin* data = (builtin*) d;
    free(data->command);
}

void output_builtin(void* d) {
    builtin* data = (builtin*) d;
    printf("command: %s\tfunction: %p\n", data->command, data->exeFunc);
}

void builtin_cd() {
    
}