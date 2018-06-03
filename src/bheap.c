#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "bheap.h"

//-------------------------------------------------------------------------------------
// Da autoria de Guilherme Viveiros && Angelo Sousa && Mateus Silva.
//-------------------------------------------------------------------------------------

//defines
#define Kb 1024

// Estruturas
struct tree
{
    char *comando;
    char *operador;
    char *resultado;
    struct tree *esq;
    struct tree *dir;
};

//Métodos publicos
void makeLTree(Tree *t, char *info);
void makeATree(Tree *t);
Tree *createTree(char *line);
void destroyTree(Tree *t);
Tree *resultFromTree(char *line, char *info);

//--------------------------------------------------------------------------------------
//executa os comando lineares(sem carateres especias primeiro)
void makeLTree(Tree *t, char *info){
    char *buffer = malloc(1024);
    int p;
    int status;
    int l;
    int my_pipe[2];
    char **x;
    pipe(my_pipe);

    if (t)
    {
        if (strcmp(t->operador, ">") == 0 || (strcmp(t->operador, ">>") == 0) || (strcmp(t->operador, "2>") == 0))
        { //caso em que nao queremos que o programa execute a sub-string da direita
            t->dir->operador = "Special";
        }
        if (strcmp(t->operador, "<") == 0)
        { //caso em que nao queremos que o progama execute as sub-sring
            t->dir->operador = "Special";
            t->esq->operador = "Special";
        }
        if (strcmp(t->operador, "None") == 0 && Pcheck(t->comando) == 0)
        {
            p = fork();
            if (p < 0)
            {
                redir_error(NULL,"Demasiados processos estão a ocorrer ao mesmo tempo.Programa cancelado\n",NULL,"Erros.txt");
                _exit(-1);
            }
            if (info != NULL && t->esq == NULL)
            { //Significa que tem que ir buscar resultados atrás
                printf("-");
                l = open("main.c", O_RDONLY, O_WRONLY, 066);
                if (l < 0)
                {
                    perror("erro\n");
                    _exit(-1);
                }
                if (p == 0)
                { //quando for filho
                    x = mysystem(t->comando);
                    dup2(l, 0);
                    close(l);
                    close(my_pipe[0]);
                    dup2(my_pipe[1], 1);
                    close(my_pipe[1]);
                    execvp(x[0], x);
                    //se chegar aqui o programa nao para , apenas este comando é que nao funciona , executo os outros
                    //aviso o pai que o comando está incorreto
                    _exit(3);
                }
                else
                { //pai
                    wait(&status);
                    if (WEXITSTATUS(status) == 3)
                    { //quando o comando nao for reconhecido pelo sistema

                        close(my_pipe[1]);
                        close(my_pipe[0]);
                        t->resultado = malloc(1024);
                        strcpy(t->resultado, "Comando nao econtrado no sistema.\n");
                        free(buffer);
                    }
                    else
                    {
                        close(my_pipe[1]);
                        t->resultado = malloc(1024 * 5);
                        read(my_pipe[0], buffer, 1024 * 5);
                        strcpy(t->resultado, buffer);
                        close(my_pipe[0]);
                    }
                }
            }
            else
            {
                //Nao vai buscar resultados atrás
                if (p == 0)
                { //quando for filho
                    free(buffer);
                    x = mysystem(t->comando);
                    close(my_pipe[0]);
                    dup2(my_pipe[1], 1);
                    close(my_pipe[1]);
                    execvp(x[0], x);
                    //se chegar aqui o programa nao para , apenas este comando é que nao funciona , executo os outros
                    //aviso o pai que o comando está incorreto
                    _exit(3);
                }
                else
                { //pai
                    wait(&status);
                    if (WEXITSTATUS(status) == 3)
                    { //quando o comando nao for reconhecido pelo sistema
                        close(my_pipe[1]);
                        close(my_pipe[0]);
                        t->resultado = malloc(1024);
                        strcpy(t->resultado, "Comando nao econtrado no sistema.\n");
                    }
                    else
                    {
                        close(my_pipe[1]);
                        t->resultado = malloc(1024 * 5);
                        read(my_pipe[0], t->resultado, 1024 * 5);
                        close(my_pipe[0]);
                    }
                }
            }
        }
        else
        { //quando tiver pipe e nao existir stdout dos comandos anteriores
            if (t->dir == NULL && t->esq == NULL && Pcheck(t->comando) == 1)
            {
                if (info == NULL)
                {
                    t->resultado = malloc(1024 * 5);
                    t->resultado = paraleloSC(t->comando, NULL);
                }
                else
                { //quando houver stdout dos comandos anteriores
                    t->resultado = malloc(1024 * 2);
                    t->resultado = paraleloSC(t->comando, info);
                }
            }
        }
        free(buffer);
        makeLTree(t->esq, info);
        makeLTree(t->dir, info);
    }
}

