#include "Cextra.h"
#include "buffer.h"
#include "auxF.h"

//-------------------------------------------------------------------------------------
    // Da autoria de Guilherme Viveiros && Angelo Sousa && Mateus Silva.
//-------------------------------------------------------------------------------------

// Métodos publicos
char verifica(char *line);
char* paraleloSC(char *line , char *texto);
char* concorrenteSC(char *command,char *texto);
char* redir (char *entrada , char *saida , char *comando,char *texto);
void redir_end(char *entrada , char *comando ,char *texto);
void redir_error(char *saida , char *comando , char *texto , int redir_end);

//--------------------------------------------------------------------------------------------------
char verifica(char *line){
    if (strstr  (line, ">>")) return '1';
    if ( strstr (line , ">")) return '>';
    if ( strstr (line , "<")) return '<';
    if ( strstr (line , "|")) return '|';
    if ( strstr (line , "&")) return '&';
    return '0';
}

char* paraleloSC(char *line,char *texto){
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

char* concorrenteSC(char *command , char *texto){
	int l=0,c=0,i,p,aux;
	char *result = malloc(1024*5);
	char *buffer = malloc(1024*5);
	char *tmp[20][20];
	char **exemplo = mysystem(command);
	
	  p = open ("resultados" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

      if(p<0){
      	perror("Couldnt create the file!\n");
      	_exit(-1);
      }

		while(*exemplo){ //defino a matriz com os comandos

        	if(strcmp(*exemplo , "&") ==0){
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
    	l++;
	if(!texto){
    	for(i=0 ; i < l ; i++){
    		if(!fork()){ //se for filho , criar processos para cada um dos comandos
    			dup2(p,1);
    			close(p);
    			execvp(tmp[i][0],tmp[i]);
    			perror("Comando nao defenido");
    			_exit(-1);
    		}
   		}

   		for(i=0; i<l; i++){
   			wait(NULL);
   		}
   		aux = open ("resultados" , O_RDONLY ,0666);
		lseek(aux,0,SEEK_SET) ;

   		while( readall( aux , result) > 0){
			strcpy(buffer , result); 
	   	}
	}
	else {

		for(i=0 ; i < l ; i++){
    		if(!fork()){ //se for filho , criar processos para cada um dos comandos
				write(p,texto,strlen(texto));
    			dup2(p,1);
    			close(p);
    			execvp(tmp[i][0],tmp[i]);
    			perror("Comando nao defenido");
    			_exit(-1);
    		}
   		}

   		for(i=0; i<l; i++){
   			wait(NULL);
   		}
	
   		aux = open ("resultados" , O_RDONLY ,0666);
		lseek(aux,0,SEEK_SET) ;	

   		while( readall( aux , result) > 0){
			strcpy(buffer , result); 
	   	}
	}
	return buffer;
}

char* redir(char *entrada , char *saida , char *comando , char *texto){
    int my_pipe[2];
    int p ;
	char **x; char *result = malloc(1024);
    pipe(my_pipe);
	if (!entrada) //quando nao existir ficheiro de entrada -> >
	{
		if(saida){
			p = open (saida , O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC  , 0666 );
			if(p<0) //caso nao consiga encontrar o ficheiro
			{	
				perror("Nao conseguimos criar o ficheiro!\n");
				_exit(-1);
			}

			if(!texto){ //caso em queremos o resultado de um comando para um ficheiro
                if(!fork()){ //crio um fork para parar o programa a meio devido ao exec
				    x = mysystem(comando);
				    dup2(p,1);
				    close(p);
				    execvp(x[1] , ++x);
				    perror("Comando nao reconhecido pelo sistema.Tente outra vez!\n");
				    _exit(-1);
			    }
        	}
				else { //caso em que queremos um texto para um ficheiro (quando estamos a falar em concorrencia)
					if(!fork()){ 
				    	write(p,texto,strlen(texto));
						close(p); //fecho o descritor
				    	_exit(-1);
			    	}
				}
		}
	}
	else //quando existir ficheiro de entrada -> <
	{
		if(!saida){
			entrada++; entrada++;
			p = open (entrada , O_RDONLY  , 0666 );
			
			if(p<0) //caso nao consiga criar o ficheiro
			{
				perror("Ficheiro de leitura nao foi criado.Tente outra vez!\n");
				_exit(-1);
			}
			else 
			{	if(!fork()){ 
				    x = mysystem(comando);
				    dup2(p,0);
                    close(my_pipe[0]);
                    dup2(my_pipe[1],1);
                    close(my_pipe[1]);
				    close(p);
				    execvp(x[1],++x);
				    perror("Comando nao reconhecido pelo sistema.Tenta outra vez!\n");
				    _exit(-1);
                }
                else {
                    close(my_pipe[1]);
                    read(my_pipe[0],result,1024);
                    close(my_pipe[0]);
                }
            }
		}
	}
    return result;
}

void redir_end(char *entrada , char *comando,char *texto){
	int p;
	char **x;
    entrada++;entrada++;entrada++;
	p = open (entrada , O_RDONLY  | O_WRONLY , 0666 );
			
	if(p<0) //caso nao consiga criar o ficheiro
	{   
	    p = open(entrada , O_CREAT | O_RDONLY | O_WRONLY , 0666);
	}
	
	x = mysystem(comando); //separa os comandos
	lseek(p,0,SEEK_END);//coloca o ficheiro no fim

    if(!fork()){
	    dup2(p,1);//ficheiro para a ser o stdout
	    close(p);
	    execvp(x[1],++x);
	    perror("Comando nao reconhecido pelo sistema.Tente outra vez!\n");
	    _exit(-1);
    }
}

void redir_error(char *saida , char *comando , char *texto , int redir_end){
	int p;
	char **x;
	saida++;saida++; //avanca caracteres desnecessarios

		if(!redir_end){ //quando nao for para escrever no fim do ficheiro
			p = open (saida , O_CREAT | O_WRONLY | O_RDONLY |  O_TRUNC  , 0666 );
			if(p<0) //caso nao consiga encontrar o ficheiro
			{	
				perror("Nao conseguimos criar o ficheiro.\n");
				_exit(-1);
			}

			if(!texto){ //caso em queremos o resultado de um comando para um ficheiro
                if(!fork()){ //crio um fork para parar o programa a meio devido ao exec
				    x = mysystem(comando);
				    dup2(p,2);
				    close(p);
				    execvp(x[1] , ++x);
				    perror("Comando nao reconhecido pelo sistema.Tente outra vez!\n");
				    _exit(-1);
			    }
        	}
				else { //caso em que queremos um texto para um ficheiro
					if(!fork()){ 
				    	write(p,texto,strlen(texto));
						close(p); //fecho o descritor
				    	_exit(-1);
			    	}
				}
		}
		else{ //caso em que queremos mandar para o fim do ficheiro de este existir
			p = open (saida , O_WRONLY , 0666 );
				if(p<0){
					//caso o ficheiro nao exista
					p = open (saida , O_CREAT | O_WRONLY | O_RDONLY  , 0666 );
				}
				lseek(p,0,SEEK_END);
				if(!texto){ //quando for para colocar o resultado de um comando no ficheiro
					x = mysystem(comando);
				    dup2(p,2);
				    close(p);
				    execvp(x[1] , ++x);
				    perror("Comando nao reconhecido pelo sistema.Tente outra vez!\n");
				    _exit(-1);
				}
				else { //caso em que queremos um texto para um ficheiro
					if(!fork()){ 
				    	write(p,texto,strlen(texto));
						close(p); //fecho o descritor
				    	_exit(-1);
			    	}
				}
		}
}
