#include "Cextra.h"
#include "buffer.h"
#include "auxF.h"


//-------------------------------------------------------------------------------------
    // Da autoria de Guilherme Viveiros && Angelo Sousa && Mateus Silva.
//-------------------------------------------------------------------------------------

//defines
#define Kb 1024

// Métodos publicos
char* verifica(char *line);
char* concorrente(char *comando1 , char *comando2);
char* double_pipe(char *comando1 , char *comando2);
char* redir_out(char *comando1 , char *saida);
char *redir_in(char *comando , char *entrada);
char* redir_end(char *comando , char *entrada);
char* redir_error(char *comando,char *texto,char *comando_texto , char *saida);

//--------------------------------------------------------------------------------------------------
char* verifica(char *line){
    //os caracteres && >> || estao primeiro , pois quando sao procurados com o strstr deixam um caracter para tras , preciso de remover esses casos 
    if (strstr (line , "&&")) return "&&"; //concorrente dependente
    if (strstr (line, ">>")) return ">>"; //escreve no fim do ficheiro
    if (strstr (line , "2>")) return "2>"; //escreve no inicio do ficheiro
    if (strstr (line , "&")) return "&"; //concorrente
    if (strstr (line , ">")) return ">"; //escreve no inicio do ficheiro
    if (strstr (line , "||")) return "||"; //caso execute bem o 1 comando executa o 2 comando direito
    if (strstr (line , ";")) return ";"; //concorrente
    if (strstr (line , "<")) return "<"; //retira informacao do ficheiro
    return "None";
}

char *paralelo(char **comando1, char *info){	
	char *result = malloc(2048);
	int aux[2];
	int my_pipe[2];
	pipe(my_pipe);pipe(aux);

	if (!fork())
	{ //filho
		close(aux[1]); close(my_pipe[0]);
    	dup2(aux[0],0);
		close(aux[0]);
		dup2(my_pipe[1],1);
		close(my_pipe[1]);
		execvp(comando1[0],comando1);
		redir_error(NULL,"Comando desconhecido pelo sistema.\n",comando1[0],"Erros.txt");
		_exit(-1);
	}
	else //pai
	{
		close(aux[0]);
		write(aux[1],info,2048);
		close(aux[1]);
		wait(NULL); //espera que o filho receba informação e devolva informação
		close(my_pipe[1]);
		read(my_pipe[0],result,2048);
		close(my_pipe[0]);
	}
	return result;//resultado encadeado
}

char *paraleloSC(char *line, char *info){
    char *result = malloc(5*Kb);
    int c = 0, l = 0;
    int my_pipe[2]; //crio os respetivos pipes
    char *x = line; //igual ao argumento recebido
    char **exemplo = mysystem(line);
    int size = 0;

    while (x != NULL)
    { //calcula quantas linhas a matriz irá ter
        x = strstr(++x, "|");
        size++;
    }

    char *tmp[size][5]; //cria a matriz de argumentos para fazer o respetivo

    while (*exemplo)
    { //defino a matriz com os comandos
        if (strcmp(*exemplo, "|") == 0)
        {
            tmp[l][c] = NULL;
            l++;
            c = 0;
        }
        else
        {
            tmp[l][c] = *exemplo;
            c++;
        }
        exemplo++;
    }
    size=0;
    tmp[l][c] = NULL;
    if (info == NULL)
    { //comandos sem resultados anteriores
        pipe(my_pipe);
        if (!fork())
        { //executa o primeiro comando para um ficheiro
            close(my_pipe[0]);
            dup2(my_pipe[1], 1);
            close(my_pipe[1]);
            execvp(tmp[size][0], tmp[size]); // size = 0;
            redir_error(NULL,"Comando desconhecido pelo sistema.\n",tmp[0][0],"Erros.txt");
            _exit(-1);
        }
        else
        { //pai guarda o resultado do primeiro filho e começa a executar os outros filhos
            wait(NULL);
            close(my_pipe[1]);
            read(my_pipe[0], result, 3*Kb);
            close(my_pipe[0]);
        } //resultado do primiero comando no result.
        size++;
    }
    else
    {
        result = info;
    }
    while (size <= l)
    {
        result = paralelo(tmp[size++], result);
 
    }
    return result;
}

char* double_concorrente(char *comando1,char *comando2){
    char *buffer = malloc(5*Kb);
    int i=0;
    if(strcmp(comando1,"Comando nao econtrado no sistema.\n") != 0 && strcmp(comando1,"")!=0){ //caso o primeiro esteja certo
        if(strcmp(comando2,"Comando nao econtrado no sistema.\n") != 0 && strcmp(comando2,"")!=0){ //caso o 2 esteja certo
		   while(*comando1){
               buffer[i++] = *comando1;
               comando1++;
           }while(*comando2){
               buffer[i++] = *comando2;
               comando2++;
           }
           return buffer;
        }
	}else{//primeiro errado
        return comando1;
    }
    free(buffer);
    return comando1;
}

