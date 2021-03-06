#ifndef MSH_H
#define MSH_H

#include <stdio.h>
#include "../datastructures/HashMap/hashMap.h"

#define TOKEN_BUFFER_SIZE 32


// initialise data structures, set non canonical buffering for terminal and program
void msh_init();
// initialise builtin hashmap
static void msh_init_builtins();
// initialise aliases hashmap
static void msh_init_aliases();
// gets user input and executes until user exits
void msh_loop();
// redraw terminal line if user adds or erases character from input buffer
static void msh_redraw(char* line);
// readline from user input essentially until return key pressed
char* msh_readline(FILE* filedes);
// prints prompt to tell user to type
static void msh_printPrompt();
// Mainly used for when user adds / removes character from middle of input buffer
static void msh_shiftString(char* stringToShift, char* shiftPoint);
// Extracts token from delimited line
static char* msh_extractToken(char** line);
// Create token list from line (uses the extractToken function)
static char** msh_parse(char* line);
// executes the command stored in the first token, with the remainders given as arguments
static void msh_execute(char** tokens);
void msh_clean();

#endif
