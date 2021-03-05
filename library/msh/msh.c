#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>

#include "msh.h"
#include "../terminal/terminalHandler.h"
#include "../datastructures/HashMap/hashMap.h"
#include "environmentVariables.h"
#include "builtins.h"
#include "aliases.h"

#define BUFFER_SIZE 128


static struct hm_hashMap* environmentVariableMap;
static struct hm_hashMap* aliasMap;
static struct hm_hashMap* builtinMap;


void msh_init() {
	setvbuf(stdin, NULL, _IONBF, 0);	// disable input buffering
	setvbuf(stdout, NULL, _IONBF, 0);	// disable output buffering
	term_saveOriginalTerm();
	term_disableLocalFlag(ICANON | ECHO | ISIG);
	
    environmentVariableMap = hm_initialise(index_environmentVariable, compare_environmentVariable, NULL, NULL, output_environmentVariable);
    aliasMap = hm_initialise(index_alias, compare_alias, NULL, NULL, output_alias);
    builtinMap = hm_initialise(index_builtin, compare_builtin, NULL, NULL, output_builtin);

    msh_init_builtins();
	msh_init_aliases();
}

static void msh_init_builtins() {
    hm_insert(builtinMap, "cd", builtin_cd);
    hm_insert(builtinMap, "alias", builtin_alias);
}

static void msh_init_aliases() {
	hm_insert(aliasMap, "export", "export me out of debt");
}

void msh_loop() {
    bool loop = true;
    while (loop) {
        msh_printPrompt();

        char* line = msh_readline(stdin);

		if (line) {
			char** tokens = msh_parse(line);
			//for (int i = 0; tokens[i] != NULL; i++) printf("Token [%d] = %s\n", i, tokens[i]);

			if (strlen(line) > 0) msh_execute(tokens);
			free(tokens);
			free(line);
		}
		else loop = false;
    }
}

static void msh_redraw(char* line) {
	term_erase_line();
	int cursorShifts = 0;
	while (*line != '\0') {	
		if (!iscntrl(*line)) {
			putchar(*line);
			cursorShifts++;
		}
		line++;
	}
	if (cursorShifts > 0) term_cursor_left(cursorShifts);
}

char* msh_readline(FILE* filedes) {
	int cursorIndex = 0;
    unsigned int currentBufferSize = BUFFER_SIZE;
    char* line = (char*) malloc(sizeof(char) * currentBufferSize);
	*line = '\0';

    int c;
    while ((c = getc(filedes)) != '\n') {
		if (!iscntrl(c)) {
			if (strlen(line) + 1 >= currentBufferSize) {	// +1 for '\0'
				currentBufferSize *= 2;
				char* temp = (char*) malloc(sizeof(char) * currentBufferSize);
				strncpy(temp, line, strlen(line) + 1); // +1 for '\0'
				free(line);
				line = temp;
			}

			msh_shiftString(&line[cursorIndex], &line[cursorIndex + 1]);
			line[cursorIndex] = (char)c;
			msh_redraw(&line[cursorIndex]);
			cursorIndex++;
			term_cursor_right(1);
		}
		else {
			if (c == TERM_EOF && strlen(line) == 0) {
				free(line);
				return NULL;
			}
			else if (c == TERM_KEY_DELETE) {
				if (cursorIndex > 0) {
					msh_shiftString(&line[cursorIndex], &line[cursorIndex - 1]);
					term_cursor_left(1);
					cursorIndex--;
					msh_redraw(&line[cursorIndex]);
				}
			}
			else {
				int key = term_handleEscapeSequence(c);
				// printf("key: %d\n", key);

				switch (key) {
					case TERM_ARROW_UP:
						break;
					case TERM_ARROW_RIGHT:
						if (cursorIndex < strlen(line)) {
							term_cursor_right(1);
							cursorIndex++;
						}
						break;
					case TERM_ARROW_DOWN:
						break;
					case TERM_ARROW_LEFT:
						if (cursorIndex > 0) {
							term_cursor_left(1);
							cursorIndex--;
						}
						break;
					default:
						break;
				}
			}
			fflush(stdin);	// else flush stdin to get rid of any additional characters like ^[D
		}
    }

	if (c == '\n') printf("\n");
    return line;
}

static void msh_printPrompt() {
    printf("msh 0.0 # ");
}

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
        else if ((c == '"' && inSpeech) || (c == '\'' && inApostraphe)) { // when found terminating char
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

static void msh_shiftString(char* stringToShift, char* shiftPoint) {
	int pointerDifference = shiftPoint - stringToShift;
	char c;

	if (pointerDifference == 0) return;
	if (pointerDifference < 0) {	// shifting to the left (string to shift is to the right of the shift point)
		while ((c = *(stringToShift)) != '\0') { // ++string so can -- at end to set another null char
			*(stringToShift + pointerDifference) = c;
			stringToShift++;
		}
		*(stringToShift + pointerDifference) = '\0';
	}
	else { // shifting to the right (string to shift is to the left of the shift point)
		char* endOfStringIndex = stringToShift;
		while (*endOfStringIndex != '\0') endOfStringIndex++;

		while (endOfStringIndex >= stringToShift) {
			c = *endOfStringIndex;
			*(endOfStringIndex + pointerDifference) = c;
			endOfStringIndex--;
		}
	}
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
	struct termios modifiedTerm;
	term_getCurrentTerm(&modifiedTerm);
	term_restoreOriginalTerm();

	char* aliasValue = NULL;
	if ((aliasValue = hm_find(aliasMap, tokens[0])) != NULL) {
		printf("You entered an alias: %s\tvalue: %s\n", tokens[0], aliasValue);
	}

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

	term_setCurrentTerm(&modifiedTerm);
}

void msh_clean() {
    hm_free(environmentVariableMap);
    hm_free(aliasMap);
    hm_free(builtinMap);
	term_restoreOriginalTerm();
    printf("\n");
}
