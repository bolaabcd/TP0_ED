//---------------------------------------------------------------------
// Arquivo      : memlog.h
// Conteudo     : definicoes da biblioteca de registro de acessos
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2021-10-18 - arquivo criado
//              : 2021-10-22 - estrutura de diretorios
//              : 2021-10-27 - (Artur Gaspar) alterado tipo do iniciaMemLog para string
//---------------------------------------------------------------------

#ifndef MEMLOGH
#define MEMLOGH

#include <string>
#include <stdio.h>
#include <time.h>

typedef struct memlog
{
	FILE *log;
	clockid_t clk_id;
	struct timespec inittime;
	long count;
	int regmem;
	int fase;
	int ativo;
} memlog_tipo;

int iniciaMemLog(std::string nome, int regmem);
int ativaMemLog();
int desativaMemLog();
int defineFaseMemLog(int f);
int leMemLog(long int pos, long int tam);
int escreveMemLog(long int pos, long int tam);
int finalizaMemLog();

#endif
