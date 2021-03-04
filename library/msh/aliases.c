#include <stdio.h>
#include <string.h>

#include "aliases.h"

int index_alias(void* key) {
    char* keyString = (char*) key;
    int total = 0;
    for (int i = 0; i < strlen(keyString); i++) total += keyString[i];

    return total;
}

bool compare_alias(void* k1, void* k2) {
    char* keyString1 = (char*) k1;
    char* keyString2 = (char*) k2;

    return (!strcmp(keyString1, keyString2)) ? true: false;
}

void output_alias(void* key, void* value) {
    char* keyString = (char*) key;
    char* valueString = (char*) value;

    printf("Name: %s\tValue: %s\n", keyString, valueString);
}
