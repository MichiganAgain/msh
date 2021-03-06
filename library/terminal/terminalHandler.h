#ifndef TERMINAL_HANDLER_H
#define TERMINAL_HANDLER_H

#include <stdio.h>
#include <termios.h>
#include <stdbool.h>

#define TERM_EOF 4
#define TERM_TAB 9
#define TERM_ESCAPE_CHAR 0x1b
#define TERM_KEY_DELETE 127

enum keys {TERM_ARROW_UP, TERM_ARROW_RIGHT, TERM_ARROW_DOWN, TERM_ARROW_LEFT, TERM_UNKNOWN};

static struct termios originalTerm;

void term_cursor_left(int x);
void term_cursor_right(int x);
void term_erase_line();

// Used to exit program if error occurs in any terminal functions
void term_error(char* errmsg);
// Ensure shell is connected to a terminal device file
void term_ensureTerminalDevice();
// Enable terminal flag for local termios member
void term_enableLocalFlag(tcflag_t flags);
// Disable terminal flag for local termios member
void term_disableLocalFlag(tcflag_t flags);
// Puts current terminal info from struct provided
void term_getCurrentTerm(struct termios* term);
// Set current terminal info from struct provided
void term_setCurrentTerm(struct termios* term);
// Used at beginning before modifying terminal
void term_saveOriginalTerm();
// Used for when exiting or launching another process
void term_restoreOriginalTerm();

// Returns an enum int value depending on key pressed or ANSI ASCII escape sequence entered
int term_handleEscapeSequence(int c);

#endif