void makeATree(Tree *t){
    if (t)
    {
        makeATree(t->dir);
        makeATree(t->esq);
        if (t->dir != NULL && t->esq != NULL)
        {
            if (strcmp(t->operador, "||") == 0)
            { //caso em que o comando especial é ||
                t->resultado = double_pipe(t->esq->resultado, t->dir->resultado);
            }
            if (strcmp(t->operador, "&&") == 0)
            { //caso em que o comando especial é &&
                t->resultado = double_concorrente(t->esq->resultado, t->dir->resultado);
            }
            if (strcmp(t->operador, "&") == 0)
            { //caso em que o comando especial é &
                t->resultado = concorrente(t->dir->resultado, t->esq->resultado);
            }
            if (strcmp(t->operador, "2>") == 0)
            { //caso em que o comando especial é &
                t->resultado = redir_error(t->esq->comando, NULL, NULL, t->dir->comando);
            }
            if (strcmp(t->operador, ";") == 0)
            { //caso em que o comando especial é ;
                t->resultado = concorrente(t->esq->resultado, t->dir->resultado);
            }
            if (strcmp(t->operador, ">") == 0)
            { //caso em que o comando especial é >
                redir_out(t->esq->resultado, t->dir->comando);
                t->resultado = "";
            }
            if (strcmp(t->operador, ">>") == 0)
            { //caso em que o comando especial é >>
                redir_end(t->esq->resultado, t->dir->comando);
                t->resultado = " ";
            }
            if (strcmp(t->operador, "<") == 0)
            { //caso em que o comando especial é <
                t->resultado = redir_in(t->esq->comando, t->dir->comando);
            }
        }
    }
}

void makeScTree(Tree *t, char *info){
    int my_pipe[2];
    int aux[2];
    char **x;
    pipe(my_pipe);
    pipe(aux);
    if (t->esq)
    {
        makeScTree(t->esq, info);
    }
    else
    {
        if (strstr(t->comando, "|") == NULL)
        {
            if (fork())
            { //executa o comando  , no pai
                close(my_pipe[0]);
                write(my_pipe[1], info, 1024); //escreve o resultado no pipe
                close(my_pipe[1]);
                wait(NULL);
                //nesta altura o pai ja tem o resultado do filho
                close(aux[1]);
                read(aux[0], t->resultado, 1024);
                close(aux[0]);
            }
            else
            { //filho
                x = mysystem(t->comando);
                close(my_pipe[1]);
                dup2(my_pipe[0], 0); //pipe anterior para intput
                close(my_pipe[0]);
                close(aux[0]);
                dup2(aux[1], 1); //resultado final vem para aqui
                close(aux[1]);
                execvp(x[0], x);
                redir_error(NULL,"Comando desconhecido pela bash.\n",x[0],"Erros.txt");
                _exit(-1);
            }
        }
    }
}

Tree *createTree(char *line){
    Tree *x = NULL;
    if (!line)
        return 0; //caso nao tenha frase

    line = strdup(line); //duplico a string
    char *left;
    char *right;
    char *opr = verifica(line);

    if (strcmp(opr, "None") != 0)
    {
        if ((strcmp(opr, "&&") == 0) && (strcmp(opr, "||") == 0) && (strcmp(opr, ">>") == 0))
        { //caso especifico em que precisamos avancar o split +1 devido ha incopetencia das funcoes
            left = strdup(line);
            char *split = strstr(left, opr);
            split++;
            *split = '\0';
            right = split + strlen(opr);
        }
        else
        {
            left = strdup(line);
            char *split = strstr(left, opr);
            *split = '\0';
            right = split + strlen(opr);
        }
    }
    else
    {
        left = NULL;
        right = NULL;
    }
    x = malloc(sizeof(struct tree));
    x->comando = line;
    x->operador = opr;
    x->esq = createTree(left);
    x->dir = createTree(right);

    //makeScTree(x,"ollla a sd   asd as das d as\n ta   sda das d asdasdastudo crl\n");
    //makeATree(x);
    return x;
}

Tree *resultFromTree(char *line, char *info){
    Tree *x = createTree(line);
    makeLTree(x, info);
    if (info != NULL)
    {
        makeScTree(x, info);
    }
    makeATree(x);
    return x;
}

void destroyTree(Tree *t){
    if (t)
    {
        destroyTree(t->dir);
        destroyTree(t->dir);
        free(&t);
    }
}
//Getters!--------------------------------------------------------------------------------
char *getComando(Tree *x){
    return x->comando;
}

char *getOperador(Tree *x){
    return x->operador;
}

char *getResultado(Tree *x){
    return x->resultado;
}

//Setters!--------------------------------------------------------------------------------
void setComando(Tree *x, char *y){
    strcpy(x->comando, y);
}

void setOperador(Tree *x, char *y){
    strcpy(x->operador, y);
}

void setResultado(Tree *x, char *y){
    strcpy(x->resultado, y);
}