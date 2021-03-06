#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "terminalHandler.h"


inline void term_cursor_left(int x) { printf("\x1b[%dD", x); }
inline void term_cursor_right(int x) { printf("\x1b[%dC", x); }
inline void term_erase_line() { printf("\x1b[K"); }

void term_error(char* errmsg) {
	printf("%s\n", errmsg);
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
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &termSettings)) != 0)
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
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &termSettings)) != 0)
		term_error("Failed to set terminal attributes");
}

/*
	Get current terminal settings and put them in struct provided
*/
void term_getCurrentTerm(struct termios* term) {
	int result;

	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, term)) != 0)
		term_error("Failed to get terminal attributes");
}

/*
	Set current terminal settings and put them in struct provided
*/
void term_setCurrentTerm(struct termios* term) {
	int result;
	
	term_ensureTerminalDevice();
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, term)) != 0)
		term_error("Failed to set terminal attributes");
}

/*
	Save current terminal settings that can be restored with the restore function
*/
void term_saveOriginalTerm() {
	int result;
	term_ensureTerminalDevice();
	if ((result = tcgetattr(STDIN_FILENO, &originalTerm)) != 0)
		term_error("Failed to save terminal attributes");
}

/*
	Restore terminal settings that have previously been saved with the save function
*/
void term_restoreOriginalTerm() {
	int result;
	term_ensureTerminalDevice();
	if ((result = tcsetattr(STDIN_FILENO,TCSADRAIN, &originalTerm)) != 0)
		term_error("Failed to restore terminal attributes");
}

/*
	Read from terminal device any escape sequence and return a constant depending on its value.
	As escape sequences can be sent to stdin as multiple character, use the read syscall to determine
	the escape sequence size and value
*/
int term_handleEscapeSequence(int c) {
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
