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
            }
    }

    return x;
}

void exec_buffer(Buff x,int filedes){
    int i=0;
    dup2(filedes,1);

    while(i<x->size){
        if (x->lista[i].check == 1){
            write(filedes,x->lista[i].phrase,strlen(x->lista[i].phrase));
            putchar('\n');
            if(!fork()){
                execvp(x->lista[i].result[1] , ++(x->lista[i].result));
                perror("Comando indefenido no sistema!\n");
                _exit(-1);
            }
            else{
                printf(">>>>\n");
                wait(NULL);
                printf("<<<<\n");  
            }
       
        }
        else{
            write(filedes,x->lista[i].phrase,strlen(x->lista[i].phrase));
            putchar('\n');
        }
        i++;

    }
}
//void remove_buffer(Buffer x);

