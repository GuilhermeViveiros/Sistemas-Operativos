#include "parser.h"
#include "execut.h"

int main(){
    int fopen , tmp;
    char **commandos;
    char array[1024];

    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fopen = open ("notbook.txt" , O_RDONLY , O_WRONLY , 0666);

    if (fopen < 0) {
        perror("Documento nao existe.\n");
        _exit(-1);
    }

    while ( (tmp = (readln(fopen , array , 1024))) > 0 ){
        array[tmp] = '\0';
        commandos = mysystem(array);
        if (commandos){
           executa(commandos);
        }
       
    }

    return 0;
}
