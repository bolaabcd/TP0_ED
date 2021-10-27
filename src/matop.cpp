//---------------------------------------------------------------------
// Arquivo      : matop.cpp
// Conteudo     : programa de avaliacao do TAD MAT 
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2021-10-18 - arquivo criado
//              : 2021-10-21 - estrutura de diretorios
//              : 2021-10-25 - opcoes de linha de comando 
//              : 2021-10-27 - (Artur Gaspar) alterado para especificacao do trabalho
//---------------------------------------------------------------------

#include <iostream>
#include <getopt.h>
#include <string>
#include "mat.h"
#include "memlog.h" 
#include "msgassert.h" 

// definicoes de operacoes a serem testadas
#define OPSOMAR 1
#define OPMULTIPLICAR 2
#define OPTRANSPOR 3

// variaveis globais para opcoes
static int opescolhida;
std::string lognome, opt1nome, opt2nome;
int regmem;

void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
  fprintf(stderr,"matop\n");
  fprintf(stderr,"\t-s \t\t(somar matrizes) \n");
  fprintf(stderr,"\t-m \t\t(multiplicar matrizes) \n");
  fprintf(stderr,"\t-t \t\t(transpor matriz)\n");
  fprintf(stderr,"\t-p <arq>\t(arquivo de registro de acesso)\n");
  fprintf(stderr,"\t-l \t\t(registrar acessos a memoria)\n");
  fprintf(stderr,"\t-1 <arq>\t(arquivo da primeira matriz)\n");
  fprintf(stderr,"\t-2 <arq>\t(arquivo da segunda matriz)\n");
  fprintf(stderr,"\t-h \t\t(obter ajuda) \n");
}


void parse_args(int argc,char ** argv)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc e argv
// Saida: optescolhida, optx, opty, regmem, lognome
{ 
  // tratando caso de nao ter argumentos
  if (argc == 1) {
    uso();
    exit(1);
  }

  // variaveis externas do getopt
  extern char * optarg;
  extern int optind;

  // variavel auxiliar
  int c;

  // inicializacao variaveis globais para opcoes
  opescolhida = -1;
  regmem = 0;
  opt1nome[0] = 0;
  opt2nome[0] = 0;
  lognome[0] = 0;

  // getopt - letra indica a opcao, : junto a letra indica parametro
  // no caso de escolher mais de uma operacao, vale a ultima
  while ((c = getopt(argc, argv, "smtp:1:2:lh")) != EOF)
    switch(c) {
      case 'm':
        avisoAssert(opescolhida==-1,"Mais de uma operacao escolhida");
        opescolhida = OPMULTIPLICAR;
        break;
      case 's':
        avisoAssert(opescolhida==-1,"Mais de uma operacao escolhida");
        opescolhida = OPSOMAR;
        break;
      case 't':
        avisoAssert(opescolhida==-1,"Mais de uma operacao escolhida");
        opescolhida = OPTRANSPOR;
        break;
      case 'p': 
        lognome = optarg;
        break;
      case '1': 
        opt1nome = optarg;
        break;
      case '2': 
        opt2nome = optarg;
        break;
      case 'l': 
        regmem = 1;
        break;
      case 'h':
      default:
        uso();
        exit(1);
    }
  // verificacao da consistencia das opcoes
  erroAssert(opescolhida>0,"matop - necessario escolher operacao.");
  erroAssert(lognome.length()>0,
    "matop - nome de arquivo de registro de acesso tem que ser definido.");
  erroAssert(opt1nome.length()>0,
    "matop - nome de arquivo da primeira matriz tem que ser definido.");
  erroAssert(opt2nome.length()>0 || opescolhida == OPTRANSPOR,
    "matop - nome de arquivo da segunda matriz tem que ser definido para a operacao escolhida.");
}


int main(int argc, char ** argv)
// Descricao: programa principal para execucao de operacoes de matrizes 
// Entrada: argc e argv
// Saida: depende da operacao escolhida
{
  // ate 3 matrizes sao utilizadas, dependendo da operacao
  mat_tipo a, b, c;

  // avaliar linha de comando
  parse_args(argc,argv);

  // iniciar registro de acesso
  iniciaMemLog(lognome);

  // ativar registro de acesso
  ativaMemLog();

  // execucao dependente da operacao escolhida
  switch (opescolhida){
    case OPSOMAR:
      // le as matrizes a e b de seus arquivos, que sao somadas para a matriz c
      // matriz c é guardada no arquivo de saida e todas as matrizes sao destruidas
      leMatriz(opt1nome, &a);
      leMatriz(opt2nome, &b);
      somaMatrizes(&a, &b, &c);
      imprimeMatriz(&c);
      destroiMatriz(&a);
      destroiMatriz(&b);
      destroiMatriz(&c);
      break;
    case OPMULTIPLICAR:
      // le as matrizes a e b de seus arquivos, que sao multiplicadas para a matriz c
      // matriz c é guardada no arquivo de saida e todas as matrizes sao destruidas
      leMatriz(opt1nome, &a);
      leMatriz(opt2nome, &b);
      multiplicaMatrizes(&a,&b,&c);
  	  imprimeMatriz(&c);
      destroiMatriz(&a);
      destroiMatriz(&b);
      destroiMatriz(&c);
	    break;
    case OPTRANSPOR:
      // le uma matriz que e transposta, impressa e destruida
      leMatriz(opt1nome, &a);
      transpoeMatriz(&a);
      imprimeMatriz(&a);
      destroiMatriz(&a);
	  break;
   default:
      // nao deve ser executado, pois ha um erroAssert em parse_args
      uso();
      exit(1);
  }
  // conclui registro de acesso
  return finalizaMemLog();
}
