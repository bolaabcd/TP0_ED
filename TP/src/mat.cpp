//---------------------------------------------------------------------
// Arquivo      : mat.cpp
// Conteudo     : implementacao do TAD MAT
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2021-10-18 - arquivo criado
//		          : 2021-10-21 - estrutura de diretorios
//              : 2021-10-27 - (Artur Gaspar) alterado para especificacao do trabalho
//---------------------------------------------------------------------

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mat.h"
#include "memlog.h"
#include "msgassert.h"

// Macro que realiza swap sem variavel auxiliar
#define ELEMSWAP(x, y) (x += y, y = x - y, x -= y)

void criaMatriz(mat_tipo *mat, int tx, int ty)
// Descricao: cria matriz com dimensoes tx X ty
// Entrada: mat, tx, ty
// Saida: mat
{
  // verifica se os valores de tx, ty e mat sao validos
  erroAssert(tx > 0, "Dimensao nula.");
  erroAssert(ty > 0, "Dimensao nula.");
  erroAssert(mat->m == nullptr, "Matriz ja foi criada.");

  // inicializa as dimensoes da matriz
  mat->tamx = tx;
  mat->tamy = ty;

  // separa um espaco pra matriz no heap
  mat->m = (double **)malloc(tx * (sizeof(double *)));
  escreveMemLog((long int)(mat->m), sizeof(double **));
  for (int i = 0; i < tx; i++)
  {
    mat->m[i] = (double *)malloc(ty * (sizeof(double)));
    escreveMemLog((long int)(&(mat->m[i])), sizeof(double *));
  }
}

void inicializaMatrizNula(mat_tipo *mat)
// Descricao: inicializa mat com valores nulos
// Entrada: mat
// Saida: mat
{
  // verifica se a matriz e valida
  erroAssert(mat->m != nullptr, "Matriz nao foi criada");
  // inicializa todos os elementos da matriz com 0, por seguranca
  for (int i = 0; i < mat->tamx; i++)
    for (int j = 0; j < mat->tamy; j++)
      escreveElemento(mat, i, j, 0);
}

void leMatriz(std::string arq_nome, mat_tipo *mat)
// Descricao: le a matriz de arq_nome e salva em mat.
// Entrada: arq_nome e mat
// Saida: mat
{
  // verifica se a matriz e o arquivo sao validos
  erroAssert(mat->m == nullptr, "Matriz ja foi criada.");
  std::ifstream arq(arq_nome);
  erroAssert(arq, "Arquivo de matriz inexistente ou invalido.");

  // variavel auxiliar
  char aux;

  // preparacao para ler linha a linha
  std::string str;
  getline(arq, str);
  std::istringstream istr(str);

  // lendo e validando tamanho da matriz
  int tx, ty;
  istr >> tx >> ty;
  erroAssert(istr.good() || istr.eof(), "Os dois primeiros valores do arquivo devem ser o tamanho da matriz.");
  erroAssert(tx >= 0 && ty >= 0, "Tamanho de matriz invalido.");

  // validando linha: nao pode ter nada alem desses dois numeros
  istr >> aux;
  erroAssert(istr.eof(), "Arquivo de matriz nao segue a especificacao.");
  criaMatriz(mat, tx, ty);

  for (int i = 0; i < tx; i++)
  {
    // pegando proxima linha e colocando na stringstream
    getline(arq, str);
    istr.str(str);
    istr.clear();

    for (int j = 0; j < ty; j++)
    {
      double v;
      // validando a leitura do numero real
      erroAssert(istr >> v && (istr.good() || istr.eof()), "Arquivo de matriz nao segue a especificacao.");
      escreveElemento(mat, i, j, v);
    }

    // validando final da linha: nao pode ter nada alem dos ty numeros
    istr >> aux;
    erroAssert(istr.eof(), "Arquivo de matriz nao segue a especificacao.");
  }

  // validando final do arquivo: nao pode ter nada alem da matriz e suas dimensoes
  erroAssert(!(arq >> aux), "Arquivo de matriz nao segue a especificacao.");
  arq.close();
}

void imprimeMatriz(std::string arq_nome, mat_tipo *mat)
// Descricao: imprime a matriz com a identificacao de linhas e colunas
// Entrada: mat
// Saida: impressao na saida padrao (stdout)
{
  int i, j;
  FILE *arq = fopen(arq_nome.c_str(), "w");

  // verifica se a matriz e o arquivo sao validos
  erroAssert(mat->m != nullptr, "Matriz nao foi criada.");
  erroAssert(arq, "Arquivo de escrita de matriz inexistente ou invalido.");

  // imprime o tamanho da matriz
  fprintf(arq, "%d %d\n", mat->tamx, mat->tamy);

  // imprime as linhas
  for (i = 0; i < mat->tamx; i++)
  {
    for (j = 0; j < mat->tamy; j++)
    {
      fprintf(arq, "%.4f ", mat->m[i][j]);
      leMemLog((long int)(&(mat->m[i][j])), sizeof(double));
    }
    fprintf(arq, "\n");
  }
  fclose(arq);
}

