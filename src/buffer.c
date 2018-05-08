#include "buffer.h"
#include "execut.h"

int contador = 0;
/*
*Tipo 1 -> $ ls ,  executa o comando
*Tipo 2 -> $| ls , executa o comando com o stdin anterior 
*Tipo 3 -> $2| ls , executa o comando com o 2 stdin 
*/

//boolean
int find(Buff x , char *result ,int i , int comando){
    char *frase = (char *)result;
    int resultado =0;

    while(readln(x->lista[i].command_file,frase)>0){
        if (comando == atoi(frase)) {
            resultado = 1;
            break;
        }
    }
    
    if (resultado){
        while(readln(x->lista[i].command_file,frase)>0){
            if (atoi(frase) == comando+1) break;
             printf ("%s\n",result);
        }
    }

    printf ("Frase2 :%s\n" , result);

    return resultado; // se tiver o pretendido comando returna 1
}

//ssize_t readln(int fildes, void *buf, size_t nbyte);
//ate ao paragafro
int readln(int fildes, void *buff ) {
	int x=0; char c;
	char *st = (char *)buff;

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
    x->buff_file = open ("testes.txt" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);
    if (x->buff_file<0){
        perror("Impossivel criar o ficheiro!");
        _exit(-1);
    }
    x->lista = malloc (sizeof (struct ligada) *nsize );
    for(i=0; i<x->size ; i++){
        x->lista[i].phrase = malloc(1024);

        readln(x->filedes , x->lista[i].phrase);
        if (checkCommand(x->lista[i].phrase)) {
            x->lista[i].check = 1;
    	    x->lista[i].tmp = contador++;
    	    x->lista[i].result = mysystem(x->lista[i].phrase);
            //quando for comando tipo 2 ou 3
            if( strcmp (x->lista[i].result[0] , "$")){
                x->lista[i].command_file = open("testes.txt" , O_RDONLY , 0666);
                if (x->lista[i].command_file < 1){
                    perror ("Ficheiro inexistente.");
                    _exit(-1);
                }
            }
        }
            else {
                 x->lista[i].check = 0;
                 x->lista[i].result = mysystem(x->lista[i].phrase);
            }
    }
    return x;
}


void load(Buff x){
    int i ,my_pipe[2];
    char *c = "\n\n\n";
    char tmp;
    char *intput = malloc(1024);

    pipe(my_pipe);

    for (i=0; i<x->size ; i++){
         if (x->lista[i].check == 1){
             //caso 1 ----------------------------------------------------------------------------------------------------
              if ( strcmp( *(x->lista[i].result) , "$" ) == 0 ){
                  if (!fork()){
                    
                    tmp = x->lista[i].tmp  +  48;
                    write (x->buff_file,&tmp,sizeof(char));
                    //write(x->buff_file,"c",sizeof(char));
                    write(x->buff_file,c,sizeof(char));
                    dup2(x->buff_file,1);
                    close(x->buff_file);
                    execvp(x->lista[i].result[1] , ++(x->lista[i].result));
                    perror("Comando inválido , nao está defenido no sistema");
                    _exit(-1);
                  }
                 else {
                     wait(NULL);
                     //write (x->buff_file,c,strlen(c));
                 } 
              }
            //caso 2 ----------------------------------------------------------------------------------------------------
                else {
                    if ( strcmp( *(x->lista[i].result) , "$|" ) == 0 ){
                    printf ("Existe comando anterior :%d\n" ,find(x,intput,i,x->lista[i].tmp -1));
                    //Se existir comando anterior entao ele faz -------------
                    //printf ("Comando : %s" ,intput);
                    if(!fork()){
                        tmp = x->lista[i].tmp  +  48;
                        write (x->buff_file,&tmp,sizeof(char));
                        //write(x->buff_file,"c",sizeof(char));
                        write(x->buff_file,c,sizeof(char));
                        close(my_pipe[0]);
                        write(my_pipe[1], intput , sizeof(intput));
                        close(my_pipe[1]);
                       // _exit(-1);
                    }
                     else {
                         
                        close(my_pipe[1]);
                        dup2(my_pipe[0],0);
                        close(my_pipe[0]);
                        dup2(x->buff_file,1);
                        close(x->buff_file);
                        execlp("ls" ,"ls", NULL);
                        //execvp(x->lista[i].result[1], ++(x->lista[i].result));
                        perror("Comando inválido , nao está defenido no sistema");
                        _exit(-1);
                    }
                }/*
            //caso 3 ----------------------------------------------------------------------------------------------------
                else {    
                    printf("dasdasdas");  
                //Se existir comando anterior entao ele faz -------------
                if( find (x ,intput ,i ,atoi(++(x->lista[i].result[0])) ) ){ 
                    if(!fork()){
                        printf("ssdasdasdas");
                        tmp = x->lista[i].tmp  +  48;
                        write (x->buff_file,&tmp,sizeof(char));
                        //write(x->buff_file,"c",sizeof(char));
                        write(x->buff_file,c,sizeof(char));
                        close(my_pipe[0]);
                        write(my_pipe[1], intput , sizeof(intput));
                        close(my_pipe[1]);
                        _exit(-1);
                    }

                    else {
                        
                        close(my_pipe[1]);
                        dup2(my_pipe[0],0);
                        close(my_pipe[0]);
                        dup2(x->buff_file,1);
                        close(x->buff_file);
                        execlp("ls" ,"ls", NULL);
                        //execvp(x->lista[i].result[1], ++(x->lista[i].result));
                        perror("Comando inválido , nao está defenido no sistema");
                        _exit(-1);
                    }
                }
                }*/
                }
        }
    }
}

