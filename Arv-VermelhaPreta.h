#ifndef FABRICA_H
#define FABRICA_H

#define RED 1
#define BLACK 0

#define QTD_TIPOS    5
#define QTD_CABELOS  3
#define QTD_SITUACOES 4
#define QTD_TAMANHOS 3
#define QTD_FAIXAS   3

#define SUCESSO            1
#define ERRO_MEMORIA       0
#define ERRO_REPETIDO      5
#define ERRO_INEXISTENTE   6
#define ERRO_ESTOQUE       7
#define ERRO_COMPRAS      8

#define QTD_CNPJ 100
#define QTD_BUSCA 10
#define QTD_CASOS 4

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

Nobase* corrigirInsercao(Nobase *raiz);

void liberar_itens(Item *i);

void liberar_compras(Compra *c);

int adicionar_item(Compra *compra, int codProduto, int quantidade, float valor);

Produto* criarProduto(int codigo, int tipo, int cabelo, int situacao,int tamanho, int estoque, FaixaPreco faixas[]);

Produto* inserirProduto(Produto *raiz, int codigo, int tipo, int cabelo,int situacao, int tamanho, int estoque,FaixaPreco faixas[], int *resp);

int add_ArvProduto(Produto **raiz, int codigo, int tipo, int cabelo,int situacao, int tamanho, int estoque,FaixaPreco faixas[]);

Produto* buscarProduto(Produto *raiz, int codigo);

Produto* removerProduto(Produto *raiz, int codigo);

int remove_produto_arv(Produto **raiz, int codigo);

void imprimir_produto(Produto *p);

void imprimir_produtos(Produto *raiz);

void liberar_produtos(Produto *raiz);

NoData* criarData(int data);

NoData* inserirData(NoData *raiz, int data, int *resp);

int add_ArvData(NoData **raiz, int data);

NoData* buscarData(NoData *raiz, int data);

NoData* removerData(NoData *raiz, int data);

int remove_data_arv(NoData **raiz, int data);

void imprimir_item(Item *i);

void imprimir_compra(Compra *c);

void imprimir_data(NoData *d);

void imprimir_datas(NoData *d);

void liberar_datas(NoData *raiz);

float preco_unitario(Produto *p, int quantidade);

int tem_estoque(Produto *p, int quantidade);

void baixar_estoque(Produto *p, int quantidade);

Item* criarItem(int codProduto, int quantidade, float valor);

Compra* criarCompra(int codCompra);

Compra* buscarCompra(NoData *d, int codCompra);

int adicionar_compra(NoData *d, int codCompra);

int remover_compra(NoData *d, int codCompra, Produto *produtos);

Cliente* criarCliente(char cnpj[], char nome[], char endereco[], char contato[]);

Cliente* inserirCliente(Cliente *raiz, char cnpj[], char nome[], char endereco[], char contato[], int *resp);

int add_ArvCliente(Cliente **raiz, char cnpj[], char nome[],char endereco[], char contato[]);

Cliente* buscarCliente(Cliente *raiz, char cnpj[]);

Cliente* removerCliente(Cliente *raiz, char cnpj[]);

int remove_cliente_arv(Cliente **raiz, char cnpj[]);

void imprimir_cliente(Cliente *c);

void imprimir_clientes(Cliente *raiz);

void liberar_clientes(Cliente *raiz);

int ler_opcao(char *rotulo, int max);


//funções do experimento

int calcular_digito(int digitos[], int quantidade);

void formatar_cnpj(int raiz, char cnpj[]);

int gerar_raiz();

int raiz_existe(int raizes[], int qtd, int valor);

void ordenar_raizes(int raizes[]);

void gerar_cnpjs(char cnpjs[][19], int raizes[]);

void gerar_cnpj_inexistente(int raizes[], char cnpj[]);

void embaralhar(int vetor[], int qtd);

void escolher_buscas(int escolhidos[]);

void ordem_crescente(int ordem[]);

void ordem_decrescente(int ordem[]);

void ordem_aleatoria(int ordem[]);

void ordem_meio_primeiro(int ordem[]);

void montar_ordens(int ordens[][QTD_CNPJ]);

int inserir_clientes(Cliente **raiz, char cnpjs[][19], int ordem[]);

Cliente *buscar_com_passos(Cliente *raiz, char cnpj[], int *passos);

int contar_passos(Cliente *raiz, char cnpj[], int *encontrou);
int altura(Nobase *raiz);

float calcular_media(int valores[], int qtd);

int calcular_maximo(int valores[], int qtd);

void executar_caso(int caso, char cnpjs[][19], int ordem[],int escolhidos[], char inexistentes[], int passos[],
int *passosInex, int *alturaArvore);

void imprimir_resultados(char cnpjs[][19], int escolhidos[],int passos[][QTD_BUSCA], char inexistente[],
int passosInex[], int alturas[]);

void menu_experimento();

#endif