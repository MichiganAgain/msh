#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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


/*
	Specify stdin / stdout to be unbuffered and set the terminal mode to raw
	Initialise hash maps with functions for various types and call functions to add values to them
*/
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


/*
	Add builtin values to builtin hash map
*/
static void msh_init_builtins() {
    hm_insert(builtinMap, "cd", builtin_cd);
    hm_insert(builtinMap, "alias", builtin_alias);
}

/*
	Add alias values to alias hash map
*/
static void msh_init_aliases() {
	hm_insert(aliasMap, "export", "export me out of debt");
}

/*
	Loop through, getting lines from user and executing them until the
	readline function return NULL.  In which case and EOF was encountered
*/
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

/*
	Redraw (currently) everything from the cursor onwards to correctly display internal
	input buffer on the terminal
*/
static void msh_redraw(char* line) {
	int cursorShifts = 0;
	while (*line != '\0') {
		if (!iscntrl(*line)) {
			putchar(*line);
			cursorShifts++;
		}
		line++;
	}
	term_erase_line(); // redundant when adding a character, necessary when erasing
	if (cursorShifts > 0) term_cursor_left(cursorShifts);
}

/*
	Read from file descriptor until a newline is encountered, in which case return.
	If user enters control character, then handle it.
*/
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
		else {	// then it is a control character
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
				int key = term_getKey(c);

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

/*
	Print prompt before accepting user internet
*/
static void msh_printPrompt() {
    printf("msh 0.0 # ");
}

/*
	Used to shift a portion of a string to another point in that string.
	Useful for when a character in the internal buffer is erased or added
	and the buffer at that / to the right of that character needs to be shifted
	to the left or the right.
*/
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

/*
	This function will always return the *value* of the string pointer that was passed to it.
	However, the actual pointer variable will be shifted until the delimiter is found, where
	the character at that location will be turned into a null char ('\0') and then the variable
	will further be incremented so it points the the next string, before retuturning from the function.

	This function acts in a very similar way to the C stdlib strsep function, but there is more
	control with this function.
*/
static char* msh_extractToken(char** line) {
    char* originalLine = *line;
    bool inApostraphe = false;
    bool inSpeech = false;

    char c;
    if (**line == '\0') return NULL;
    while ((c = **line) != '\0') {
		if ((c == ' ' && !inApostraphe && !inSpeech) || c == '\n') {
            **line = '\0';
            (*line)++;
            return originalLine;
        }

		if (c == '"' && !inSpeech && !inApostraphe) {
            inSpeech = true;
			msh_shiftString((*line) + 1, *line);
			(*line)--; // mitigate the increment at end of loop
        }
        else if (c == '\'' && !inSpeech && !inApostraphe) {
            inApostraphe = true;
			msh_shiftString((*line) + 1, *line);
			(*line)--; // mitigate the increment at end of loop
        }
        else if ((c == '"' && inSpeech) || (c == '\'' && inApostraphe)) { // when found terminating char
            inApostraphe = inSpeech = false;
			msh_shiftString((*line) + 1, *line);
        }

        (*line)++;
    }

    return originalLine;
}

/*
	This function will generate an array of char* tokens, where each token will point to a string
	containing either the command or the command arguments.  It uses the extractToken function to acheive this.
*/
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

/*
	The tokens generated from parsing the input line will be sent to this function to be executed.
	The first token in the array will be the command, and the rest (if any) will act as its arguments.
	The original terminal will be restored as most applications launched, if they are expecting
	input, will be line buffered (canonical) but the terminal will be reverted back to its modified
	state before the function returns.

	The parent will fork itself and wait for the child to terminate before proceeding,
*/
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

/*
	Free data structures and restore terminal to its original state.
*/
void msh_clean() {
    hm_free(environmentVariableMap);
    hm_free(aliasMap);
    hm_free(builtinMap);
	term_restoreOriginalTerm();
    printf("\n");
}