/*
void exec_buffer(Buff x,int filedes){
    int i=0;
    char *c1 = ">>>>"; char *c2 = "<<<<"; char c = '\n'; 
    int my_pipe[2];
    char **result_tmp;
    pipe(my_pipe);


    while(i<x->size){
        //se for comando
        if (x->lista[i].check == 1){
           
            //se for comando e for do tipo 1 ------------------------------------------------------------
            if ( strcmp( *(x->lista[i].result) , "$" ) == 0 ){
                write(filedes,x->lista[i].phrase,strlen(x->lista[i].phrase));
                write(filedes , &c , 1);
                if(!fork()){
                    dup2(filedes,1);
                    close(filedes);
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
                   // _exit(-1);
                }
            }
            
            //Quero funcao
            //se for comando e for do tipo 2 ------------------------------------------------------------
             if ( strcmp( *(x->lista[i].result) , "$|" ) == 0 ){
                write(filedes,x->lista[i].phrase,strlen(x->lista[i].phrase));
                write(filedes , &c , 1);
                //quando for filho
                if(!fork()){
                  close(my_pipe[0]);
                  dup2(my_pipe[1],1);
                  close(my_pipe[1]);

                  //encontra o comando da instrucao anterior
                  //result_tmp = find(x,x->lista[i].tmp -1);
                  //execvp(*result_tmp , ++(result_tmp));
                  execlp("ls","ls",NULL);
                  perror("Comando indefenido no sistema!\n");
                  _exit(-1);

                }
                    else {
                            if(!fork()){
                                close(my_pipe[1]);
                                dup2(my_pipe[0],0);
                                close(my_pipe[0]);
                                dup2(filedes,1);
                                //close(filedes);
                                execvp(x->lista[i].result[1] , ++(x->lista[i].result));
                                perror("Comando indefenido no sistema!\n");
                                _exit(-1);
                            }
                            else {
                                write(filedes , c1 , strlen(c1));
                                wait(NULL);
                                write(filedes,&c,1);
                                write(filedes , c2 , strlen(c2));
                                write(filedes,&c,1);
                            }
                        //close(filedes);
                        perror("Comando indefenido no sistema!\n");
                       
                    }
            }
            //se for comando e for do tipo 3 ------------------------------------------------------------
        }

        else{
            write(filedes , x->lista[i].phrase , strlen(x->lista[i].phrase));
            write (filedes , &c ,1);
        }
        i++;

    }
}
*/

//void remove_buffer(Buffer x);

