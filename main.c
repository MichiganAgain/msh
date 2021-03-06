#include "library/msh/msh.h"
#include <ctype.h>

int main(int argc, char* argv[], char* envp[]) {
	msh_init();
	msh_loop();
	msh_clean();

	return 0;
}
