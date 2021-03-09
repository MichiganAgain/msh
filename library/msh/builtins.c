#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtins.h"


int index_builtin(void* key) {
    char* keyString = (char*) key;
    int total = 0;
    for (int i = 0; i < strlen(keyString); i++) total += keyString[i];

    return total;
}

bool compare_builtin(void* k1, void* k2) {
    char* keyString1 = (char*) k1;
    char* keyString2 = (char*) k2;

    return (!strcmp(keyString1, keyString2)) ? true: false;
}

void output_builtin(void* key, void* value) {
    char* keyString = (char*) key;

    printf("Built in name: %s\tFunction pointer: %p\n", keyString, value);
}


// built in functions

int builtin_cd(char** tokens) {
	
	chdir(tokens[1]);
	return 0;
}

int builtin_alias(char** tokens) {


    return 0;
}