#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "msh.h"
#include "../terminal/terminalHandler.h"
#include "../datastructures/HashMap/hashMap.h"
#include "environmentVariables.h"
#include "builtins.h"
#include "aliases.h"

#define BUFFER_SIZE 1024


static struct hm_hashMap* environmentVariableMap;
static struct hm_hashMap* aliasMap;
static struct hm_hashMap* builtinMap;


void msh_init() {
	term_saveTerm();
	term_disableCanonicalMode();
	term_disableEcho();

    environmentVariableMap = hm_initialise(index_environmentVariable, compare_environmentVariable, NULL, NULL, output_environmentVariable);
    aliasMap = hm_initialise(index_alias, compare_alias, NULL, NULL, output_alias);
    builtinMap = hm_initialise(index_builtin, compare_builtin, NULL, NULL, output_builtin);

    msh_init_builtins();
}

static void msh_init_builtins() {
    hm_insert(builtinMap, "cd", builtin_cd);
    hm_insert(builtinMap, "alias", builtin_alias);
}

void msh_loop() {
    char* line = NULL;
    bool loop = true;
    while (loop) {
        msh_printPrompt();

        line = msh_readline(stdin);

        char** tokens = msh_parse(line);
        //for (int i = 0; tokens[i] != NULL; i++) printf("Token [%d] = %s\n", i, tokens[i]);
        if (strlen(line) > 0) msh_execute(tokens);
        free(tokens);
    }
    free(line);
}

char* msh_readline(FILE* filedes) {
    unsigned int characterIndex = 0;    // current free index
    unsigned int currentBufferSize = BUFFER_SIZE;
    char* line = (char*) malloc(sizeof(char) * currentBufferSize);

    int c;
    while ((c = getc(filedes)) != '\n') {
		if (!iscntrl(c)) {
			putchar(c);	// if is a printable character
			if (characterIndex + 1 >= currentBufferSize) {	// +1 for '\0' after while loop
				currentBufferSize *= 2;
				char* temp = (char*) malloc(sizeof(char) * currentBufferSize);
				strncpy(temp, line, characterIndex);
				free(line);
				line = temp;
			}
        	line[characterIndex++] = (char)c;
		}
		else {
			printf("Control char entered: %d\n", c);
			fflush(stdin);	// else flush stdin to get rid of any additional characters like ^[D
		}
    }
    line[characterIndex] = '\0';
	if (c == '\n') printf("\n");
    else if (c == EOF) exit(0);
    return line;
}

static void msh_printPrompt() {
    printf("msh 0.0 # ");
}

static void msh_shiftString();

static char* msh_extractToken(char** line) {
    char* originalLine = *line;
    char* firstQuotationAppearance;
    bool inApostraphe = false;
    bool inSpeech = false;

    char c;
    if (**line == '\0') return NULL;
    while ((c = **line) != '\0' && c != '\n') {
        if (c == ' ' && !inApostraphe && !inSpeech) {
            **line = '\0';
            (*line)++;
            return originalLine;
        }
        else if ((c == '"' && inSpeech) || (c == '\'' && inApostraphe)) {
            *firstQuotationAppearance = '\e';
            **line = '\e';
            inApostraphe = inSpeech = false;
        }

        if (c == '"' && !inSpeech && !inApostraphe) {
            inSpeech = true;
            firstQuotationAppearance = *line;
        }
        else if (c == '\'' && !inSpeech && !inApostraphe) {
            inApostraphe = true;
            firstQuotationAppearance = *line;
        }

        (*line)++;
    }

    return originalLine;
}

static char** msh_parse(char* line) {
    unsigned int currentBufferSize = TOKEN_BUFFER_SIZE;
    char** tokens = (char**) malloc(sizeof(char*) * currentBufferSize);
    char* token;
    int tokensRead = 0;

    while((token = msh_extractToken(&line)) != NULL) {
        if (tokensRead + 1 >= currentBufferSize) {
            currentBufferSize *= 2;
            char** temp = (char**) malloc(sizeof(char*) * currentBufferSize);
            memcpy(temp, tokens, sizeof(char*) * tokensRead); // copy pointers to strings
            free(tokens); // free pointers to strings, not actual strings
            tokens = temp;
        }
        tokens[tokensRead++] = token;
    }
    tokens[tokensRead] = NULL;

    return tokens;
}

static void msh_execute(char** tokens) {
    pid_t pid = fork();

    if (pid == -1) {
        printf("Error forking self\n");
        exit(-1);
    }
    else if (pid == 0) { // in child
        execvp(tokens[0], tokens);
        printf("msh: %s command does not exist\n", tokens[0]);
        exit(EXIT_FAILURE); // kill the child
    }
    else { // in parent
        waitpid(pid, NULL, 0);
    }
}

void msh_clean() {
    hm_free(environmentVariableMap);
    hm_free(aliasMap);
    hm_free(builtinMap);
    printf("\n");
}
