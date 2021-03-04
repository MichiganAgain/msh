#ifndef TERMINAL_HANDLER_H
#define TERMINAL_HANDLER_H

#include <termios.h>
#include <stdbool.h>

static struct termios savedTerm;

void term_error(char* errmsg);
void term_ensureTerminalDevice();
static void term_enableFlag(tcflag_t flags);
static void term_disableFlag(tcflag_t flags);
void term_enableCanonicalMode();
void term_disableCanonicalMode();
void term_enableEcho();
void term_disableEcho();
void term_saveTerm();
void term_restoreTerm();

#endif
