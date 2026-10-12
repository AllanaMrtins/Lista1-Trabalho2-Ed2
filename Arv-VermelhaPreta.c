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

    if (raiz->esq != NULL && raiz->esq->esq != NULL && cor(raiz->esq->esq) == RED){
        raiz = rotacionaDir(raiz);
        trocaCor(raiz);
    }
    return raiz;    
}

Nobase* moverEsqRED(Nobase *raiz){
    trocaCor(raiz);

    if (raiz->dir != NULL && raiz->dir->dir != NULL && cor(raiz->dir->esq) == RED){
        raiz->dir = rotacionaDir(raiz->dir);
        raiz = rotacionaEsq(raiz);
        trocaCor(raiz);
    }
    return raiz;
}

Nobase* balancear(Nobase *raiz){
    if (cor(raiz->dir) == RED)
        raiz = rotacionaEsq(raiz);
    
    if (cor(raiz->esq) == RED && cor(raiz->esq) == RED)
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