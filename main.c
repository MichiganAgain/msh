#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define TOKEN_BUFFER_SIZE 32
#define STRING_BUFFER_SIZE 512

void msh_loop();
char* msh_getline(FILE* stream);
char** msh_parse(char* line);
void msh_execute(char** tokens);

char* msh_getline(FILE* stream) {
	int currentBufferSize = STRING_BUFFER_SIZE;
	char* line = (char*) malloc(sizeof(char) * currentBufferSize);
	
	int charactersRead = 0;
	int c;
	while ((c = getc(stdin)) != EOF && c != '\n') {
		if (charactersRead + 1 >= currentBufferSize) {
			currentBufferSize *= 2;
			char* temp = (char*) malloc(sizeof(char) * currentBufferSize);
			strncpy(temp, line, charactersRead);
			free(line);
			line = temp;
		}
		line[charactersRead++] = (char)c;
	}
	line[charactersRead] = '\0'; // replace EOF or newline with null char

	if (c == EOF) exit(0);

	return line;
}

void msh_loop() {
	char* line = NULL;
	size_t linecap = 0;
	ssize_t linelen;

	bool loop = true;
	while (loop) {
		printf("msh 0.0 # ");
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
		if (tokens[0] != NULL && strlen(tokens[0]) > 1) msh_execute(tokens);

		for (int i = 0; tokens[i] != NULL; i++) free(tokens[i]);
	}
	free(line);
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
		strerror(errno);
		printf("msh: error executing %s\n", tokens[0]);
		exit(EXIT_FAILURE);
	}
	else { // in parent
		waitpid(pid, NULL, 0);
	}
}

int main(int argc, char* argv[], char* envp[]) {
	msh_loop();

	return 0;
}
