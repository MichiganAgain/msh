#ifndef TERMINAL_HANDLER_H
#define TERMINAL_HANDLER_H

#include <stdio.h>
#include <termios.h>
#include <stdbool.h>


#define TERM_EOF 4
#define TERM_ESCAPE_CHAR 0x1b
#define TERM_KEY_DELETE 127

#define term_cursor_left(x) printf("\x1b[%dD", x);
#define term_cursor_right(x) printf("\x1b[%dC", x);
#define term_erase_line() printf("\x1b[K");
#define term_save_cursor() printf("\x1b[s");
#define term_restore_cursor() printf("\x1b[u");

enum keys {TERM_ARROW_UP, TERM_ARROW_RIGHT, TERM_ARROW_DOWN, TERM_ARROW_LEFT, TERM_UNKNOWN};

static struct termios originalTerm;

void term_error(char* errmsg);
void term_ensureTerminalDevice();
void term_enableLocalFlag(tcflag_t flags);
void term_disableLocalFlag(tcflag_t flags);
void term_getCurrentTerm(struct termios* term);
void term_setCurrentTerm(struct termios* term);
void term_saveOriginalTerm();
void term_restoreOriginalTerm();

int term_handleEscapeSequence(int c);

#endif
