#include "Arv-VermelhaPreta.h"

int cor(Nobase *raiz){
    if (raiz == NULL)
        return BLACK;
    return raiz->cor;
}

void trocaCor(Nobase *raiz){
    raiz->cor = !raiz->cor;
    if (raiz->esq != NULL)
        raiz->esq->cor = !raiz->esq->cor;
    if (raiz->dir != NULL)
        raiz->dir->cor = !raiz->dir->cor;
}

Nobase* rotacionaEsq(Nobase *raiz){
    Nobase *aux = raiz->dir;
    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = RED;
    return aux;
}

Nobase* rotacionaDir(Nobase *raiz){
    Nobase *aux = raiz->esq;
    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = RED;
    return aux;
}

Nobase* moverDirRED(Nobase *raiz){
    trocaCor(raiz);

    if (raiz->esq != NULL && cor(raiz->esq->esq) == RED){
        raiz = rotacionaDir(raiz);
        trocaCor(raiz);
    }
    return raiz;
}

Nobase* moverEsqRED(Nobase *raiz){
    trocaCor(raiz);

    if (raiz->dir != NULL && cor(raiz->dir->esq) == RED){
        raiz->dir = rotacionaDir(raiz->dir);
        raiz = rotacionaEsq(raiz);
        trocaCor(raiz);
    }
    return raiz;
}

Nobase* balancear(Nobase *raiz){
    if (cor(raiz->dir) == RED)
        raiz = rotacionaEsq(raiz);

    if (cor(raiz->esq) == RED && cor(raiz->esq->esq) == RED)
        raiz = rotacionaDir(raiz);

    if (cor(raiz->esq) == RED && cor(raiz->dir) == RED)
        trocaCor(raiz);

    return raiz;
}

Nobase* procuraMenor(Nobase *raiz){
    Nobase *atual = raiz;

    while(atual != NULL && atual->esq != NULL){
        atual = atual->esq;
    }
    return atual;
}

Nobase* removerMenor(Nobase *raiz){
    if (raiz == NULL)
        return NULL;

    if (raiz->esq == NULL)
    {
        free(raiz);
        return NULL;
    }

    if (cor(raiz->esq) == BLACK && cor(raiz->esq->esq) == BLACK)
        raiz = moverEsqRED(raiz);

    raiz->esq = removerMenor(raiz->esq);
    return balancear(raiz);
}

Nobase* corrigirInsercao(Nobase *raiz){
    if (cor(raiz->dir) == RED && cor(raiz->esq) == BLACK)
        raiz = rotacionaEsq(raiz);

    if (cor(raiz->esq) == RED && cor(raiz->esq->esq) == RED)
        raiz = rotacionaDir(raiz);

    if (cor(raiz->esq) == RED && cor(raiz->dir) == RED)
        trocaCor(raiz);

    return raiz;
}

Produto* criarProduto(int codigo, int tipo, int cabelo, int situacao,int tamanho, int estoque, FaixaPreco faixas[]){
    int i;
    Produto *novo = (Produto*)malloc(sizeof(Produto));
    if (novo != NULL)
    {
        novo->codigo = codigo;
        novo->tipo = tipo;
        novo->cabelo = cabelo;
        novo->situacao = situacao;
        novo->tamanho = tamanho;
        novo->estoque = estoque;

        for (i = 0; i < QTD_FAIXAS; i++)
            novo->faixas[i] = faixas[i];

        novo->base.cor = RED;
        novo->base.esq = NULL;
        novo->base.dir = NULL;
    }
    return novo;
}

Produto* inserirProduto(Produto *raiz, int codigo, int tipo, int cabelo,int situacao, int tamanho, int estoque,FaixaPreco faixas[], int *resp){
    if (raiz == NULL)
    {
        raiz = criarProduto(codigo, tipo, cabelo, situacao, tamanho, estoque, faixas);
        if (raiz != NULL)
            *resp = SUCESSO;
        else
            *resp = ERRO_MEMORIA;
    }else{
        if (codigo < raiz->codigo){
            raiz->base.esq = (Nobase*)inserirProduto((Produto*)raiz->base.esq, codigo, tipo, cabelo, situacao, tamanho, estoque, faixas, resp);
        }else if (codigo > raiz->codigo){
            raiz->base.dir = (Nobase*)inserirProduto((Produto*)raiz->base.dir, codigo, tipo, cabelo, situacao, tamanho, estoque, faixas, resp);
        }else{
            *resp = ERRO_REPETIDO;
        }
        if (*resp == SUCESSO)
            raiz = (Produto*)corrigirInsercao((Nobase*)raiz);
    }
    return raiz;
}

