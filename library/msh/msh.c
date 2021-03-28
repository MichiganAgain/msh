#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <termios.h>
#include <errno.h>

#include "msh.h"
#include "../terminal/terminalHandler.h"
#include "../datastructures/hashMap.h"
#include "../datastructures/list.h"
#include "../string/string.h"
#include "environmentVariables.h"
#include "builtins.h"
#include "aliases.h"




/*
    Specify stdin / stdout to be unbuffered and set the terminal mode to raw
    Initialise hash maps with functions for various types and call functions to add values to them
*/
void msh_start(char* envp[]) {
    msh_initTerminal();
	
	term_getWindowSize(&terminalWindowSize);
	term_getCursorPosition(&globalCursorState);

    msh_init_builtins();
    msh_init_environments(envp);
    msh_init_aliases();

	// printf("COMPLETE: %d COMMAND: %d APOSTROPHE: %d QUOTATION: %d\r\n", TOKEN_COMPLETE, TOKEN_MISSING_COMMAND, TOKEN_MISSING_APOSTROPHE, TOKEN_MISSING_QUOTATION);

    msh_loop();
    msh_clean();
}

void msh_initTerminal() {
	setvbuf(stdin, NULL, _IONBF, 0);	// disable input buffering
    setvbuf(stdout, NULL, _IONBF, 0);	// disable output buffering

    struct termios originalTerm = term_getTerm();
    term_saveTerm(originalTerm);
    struct termios modifiedTerm = originalTerm;
    modifiedTerm.c_iflag &= ~(ICRNL | IXON);			// Disable carriage return to newline conversion and software control flow (ctr-s / ctr-q)
    modifiedTerm.c_oflag &= ~OPOST;					    // Disable output data processing (i.e \n being converted to \r\n) (output flags)
    modifiedTerm.c_lflag &= ~(ICANON | ECHO | ISIG);	// Disable canonical input, echo back and whether INTR, QUIT and SUSP characters are recognised
    modifiedTerm.c_cc[VMIN] = 0;                        // Requires no bytes read before returning from a read call
    modifiedTerm.c_cc[VTIME] = 1;                       // Wait for 0.1 seconds of no input before returning from a read call (but if a character is entered, it will return immediately)
    term_setTerm(modifiedTerm);
}

/*
    Add builtin values to builtin hash map
*/
static void msh_init_builtins() {
	builtinMap = hm_initialise(index_builtin, compare_builtin, NULL, NULL, output_builtin);
    hm_insert(builtinMap, "cd", builtin_cd);
    hm_insert(builtinMap, "alias", builtin_alias);
    hm_insert(builtinMap, "exit", builtin_exit);
}

/*
    Add alias values to alias hash map
*/
static void msh_init_aliases() {
	aliasMap = hm_initialise(index_alias, compare_alias, NULL, NULL, output_alias);
    hm_insert(aliasMap, "export", "exporting goods");
}

static void msh_init_environments(char* envp[]) {
	environmentVariableMap = hm_initialise(index_environmentVariable, compare_environmentVariable, NULL, NULL, output_environmentVariable);
    char* key, *value;
    if (envp != NULL) {
        for (int i = 0; envp[i] != NULL; i++) {
            key = string_strsep(&envp[i], "=");
            value = envp[i];
            envp[i] = key;
            hm_insert(environmentVariableMap, key, value);
        }
    }
}

/*
    Loop through, getting lines from user and executing them until the
    readline function return NULL.  In which case and EOF was encountered
*/
void msh_loop() {
    bool loop = true;
    while (loop) {
        char* line = msh_readinput(stdin);	// returns a complete token (which can contain subtokens (i.e seperated by &&))
        if (line) {
            char** tokens = msh_parse(line);

            if (strlen(line) > 0) msh_execute(tokens);
            free(tokens);
            free(line);
        }
        else loop = false;
    }
}

