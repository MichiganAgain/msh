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

#define term_cursor_left(x) printf("\x1b[%dD", x)
#define term_cursor_right(x) printf("\x1b[%dC", x)
#define term_erase_line() printf("\x1b[K")
#define term_get_cursor_pos() printf("\x1b[6n")


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
int term_getKey(int c);

#endif