char* concorrente(char *comando1 , char *comando2){
	int p,s,i=0;
    char c;
	char *buffer = malloc(5*Kb);
	
	p = open ("resultados" , O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC , 0666);

    if(p<0){
        redir_error(NULL,"Nao é possivel criar mais ficheiros!\n",NULL,"Erros.txt");
      	_exit(-1);
    }

	write(p,comando1,strlen(comando1));
	write(p,comando2,strlen(comando2));

	close(p);

	s = open("resultados" , O_RDONLY ,066);
	while(read(s,&c,1)>0){
        buffer[i++] = c;
    };
	close(s);
	return buffer;
}

char* double_pipe(char *comando1 , char *comando2){
    if(strcmp(comando1,"Comando nao econtrado no sistema.\n") != 0 && strcmp(comando1,"")!=0){ //caso o primeiro esteja certo
        return comando1;
	}
    if(strcmp(comando2,"Comando nao econtrado no sistema.\n") != 0 && strcmp(comando2,"")!=0){ //caso o 2 esteja certo
		return comando2;
	}
		
    return "Comando nao econtrado no sistema.\nComando nao econtrado no sistema.\n"; //ambos errados
}

char* redir_out(char *comando1 , char *saida){
	int p=0;
    while(*saida == ' '){//Para remover o espaço que fica após caracter
        saida++;
    }
	p = open (saida , O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC  , 0666 );
		if(p<0) //caso nao consiga encontrar o ficheiro
		{	
            redir_error(NULL,"Nao conseguimos criar o ficheiro!\n",NULL,"Erros.txt");
			_exit(-1);
		}
	write(p,comando1,strlen(comando1));
	close(p);	
	return "";	
}

char *redir_in(char *comando , char *entrada){
	int p ,i=0; char c; 
	int my_pipe[2];
	char *result = malloc(5*Kb); char **x;
    //Para remover o espaço que fica após caracter
    while(*entrada == ' '){entrada++;}

    p =  open (entrada , O_RDONLY , 0666 );
	pipe(my_pipe);
	
    	if(p<0) //caso nao consiga criar o ficheiro
		{   
            redir_error(NULL,"Ficheiro de leitura nao foi criado.Tente outra vez!\n",NULL,"Erros.txt");
			_exit(-1);
		}	
			else {	
				if(!fork()){ 
				    x = mysystem(comando);
				    dup2(p,0);
                    close(my_pipe[0]);
                    dup2(my_pipe[1],1);
                    close(my_pipe[1]);
				    close(p);
				    execvp(x[0],x);
                    redir_error(NULL,"Comando nao reconhecido pelo sistema.Tenta outra vez!\n",x[0],"Erros.txt");
				    _exit(-1);
                }
                else {
                    close(my_pipe[1]);
                    while(read(my_pipe[0],&c,1)>0){
                        result[i++]=c;
                    };
                    close(my_pipe[0]);
                }
            }
    return result;
}

char *redir_end(char *comando , char *entrada){
	int p;
	char *c = "\n";
    while(*entrada==' '){
        entrada++;
    }
	p = open (entrada , O_WRONLY , 0666 );
			
	if(p<0) //caso nao consiga criar o ficheiro cria-o
	{   
	    p = open(entrada , O_CREAT | O_RDONLY | O_WRONLY , 0666);
	}
	
	lseek(p,0,SEEK_END);//coloca o ficheiro no fim
	write(p,c,sizeof(char));
	write(p,comando,strlen(comando));
    close(p);
	return "";
}

char* redir_error(char *comando,char *texto,char *comando_texto , char *saida){
	int p;
    char **x;
    //Para remover os espaços que ficam antes de cada caracter
    while(*saida == ' '){saida++;}
	if(!texto){//quando for para redirecionar comandos errados
        p = open (saida , O_RDONLY  | O_WRONLY , 0666 );
			
	    if(p<0) //caso nao consiga abir o ficheiro cria-o
	    {   
	        p = open(saida , O_CREAT | O_RDONLY | O_WRONLY | O_TRUNC , 0666);
	    }	
        x = mysystem(comando); //separa os comandos
	    lseek(p,0,SEEK_END);//coloca o ficheiro no fim
        dup2(p,2);//ficheiro para a ser o stderror
        perror(*x);
        close(p);
	}else{//ridereciona texto e acaba o programa
		p = open ("Erros.txt" , O_CREAT | O_WRONLY |  O_TRUNC , 0666);
        dup2(p,2);
		lseek(p,0,SEEK_END);//coloca o ficheiro no fim
		perror(texto);
        perror(comando_texto);
        close(p);
    }
    return "";
}
