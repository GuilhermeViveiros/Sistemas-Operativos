#ifndef buffer_h
#define buffer_h


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>


struct ligada{
    //1 se for comando , 0 se nao for
    int check;
    //ordem do comando (ordenado por ordem crescente)
    int tmp;  
    char *phrase;
    //lista de palavras da frase
    char **result;
    //lista o resultado do ficheiro
    char *command_result;
};

// ideia é receber um int filedes e um Tamanho e este preenche se automaticamente!
typedef struct buff{
    //wc -l -> diz quantas linhas tem o ficheiro
    int size; // ja recebemos o size
    int filedes;
    struct ligada *lista;    
}*Buff;

//le do fildes para o buf
int readln(int fildes, void *buf );

//executa o buffer para os comandos com $
void exec_buffer(Buff x,int filedes);

//remove um buffer
//void rm_buffer(Buff x)

//inicializa o buffer
Buff create_buffer(int filedes , int size);

#endif  
