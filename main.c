#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TOKEN_BUFFER_SIZE 32


void msh_loop();
char** msh_parse(char* line);

void msh_loop() {
	char* line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while (true) {
		printf("msh 0.0 # ");
		linelen = getline(&line, &linecap, stdin);
		if (linelen < 0 && !feof(stdin)) {
			printf("Error reading line\n");
			exit(-1);
		}
		if (feof(stdin)) exit(0);


		char** tokens = NULL;
		tokens = msh_parse(line);
	}
	free(line);	// free once as end as getline does realloc
}

char** msh_parse(char* line) {
	char** tokens = (char**) malloc(sizeof(char*) * TOKEN_BUFFER_SIZE);
	char* token;
	size_t tokenLength;

	int tokensRead = 0;
	while((token = strsep(&line, " ")) != NULL) {
		tokenLength = strlen(token);
		tokens[tokensRead] = (char*) malloc(sizeof(char) * tokenLength + 1);
		strcpy(tokens[tokensRead], token);
		tokensRead++;
	}

	return tokens;
}

int main(int argc, char* argv[], char* envp[]) {
	msh_loop();

	return 0;
}
