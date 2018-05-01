#include "execut.h"
#include "buffer.h"

int main(){
    int fopen , fout,
    //varaiveis para testes depois tirao de ser removidas
    int tmp=0,teste=0;
    Buff my_buff ;

    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fopen = open ("exemplo.txt" , O_RDONLY , O_WRONLY , 0666);
    fout = open ("nootbook" , O_WRONLY , O_RDONLY , O_CREAT , O_TRUNC , 0666);

    if (fopen < 0) {
        perror("Documento nao existe.\n");
        _exit(-1);
    }

    my_buff = create_buffer(fopen,5);
    while(teste < 4){
    printf("Size :%d\n", my_buff->size );
    printf("Frase: %s\n", my_buff->lista[teste].phrase );
    printf("Comando: %d\n", my_buff->lista[teste].check );
    printf("Ordem do comando :%d\n", my_buff->lista[teste].tmp );
        while(*my_buff->lista[teste].result){
            printf("Plavara %d = %s\n",tmp++, *(my_buff->lista[teste].result++) );
        }
        tmp=0;
        teste++;
        putchar('\n');
    }
    return 0;
}
