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