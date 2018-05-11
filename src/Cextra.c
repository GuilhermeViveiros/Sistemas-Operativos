#include "Cextra.h"
#include "buffer.h"
#include "auxF.h"

char verifica(char *line){
    
    if ( strtok (line , ">")) return '>';
    if ( strtok (line , "<")) return '<';
    if ( strtok (line , "|")) return '|';
    if ( strtok (line , "&")) return '&';
    return '0';
}

//struct ligada executa0(struct ligada x){

//}

//dado um resultado x , e uma diretoria y passa todo o x para y
void specialC_0(char *x , char*y ){
    int l = open ( y , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);
    if (l<0){
        perror("Impossivel escrever para tal ficheiro\n");
        _exit(-1);
    }
    write(l,x,strlen(x));
    close(l);
}

//dado uma diretoria y passa tudo para x
char* specialC_1(char *x , char*y){
     int l = open ( y , O_RDONLY , 0666);
     if(l<0){
         perror("Ficheiro no qual pretende ler ao se econtra na mesma diretoria.Tente outra vez!\n");
         _exit(-1);
     }
     while(readln(l,x)>0);
    
}