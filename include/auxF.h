#ifndef auxF_h
#define auxF_h

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

//Le de um ficheiro para um buffer até encontrar '\n'
int readln(int fildes, void *buf );


/**
 * Dado um dado ficheiro , guarda me tudo diretamente num buffer
 * */// -> estou a usar na concorrentesSC
int readall(int fildes, void *buff );

#endif /* aux */
