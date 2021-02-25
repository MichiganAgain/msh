#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "library/msh/msh.h"
#include "library/signal/signalHandlers.h"


int main(int argc, char* argv[], char* envp[]) {
	setSignalHandlers();
	msh_init();

	return 0;
}

