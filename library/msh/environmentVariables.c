#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "environmentVariables.h"


struct environmentVariable* createEnvironmentVariable(char* name, char* value) {
    struct environmentVariable* ev = (struct environmentVariable*) malloc(sizeof(struct environmentVariable));
    ev->name = name;
    ev->value = value;

    return ev;
}

int index_environmentVariable(struct hm_map* map, void* d) {
    environmentVariable* data = (environmentVariable*) d;
    int total = 0;
    for (int i = 0; i < strlen(data->name); i++) total += data->name[i];

    return total % map->bucketNum;
}

bool compare_environmentVariable(void* nd, void* d) {
    environmentVariable* nodeData = (environmentVariable*) nd;
    environmentVariable* data = (environmentVariable*) d;

    return (!strcmp(nodeData->name, data->name)) ? true: false;
}

void free_environmentVariable(void* d) {
    environmentVariable* data = (environmentVariable*) d;
    free(data->name);
    free(data->value);
}

void output_environmentVariable(void* d) {
    environmentVariable* data = (environmentVariable*) d;
    printf("Variable name: %s\tVariable value: %s\n", data->name, data->value);
}