#ifndef execut_h
#define execut_h

#include "buffer.h"
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


//devolve o conjuntos de palavras de uma determinada frase
char** mysystem(char *command);

//verifica se uma determinada instrucao é comando ou nao
int checkCommand(char* frase);


#endif /* execut */
