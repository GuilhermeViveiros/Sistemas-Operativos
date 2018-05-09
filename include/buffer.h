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


struct ligada{
    //1 se for comando , 0 se nao for
    int check;
    //ordem do comando (ordenado por ordem crescente)
    int tmp;  
    char *phrase;
    //este file serve para quando for um comando to tipo $x| voltarmos ao inicio do ficheiro e recolhermos o intput
    int command_file;
    //lista de palavras da frase
    char **result;
};

// ideia é receber um int filedes e um Tamanho e este preenche se automaticamente!
typedef struct buff{
    //wc -l -> diz quantas linhas tem o ficheiro
    int size; // ja recebemos o size
    int filedes; //file que o buff vai escrever
    int buff_file; // file que o buff guarda os resultados
    struct ligada *lista;    
}*Buff;


//le do fildes para o buf
int readln(int fildes, void *buf );

//encontra o resultado de um comando , devolve um boolean
int find(Buff x , char *result ,int i, int comando);

//executa os comandos para o file do buffer
void load(Buff x);

//executa o buffer para os comandos com $
void exec_buffer(Buff x,int filedes);

//remove um buffer
//void rm_buffer(Buff x)

//inicializa o buffer
Buff create_buffer(int filedes , int size);

#endif  
