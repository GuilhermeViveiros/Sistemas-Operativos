#ifndef bheap_h
#define bheap_h

#include "Cextra.h"
#include <fcntl.h>
#include <unistd.h>
#include "auxF.h"

/**
*Estrutura de um árvore binária nomeada Tree
*/
typedef struct tree Tree;

/**
 * Inicializa a arvore a partir de um comando que contenha carecteres especiais
 * &  |  ;  |  ||  && > >> <
 * A partir do comando a função distribiu de forma estruturada vários comandos , cada nodo contem irá conter outro comando.
 * Resultado recursivo no qual primeiro executa os comandos mais faceis para conseguir resolver os mais complexos
 * */
Tree* resultFromTree(char *line,char* info);

//Cria a respetiva arvore a partir de um comando
Tree* createTree(char *line);

//Destroi a memoria alocada pela arvore
void destroyTree(Tree *t);

//Executa os nodos inferiores , ou seja os comandos que nao contêm caracteres especiais
void makeLTree(Tree *t,char *info);

//Executa todos os comandos respetivos das folhas a partir dos resultados dos filhos
void makeATree(Tree *t);

//Caso o comando necessite do stdout de um comando anterior , entao esta função executa o nodo inferior esquerdo(primeiro comando) através do determinado stdin(info)
void makeScTree(Tree *t, char *info);

//Getters!--------------------------------------------------------------------------------------------------
char* getComando(Tree *x);//devolve o comando final resultante da arvore binaria

char* getOperador(Tree *x);//devolve o primeiro operador(caracter especial) da arvore binaria

char* getResultado(Tree *x);//devolve o resultado final resultante da arvore binaria

//Setters!--------------------------------------------------------------------------------------------------
void setComando(Tree *x , char *y);//define o comando da arvore binaria

void setOperador(Tree *x , char *y);//define o operador da arvore binaria

void setResultado(Tree *x , char *y);//define o resultado da arvore binaria

#endif /* bHeap */
