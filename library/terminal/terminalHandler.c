#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "terminalHandler.h"

void term_error(char* errmsg) {
	printf("%s\n", errmsg);
	exit(EXIT_FAILURE);
}

void term_ensureTerminalDevice() {
	if (!isatty(STDIN_FILENO))
		term_error("Standard input is not connected to a terminal device file");
}

static void term_enableFlag(tcflag_t flags) {
	struct termios termSettings;
	int result;

	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, &termSettings)) != 0)
		term_error("Failed to get terminal attributes");

	termSettings.c_lflag &= flags;
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &termSettings)) != 0)
		term_error("Failed to set terminal attributes");
}

static void term_disableFlag(tcflag_t flags) {
	struct termios termSettings;
	int result;

	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, &termSettings)) != 0)
		term_error("Failed to get terminal attributes");

	termSettings.c_lflag &= ~flags;
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &termSettings)) != 0)
		term_error("Failed to set terminal attributes");
}


void term_enableCanonicalMode() {
	term_enableFlag(ICANON);
}

void term_disableCanonicalMode() {
	term_disableFlag(ICANON);
}

void term_enableEcho() {
	term_enableFlag(ECHO);
}

void term_disableEcho() {
	term_disableFlag(ECHO);
}

void term_saveTerm() {
	int result;
	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, &savedTerm)) != 0)
		term_error("Failed to restore terminal attributes");
}

void term_restoreTerm() {
	int result;
	term_ensureTerminalDevice();
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &savedTerm)) != 0)
		term_error("Failed to restore terminal attributes");
}
