#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef buffer_h
#define buffer_h


          
struct ligada{
    //1 se for comando , 0 se nao for
    int check;
    //ordem do comando (ordenado por ordem crescente)
    int tmp;  

    char *phrase;
    //lista de palavras da frase
    char **result;
    //struct tipo comando;
};

// ideia é receber um int filedes e um Tamanho e este preenche se automaticamente!
typedef struct buff{
    //wc -l -> diz quantas linhas tem o ficheiro
    int size; // ja recebemos o size
    int filedes;
    struct ligada *lista;    
}*Buff;



ssize_t readln(int fildes, void *buf );




//remove um buffer
//void rm_buffer(Buff x)

//inicializa o buffer
Buff create_buffer(int filedes , int size);


#endif  