#include "buffer.h"
#include "execut.h"

int contador = 0;

//ssize_t readln(int fildes, void *buf, size_t nbyte);
int readln(int fildes, void *buf ) {
	int x=0; char c;
	char *st = (char *)buf;

	while ( read (fildes , &c , 1) > 0 ) {
		if ( c == '\n') break;
			*st = c;
			st++;
			x++;
	}
 	*st = '\0';
 	return x;
}

Buff create_buffer(int files , int nsize){
    int i;
    Buff x = malloc (sizeof (struct buff));
    x->size = nsize;
    x->filedes = files;
    x->lista = malloc (sizeof (struct ligada) *nsize );
    for(i=0; i<x->size ; i++){
        x->lista[i].phrase = malloc(1024);

        readln(x->filedes , x->lista[i].phrase);    
        if (checkCommand(x->lista[i].phrase)) {
            x->lista[i].check = 1;
    	    x->lista[i].tmp = contador++;
    	    x->lista[i].result = mysystem(x->lista[i].phrase);
        }
            else {
                 x->lista[i].check = 0;
                 x->lista[i].result = mysystem(x->lista[i].phrase);
            }
    }

    return x;
}

/*
void exec_buffer(Buff x){
    int i=0;
    int my_pipe[2];
    pipe(my_pipe);

    while(i<x->size){
        if (x->lista[i].check == 1) {
            
            if ( strcmp ( *(x->lista[i].result) , "$") == 0){
                x->lista[i].command_result = malloc(1024*10);
                if(!fork()){
                  
                    close(my_pipe[0]);
                    dup2(my_pipe[1], 1);
                    close(my_pipe[0]);
                    
                    execvp(x->lista[i].result[1] , ++(x->lista[i].result));
                    perror("Comando nao defenido pelo sistema!\n");
                    _exit(-1);
                }
                else {
                    close(my_pipe[1]);
                    while ( read(my_pipe[0] , x->lista[i].command_result , 1024)){
                        (x->lista[i].command_result)++;
                     }
                    close(my_pipe[1]);
                    _exit(-1);
                }
            }
        }
    printf("%s\n" , x->lista[i].command_result );
    i++;
    }
}
*/
/*
*Tipo 1 -> $ ls ,  executa o comando
*Tipo 2 -> $| ls , executa o comando com o stdin anterior 
*Tipo 3 -> $2| ls , executa o comando com o 2 stdin 
*/
void exec_buffer(Buff x,int filedes){
    int i=0;
    char *c1 = ">>>>";
    char *c2 = "<<<<";
    char c = '\n'; 

    dup2(filedes,1);

    while(i<x->size){
        //se for comando
        if (x->lista[i].check == 1){
            //se for comando e for do tipo 1 ------------------------------------------------------------
            if ( strcmp( *(x->lista[i].result) , "$" ) == 0 ){
                write(filedes,x->lista[i].phrase,strlen(x->lista[i].phrase));
                write(filedes , &c , 1);
                if(!fork()){
                    execvp(x->lista[i].result[1] , ++(x->lista[i].result));
                    perror("Comando indefenido no sistema!\n");
                    _exit(-1);
                }
                else{
                    write(filedes , c1 , strlen(c1));
                    write(filedes , &c , 1);
                    wait(NULL);
                    write(filedes , c2 , strlen(c2));
                    write(filedes , &c , 1);
                }
            }
          
        }

        else{
            write(filedes , x->lista[i].phrase , strlen(x->lista[i].phrase));
            write (filedes , &c ,1);
        }
        i++;

    }
}

//void remove_buffer(Buffer x);

