#include "auxF.h"
#include "buffer.h"


//-------------------------------------------------------------------------------------
    // Da autoria de Guilherme Viveiros && Angelo Sousa && Mateus Silva.
//-------------------------------------------------------------------------------------

//defines
#define Max 30

// Métodos publicos
char** mysystem(char *command);
int checkCommand(char* frase);
int readln(int fildes, void *buf );
int readall(int fildes, void *buff );
int Pcheck(char *line);

//--------------------------------------------------------------------------------------------------
char** mysystem (char *command){
    int a=0;
    int i=0, x=0;
    char tmp[1024] ;
    char **argumentos;
    argumentos = malloc (sizeof (char*)*Max);
    *argumentos = malloc (sizeof (char)*Max);

    while (*command){
        if (*command == ' ' && *(command+1) == ' ')  (command++);//quando tem dois espaços seguintes passamos ha frente
            else {
                if (*command == ' ' && a!=0) { //quando acontece um espaço sem ser o primeiro começamos noutra palavra
                    tmp[i] = '\0';
                    argumentos[x] = malloc (sizeof (char)*20);
                    strcpy ( argumentos[x++] , tmp);
                    i=0; 
                    (command++);
                }
                    else { //quando chega ao fim da palavra
                        if (*(command+1) == '\0') {
                            tmp[i++] = *(command);
                            tmp[i] = '\0';
                            argumentos[x] = malloc (sizeof (char)*20);
                             strcpy ( argumentos[x++] , tmp);
                            i=0; 
                            (command++);
                        }
                            else{//no primeiro caso quando a palavra tem 1 espaço por exemplo
                                if(*command == ' ') {
                                    command++;
                                }
                                    else {//quando a funcao esta na palavra
                                        tmp[i++] = *(command++);
                                        a++;
                                    }
                            }
                    }
            }  
        }
    argumentos[x] = NULL;
    return argumentos;   
}

int Pcheck(char *line){
    int i=0;
    while(line[i]){
        if(line[i] == '|' && line[i+1] != '|' ) return 1;
        i++;
    }
    return 0;
}

int checkCommand(char* frase){
    while(*frase){
        if (*frase++=='$')
            return 1;
    }
    return 0;
}


int readln(int fildes, void *buff ) {
	int x; char c;
	char *st = (char *)buff;

	while ((x=read (fildes , &c , 1)) > 0 ) {

        if (c == '\n') break; //quando encontrar paragrafo para
			*st = c;
			st++;
	}
 	*st = '\0';    
 	return x;
}

