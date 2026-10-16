
#include "Arv-VermelhaPreta.h"

int calcular_digito(int digitos[], int quantidade){
    int soma = 0, peso = 2, i, resto, resultado;

    for ( i = quantidade - 1; i >= 0; i--)
    {
        soma += digitos[i] * peso;
        peso++;

        if (peso > 9)
            peso = 2;
    }
    resto = soma % 11;

    if (resto < 2)
        resultado = 0;
    else
        resultado = 11 - resto;

    return resultado;
}

void formatar_cnpj(int raiz, char cnpj[]){
    int digitos[14], divisor = 10000000;
    int i;

    for ( i = 0; i < 8; i++)
    {
        digitos[i] = (raiz/divisor) % 10;
        divisor /= 10;
    }

    digitos[8] = 0;
    digitos[9] = 0;
    digitos[10] = 0;
    digitos[11] = 1;
    digitos[12] = calcular_digito(digitos, 12);
    digitos[13] = calcular_digito(digitos, 13);

    sprintf(cnpj, "%02d.%03d.%03d/0001-%d%d",
    raiz / 1000000, (raiz / 1000) % 1000,
    raiz % 1000, digitos[12], digitos[13]);
}

int gerar_raiz(){
    int raiz;

    raiz = 10000000 + rand() % 90000000;

    return raiz;
}

int raiz_existe(int raizes[], int qtd, int valor){
    int i, existe = 0;

    for ( i = 0; i < qtd; i++)
    {
        if (raizes[i] == valor)
            existe = 1;
    }

    return existe;
}

void ordenar_raizes(int raizes[]){
    int i, j, chave;

    for ( i = 1; i < QTD_CNPJ; i++)
    {
        chave = raizes[i];
        j = i - 1;

        while (j >= 0 && raizes[j] > chave){
            raizes[j + 1] = raizes[j];
            j--;
        }

        raizes[j + 1] = chave;
    }
}

void gerar_cnpjs(char cnpjs[][19], int raizes[]){
    int qtd = 0, raiz, i;

    while (qtd < QTD_CNPJ)
    {
        raiz = gerar_raiz();

        if (!raiz_existe(raizes, qtd, raiz))
        {
            raizes[qtd] = raiz;
            qtd++;
        }
    }

    ordenar_raizes(raizes);

    for (i = 0; i < QTD_CNPJ; i++)
        formatar_cnpj(raizes[i], cnpjs[i]);
}

void gerar_cnpj_inexistente(int raizes[], char cnpj[]){
    int raiz;

    raiz = gerar_raiz();

    while (raiz_existe(raizes, QTD_CNPJ, raiz))
        raiz = gerar_raiz();

    formatar_cnpj(raiz, cnpj);
}