/*
    Redraw everything from the cursor onwards to correctly display internal
    input buffer on the terminal
*/
static void msh_redraw(char* line) {
	// term_set_cursor_pos(previousCursorState.row, previousCursorState.column);
    int cursorShifts = 0;
    char* originalLine = line;
    while (*line != '\0') {
        if (!iscntrl(*line)) cursorShifts++;
        line++;
    }

    printf("%s", originalLine);
    term_erase_line(); // redundant when adding a character, necessary when erasing
    if (cursorShifts > 0) term_cursor_left(cursorShifts);
}

/*
    Read from file descriptor until a newline is encountered, in which case return.
    If user enters control character, then handle it.
*/
char* msh_readinput() {
    int internalCursorIndex = 0, cursorBoundaryIndex = 0;
	int tokenStatus;
	string input;
	string_initialise(&input);
    
	msh_printPrompt(MSH_PROMPT_DEFAULT);
    bool readLoop = true;
    while (readLoop) {
        char c;
        size_t numRead = read(STDIN_FILENO, &c, 1);

        if (numRead <= 0) continue;
		int status = msh_handleKey(&input, c, &internalCursorIndex, &cursorBoundaryIndex);
		switch (status) {
			case EOF_PRESSED:
				string_free(&input);
				return NULL;
			case TOKEN_COMPLETE:
				readLoop = false;
		}
		// printf("Character val: %d\r\n", c);

        fflush(stdin);	// flush stdin to get rid of any redundant characters
    }
	char* inputStringBuffer = string_copyToBuffer(&input);
	// printf("Command: %s\r\n", inputStringBuffer);
	string_free(&input);

    return inputStringBuffer;
}

int msh_handleKey(struct string* input, char c, int* internalCursorIndex, int* cursorBoundaryIndex) {
	if (!iscntrl(c)) {
		term_getWindowSize(&terminalWindowSize);
		term_getCursorPosition(&globalCursorState);
		string_insertCharAt(input, c, *internalCursorIndex);
		msh_redraw(&(input->string[*internalCursorIndex]));
		(*internalCursorIndex)++;
		if (globalCursorState.column >= terminalWindowSize.column) printf("\r\n");
		else term_cursor_right(1);
		
		return NON_CONTROL_PRESSED;
	}
	else if (c == '\x1b') {  // Handle escape sequence
		int key = term_getEscapeKey(c);
		term_getWindowSize(&terminalWindowSize);
		term_getCursorPosition(&globalCursorState);
		switch (key) {
			case TERM_ESCAPE_CHAR:
				break;
			case TERM_ARROW_UP:
				break;
			case TERM_ARROW_RIGHT:
				if (*internalCursorIndex < input->freeIndex) {
					if (globalCursorState.column >= terminalWindowSize.column) printf("\r\n");
					else term_cursor_right(1);
					(*internalCursorIndex)++;
				}
				break;
			case TERM_ARROW_DOWN:
				break;
			case TERM_ARROW_LEFT:
				if (*internalCursorIndex > *cursorBoundaryIndex) {
					term_cursor_left(1);
					(*internalCursorIndex)--;
				}
				break;
		}
	}
	else {
		term_getWindowSize(&terminalWindowSize);
		term_getCursorPosition(&globalCursorState);
		int tokenStatus;
		switch (c) {
			case TERM_KEY_EOF:
				return EOF_PRESSED;
			case TERM_KEY_DELETE:
				if (*internalCursorIndex > *cursorBoundaryIndex) {
					if (globalCursorState.column == 1) term_set_cursor_pos(globalCursorState.row - 1, terminalWindowSize.column);
					else term_cursor_left(1);
					(*internalCursorIndex)--;
					string_erase(input, *internalCursorIndex, 1);
					msh_redraw(&(input->string[*internalCursorIndex]));
				}
				break;
			case TERM_KEY_ENTER:
				printf("\r\n");
				tokenStatus = msh_getTokenStatus(input->string);
				// printf("Token: %s\r\n", input.string);
				// printf("Token Status: %d\r\n", tokenStatus);
				switch (tokenStatus) {
					case TOKEN_MISSING_APOSTROPHE:
						msh_printPrompt(MSH_PROMPT_APOSTROPHE);
						string_appendChar(input, '\n');
						break;
					case TOKEN_MISSING_QUOTATION:
						msh_printPrompt(MSH_PROMPT_QUOTATION);
						string_appendChar(input, '\n');
						break;
					case TOKEN_MISSING_COMMAND:
						msh_printPrompt(MSH_PROMPT_COMMAND);
						break;
					case TOKEN_COMPLETE:
						return TOKEN_COMPLETE;
				}

				*internalCursorIndex = *cursorBoundaryIndex = input->freeIndex;
				return ENTER_PRESSED;

			case TERM_KEY_TAB:
				
				break;
		}
	}
	return UNKNOWN_KEY_PRESSED;
}

