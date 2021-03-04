#ifndef TERMINAL_HANDLER_H
#define TERMINAL_HANDLER_H

#include <stdio.h>
#include <termios.h>
#include <stdbool.h>

#define TERM_ESCAPE_HEX "\x1b"

#define TERM_EOF 4
#define TERM_KEY_DELETE 127

#define term_cursor_left(x) printf(TERM_ESCAPE_HEX "[%dD", x);
#define term_erase_line() printf(TERM_ESCAPE_HEX "[K");

static struct termios originalTerm;

void term_error(char* errmsg);
void term_ensureTerminalDevice();
void term_enableLocalFlag(tcflag_t flags);
void term_disableLocalFlag(tcflag_t flags);
void term_getCurrentTerm(struct termios* term);
void term_setCurrentTerm(struct termios* term);
void term_saveOriginalTerm();
void term_restoreOriginalTerm();

#endif
