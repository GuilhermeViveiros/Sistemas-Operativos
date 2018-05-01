#include "execut.h"
#include "buffer.h"

int main(){
    
    int fint , fout , size=1;
    char ch;
    //variaveis para teste
    int i=0;
    Buff my_buff ;

    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fint = open ("exemplo.txt" , O_RDONLY , 0666);
    fout = open ("nootbook.txt" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

    if (fint < 0) {
        perror("Documento nao existe.\n");
        _exit(-1);
    }
    
   

    my_buff = create_buffer(fint,7);

    while(i<my_buff->size){
        printf("%s\n" , my_buff->lista[i++].phrase);
    }

    exec_buffer(my_buff,fout);
    
    
    //Falta acabar esta funçao
    //destroy_buffer(my_buff);
   
    return 0;
}
