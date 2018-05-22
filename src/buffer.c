#include "buffer.h"
#include "auxF.h"
#include "Cextra.h"

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
    int used; //blocos usados;
    int is_ON; //verifica se o Buff está on
    int file; //file que o buff vai receber
    struct blocos *block;
} * Buff;


// Métodos privados
static int find (Buff x , struct blocos y , int n );
static struct blocos case1(struct blocos x);
static struct blocos case2(Buff x,struct blocos b,int c);
static struct blocos specialC(struct blocos x , char c);

// Métodos publicos
int readln(int fildes,void *buff);
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
    int my_pipe[2],p;

    pipe(my_pipe);
    p = fork();
    if(p == -1) {
        perror("Sistema nao consege criar mais processos!\n");
        _exit(-1);
    }
    if(!p){
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
    int i,ant[2],act[2],p;
    pipe(ant);
    pipe(act);
  
    if (c == -1){
        i = find(x,b,1);
    }
    else {
        i = find(x,b,c);
    }

    if (i != -1) {
        p = fork();
        if(p == -1) {
            perror("Sistema nao consege criar mais processos!\n");
            _exit(-1);
        }
        if (p){
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

static struct blocos specialC(struct blocos x , char c){
char *aux   =malloc(1024); 
char *tmp1 = malloc(1024);
char *tmp2 = malloc(1024);
        switch (c){
            case '|' :
                x.result = paraleloSC(x.line,NULL);
                break;
            case '>' :
                strcpy(aux,x.line);
                tmp1 = strstr(x.line,">"); // da me a resultade palavra a partir do > +1 
                tmp2 = strtok(aux,">"); //da me a resultante até ao >
                tmp1++;tmp1++; //estou a saltar os caracteres desnessesarios
                redir(NULL,tmp1,tmp2,NULL);
                break;
            case '<' :
                strcpy(aux,x.line);
                tmp1 = strstr(x.line,"<"); // da me a resultade palavra a partir do < +1 
                tmp2 = strtok(aux,"<"); //da me a resultante até ao <
                x.result = redir(tmp1,NULL,tmp2,NULL); // FAZERRRR MELHOR
                break;
            case '1' : //representa >>
                strcpy(aux,x.line);
                tmp1 = strstr(x.line,">>"); // da me a resultade palavra a partir do >> +1 
                tmp2 = strtok(aux,">>"); //da me a resultante até ao >>
                redir_end(tmp1,tmp2,NULL);
                break;
            default: break;
        }
    return x;
}
//---------------------------------------------------------------------------------------------------------
Buff create_buffer(int filedes){
    Buff x = malloc(sizeof (struct buff));
    char *phrase = malloc(1024);

    x->size = 1;
    x->used = 0;
    x->block = malloc(sizeof(struct blocos));
    x->is_ON = True;
    x->file = filedes;
    
    //porque nao funciona com EOF -> MUDAR PARA -1 E VERIFICAR PORQUE NAO FUNCIONA
    while(readln(x->file,phrase) > 0){
        //se for 0 quer dizer que encontrou um paragrafo , avancamos uma linha
        x = load_buffer(x,phrase);
    }
    free(phrase);
    return x;
}

Buff load_buffer(Buff x,char *y){//growing array
    char c;
    float taxa_ocupacao = (float) x->used/x->size;

        //quando for igual a 0 é só para o primeiro caso
        if (taxa_ocupacao > 0.8 || taxa_ocupacao == 0){
            x->block = realloc ( x->block, x->size*2*(sizeof (struct blocos)));
            x->size*=2;
        }

        x->block[x->used].line = malloc(1024);
        strcpy(x->block[x->used].line,y);
        
        if (checkCommand(x->block[x->used].line)) {   
            x->block[x->used].checkC = True;
    	    x->block[x->used].numberC = contador++;
    	    x->block[x->used].words = mysystem(x->block[x->used].line);
            free(mysystem(x->block[x->used].line));
            x->block[x->used].result = malloc(1024);
            //quando for comando tipo 1
            if(!strcmp (x->block[x->used].words[0] , "$")){

                if((c=verifica(x->block[x->used].line)) != '0' ){ //quando ocorrer comando especial
                //ls | cat && ls & cat && ls > cat && ls < cat
                    x->block[x->used] = specialC(x->block[x->used] , c);
                }
                    else {
                        x->block[x->used] = case1(x->block[x->used]);
                    }

            }
                else{ 
                    if(!strcmp (x->block[x->used].words[0] , "$|")){
                          x->block[x->used] = case2(x,x->block[x->used],1);
                    }
                    else{
                        //o atoi representa o número que está entre o comando , ex :$2|
                        x->block[x->used] = case2(x,x->block[x->used], atoi(++x->block[x->used].words[0]) );
                    }
                }
        }
            else {
                 x->block[x->used].checkC = False;
                x->block[x->used].words = mysystem(x->block[x->used].line);
            }
          
    x->used++;
    return x;
}

void destroy_buffer(Buff x){
    int i=0;
    for(i=0;i<x->size;i++){
        free(x->block[i].result);
        free(x->block[i].line);
        free(x->block[i].words);
    }
    free(x->block);
    free(x);
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

void *setResult(Buff x, char *r , int i){ strcpy(x->block[i].result ,r); _exit(-1);} //poque dá erro sem o exit???