/*
    Print prompt before accepting user internet
*/
static void msh_printPrompt(char* prompt) {
    printf("%s", prompt);
}

/*
    This function will always return the *value* of the string pointer that was passed to it.
    However, the actual pointer variable will be shifted until the delimiter is found, where
    the character at that location will be turned into a null char ('\0') and then the variable
    will further be incremented so it points the the next string, before retuturning from the function.

    This function acts in a similar way to the C stdlib strsep function, but there is more
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
            string_shiftString((*line) + 1, *line);
            (*line)--; // mitigate the increment at end of loop
        }
        else if (c == '\'' && !inSpeech && !inApostraphe) {
            inApostraphe = true;
            string_shiftString((*line) + 1, *line);
            (*line)--; // mitigate the increment at end of loop
        }
        else if ((c == '"' && inSpeech) || (c == '\'' && inApostraphe)) { // when found terminating char
            inApostraphe = inSpeech = false;
            string_shiftString((*line) + 1, *line);
            (*line)--; // mitigate the increment at end of loop
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
    unsigned int currentBufferSize = MSH_TOKEN_BUFFER_SIZE;
    char** tokens = (char**) malloc(sizeof(char*) * currentBufferSize);
    char* token;
    int tokensRead = 0;

    while((token = msh_extractToken(&line)) != NULL) {
        if (tokensRead + 1 >= currentBufferSize) {
            currentBufferSize *= 2;
            char** temp = (char**) malloc(sizeof(char*) * currentBufferSize);
            memcpy(temp, tokens, sizeof(char*) * tokensRead); // copy pointers to strings
            free(tokens); // free old pointers to strings, not actual strings
            tokens = temp;
        }
        tokens[tokensRead++] = token;
    }
    tokens[tokensRead] = NULL;

    return tokens;
}

struct list_list* msh_generateEnvironmentTokens() {
    struct list_list* pairList = hm_list(environmentVariableMap);
    struct list_list* keyValueList = list_init(env_listFree);

    for (int i = 0; i < pairList->currentFreeIndex; i++) {
        struct hmll_pair* pair = pairList->data[i];
        char* key = pair->key;
        char* value = pair->value;

        char* environmentVariableToken = (char*) malloc(sizeof(char) * (strlen(key) + strlen("=") + strlen(value) + 1)); // + 1 for null char
        strcpy(environmentVariableToken, key);
        strcat(environmentVariableToken, "=");
        strcat(environmentVariableToken, value);

        list_append(keyValueList, environmentVariableToken);
    }

    list_free(pairList);
    list_append(keyValueList, NULL);
    return keyValueList;
}


/*
	This function checks if the token or tokens currently entered by the user is complete.
	Just because this function returns true, does not mean it is valid.  For example,
	echo "hi" && echo "bye"		is considered a complete token, which when later parsed will consist of two tokens.
	echo "hi" && %		is also considered a complete token, even though when parsed, the last token will be considered incorrect.
	echo "hi &&		is not considered a complete token as the user still needs to enter the closing quotation.
*/
static int msh_getTokenStatus(char* line) {
	size_t lineLength = strlen(line);
	bool inApostrophe, inQuotation, expectingCommand;
	inApostrophe = inQuotation = expectingCommand = false;

	for (int i = 0; i < lineLength; i++) {
		char c = line[i];
		if (c == '\'') {
			if (inApostrophe) inApostrophe = false;
			else if (!inApostrophe && !inQuotation) inApostrophe = true;
		}
		else if (c == '"') {
			if (inQuotation) inQuotation = false;
			else if (!inApostrophe && !inQuotation) inQuotation = true;
		}

		// printf("i: %d\tc: %c\r\n", i, c);
		// printf("In Apostrophe: %d\r\n", inApostrophe);
		// printf("In Quotation: %d\r\n\n", inQuotation);

		if (expectingCommand && !isspace(c)) expectingCommand = false;
		else if (!strcmp(&line[i], "&&") && !inQuotation && !inApostrophe) {
			expectingCommand = true;
			i++;	// only increment by one as end of loop will add the additional 1
		}
	}

	if (inApostrophe) return TOKEN_MISSING_APOSTROPHE;
	if (inQuotation) return TOKEN_MISSING_QUOTATION;
	if (expectingCommand) return TOKEN_MISSING_COMMAND;
	return TOKEN_COMPLETE;
}

