#include "execut.h"
#include "buffer.h"


#define Max 10

//Recebe como argumentos uma frase e separa-a em várias palavras
char** mysystem (char *command) {

    int i=0, x=0;
    char tmp[1024];
    char **argumentos;
    argumentos = malloc (sizeof (char*)*Max);
    *argumentos = malloc (sizeof (char)*Max);

        while (*command){
            if (*command == ' ' && *(command+1) == ' '){
                (command++);
            }
            else {
                if (*command == ' ') {
                tmp[i] = '\0';
                argumentos[x] = malloc (sizeof (char)*20);
                strcpy ( argumentos[x++] , tmp);
                i=0; 
                (command++);
                }
                    else {
                        if (*(command+1) == '\0') {
                            tmp[i++] = *(command);
                            tmp[i] = '\0';
                            argumentos[x] = malloc (sizeof (char)*20);
                             strcpy ( argumentos[x++] , tmp);
                            i=0; 
                            (command++);
                        }
                        else tmp[i++] = *(command++);
                    }
            }  
        }
    argumentos[x] = NULL;
    //execvp("./Ex6" , argumentos);
   // perror ("Ex6 not found!\n");
    return argumentos;   
}