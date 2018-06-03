#include "buffer.h"
#include "auxF.h"
#include "Cextra.h"
#include "bheap.h"

//-------------------------------------------------------------------------------------
    // Da autoria de Guilherme Viveiros && Angelo Sousa && Mateus Silva.
//-------------------------------------------------------------------------------------

//defines
#define Kb 1024
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

struct buff{
    int size; //quantidade de blocos
    int used; //blocos usados;
    int is_ON; //verifica se o Buff está on
    int file; //file que o buff vai receber
    struct blocos *block;
};


// Métodos privados
static int find (Buff x , struct blocos y , int n );
static struct blocos case1(struct blocos x);
static struct blocos case2(Buff x,struct blocos b,int c);

// Métodos publicos
Buff create_buffer(int filedes);
Buff load_buffer(Buff x,char *y);
void destroy_buffer(Buff x);

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
        if (x->block[i].checkC){
            if (x->block[i].numberC == b.numberC - n) return i;
            }
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
    int my_pipe[2],p,i=0;
    char c;
    pipe(my_pipe);
    p = fork();
    if(p == -1) {
        redir_error(NULL,"Sistema nao consege criar mais processos.\nPrograma terminado , ficheiro inicial não modificado.\n",x.line,"Erros.txt");
        _exit(-1);
    }
    if(!p){
        close(my_pipe[0]);
        dup2(my_pipe[1],1);
        close(my_pipe[1]);
        execvp(x.words[1],(x.words)+1);
        redir_error(NULL,"Comando desconhecido pelo sistema.\n",x.line,"Erros.txt");
        _exit(-1);   
    }
        else{
            close(my_pipe[1]);
            while(read(my_pipe[0],&c,1)>0){
                x.result[i++] = c;
            }
            close(my_pipe[0]);
        }
    return x;
}

//Tipo 2 / 3
static struct blocos case2(Buff x,struct blocos b, int c){
    int i,ant[2],act[2],p;
    char tmp;
    pipe(ant);
    pipe(act);
  
    i = find(x,b,c);//encontra o local do comando a ser encontrado
    if(i==-1 || x->used < i){
        redir_error(NULL,"Tentou aceder a resultados nao concebidos pelo ficheiro.\nTente outra vez.",NULL,"Erros.txt");
        _exit(-1);
    }
        p = fork();
        if(p == -1) {
            redir_error(NULL,"Sistema nao consege criar mais processos.\nPrograma terminado , ficheiro inicial não modificado.\n",NULL,"Erros.txt");
            _exit(-1);
        }
        if (p){
            close(ant[0]);//fecha o pipe 0 que representa o comando anterior
            write(ant[1] , x->block[i].result , 5*Kb);//escreve no pipe 1 que repsenta o resultado do comando anterior
            close(ant[1]);//fecha o pipe 1 que representa o comando anterior
            close(act[1]);// fecha o pipe 1 do atual
            wait(NULL);//espera que o filho acabe de executar o comando atual 
            i=0;
            while(read(act[0], &tmp ,1)>0){
                b.result[i++] = tmp; //le para o b o resultado do comando que o filho executou(comando atual) 
            }
        }
            else {
                close(ant[1]); // fecha o pipe 1 anterior
                dup2(ant[0],0); //modifca o stdin do pipe anterior 
                close(act[0]); //fecha o pipe anterior
                dup2(act[1],1); //modifica o stdout de output para o pipe atual
                close(act[1]); //fecha o pipe atual
                execvp(b.words[1],(b.words)+1); //executa o comando atual
                redir_error(NULL,"Comando indefenido pelo sistema.Tente outra vez!\n.",b.line,"Erros.txt");
                _exit(-1);
            }
    return b;
}

//specialCase
static char* Scase(Buff x, struct blocos b , int c){
    int i = find(x,b,c);//encontra o local do comando a ser encontrado
    if(x->used < i || i == -1){
        redir_error(NULL,"Tentou aceder a resultados nao concebidos pelo ficheiro.\nTente outra vez.",NULL,"Erros.txt");
        _exit(-1);
    }
    return x->block[i].result;
}

