#include <string.h>
#include <ctype.h>

#include "library/msh/msh.h"

int main(int argc, char* argv[], char* envp[]) {
	// char* fakeEnvp[] = {strdup("OLIVE=oil"), strdup("HELLO=world"), strdup("nani=anime"), NULL};
	for (int i = 0; envp[i] != NULL; i++) printf("%s\n", envp[i]);
	msh_init(envp);
	msh_loop();
	msh_clean();

	return 0;
}
