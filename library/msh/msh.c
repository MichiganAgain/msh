#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "msh.h"
#include "../datastructures/hashMap.h"
#include "environmentVariables.h"


static hm_map* environmentVariableMap;

void msh_init() {
    environmentVariableMap = hm_initialise(index_environmentVariable, compare_environmentVariable, free_environmentVariable, output_environmentVariable);
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("HOME"), strdup("THIS IS HOME")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("JAVA"), strdup("THIS IS JAVA")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("LAVA"), strdup("THIS IS LAVA")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("JOHN"), strdup("THIS IS JOHN")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("DEAN"), strdup("THIS IS DEAN")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("MAIL"), strdup("THIS IS MAIL")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("LAWS"), strdup("THIS IS LAWS")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("BACK"), strdup("THIS IS BACK")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("NACK"), strdup("THIS IS NACK")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("REDD"), strdup("THIS IS REDD")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("PAWN"), strdup("THIS IS PAWN")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("KILL"), strdup("THIS IS KILL")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("INTT"), strdup("THIS IS INTT")));
    hm_insert(environmentVariableMap, createEnvironmentVariable(strdup("HOME"), strdup("THIS IS HOME")));

    msh_loop();
}

void msh_loop() {
    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    bool loop = true;
    while (loop) {
        msh_printPrompt();
        linelen = getline(&line, &linecap, stdin);
        if (linelen < 0 && !feof(stdin)) {
            printf("linelen: %d\n", (int)linelen);
            printf("Error reading line\n");
            exit(-1);
        }
        if (feof(stdin)) loop = false;
        line[linelen - 1] = '\0'; // replace \n or EOF with \0 (C library)

        char** tokens = NULL;
        tokens = msh_parse(line);
        if (tokens[0] != NULL && strlen(tokens[0]) > 0) msh_execute(tokens);

        for (int i = 0; tokens[i] != NULL; i++) free(tokens[i]);
    }
    free(line);
}

void msh_printPrompt() {
    printf("msh 0.0 # ");
}

char** msh_parse(char* line) {
    unsigned int currentBufferSize = TOKEN_BUFFER_SIZE;
    char** tokens = (char**) malloc(sizeof(char*) * currentBufferSize);
    char* token;
    size_t tokenLength;

    int tokensRead = 0;
    while((token = strsep(&line, " ")) != NULL) {
        if (tokensRead + 1 >= currentBufferSize) {
            currentBufferSize *= 2;
            char** temp = (char**) malloc(sizeof(char*) * currentBufferSize);
            memcpy(temp, tokens, sizeof(char*) * tokensRead); // copy pointers to strings
            free(tokens); // free pointers to strings, not actual strings
            tokens = temp;
        }
        tokenLength = strlen(token);
        tokens[tokensRead] = (char*) malloc(sizeof(char) * tokenLength + 1);
        strcpy(tokens[tokensRead], token);
        tokens[tokensRead][tokenLength] = '\0';
        tokensRead++;
    }
    tokens[tokensRead] = NULL;

    return tokens;
}

void msh_execute(char** tokens) {
    pid_t pid = fork();

    if (pid == -1) {
        printf("Error forking self\n");
        exit(-1);
    }
    else if (pid == 0) { // in child
        execvp(tokens[0], tokens);
        printf("msh: %s command does not exist\n", tokens[0]);
        exit(EXIT_FAILURE);
    }
    else { // in parent
        waitpid(pid, NULL, 0);
    }
}