int add_ArvProduto(Produto **raiz, int codigo, int tipo, int cabelo,int situacao, int tamanho, int estoque,FaixaPreco faixas[]){
    int resp = SUCESSO;

    *raiz = inserirProduto(*raiz, codigo, tipo, cabelo, situacao, tamanho, estoque, faixas, &resp);
    if (*raiz != NULL)
        (*raiz)->base.cor = BLACK;
    return resp;
}

Produto* buscarProduto(Produto *raiz, int codigo){
    Produto *resultado = NULL;

    if (raiz != NULL){
        if (codigo == raiz->codigo)
            resultado = raiz;
        else if (codigo < raiz->codigo)
            resultado = buscarProduto((Produto*) raiz->base.esq, codigo);
        else
            resultado = buscarProduto((Produto*) raiz->base.dir, codigo);
    }
    return resultado;
}

Produto* removerProduto(Produto *raiz, int codigo){
    if (raiz == NULL)
        return NULL;

    if (codigo < raiz->codigo) {
        if (cor(raiz->base.esq) == BLACK && cor(raiz->base.esq->esq) == BLACK)
            raiz = (Produto*) moverEsqRED((Nobase*) raiz);

        raiz->base.esq = (Nobase*) removerProduto((Produto*) raiz->base.esq, codigo);
    } else {
        if (cor(raiz->base.esq) == RED)
            raiz = (Produto*) rotacionaDir((Nobase*) raiz);

        if (codigo == raiz->codigo && raiz->base.dir == NULL) {
            free(raiz);
            return NULL;
        }

        if (cor(raiz->base.dir) == BLACK && cor(raiz->base.dir->esq) == BLACK)
            raiz = (Produto*) moverDirRED((Nobase*) raiz);

        if (codigo == raiz->codigo) {
            Produto *aux = (Produto*) procuraMenor(raiz->base.dir);
            int i;

            raiz->codigo   = aux->codigo;
            raiz->tipo     = aux->tipo;
            raiz->cabelo   = aux->cabelo;
            raiz->situacao = aux->situacao;
            raiz->tamanho  = aux->tamanho;
            raiz->estoque  = aux->estoque;
            for (i = 0; i < QTD_FAIXAS; i++)
                raiz->faixas[i] = aux->faixas[i];

            raiz->base.dir = removerMenor(raiz->base.dir);
        } else {
            raiz->base.dir = (Nobase*) removerProduto((Produto*) raiz->base.dir, codigo);
        }
    }

    if (raiz != NULL)
        raiz = (Produto*) balancear((Nobase*) raiz);
    return raiz;
}

int remove_produto_arv(Produto **raiz, int codigo){
    int status = SUCESSO;

    if (buscarProduto(*raiz, codigo) == NULL){
        status = ERRO_INEXISTENTE;
    }else{
        *raiz = removerProduto(*raiz, codigo);
        if (*raiz != NULL)
            (*raiz)->base.cor = BLACK;
    }
    return status;
}

void imprimir_produto(Produto *p){
    int i;

    printf("-----------------------------\n");
    printf("Codigo   : %d\n", p->codigo);
    printf("Tipo     : %d\n", p->tipo);
    printf("Cabelo   : %d\n", p->cabelo);
    printf("Situacao : %d\n", p->situacao);
    printf("Tamanho  : %d\n", p->tamanho);
    printf("Estoque  : %d\n", p->estoque);
    for (i = 0; i < QTD_FAIXAS; i++)
        printf("Faixa %d: a partir de %d un. -> R$ %.2f\n", i + 1, p->faixas[i].qtdMIN, p->faixas[i].preco);
    printf("-----------------------------\n");
}

void imprimir_produtos(Produto *raiz){
    if (raiz != NULL){
        imprimir_produtos((Produto*) raiz->base.esq);
        imprimir_produto(raiz);
        imprimir_produtos((Produto*) raiz->base.dir);
    }
}

void liberar_produtos(Produto *raiz) {
    if (raiz != NULL) {
        liberar_produtos((Produto*) raiz->base.esq);
        liberar_produtos((Produto*) raiz->base.dir);
        free(raiz);
    }
}

