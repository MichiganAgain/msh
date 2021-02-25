#ifndef MSH_H
#define MSH_H

#define TOKEN_BUFFER_SIZE 32

void msh_loop();
void msh_printPrompt();
char** msh_parse(char* line);
void msh_execute(char** tokens);

#endif
