#include "auxF.h"
#include "buffer.h"
#include "Cextra.h"
#include "bheap.h"

//Termina o programa caso o utilizador prima CTRL + C ou algum dos programas esteja incorreto
void handle_singal(int s){
	switch (s){
		case SIGINT :
			printf("  -> Certo.NoteBook original nao modificado.\n");
			_exit(-1);
			break;
		case SIGSTOP :
			printf("NoteBook original nao modificado.Tente outra vez.\n");
			break;
		default : break;
	}
}

int main(int argc , char *argv[]){
    int a,i=0;
    int fint , fout ,ferr;
    Buff x; char *aux= malloc(1024);
    signal(SIGINT , handle_singal); // caso  utilizador prima CTRL + C

    
    if (argc < 2)
    {
        printf("--------------------------------------------------------\n");
        printf("\tIndica o ficheiro quando invocas o programa\n");
        printf("--------------------------------------------------------\n");
        exit(-1);
    }
    
    for(a = 1 ; a < argc ; a++){
    // If successful, open() returns a non-negative integer, termed a file descriptor.  It returns -1 on failure, and sets errno to indicate the error
    fint = open (argv[a] , O_RDONLY , 0666);

    strcpy(aux,argv[a]); aux[0] = 'l';//estou a criar vários ficheiros para um dos recebidos
    
    fout = open (aux , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

    
    if (fint < 0) {
        perror("Documento nao existe.\nVerifique se o ficheiro está na mesma diretoria que o programa.\n");
        _exit(-1);
    }

    x = create_buffer(fint);//inicializa buffer
    
    for(i=0;i<getUsed(x);i++){
        if (getCheck(x,i)){ //quando for comando
            
            write(fout,getLine(x,i),strlen(getLine(x,i)));//escreve a linha do comando
            write(fout,"\n",sizeof(char)); // \n
            write(fout,">>>\n",sizeof(char)*4);//>>>
            write(fout,getResult(x,i),strlen(getResult(x,i)));//escreve o resultado do comando
            write(fout,"\n",sizeof(char)); // \n
            write(fout,"<<<\n",sizeof(char)*4);//<<<
        }
        else{ //quando nao for comando
            write(fout, getLine(x,i) , strlen(getLine(x,i)));//escreve a frase
            write(fout,"\n",sizeof(char));//\n
        }
    }

    //destroy_buffer(x);//destroi um dterminado buffer 
    
    ferr = open ("Erros.txt",O_RDONLY , 0666);
    if(ferr  > 0){
        printf("--------------------------------------------------------\n");
        printf("             Ficheiro %s incorreto.                 \n",argv[a]);
        printf("         Verifique o ficheio Erros.txt                  \n");
        printf("--------------------------------------------------------\n");
        _exit(-1);
    }
  
    
    if(!fork()){ //filho apaga o ficheiro incial, devolve ficheiro final
        execlp("mv","mv",aux,argv[a],NULL);
       _exit(-1);
    }
    
    }
    printf("--------------------------------------------------------\n");
    printf("---                                                  ---\n");
    printf("---      Ficheiro/s compilado/s, nenhum erro.        ---\n");
    printf("---                                                  ---\n");
    printf("--------------------------------------------------------\n");

    return 0;
}
