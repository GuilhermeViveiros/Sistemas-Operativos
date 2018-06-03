#ifndef Cextra_h
#define Cextra_h

#include "buffer.h"


//verifica qual o caracter do comando especial a ser utilizado
char* verifica(char *line);

//Executa comandos -> ls -l | cat | head -1
char *paraleloSC(char *line, char *info);

//Executa comandos sequecnias -> ls -l && ps 
char* double_concorrente(char *comando1,char *comando2);

//Executa comandos paralelos -> ls -l & ps & ps -a
char* concorrente(char *comando1 , char *comando2);

//Executa ou comando1 ou comando2 -> ls -l || p
char* double_pipe(char *comando1 , char *comando2);

//Redireciona o resultado de um comando para um ficheiro de saida -> ls > Novo.txt
char* redir_out(char *comando1 , char *saida);

//Aplica um comando ao conteudo de um ficheiro -> head -1 < Novo.txt
char *redir_in(char *comando , char *entrada);

//Redireciona o resultado de um comando para o fim de um ficheiro -> ls -l >> Novo.txt
char* redir_end(char *comando , char *entrada);

//Redireciona um erro para um ficheiro -> -l 2> Erros.txt
char* redir_error(char *comando,char *texto,char *comando_texto , char *saida);


#endif /* Cextra */