void embaralhar(int vetor[], int qtd){
    int i, j, temp;

    for ( i = qtd - 1; i > 0; i--)
    {
        j = rand() % (i + 1);

        temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

void escolher_buscas(int escolhidos[]){
    int posicoes[QTD_CNPJ], i;

    for ( i = 0; i < QTD_CNPJ; i++)
        posicoes[i] = i;

    embaralhar(posicoes, QTD_CNPJ);

    for ( i = 0; i < QTD_BUSCA; i++)
    {
        escolhidos[i] = posicoes[i];
    }
}

void ordem_crescente(int ordem[]){
    for (int i = 0; i < QTD_CNPJ; i++)
        ordem[i] = i;
}

void ordem_decrescente(int ordem[]){
    for (int i = 0; i < QTD_CNPJ; i++)
        ordem[i] = QTD_CNPJ - 1 - i;
}

void ordem_aleatoria(int ordem[]){
    ordem_crescente(ordem);
    embaralhar(ordem, QTD_CNPJ);
}

void ordem_meio_primeiro(int ordem[]){
    int restante[QTD_CNPJ - 1], meio = QTD_CNPJ/2;
    int j = 0;

    for (int i = 0; i < QTD_CNPJ; i++)
    {
        if (i != meio)
        {
            restante[j] = i;
            j++;
        }
    }

    embaralhar(restante, QTD_CNPJ - 1);
    ordem[0] = meio;

    for (int i = 0; i < QTD_CNPJ - 1; i++)
        ordem[i + 1] = restante[i];
}

void montar_ordens(int ordens[][QTD_CNPJ]){
    ordem_crescente(ordens[0]);
    ordem_decrescente(ordens[1]);
    ordem_meio_primeiro(ordens[2]);
    ordem_aleatoria(ordens[3]);
}

int inserir_clientes(Cliente **raiz, char cnpjs[][19], int ordem[]){
    int i, inseridos = 0;
    char nome[50], endereco[80];

    for ( i = 0; i < QTD_CNPJ; i++)
    {
        sprintf(nome, "Cliente %d", ordem[i]);
        sprintf(endereco, "Rua %d", ordem[i]);

        if (add_ArvCliente(raiz, cnpjs[ordem[i]],
                           nome, endereco, "000") == SUCESSO)
        {
            inseridos++;
        }
    }

    return inseridos;
}

Cliente *buscar_com_passos(Cliente *raiz, char cnpj[], int *passos){
    Cliente *resultado = NULL;
    int comparacao;

    if (raiz != NULL)
    {
        (*passos)++;
        comparacao = strcmp(cnpj, raiz->cnpj);

        if (comparacao == 0)
        {
            resultado = raiz;
        }
        else if (comparacao < 0)
        {
            resultado = buscar_com_passos(
                (Cliente*) raiz->base.esq, cnpj, passos);
        }
        else
        {
            resultado = buscar_com_passos(
                (Cliente*) raiz->base.dir, cnpj, passos);
        }
    }

    return resultado;
}

int contar_passos(Cliente *raiz, char cnpj[], int *encontrou){
    int passos = 0;
    Cliente *resultado;

    resultado = buscar_com_passos(raiz, cnpj, &passos);

    if (resultado != NULL)
        *encontrou = 1;
    else
        *encontrou = 0;

    return passos;
}

int altura(Nobase *raiz){
    int esq = 0, dir = 0, resultado = 0;

    if (raiz != NULL)
    {
        esq = altura(raiz->esq);
        dir = altura(raiz->dir);

        if (esq > dir)
            resultado = esq + 1;
        else
            resultado = dir + 1;
    }

    return resultado;
}

float calcular_media(int valores[], int qtd){
    int i;
    float resultado, soma = 0;

    for ( i = 0; i < qtd; i++)
        soma += valores[i];

    resultado = soma / qtd;

    return resultado;
}

int calcular_maximo(int valores[], int qtd){
    int maior = valores[0];

    for (int i = 1; i < qtd; i++)
    {
        if (valores[i] > maior)
            maior = valores[i];
    }

    return maior;
}

void executar_caso(int caso, char cnpjs[][19], int ordem[],
int escolhidos[], char inexistentes[], int passos[],
int *passosInex, int *alturaArvore){
    Cliente *arvore = NULL;
    int encontrou, inseridos, i;

    inseridos = inserir_clientes(&arvore, cnpjs, ordem);

    if (inseridos != QTD_CNPJ)
        printf("Erro: foram inseridos %d clientes no caso %d.\n",
               inseridos, caso + 1);

    for ( i = 0; i < QTD_BUSCA; i++)
    {
        passos[i] = contar_passos(
            arvore, cnpjs[escolhidos[i]], &encontrou);

        if (!encontrou)
            printf("Erro: CNPJ nao encontrado no caso %d.\n",
                   caso + 1);
    }

    *passosInex = contar_passos(
        arvore, inexistentes, &encontrou);

    if (encontrou)
        printf("Erro: CNPJ inexistente encontrado no caso %d.\n",
               caso + 1);

    *alturaArvore = altura((Nobase*) arvore);

    liberar_clientes(arvore);
}

void imprimir_resultados(char cnpjs[][19], int escolhidos[],
int passos[][QTD_BUSCA], char inexistente[],
int passosInex[], int alturas[]){
    char *nomes[] = {
        "Crescente", "Decrescente", "Meio+aleat.", "Aleatorio"
    };

    int c, i;

    printf("\n%-20s", "CNPJ buscado");

    for (c = 0; c < QTD_CASOS; c++)
        printf("%13s", nomes[c]);

    printf("\n");

    for (i = 0; i < QTD_BUSCA; i++) {
        printf("%-20s", cnpjs[escolhidos[i]]);

        for (c = 0; c < QTD_CASOS; c++)
            printf("%13d", passos[c][i]);

        printf("\n");
    }

    printf("%-20s", "Media dos 10");

    for (c = 0; c < QTD_CASOS; c++)
        printf("%13.1f", calcular_media(passos[c], QTD_BUSCA));

    printf("\n%-20s", "Maximo dos 10");

    for (c = 0; c < QTD_CASOS; c++)
        printf("%13d", calcular_maximo(passos[c], QTD_BUSCA));

    printf("\n\nCNPJ nao cadastrado: %s\n", inexistente);

    printf("%-20s", "Passos ate concluir");

    for (c = 0; c < QTD_CASOS; c++)
        printf("%13d", passosInex[c]);

    printf("\n%-20s", "Altura da arvore");

    for (c = 0; c < QTD_CASOS; c++)
        printf("%13d", alturas[c]);

    printf("\n");
}

void menu_experimento() {
    int raizes[QTD_CNPJ];
    int escolhidos[QTD_BUSCA];
    int ordens[QTD_CASOS][QTD_CNPJ];

    int passos[QTD_CASOS][QTD_BUSCA];
    int passosInex[QTD_CASOS];
    int alturas[QTD_CASOS];

    char cnpjs[QTD_CNPJ][19];
    char inexistente[19];

    int i;

    srand(2026);

    gerar_cnpjs(cnpjs, raizes);
    gerar_cnpj_inexistente(raizes, inexistente);
    escolher_buscas(escolhidos);
    montar_ordens(ordens);

    printf("%d CNPJs gerados.\n", QTD_CNPJ);
    printf("Primeiro CNPJ: %s\n", cnpjs[0]);
    printf("Ultimo CNPJ: %s\n", cnpjs[QTD_CNPJ - 1]);

    printf("CNPJ inserido primeiro no caso 3: %s\n",
           cnpjs[QTD_CNPJ / 2]);

    for (i = 0; i < QTD_CASOS; i++) {
        executar_caso(
            i, cnpjs, ordens[i], escolhidos, inexistente,
            passos[i], &passosInex[i], &alturas[i]);
    }

    imprimir_resultados(
        cnpjs, escolhidos, passos, inexistente,
        passosInex, alturas);

    return 0;
}
