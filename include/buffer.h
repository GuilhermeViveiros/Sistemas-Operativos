#ifndef buffer_h
#define buffer_h

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>


typedef int (*Filter)( void * ,void *) ;
/*
    Filter function.
*/
typedef int (*Fcomparator)( void * ,void *) ;
/*
    Compare function.
*/
typedef void (*Ffree)( void *) ;
/*
    Free function.
*/

/**
*Estrutura de um bloco auxiliar ao Buff 
*/
struct blocos;

/**
*Estrutura de um buff nomeada Buff
*/
typedef struct buff *Buff;

//-------------------------------------------------------------------------------------------------

//Le de um ficheiro para um buffer
int readln(int fildes, void *buf );

//Inicializa o buffer
Buff create_buffer(int filedes , int size);

//Começa a prencher os blocos do buffer a partir do ficheiro
Buff load_buffer(Buff x);


//Getters//--------------------------------------------------------------------------------------------------

int getSize(Buff x);//devolve o tamanho do buff

int getIs_on(Buff x);//verifica se o buff está ativado ou nao

int getFile(Buff x);//devolve a file que o buff está a usar

int getCheck(Buff x , int i);//verifica se é comando

int getNumberC(Buff x , int i);//devolve o numero do comando

char *getLine(Buff x , int i);//devolve a frase

char **getWords(Buff x , int i);//devolve as palavras de uma frase

char *getResult(Buff x, int i);//devolve o resultado do comando

//Setters//--------------------------------------------------------------------------------------------------
void setSize(Buff x , int y);//define o tamanho do buffer

void setFile(Buff x,int filedes);//define a file do buffer

void *setResult(Buff x, char *r , int i);//define o resultado de um comando

#endif  
