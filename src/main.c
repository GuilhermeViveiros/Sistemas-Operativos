#include "auxF.h"
#include "buffer.h"

int main(int argc , char *argv[]){
    int i=0;
    int fint , fout ;
    Buff x;

     if (argc < 2)
    {
        printf("--------------------------------------------------------\n");
        printf("\tIndica o ficheiro quando invocas o programa\n");
        printf("--------------------------------------------------------\n");
        exit(-1);
    }
   
    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fint = open (argv[1] , O_RDONLY , 0666);
    fout = open ("Notebook.txt" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

    if (fint < 0) {
        perror("Documento nao existe.\nVerifique que os decomentos estao na mesma diretoria.\n");
        _exit(-1);
    }
    
    
    x = create_buffer(fint);//inicializa buffer
  

    for(i=0;i<getUsed(x);i++){
        if (getCheck(x,i)){ //quando for comando
            
            write(fout,getLine(x,i),strlen(getLine(x,i)));//escreve a linha do comando
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

    /* Para o caso de querer ver os resultados no ecra
    for (i=0; i < getUsed(x) ; i++){
         if (getCheck(x,i)){ //quando for comando
            write(1,getLine(x,i),strlen(getLine(x,i)));//escreve a linha do comando
            write(1,"\n",sizeof(char)); // \n
            write(1,">>>\n",sizeof(char)*4);//>>>
            write(1,getResult(x,i),strlen(getResult(x,i)));//escreve o resultado do comando
            write(1,"<<<\n",sizeof(char)*4);//<<<
        }
        else{ //quando nao for comando
            write(1, getLine(x,i) , strlen(getLine(x,i)));//escreve a frase
            write(1,"\n",sizeof(char));//\n
        }
    }
    */
     //NAO SEI SE ESTA FUNCAO ESTA DIREITA(NAO ESTA , MODIFICAR)
    //destroy_buffer(x); //destroi um dterminado buffer 
 
   
    return 0;
}