/*
    execvpe is a GNU extension, hence will not always be supported.
    Therefore this function was created that uses the execve syscall and iterates through
    the PATH environment variable, adding the executable file name to the end of it and
    attempting to execute this path.  If this function ever returns then the executable
    file was not found in any of the PATH directories, as execve replaces the current
    running process with a new process.
*/
static void msh_execvpe(char* execFile, char* argv[], char* envp[]) {
    char* paths = hm_find(environmentVariableMap, "PATH");	// check for if paths becomes NULL
    char* splitPaths = (char*) malloc(strlen(paths) + 1);
    char* originalSplitPaths = splitPaths;
    char* pathToken;
    char* filePath;
    bool pathSeperatorExists;
    int execFileLength = strlen(execFile);

    strcpy(splitPaths, paths);
    while ((pathToken = string_strsep(&splitPaths, ":")) != NULL) {
        pathSeperatorExists = false;
        if (pathToken[strlen(pathToken) - 1] == '/') pathSeperatorExists = true;
        filePath = (char*) malloc(strlen(pathToken) + (pathSeperatorExists ? 0: 1) + execFileLength + 1); // + 1 for null char
        strcpy(filePath, pathToken);
        if (!pathSeperatorExists) strcat(filePath, "/");
        strcat(filePath, execFile);

        execve(filePath, argv, envp);
        free(filePath);
    }
    free(originalSplitPaths);
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
    struct termios modifiedTerm = term_getTerm();
    term_restoreTerm();

    struct list_list* environmentVariableList = msh_generateEnvironmentTokens();
    char** envp = (char**)environmentVariableList->data;
    

    char* aliasValue = NULL;
    if ((aliasValue = hm_find(aliasMap, tokens[0])) != NULL) {
        printf("You entered an alias: %s\tvalue: %s\n", tokens[0], aliasValue);
    }

    void (*builtInValue)() = NULL;
    if ((builtInValue = hm_find(builtinMap, tokens[0])) != NULL) builtInValue(tokens);


    else {
        pid_t pid = fork();
        if (pid == -1) {
            printf("Error forking self\n");
            exit(-1);
        }
        else if (pid == 0) { // in child
            msh_execvpe(tokens[0], tokens, envp);
            printf("msh: %s command does not exist\n", tokens[0]);
            exit(EXIT_FAILURE); // kill the child
        }
        else { // in parent
            waitpid(pid, NULL, 0);
        }
    }

    list_free(environmentVariableList);
    term_setTerm(modifiedTerm);
}

/*
    Free data structures and restore terminal to its original state.
*/
void msh_clean() {
    hm_free(environmentVariableMap);
    hm_free(aliasMap);
    hm_free(builtinMap);
    term_restoreTerm();
    printf("\n");
}
