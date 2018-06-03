#ifndef auxF_h
#define auxF_h

#include "buffer.h"
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Devolve o conjuntos de palavras de uma determinada frase
char** mysystem(char *phrase);

//verifica se uma determinada instrucao é comando
int checkCommand(char* frase);


//Le toda a informação de um ficheiro para um buff
int readln(int fildes, void *buf );

//verifica se existem pipes(|) num comando
int Pcheck(char *line);


#endif /* aux */