float preco_unitario(Produto *p, int quantidade) {
    float preco = 0;
    int melhor = -1, i;

    for (i = 0; i < QTD_FAIXAS; i++) {
        if (quantidade >= p->faixas[i].qtdMIN && p->faixas[i].qtdMIN >= melhor) {
            melhor = p->faixas[i].qtdMIN;
            preco = p->faixas[i].preco;
        }
    }
    return preco;
}

int tem_estoque(Produto *p, int quantidade) {
    int ok = 0;

    if (quantidade > 0 && quantidade <= p->estoque)
        ok = 1;
    return ok;
}

void baixar_estoque(Produto *p, int quantidade) {
    p->estoque = p->estoque - quantidade;
}

Item* criarItem(int codProduto, int quantidade, float valor) {
    Item *novo = (Item*) malloc(sizeof(Item));

    if (novo != NULL) {
        novo->codProduto = codProduto;
        novo->quantidade = quantidade;
        novo->ValorPago  = valor;
        novo->prox       = NULL;
    }
    return novo;
}

int adicionar_item(Compra *compra, int codProduto, int quantidade, float valor) {
    int status = SUCESSO;
    Item *novo, *aux;

    if (compra == NULL) {
        status = ERRO_INEXISTENTE;
    } else {
        novo = criarItem(codProduto, quantidade, valor);
        if (novo == NULL) {
            status = ERRO_MEMORIA;
        } else if (compra->itens == NULL) {
            compra->itens = novo;
        } else {
            aux = compra->itens;
            while (aux->prox != NULL)
                aux = aux->prox;
            aux->prox = novo;
        }
    }
    return status;
}

void imprimir_item(Item *i) {
    printf("    Produto %d | qtd %d | pago R$ %.2f\n",
           i->codProduto, i->quantidade, i->ValorPago);
}

void liberar_itens(Item *i){
    Item *prox;
    while (i != NULL)
    {
        prox = i->prox;
        free(i);
        i = prox;
    }
}

Compra* criarCompra(int codCompra) {
    Compra *nova = (Compra*) malloc(sizeof(Compra));

    if (nova != NULL) {
        nova->codigo = codCompra;
        nova->itens  = NULL;
        nova->prox   = NULL;
    }
    return nova;
}

Compra* buscarCompra(NoData *d, int codCompra) {
    Compra *aux = NULL;

    if (d != NULL) {
        aux = d->compras;
        while (aux != NULL && aux->codigo != codCompra)
            aux = aux->prox;
    }
    return aux;
}

int adicionar_compra(NoData *d, int codCompra) {
    int status = SUCESSO;
    Compra *nova;

    if (d == NULL) {
        status = ERRO_INEXISTENTE;
    } else if (buscarCompra(d, codCompra) != NULL) {
        status = ERRO_REPETIDO;
    } else {
        nova = criarCompra(codCompra);
        if (nova == NULL) {
            status = ERRO_MEMORIA;
        } else {
            nova->prox = d->compras;
            d->compras = nova;
        }
    }
    return status;
}

int remover_compra(NoData *d, int codCompra) {
    int status = SUCESSO;
    Compra *atual = NULL, *anterior = NULL;

    if (d != NULL) {
        atual = d->compras;
        while (atual != NULL && atual->codigo != codCompra) {
            anterior = atual;
            atual = atual->prox;
        }
    }

    if (atual == NULL) {
        status = ERRO_INEXISTENTE;
    } else {
        if (anterior == NULL)
            d->compras = atual->prox;
        else
            anterior->prox = atual->prox;

        liberar_itens(atual->itens);
        free(atual);
    }
    return status;
}

void imprimir_compra(Compra *c) {
    Item *i;

    printf("  Compra %d\n", c->codigo);
    for (i = c->itens; i != NULL; i = i->prox)
        imprimir_item(i);
}

void liberar_compras(Compra *c) {
    Compra *prox;

    while (c != NULL) {
        prox = c->prox;
        liberar_itens(c->itens);
        free(c);
        c = prox;
    }
}

NoData* criarData(int data){
    NoData *novo = (NoData*)malloc(sizeof(NoData));
    if (novo != NULL){
        novo->data = data;
        novo->compras = NULL;
        novo->base.cor = RED;
        novo->base.esq = NULL;
        novo->base.dir = NULL;
    }
    return novo;
}

