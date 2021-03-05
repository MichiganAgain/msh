#ifndef MSH_H
#define MSH_H

#include <stdio.h>
#include "../datastructures/HashMap/hashMap.h"

#define TOKEN_BUFFER_SIZE 32


void msh_init();
static void msh_init_builtins();
static void msh_init_aliases();
void msh_loop();
char* msh_readline(FILE* filedes);
static void msh_printPrompt();
static char* msh_extractToken(char** line);
static void msh_shiftString(char* stringToShift, char* shiftPoint);
static char** msh_parse(char* line);
static void msh_execute(char** tokens);
void msh_clean();

static void msh_redraw(char* line);

#endif
