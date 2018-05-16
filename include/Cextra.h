#ifndef Cextra_h
#define Cextra_h

#include "buffer.h"

//verifica qual o tipo de comando especial
char verifica(char *line);

char* concurrent_execution(char *line);

//funcao que recebe um comando e um ficheiro de entrada ou saida (nao receber os dois ao mesmo tempo)
void redir (char *entrada , char *saida , char *comando);

void redir_end(char *entrada , char *comando);

#endif