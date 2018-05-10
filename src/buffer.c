#include "buffer.h"
#include "execut.h"

//-------------------------------------------------------------------------------------
    // Da autoria de Guilherme Viveiros && Angelo Sousa && Mateus Silva.
//-------------------------------------------------------------------------------------

//defines
#define True 1
#define False 0


// Estruturas
struct blocos{
    int checkC;//1 se for comando , 0 se nao for
    int numberC;//ordem do comando (ordenado por ordem crescente)
    char *line; //contem a frase respetiva
    char **words; //contem todas as palavras da respetiva linha
    char *result; //contem o resultado do respetivo comando
};

typedef struct buff{
    int size; //quantidade de blocos
    int is_ON; //verifica se o Buff está on
    int file; //file que o buff vai receber
    struct blocos *block;
}*Buff;


// Métodos privados
static int find (Buff x , struct blocos y , int n );
static struct blocos case1(struct blocos x);
static struct blocos case2(Buff x,struct blocos b,int c);


// Métodos publicos
int readln(int fildes,void *buff);
Buff create_buffer(int filedes,int size);
Buff load_buffer(Buff x);

//Variavel externa
int contador = 0;

//---------------------------------------------------------------------------------------------------------
/**
 * Percorre o buff e caso encontre o n comando anterior ao atual devolve a posivao desse bloco
 * Caso nao encontro devolve -1
 * */
static int find (Buff x , struct blocos b , int n ){
    int i;

    for(i=0; i<x->size ; i++){
        if (x->block[i].numberC == b.numberC - n) return i;
    }
    return -1;
}

/**
 * 3 casos possíveis:
 *Tipo 1 -> $ ls ,  executa o comando
 *Tipo 2 -> $| ls , executa o comando com o stdin anterior 
 *Tipo 3 -> $2| ls , executa o comando com o 2 stdin 
**/

//Tipo 1
static struct blocos case1(struct blocos x){
    int my_pipe[2];

    pipe(my_pipe);

    if(!fork()){
        close(my_pipe[0]);
        dup2(my_pipe[1],1);
        close(my_pipe[1]);
        execvp(x.words[1],++(x.words));
        perror("Comando indefenido pelo sistema.Tente outra vez!\n");
        _exit(-1);
    }
        else{
            close(my_pipe[1]);
            read(my_pipe[0],x.result,1024);
            close(my_pipe[0]);
        }
    return x;
}

//Tipo 2 / 3
static struct blocos case2(Buff x,struct blocos b, int c){
    int i,ant[2],act[2];
    pipe(ant);
    pipe(act);
  
    if (c == -1){
        i = find(x,b,1);
    }
    else {
        i = find(x,b,c);
    }

    printf("Meu comando :%d , %s \n" , b.numberC ,   b.line);
    printf("Comando a ser procurado -> %d\n" ,  x->block[i].numberC);
    printf("Comando = %s\n", x->block[i].line);
    if (i != -1) {
        if (fork()){
            close(ant[0]);//fecha o pipe 0 que representa o comando anterior
            write(ant[1] , x->block[i].result , 1024);//escreve no pipe 1 que repsenta o resultado do comando anterior
            close(ant[1]);//fecha o pipe 1 que representa o comando anterior
            close(act[1]);// fecha o pipe 1 do atual
            wait(NULL);//espera que o filho acabe de executar o comando atual
            read(act[0], b.result ,1024);//le para o b o resultado do comando que o filho executou(comando atual)
          
        }
            else {
                close(ant[1]); // fecha o pipe 1 anterior
                dup2(ant[0],0); //modifca o stdin do pipe anterior 
                close(act[0]); //fecha o pipe anterior
                dup2(act[1],1); //modifica o stdout de output para o pipe atual
                close(act[1]); //fecha o pipe atual
                execvp(b.words[1],++(b.words)); //executa o comando atual
                perror("Comando indefenido pelo sistema.Tente outra vez!\n");
                _exit(-1);
            }
    }

    return b;
}

//---------------------------------------------------------------------------------------------------------
Buff create_buffer(int filedes , int size){
    Buff x;

    x = malloc(sizeof (struct buff));
    x->size = size;
    x->is_ON = True;
    x->file = filedes;
    x->block = malloc(sizeof(struct blocos) *size);

    return x;
}

Buff load_buffer(Buff x){
int i;
    
    for(i=0; i < x->size ; i++){

        x->block[i].line = malloc(1024);
        readln(x->file , x->block[i].line);

        if (checkCommand(x->block[i].line)) {
            x->block[i].checkC = True;
    	    x->block[i].numberC = contador++;
    	    x->block[i].words = mysystem(x->block[i].line);
            x->block[i].result = malloc(1024);
            //quando for comando tipo 2 ou 3
            if(!strcmp (x->block[i].words[0] , "$")){
               x->block[i] = case1(x->block[i]);
            }
                else{ 
                    if(!strcmp (x->block[i].words[0] , "$|")){
                        x->block[i] = case2(x,x->block[i],-1);
                        printf("%s\n" , x->block[i].result);
                    }
                    else{
                        //o atoi representa o número que está entre o comando , ex :$2|
                        x->block[i] = case2(x,x->block[i], atoi(++x->block[i].words[0]) );
                        printf("%s\n" , x->block[i].result);
                    }
                }
        }
            else {
                 x->block[i].checkC = False;
                 x->block[i].words = mysystem(x->block[i].line);
            }
    }
    return x;
}


/**
 * Le de uma ficheiro para um buff
 * Para quando encontrar um '\n'
 * */
int readln(int fildes, void *buff ) {
	int x=0; char c;
	char *st = (char *)buff;

	while ( read (fildes , &c , 1) > 0 ) {
		if ( c == '\n') break; //quando encontrar paragrafo acaba
			*st = c;
			st++;
			x++;
	}
 	*st = '\0';
 	return x;
}


//Getters//--------------------------------------------------------------------------------------------------
int getSize(Buff x){return x->size;}

int getIs_on(Buff x){return x->is_ON;}

int getFile(Buff x){return x->file;}

int getCheck(Buff x , int i){return x->block[i].checkC;}

int getNumberC(Buff x , int i){return x->block[i].numberC;}

char * getLine(Buff x , int i){return x->block[i].line;}

char **getWords(Buff x , int i){return x->block[i].words;}

char *getResult(Buff x, int i){return x->block[i].result;}

//Setters//--------------------------------------------------------------------------------------------------
void setSize(Buff x , int y){x->size = y;}

void setFile(Buff x,int filedes){x->file = filedes;}

void *setResult(Buff x, char *r , int i){ strcpy(x->block[i].result ,r); _exit(-1);} //poque dá erro sem o exit???