#ifndef Cextra_h
#define Cextra_h

#include "buffer.h"


//verifica qual o caracter do comando a especial a ser utilizado
char verifica(char *line);

//Executa programas concorrentes -> ls -l | cat | head -1
char* paraleloSC(char *line,char *texto);

//Executa programas concorrentemente -> ls -l && cat | head -1
char* concorrenteSC(char *command,char *texto);

//funcao que recebe um comando e um ficheiro de entrada ou saida (nao receber os dois ao mesmo tempo)
char* redir (char *entrada , char *saida , char *comando ,char *texto);

/**
 * funcao que executa um comando e guarda o resultado sucessivamente no ficheiro
 *  se a funcao receber texto quer dizer que nao irá executar comando , pois pode ser comando paralelo , ls -l | head < x
 * */ 
void redir_end(char *entrada , char *comando ,char *texto);

/**
 * funcao que executa redireciona o stderror de um comando nao defenido ou texto para um ficheiro de error
 * */
void redir_error(char *saida , char *comando , char *texto , int redir_end);

#endif