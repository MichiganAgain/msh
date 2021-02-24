#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "msh.h"
#include "signalHandlers.h"


int main(int argc, char* argv[], char* envp[]) {
	setSignalHandlers();
	msh_loop();

	return 0;
}