//---------------------------------------------------------------------------------------------------------
Buff create_buffer(int filedes){
    Buff x = malloc(sizeof (struct buff));
    char *phrase = malloc(10*Kb);
    x->size = 1;
    x->used = 0;
    x->block = malloc(sizeof(struct blocos));
    x->is_ON = True;
    x->file = filedes;
    
    while(readln(x->file,phrase) > 0){
        if(strcmp(phrase, ">>>") == 0){
            while(strcmp(phrase, "<<<") != 0){
                readln(x->file,phrase);
            };
        }
        else x = load_buffer(x,phrase);
    }
    x = load_buffer(x,phrase);
    
    free(phrase);    
    return x;
}

Buff load_buffer(Buff x,char *y){//growing array
    float taxa_ocupacao = (float) x->used/x->size;

        //quando for igual a 0 é só para o primeiro caso
        if (taxa_ocupacao > 0.8 || taxa_ocupacao == 0){
            x->block = realloc ( x->block, x->size*2*(sizeof (struct blocos)));
            x->size*=2;
        }

        x->block[x->used].line = malloc(5*Kb);
        strcpy(x->block[x->used].line,y);
        
        if (checkCommand(x->block[x->used].line)) {   
            x->block[x->used].checkC = True;
    	    x->block[x->used].numberC = contador++;
    	    x->block[x->used].words = mysystem(x->block[x->used].line);
            x->block[x->used].result = malloc(1024);

            //quando for comando tipo 1
            if(!strcmp (x->block[x->used].words[0] , "$")){
                if(strcmp (verifica(x->block[x->used].line) , "None") || strstr(x->block[x->used].line , "|") !=NULL){ //quando ocorrer comando especial
                    Tree *t = resultFromTree(x->block[x->used].line+1 , NULL);
                    x->block[x->used].result = getResultado(t); 
                    destroyTree(t);
                }else x->block[x->used] = case1(x->block[x->used]);
            }   
                //quando for comando tipo 2
                else{ 
                    if(!strcmp (x->block[x->used].words[0] , "$|")){
                        if(strcmp (verifica(x->block[x->used].line) , "None") || strstr(x->block[x->used].line+2 , "|") !=NULL){ //quando ocorrer comando especial
                            //modifca me o resultado do atual , através do stdout do bloco anterior
                            Tree *t = resultFromTree(x->block[x->used].line+2 , Scase(x,x->block[x->used],1));
                            x->block[x->used].result = getResultado(t); 
                            destroyTree(t);
                        }else  x->block[x->used] = case2(x,x->block[x->used],1);//sem comandos especiais
                    }
                        else{ //quando for comando tipo 3
                            if(strcmp (verifica(x->block[x->used].line) , "None") || strstr(x->block[x->used].line+3 , "|") !=NULL){ //quando ocorrer comando especial
                                //modifca me o resultado do atual , através do stdout do bloco anterior
                                Tree *t = resultFromTree(x->block[x->used].line+3 , Scase(x,x->block[x->used],atoi(x->block[x->used].words[0]+1)));
                                x->block[x->used].result = getResultado(t); 
                                destroyTree(t);
                            }else x->block[x->used] = case2(x,x->block[x->used], atoi(x->block[x->used].words[0]+1));
                        } 
                }
        //quando for documentação
        }else x->block[x->used].checkC = False;
          
    x->used++;
    return x;
}

void destroy_buffer(Buff x){
    int i=0;
    if(x){
    for(i=0;i<=x->used;i++){
        if(x->block[x->used].checkC == True){//para fragmentos de código
            free(x->block[i].result);
            free(x->block[i].line);
        }else{//para documentação
            free(x->block[i].line);
        }
    }
    free(x->block);
    free(x);
    }
}

//Getters//--------------------------------------------------------------------------------------------------
int getSize(Buff x){return x->size;}

int getUsed(Buff x){return x->used;}

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

void setResult(Buff x, char *r , int i){ strcpy(x->block[i].result ,r);}

