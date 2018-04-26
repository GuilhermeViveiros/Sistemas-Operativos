#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <unistd.h>   /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h>    /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <string.h>


//Módulo destinado ao read 

//Le para um buf de tamanho n oque estiver contido nas fildes
ssize_t readln(int fildes, void *buf, size_t nbyte) ;


//Le uma determinado frase e devolve as palavras nela contida
char** mysystem (char *command) ;


#endif /* parser */