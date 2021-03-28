#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>

#include "terminalHandler.h"


void term_error(char* errmsg) {
	printf("TERMINAL ERROR: %s\n", errmsg);
	exit(EXIT_FAILURE);
}

/*
	Ensure stdin is connected to a terminal device (tty)
*/
void term_ensureTerminalDevice() {
	if (!isatty(STDIN_FILENO))
		term_error("Standard input is not connected to a terminal device file");
}

/*
	Enable flags on local member of termios structure
*/
void term_enableLocalFlag(tcflag_t flags) {
	struct termios termSettings;
	int result;

	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, &termSettings)) != 0)
		term_error("Failed to get terminal attributes");

	termSettings.c_lflag &= flags;
	if ((result = tcsetattr(STDIN_FILENO, TCSADRAIN, &termSettings)) != 0)
		term_error("Failed to set terminal attributes");
}

/*
	Disable flags on local member of termios structure
*/
void term_disableLocalFlag(tcflag_t flags) {
	struct termios termSettings;
	int result;

	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, &termSettings)) != 0)
		term_error("Failed to get terminal attributes");

	termSettings.c_lflag &= ~flags;
	if ((result = tcsetattr(STDIN_FILENO, TCSADRAIN, &termSettings)) != 0)
		term_error("Failed to set terminal attributes");
}

/*
	Get current terminal settings and put them in struct provided
*/
struct termios term_getTerm() {
	int result;
	struct termios term;

	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, &term)) != 0)
		term_error("Failed to get terminal attributes");
	
	return term;
}

/*
	Set current terminal settings and put them in struct provided
*/
void term_setTerm(struct termios term) {
	int result;
	
	term_ensureTerminalDevice();
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &term)) != 0)
		term_error("Failed to set terminal attributes");
}

/*
	Save current terminal settings that can be restored with the restore function
*/
void term_saveTerm(struct termios term) {
	savedTerminal = term;
}

/*
	Restore terminal settings that have previously been saved with the save function
*/
void term_restoreTerm() {
	int result;
	term_ensureTerminalDevice();
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &savedTerminal)) != 0)
		term_error("Failed to restore terminal attributes");
}

void term_getCursorPosition(struct term_position* pos) {
	const int BUFF_SIZE = 32;
	char posBuff[BUFF_SIZE];
	int i = 0;

	term_get_cursor_pos();
	while (i < BUFF_SIZE - 1) {	// - 1 for null char
		if (read(STDIN_FILENO, &posBuff[i], 1) != 1) break;
		if (posBuff[i] == 'R') break;
		i++;
	}
	posBuff[i] = '\0';

	if (posBuff[0] != '\x1b' || posBuff[1] != '[') return;
	sscanf(&posBuff[2], "%d;%d", &(pos->row), &(pos->column));
}

void term_getWindowSize(struct term_position* winSize) {
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) return;
	winSize->column = ws.ws_col;
	winSize->row = ws.ws_row;
}

/*
	Read from terminal device any escape sequence and return a constant depending on its value.
	As escape sequences can be sent to stdin as multiple character, use the read syscall to determine
	the escape sequence size and value
*/
int term_getEscapeKey(char c) {
	char seq[3];
	if (read(STDIN_FILENO, &seq[0], 1) != 1) return TERM_ESCAPE_CHAR;
	if (read(STDIN_FILENO, &seq[1], 1) != 1) return TERM_ESCAPE_CHAR;

	if (seq[0] == '[') {
		switch (seq[1]) {
			case 'A': return TERM_ARROW_UP;
			case 'B': return TERM_ARROW_DOWN;
			case 'C': return TERM_ARROW_RIGHT;
			case 'D': return TERM_ARROW_LEFT;
		}
	}

	return TERM_UNKNOWN;
}
