#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "signalHandlers.h"


void setSignalHandlers() {  
    signal(SIGINT, interruptSignalHandler);
}

void interruptSignalHandler(int signal) {
    static const char* const INT_MESSAGE = "\n";
    write(STDIN_FILENO, INT_MESSAGE, strlen(INT_MESSAGE));
}