NoData* inserirData(NoData *raiz, int data, int *resp){
    if (raiz == NULL) {
        raiz = criarData(data);
        if (raiz != NULL)
            *resp = SUCESSO;
        else
            *resp = ERRO_MEMORIA;
    } else {
        if (data < raiz->data)
            raiz->base.esq = (Nobase*) inserirData((NoData*) raiz->base.esq, data, resp);
        else if (data > raiz->data)
            raiz->base.dir = (Nobase*) inserirData((NoData*) raiz->base.dir, data, resp);
        else
            *resp = ERRO_REPETIDO;

        if (*resp == SUCESSO)
            raiz = (NoData*) corrigirInsercao((Nobase*) raiz);
    }
    return raiz;
}

int add_ArvData(NoData **raiz, int data) {
    int resp = SUCESSO;

    *raiz = inserirData(*raiz, data, &resp);
    if (*raiz != NULL)
        (*raiz)->base.cor = BLACK;
    return resp;
}

NoData* buscarData(NoData *raiz, int data) {
    NoData *result = NULL;

    if (raiz != NULL) {
        if (data == raiz->data)
            result = raiz;
        else if (data < raiz->data)
            result = buscarData((NoData*) raiz->base.esq, data);
        else
            result = buscarData((NoData*) raiz->base.dir, data);
    }
    return result;
}

NoData* removerData(NoData *raiz, int data) {
    if (raiz == NULL)
        return NULL;

    if (data < raiz->data) {
        if (cor(raiz->base.esq) == BLACK && cor(raiz->base.esq->esq) == BLACK)
            raiz = (NoData*) moverEsqRED((Nobase*) raiz);

        raiz->base.esq = (Nobase*) removerData((NoData*) raiz->base.esq, data);
    } else {
        if (cor(raiz->base.esq) == RED)
            raiz = (NoData*) rotacionaDir((Nobase*) raiz);

        if (data == raiz->data && raiz->base.dir == NULL) {
            free(raiz);
            return NULL;
        }

        if (cor(raiz->base.dir) == BLACK && cor(raiz->base.dir->esq) == BLACK)
            raiz = (NoData*) moverDirRED((Nobase*) raiz);

        if (data == raiz->data) {
            NoData *aux = (NoData*) procuraMenor(raiz->base.dir);

            raiz->data    = aux->data;
            raiz->compras = aux->compras;

            raiz->base.dir = removerMenor(raiz->base.dir);
        } else {
            raiz->base.dir = (Nobase*) removerData((NoData*) raiz->base.dir, data);
        }
    }

    if (raiz != NULL)
        raiz = (NoData*) balancear((Nobase*) raiz);
    return raiz;
}

int remove_data_arv(NoData **raiz, int data) {
    int status = SUCESSO;
    NoData *no = buscarData(*raiz, data);

    if (no == NULL) {
        status = ERRO_INEXISTENTE;
    } else if (no->compras != NULL) {
        status = ERRO_COMPRAS;
    } else {
        *raiz = removerData(*raiz, data);
        if (*raiz != NULL)
            (*raiz)->base.cor = BLACK;
    }
    return status;
}

void imprimir_data(NoData *d) {
    Compra *c;

    printf("Data: %d\n", d->data);
    for (c = d->compras; c != NULL; c = c->prox)
        imprimir_compra(c);
}

void imprimir_datas(NoData *raiz) {
    if (raiz != NULL) {
        imprimir_datas((NoData*) raiz->base.esq);
        imprimir_data(raiz);
        imprimir_datas((NoData*) raiz->base.dir);
    }
}

void liberar_datas(NoData *raiz) {
    if (raiz != NULL) {
        liberar_datas((NoData*) raiz->base.esq);
        liberar_datas((NoData*) raiz->base.dir);
        liberar_compras(raiz->compras);
        free(raiz);
    }
}

Cliente* criarCliente(char cnpj[], char nome[], char endereco[], char contato[]) {
    Cliente *novo = (Cliente*) malloc(sizeof(Cliente));

    if (novo != NULL) {
        strcpy(novo->cnpj, cnpj);
        strcpy(novo->nome, nome);
        strcpy(novo->endereco, endereco);
        strcpy(novo->contato, contato);
        novo->raizCompras = NULL;
        novo->base.cor = RED;
        novo->base.esq = NULL;
        novo->base.dir = NULL;
    }
    return novo;
}

