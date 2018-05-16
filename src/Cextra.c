#include "Cextra.h"
#include "buffer.h"
#include "auxF.h"

//-------------------------------------------------------------------------------------
    // Da autoria de Guilherme Viveiros && Angelo Sousa && Mateus Silva.
//-------------------------------------------------------------------------------------

char verifica(char *line){
    if (strstr  (line, ">>")) return '1';
    if ( strstr (line , ">")) return '>';
    if ( strstr (line , "<")) return '<';
    if ( strstr (line , "|")) return '|';
    if ( strstr (line , "&")) return '&';
    return '0';
}



//Executa programas concorrentes -> ls -l | cat | head -1
char* concurrent_execution(char *line){
    char *result = malloc(1024);
    int i,a , c=0,l=0;
    char *x = line; //igual ao argumento recebido
    char **exemplo = mysystem(line);
    exemplo++;
    int aux[2];
    int size=0;
    
    while(x!=NULL){ //calcula quantas linhas a matriz irá ter
        x=strstr(++x,"|");
        size++;
    }

    int my_pipe[size][2]; //crio os respetivos pipes
    char *tmp[size][5]; //crio a matriz

    while(*exemplo){ //defino a matriz com os comandos

        if(strcmp(*exemplo , "|") ==0){
            tmp[l][c]=NULL;
            l++;
            c=0;
        }
            else{
                tmp[l][c]=*exemplo;
                c++;
            }
    exemplo++;
    }
    tmp[l][c] = NULL;   
    
    for (i=0; i < size ; i++){//inicializa se o pipe
        pipe(my_pipe[i]);
    }
    pipe(aux);


    for (i=0; i < size ; i++){//Executamos os comandos com concorrencia
        if(!fork()){//filho
            if (i == 0){
                close(my_pipe[i][0]);
                dup2(my_pipe[i][1] , 1);
                close(my_pipe[i][1]);
                execvp(tmp[i][0] ,  tmp[i]);
                perror("Primeiro comando inexistente na bash!\n");
                _exit(-1);
            }
                else {
                    if (i==size-1){
                        for ( a = 0; a < i; a++){
                            close(my_pipe[a][1]);//fecho o pipe anterior   }
                        }
                if(!fork()){ // filho
                    close(aux[0]);
                    dup2(aux[1],1);
                    close(aux[1]);
                    dup2(my_pipe[i-1][0],0);//modifco o stdin 
                    close(my_pipe[i][0]);//fecho o pipe atual
                    execvp(tmp[i][0] ,  tmp[i]);
                    perror("Comando inexistente na bash!\n");
                    _exit(-1);
                }
                else {
                    close(aux[1]);
                    read(aux[0],result,1024);
                    close(aux[0]);
                }
               
                    }
                
                else {
                    for ( a = 0; a < i; a++){
                        close(my_pipe[a][1]);//fecho o pipe anterior   
                }
                    dup2(my_pipe[i-1][0],0);//modifco o stdin 
                    close(my_pipe[i][0]);//fecho o pipe atual
                    dup2(my_pipe[i][1],1);//modifico o stdout
                    execvp(tmp[i][0],tmp[i]);//executo
                    perror("Comando desconhecido pelo sistema!\n");
                    _exit(-1);
                }   
                }
        }
    }
    return result;
}



//funcao que recebe um comando e um ficheiro de entrada ou saida (nao receber os dois ao mesmo tempo)
void redir (char *entrada , char *saida , char *comando){
   
    int p ;
	char **x;
	if (!entrada) //quando nao existir ficheiro de entrada
	{
		if(saida)
		{   saida++;saida++;
			p = open (saida , O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC , 0666 );
			if(p<0) //caso nao consiga criar o ficheiro
			{
				perror("Ficheiro de saida nao foi criado.Tente outra vez!\n");
				_exit(-1);
			}
			else{
                if(!fork()){ //crio um fork para parar o programa a meio devido ao exec
				    x = mysystem(comando);
				    dup2(p,1);
				    close(p);
				    execvp(x[1] , ++x);
				    perror("Comando nao reconhecido pelo sistema.Tente outra vez!\n");
				    _exit(-1);
			    }
        	}
		}
	}
	else //quando existir ficheiro de entrada 
	{
		if(!saida){
			entrada++; entrada++;
			p = open (entrada , O_RDONLY  , 0666 );
			
			if(p<0) //caso nao consiga criar o ficheiro
			{
				perror("Ficheiro de saida nao foi criado.Tente outra vez!\n");
				_exit(-1);
			}
			else 
			{	if(!fork()){ 
				    x = mysystem(comando);
				    dup2(p,0);
				    close(p);
				    execvp(x[1],++x);
				    perror("Comando nao reconhecido pelo sistema.Tenta outra vez!\n");
				    _exit(-1);
                }
            }
		}
	}
}

//funcao que executa um comando e guarda o resultado sucessivamente no ficheiro
void redir_end(char *entrada , char *comando){
	int p;
	char **x;
    entrada++;entrada++;entrada++;
	p = open (entrada	 ,O_CREAT | O_RDONLY  | O_WRONLY , 0666 );
			
	if(p<0) //caso nao consiga criar o ficheiro
	{   
        perror("Ficheiro nao encontrado.Tente outra vez com outros ficheiros\n");
		_exit(-1);
	}
	
	x = mysystem(comando); //separa os comandos
	
	lseek(p,0,SEEK_END);//coloca o ficheiro no fim

	dup2(p,1);//ficheiro para a ser o stdout
	close(p);
	
	execvp(x[1],++x);
	perror("Comando nao reconhecido pelo sistema.Tente outra vez!\n");
	//_exit(-1);
}