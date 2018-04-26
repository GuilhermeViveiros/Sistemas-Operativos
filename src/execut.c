#include "execut.h"


void executa(char **commandos){
    /*so temos 3 casos 
    ** $ ls  -> executa so ls
    ** $ |ls -> executa ls com o stdin recebido anteriormente
    ** $ (numero) | ls -> executa ls com o stdin recebido na ordem do comando igual ao numero
    */

    int my_pipe[2];
    //estou a criar um pipe para interagir entre comandos
    pipe(my_pipe);

    if ( strcmp (*commandos , "$") == 0 ) {
        //primeiro commando é o caracter "$"
        //close(my_pipe[0]);
        //dup2 (my_pipe[1] , 1);
        commandos++;
        execvp( *commandos , commandos);
        perror("Se chegar aqui temos que devolver o ficheiro intacto!\n");
        close(my_pipe[1]);
        _exit(-1);
   
    }
}