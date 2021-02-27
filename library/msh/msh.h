#ifndef MSH_H
#define MSH_H


#include "../datastructures/hashMap.h"

#define TOKEN_BUFFER_SIZE 32


void msh_init();
void msh_init_builtins();
void msh_loop();
void msh_printPrompt();
char** msh_parse(char* line);
void msh_execute(char** tokens);
void msh_execute_builtin(char** tokens);
void msh_clean();

#endif