Cliente* inserirCliente(Cliente *raiz, char cnpj[], char nome[],char endereco[], char contato[], int *resp){
    int cmp;

    if (raiz == NULL) {
        raiz = criarCliente(cnpj, nome, endereco, contato);
        if (raiz != NULL)
            *resp = SUCESSO;
        else
            *resp = ERRO_MEMORIA;
    } else {
        cmp = strcmp(cnpj, raiz->cnpj);

        if (cmp < 0)
            raiz->base.esq = (Nobase*) inserirCliente((Cliente*) raiz->base.esq,
                                   cnpj, nome, endereco, contato, resp);
        else if (cmp > 0)
            raiz->base.dir = (Nobase*) inserirCliente((Cliente*) raiz->base.dir,
                                   cnpj, nome, endereco, contato, resp);
        else
            *resp = ERRO_REPETIDO;

        if (*resp == SUCESSO)
            raiz = (Cliente*) corrigirInsercao((Nobase*) raiz);
    }
    return raiz;
}

int add_ArvCliente(Cliente **raiz, char cnpj[], char nome[], char endereco[], char contato[]) {
    int resp = SUCESSO;

    *raiz = inserirCliente(*raiz, cnpj, nome, endereco, contato, &resp);
    if (*raiz != NULL)
        (*raiz)->base.cor = BLACK;
    return resp;
}

Cliente* buscarCliente(Cliente *raiz, char cnpj[]) {
    Cliente *result = NULL;
    int cmp;

    if (raiz != NULL) {
        cmp = strcmp(cnpj, raiz->cnpj);
        if (cmp == 0)
            result = raiz;
        else if (cmp < 0)
            result = buscarCliente((Cliente*) raiz->base.esq, cnpj);
        else
            result = buscarCliente((Cliente*) raiz->base.dir, cnpj);
    }
    return result;
}

Cliente* removerCliente(Cliente *raiz, char cnpj[]){
    if (raiz == NULL)
        return NULL;

    if (strcmp(cnpj, raiz->cnpj) < 0) {
        if (cor(raiz->base.esq) == BLACK && cor(raiz->base.esq->esq) == BLACK)
            raiz = (Cliente*) moverEsqRED((Nobase*) raiz);

        raiz->base.esq = (Nobase*) removerCliente((Cliente*) raiz->base.esq, cnpj);
    } else {
        if (cor(raiz->base.esq) == RED)
            raiz = (Cliente*) rotacionaDir((Nobase*) raiz);

        if (strcmp(cnpj, raiz->cnpj) == 0 && raiz->base.dir == NULL) {
            free(raiz);
            return NULL;
        }

        if (cor(raiz->base.dir) == BLACK && cor(raiz->base.dir->esq) == BLACK)
            raiz = (Cliente*) moverDirRED((Nobase*) raiz);

        if (strcmp(cnpj, raiz->cnpj) == 0) {
            Cliente *aux = (Cliente*) procuraMenor(raiz->base.dir);

            strcpy(raiz->cnpj, aux->cnpj);
            strcpy(raiz->nome, aux->nome);
            strcpy(raiz->endereco, aux->endereco);
            strcpy(raiz->contato, aux->contato);
            raiz->raizCompras = aux->raizCompras;

            raiz->base.dir = removerMenor(raiz->base.dir);
        } else {
            raiz->base.dir = (Nobase*) removerCliente((Cliente*) raiz->base.dir, cnpj);
        }
    }

    if (raiz != NULL)
        raiz = (Cliente*) balancear((Nobase*) raiz);
    return raiz;
}

int remove_cliente_arv(Cliente **raiz, char cnpj[]) {
    int status = SUCESSO;
    Cliente *cli = buscarCliente(*raiz, cnpj);

    if (cli == NULL) {
        status = ERRO_INEXISTENTE;
    } else if (cli->raizCompras != NULL) {
        status = ERRO_COMPRAS;
    } else {
        *raiz = removerCliente(*raiz, cnpj);
        if (*raiz != NULL)
            (*raiz)->base.cor = BLACK;
    }
    return status;
}

void imprimir_cliente(Cliente *c) {
    printf("------------------------------\n");
    printf("CNPJ     : %s\n", c->cnpj);
    printf("Nome     : %s\n", c->nome);
    printf("Endereco : %s\n", c->endereco);
    printf("Contato  : %s\n", c->contato);
    printf("------------------------------\n");
}

void imprimir_clientes(Cliente *raiz) {
    if (raiz != NULL) {
        imprimir_clientes((Cliente*) raiz->base.esq);
        imprimir_cliente(raiz);
        imprimir_clientes((Cliente*) raiz->base.dir);
    }
}

void liberar_clientes(Cliente *raiz) {
    if (raiz != NULL) {
        liberar_clientes((Cliente*) raiz->base.esq);
        liberar_clientes((Cliente*) raiz->base.dir);
        liberar_datas(raiz->raizCompras);
        free(raiz);
    }
}