void escreveElemento(mat_tipo *mat, int x, int y, double v)
// Descricao: atribui o valor v ao elemento (x,y) de mat
// Entrada: mat, x, y, v
// Saida: mat
{
  // verifica se x, y e mat sao validos
  erroAssert(mat->m != nullptr, "Matriz nao foi criada.");
  erroAssert((x >= 0) && (x < mat->tamx), "Indice invalido");
  erroAssert((y >= 0) && (y < mat->tamy), "Indice invalido");

  mat->m[x][y] = v;
  escreveMemLog((long int)(&(mat->m[x][y])), sizeof(double));
}

double leElemento(mat_tipo *mat, int x, int y)
// Descricao: retorna o elemento (x,y) de mat
// Entrada: mat, x, y
// Saida: mat[x][y]
{
  // verifica se x, y e mat sao validos
  erroAssert(mat->m != nullptr, "Matriz nao foi criada.");
  erroAssert((x >= 0) && (x < mat->tamx), "Indice invalido");
  erroAssert((y >= 0) && (y < mat->tamy), "Indice invalido");

  leMemLog((long int)(&(mat->m[x][y])), sizeof(double));
  return mat->m[x][y];
}

void somaMatrizes(mat_tipo *a, mat_tipo *b, mat_tipo *c)
// Descricao: soma as matrizes a e b e armazena o resultado em c
// Entrada: a, b
// Saida: c
{
  // verifica se as matrizes sao validas
  erroAssert(a->m != nullptr, "Matriz nao foi criada.");
  erroAssert(b->m != nullptr, "Matriz nao foi criada.");
  erroAssert(c->m == nullptr, "Matriz ja foi criada.");

  // verifica se as dimensoes das matrizes a e b sao as mesmas
  erroAssert(a->tamx == b->tamx, "Dimensoes incompativeis");
  erroAssert(a->tamy == b->tamy, "Dimensoes incompativeis");

  // inicializa a matriz c garantindo a compatibilidade das dimensoes
  criaMatriz(c, a->tamx, a->tamy);
  inicializaMatrizNula(c);

  // faz a soma elemento a elemento
  for (int i = 0; i < a->tamx; i++)
  {
    for (int j = 0; j < a->tamy; j++)
    {
      c->m[i][j] = a->m[i][j] + b->m[i][j];
      leMemLog((long int)(&(a->m[i][j])), sizeof(double));
      leMemLog((long int)(&(b->m[i][j])), sizeof(double));
      escreveMemLog((long int)(&(c->m[i][j])), sizeof(double));
    }
  }
}

void multiplicaMatrizes(mat_tipo *a, mat_tipo *b, mat_tipo *c)
// Descricao: multiplica as matrizes a e b e armazena o resultado em c
// Entrada: a,b
// Saida: c
{
  // verifica se as matrizes sao validas
  erroAssert(a->m != nullptr, "Matriz nao foi criada.");
  erroAssert(b->m != nullptr, "Matriz nao foi criada.");
  erroAssert(c->m == nullptr, "Matriz nao foi criada.");

  // verifica a compatibilidade das dimensoes
  erroAssert(a->tamy == b->tamx, "Dimensoes incompativeis");

  // cria e inicializa a matriz c
  criaMatriz(c, a->tamx, b->tamy);
  inicializaMatrizNula(c);

  // realiza a multiplicacao de matrizes
  for (int i = 0; i < c->tamx; i++)
  {
    for (int j = 0; j < c->tamy; j++)
    {
      for (int k = 0; k < a->tamy; k++)
      {
        c->m[i][j] += a->m[i][k] * b->m[k][j];
        leMemLog((long int)(&(a->m[i][k])), sizeof(double));
        leMemLog((long int)(&(b->m[k][j])), sizeof(double));
        escreveMemLog((long int)(&(c->m[i][j])), sizeof(double));
      }
    }
  }
}

void transpoeMatriz(mat_tipo *a)
// Descricao: transpoe a matriz a
// Entrada: a
// Saida: a
{
  // verifica se a matriz e valida
  erroAssert(a->m != nullptr, "Matriz nao foi criada.");

  // onde ficara a versao transposta
  mat_tipo b;
  criaMatriz(&b, a->tamy, a->tamx);

  // faz a transposicao salvando na matriz b
  for (int i = 0; i < a->tamx; i++)
  {
    for (int j = 0; j < a->tamy; j++)
    {
      b.m[j][i] = a->m[i][j];
      escreveMemLog((long int)(&(a->m[i][j])), sizeof(double));
      escreveMemLog((long int)(&(b.m[j][i])), sizeof(double));
    }
  }
  destroiMatriz(a);
  *a = b;
}

void destroiMatriz(mat_tipo *a)
// Descricao: destroi a matriz a, que se torna inacessível
// Entrada: a
// Saida: a
{
  // apenas um aviso se a matriz for destruida mais de uma vez
  avisoAssert(a->m != nullptr, "Matriz ja foi destruida ou nem foi construida.");

  // desaloca o espaco do heap
  for (int i = 0; i < a->tamx; i++)
    free(a->m[i]);

  free(a->m);

  // torna o ponteiro inicial invalido
  a->m = nullptr;
}
