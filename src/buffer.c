#include "buffer.h"
#include "execut.h"

int contador = 0;

//ssize_t readln(int fildes, void *buf, size_t nbyte);
ssize_t readln(int fildes, void *buf ) {
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
    int tmp,i;
    Buff x = malloc (sizeof (struct buff));
    x->size = nsize;
    x->filedes = files;
    x->lista = malloc (sizeof (struct ligada) *nsize );
    for(i=0; i<x->size ; i++){
        x->lista[i].phrase = malloc(1024);
        readln(x->filedes , x->lista[i].phrase);     
        //Falta o Mateus fazer esta funcao
        //if (check_command(x->lista[i].phrase)) {
            x->lista[i].check = 1;
    	    x->lista[i].tmp = contador++;
    	    x->lista[i].result = mysystem(x->lista[i].phrase);
    
        //else {
            // x->lista[i].check = 0;
        //}
    }
    return x;
}

//void remove_buffer(Buffer x);
