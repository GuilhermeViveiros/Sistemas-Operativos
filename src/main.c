#include "execut.h"
#include "buffer.h"

int main(){
    
    int fint , fout , fint2 ,size=1;
    char c;
    Buff my_buff ;
    
    //variaveis para testes
    int i=0;
    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fint2 = open ("exemplo.txt", O_RDONLY ,0666);
    fint = open ("exemplo.txt" , O_RDONLY , 0666);
    fout = open ("nootbook.txt" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

    if (fint < 0) {
        perror("Documento nao existe.\n");
        _exit(-1);
    }
    
    //tamanho do ficheiro
    while(read(fint2,&c,1)>0){
        if (c == '\n') size++;
    }
    close(fint2);

 
    my_buff = create_buffer(fint,size);
/*
    while(i<my_buff->size){
        
        if (checkCommand(my_buff->lista[i].phrase)){
            printf("Ordem :%d\n" ,my_buff->lista[i].tmp);
             printf("Comando :%s\n" ,my_buff->lista[i].phrase);
        }

        else {
            printf("%s\n\n" , my_buff->lista[i].phrase);
        }
        i++;
    }
*/
    load(my_buff);
    //exec_buffer(my_buff,fout);
    
    close(fint);
    close(fout);
    //Falta acabar esta funçao
    //destroy_buffer(my_buff);
   
    return 0;
}
