#include "auxF.h"
#include "Cextra.h"
#include "buffer.h"

int main(int argc , char *argv[]){
    
    int fint , fout , fint2 ,size=0,i;
    char *c = malloc(1024);
    Buff x;

     if (argc < 2)
    {
        printf("--------------------------------------------------------\n");
        printf("\tIndica o ficheiro quando invocas o programa\n");
        printf("--------------------------------------------------------\n");
        exit(-1);
    }

    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fint2 = open (argv[1], O_RDONLY ,0666);
    fint = open (argv[1] , O_RDONLY , 0666);
    fout = open ("Notebook.txt" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

    if (fint < 0 && fint2 < 0) {
        perror("Documento nao existe.\nVerifique que os decomentos estao na mesma diretoria.\n");
        _exit(-1);
    }
    
    while(readln(fint2,c)>0) size++; close (fint2); //so para saber o tamanho do ficheiro
    
    x = create_buffer(fint,size);//inicializa buffer
    x = load_buffer(x);//preenche buffer


    for(i=0;i<size;i++){
        if (getCheck(x,i)){ //quando for comando
            write(fout,getLine(x,i),strlen(getLine(x,i)));//escreve o comando
            write(fout,"\n",sizeof(char)); // \n
            write(fout,">>>\n",sizeof(char)*4);//>>>
            write(fout,getResult(x,i),strlen(getResult(x,i)));//escreve o resultado do comando
            write(fout,"<<<\n",sizeof(char)*4);//<<<
        }
        else{ //quando nao for comando
            write(fout, getLine(x,i) , strlen(getLine(x,i)));//escreve a frase
            write(fout,"\n",sizeof(char));//\n
        }
    }

     //NAO SEI SE ESTA FUNCAO ESTA DIREITA S
    destroy_buffer(x); //destroi um dterminado buffer 
 
   
    return 0;
}
