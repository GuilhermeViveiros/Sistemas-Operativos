#include "execut.h"
#include "buffer.h"

int main(){
    
    int fint , fout , fint2 ,size=0,i;
    char *c = malloc(1024);
    Buff x;

    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fint2 = open ("exemplo.txt", O_RDONLY ,0666);
    fint = open ("exemplo.txt" , O_RDONLY , 0666);
    fout = open ("nootbook.txt" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

    if (fint < 0 && fint2 < 0 && fout < 0) {
        perror("Documento nao existe.\nVerifique que os decomentos estao na mesma diretoria.\n");
        _exit(-1);
    }
    
    while(readln(fint2,c)>0) size++; close (fint2); //so para saber o tamanho do ficheiro
    
    x = create_buffer(fint,size);//inicializa buffer
    x = load_buffer(x);//preenche buffer


    for(i=0;i<size;i++){
        if (getCheck(x,i)){ //quando for comando
            write(fout,">>>\n",sizeof(char)*4);//>>>
            write(fout,getLine(x,i),strlen(getLine(x,i)));//escreve o comando
            write(fout,"\n",sizeof(char)); // \n
            write(fout,getResult(x,i),strlen(getResult(x,i)));//escreve o resultado do comando
            write(fout,"<<<\n",sizeof(char)*4);//<<<
        }
        else{ //quando nao for comando
            write(fout, getLine(x,i) , strlen(getLine(x,i)));//escreve a frase
            write(fout,"\n",sizeof(char));//\n
        }
    }

   
    return 0;
}
