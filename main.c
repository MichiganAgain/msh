#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void msh_loop();
void msh_parse(char* line);

void msh_loop() {
	char* line;
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
	}
}

void msh_parse(char* line) {
	char* cmd[];
	char* token;
	char* tofree = line;
	while((token = strsep(&line, " ")) != NULL) {
		printf("Token: %s\n", token);
	}
	free(tofree);
}

int main(int argc, char* argv[], char* envp[]) {
	msh_loop();

	return 0;
}
