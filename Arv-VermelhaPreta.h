#ifndef FABRICA_H
#define FABRICA_H

#define RED 1
#define BLACK 0

#define QTD_TIPOS    5
#define QTD_CABELOS  3
#define QTD_SITACOES 4
#define QTD_TAMANHOS 3
#define QTD_FAIXAS   3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nobase{
    struct Nobase *esq;
    struct Nobase *dir;
    int cor;
}Nobase;

typedef struct FaixaPreco{
    int qtdMIN;
    float preco;
}FaixaPreco;

typedef struct Produto{
    Nobase base;
    int codigo;
    int tipo;
    int cabelo;
    int situacao;
    int tamanho;
    int estoque;
    FaixaPreco faixas[QTD_FAIXAS];
}Produto;

typedef struct Item{
    int codProduto;
    int quantidade;
    float ValorPago;
    struct Item *prox;
}Item;

typedef struct Compra{
    int codigo;
    Item *itens;
    struct Compra *prox;
}Compra;

typedef struct NoData{
    Nobase base;
    int data;
    Compra *compras;
}NoData;

typedef struct Cliente{
    Nobase base;
    char cnpj[19];
    char nome[50];
    char endereco[80];
    char contato[40];
    NoData *raizCompras;
}Cliente;

int cor(Nobase *raiz);

void trocaCor(Nobase *raiz);

Nobase* rotacionaEsq(Nobase *raiz);

Nobase* rotacionaDir(Nobase *raiz);

Nobase* moverDirRED(Nobase *raiz);

Nobase* moverEsqRED(Nobase *raiz);

Nobase*  balancear(Nobase *raiz);

Nobase* procuraMenor(Nobase *raiz);

Nobase* removerMenor(Nobase *raiz);

#endif