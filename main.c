#include "Arv-VermelhaPreta.h"

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ler_texto(char *rotulo, char destino[], int tam) {
    char linha[256];

    printf("%s", rotulo);
    if (fgets(linha, sizeof(linha), stdin) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        strncpy(destino, linha, tam - 1);
        destino[tam - 1] = '\0';
    } else {
        destino[0] = '\0';
    }
}

int ler_int(char *rotulo) {
    int v = 0;
    printf("%s", rotulo);
    if (scanf("%d", &v) != 1)
        v = 0;
    limpar_buffer();
    return v;
}

void exibir_menu() {
    printf("\n========== FABRICA ==========\n");
    printf(" 1  - Cadastrar produto\n");
    printf(" 2  - Buscar produto\n");
    printf(" 3  - Remover produto\n");
    printf(" 4  - Listar produtos\n");
    printf(" 5  - Cadastrar cliente\n");
    printf(" 6  - Buscar cliente\n");
    printf(" 7  - Remover cliente\n");
    printf(" 8  - Listar clientes\n");
    printf(" 9  - Registrar compra de um cliente\n");
    printf("10  - Buscar data de compra de um cliente\n");
    printf("11  - Remover data de compra de um cliente\n");
    printf("12  - Listar compras de um cliente\n");
    printf("13  - Remover compra de um cliente\n");
    printf(" 0  - Sair\n");
    printf("Opcao: ");
}

void menu_cadastrar_produto(Produto **raiz) {
    int cod, tipo, cabelo, sit, tam, estoque, i, resp;
    FaixaPreco faixas[QTD_FAIXAS];

    printf("\n CADASTRAR PRODUTO \n");
    cod     = ler_int("Codigo: ");
    tipo    = ler_int("Tipo: ");
    cabelo  = ler_int("Cabelo: ");
    sit     = ler_int("Situacao: ");
    tam     = ler_int("Tamanho: ");
    estoque = ler_int("Estoque: ");

    for (i = 0; i < QTD_FAIXAS; i++) {
        printf("Faixa %d\n", i + 1);
        faixas[i].qtdMIN = ler_int("  Quantidade minima: ");
        printf("  Preco unitario: ");
        if (scanf("%f", &faixas[i].preco) != 1)
            faixas[i].preco = 0;
        limpar_buffer();
    }

    resp = add_ArvProduto(raiz, cod, tipo, cabelo, sit, tam, estoque, faixas);

    if (resp == SUCESSO)
        printf("Produto cadastrado com sucesso.\n");
    else if (resp == ERRO_REPETIDO)
        printf("Codigo de produto ja cadastrado.\n");
    else
        printf("Falha no cadastro (memoria).\n");
}

void menu_buscar_produto(Produto *raiz) {
    Produto *p = buscarProduto(raiz, ler_int("Codigo do produto: "));
    if (p != NULL)
        imprimir_produto(p);
    else
        printf("Produto nao encontrado.\n");
}

void menu_remover_produto(Produto **raiz) {
    int resp = remove_produto_arv(raiz, ler_int("Codigo do produto: "));
    if (resp == SUCESSO)
        printf("Produto removido.\n");
    else
        printf("Produto nao encontrado.\n");
}

void menu_cadastrar_cliente(Cliente **raiz) {
    char cnpj[19], nome[50], endereco[80], contato[40];
    int resp;

    printf("\n CADASTRAR CLIENTE \n");
    ler_texto("CNPJ: ", cnpj, sizeof(cnpj));
    ler_texto("Nome: ", nome, sizeof(nome));
    ler_texto("Endereco: ", endereco, sizeof(endereco));
    ler_texto("Contato: ", contato, sizeof(contato));

    resp = add_ArvCliente(raiz, cnpj, nome, endereco, contato);

    if (resp == SUCESSO)
        printf("Cliente cadastrado com sucesso.\n");
    else if (resp == ERRO_REPETIDO)
        printf("CNPJ ja cadastrado.\n");
    else
        printf("Falha no cadastro (memoria).\n");
}

void menu_buscar_cliente(Cliente *raiz) {
    char cnpj[19];
    Cliente *c;

    ler_texto("CNPJ: ", cnpj, sizeof(cnpj));
    c = buscarCliente(raiz, cnpj);
    if (c != NULL)
        imprimir_cliente(c);
    else
        printf("Cliente nao encontrado.\n");
}

void menu_remover_cliente(Cliente **raiz) {
    char cnpj[19];
    int resp;

    ler_texto("CNPJ: ", cnpj, sizeof(cnpj));
    resp = remove_cliente_arv(raiz, cnpj);

    if (resp == SUCESSO)
        printf("Cliente removido.\n");
    else if (resp == ERRO_COMPRAS)
        printf("Cliente possui compras. Remova as datas de compra antes.\n");
    else
        printf("Cliente nao encontrado.\n");
}

void menu_registrar_compra(Cliente *clientes, Produto *produtos) {
    char cnpj[19];
    Cliente *cli;
    NoData *d;
    Produto *prod;
    int data, codCompra, codProd, qtd, resp;
    float valor;

    ler_texto("CNPJ do cliente: ", cnpj, sizeof(cnpj));
    cli = buscarCliente(clientes, cnpj);
    if (cli == NULL) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    data = ler_int("Data (aaaammdd): ");
    d = buscarData(cli->raizCompras, data);
    if (d == NULL) {
        add_ArvData(&cli->raizCompras, data);
        d = buscarData(cli->raizCompras, data);
    }
    if (d == NULL) {
        printf("Falha ao criar a data (memoria).\n");
        return;
    }

    codCompra = ler_int("Codigo da compra: ");
    resp = adicionar_compra(d, codCompra);
    if (resp == ERRO_REPETIDO) {
        printf("Codigo de compra ja existe nessa data.\n");
        return;
    }
    if (resp != SUCESSO) {
        printf("Falha ao registrar a compra.\n");
        return;
    }

    do {
        codProd = ler_int("Codigo do produto (0 para terminar): ");
        if (codProd != 0) {
            prod = buscarProduto(produtos, codProd);
            if (prod == NULL) {
                printf("Produto nao encontrado.\n");
            } else {
                qtd = ler_int("Quantidade: ");
                if (!tem_estoque(prod, qtd)) {
                    printf("Quantidade invalida ou estoque insuficiente.\n");
                } else {
                    valor = preco_unitario(prod, qtd) * qtd;
                    resp = adicionar_item(d->compras, prod->codigo, qtd, valor);
                    if (resp == SUCESSO) {
                        baixar_estoque(prod, qtd);
                        printf("Item adicionado.\n");
                    } else {
                        printf("Falha ao adicionar item.\n");
                    }
                }
            }
        }
    } while (codProd != 0);
}

void menu_buscar_data(Cliente *clientes) {
    char cnpj[19];
    Cliente *cli;
    NoData *d;

    ler_texto("CNPJ do cliente: ", cnpj, sizeof(cnpj));
    cli = buscarCliente(clientes, cnpj);
    if (cli == NULL) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    d = buscarData(cli->raizCompras, ler_int("Data (aaaammdd): "));
    if (d == NULL)
        printf("Nao ha compras nessa data.\n");
    else
        imprimir_data(d);
}

void menu_remover_data(Cliente *clientes) {
    char cnpj[19];
    Cliente *cli;
    int resp;

    ler_texto("CNPJ do cliente: ", cnpj, sizeof(cnpj));
    cli = buscarCliente(clientes, cnpj);
    if (cli == NULL) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    resp = remove_data_arv(&cli->raizCompras, ler_int("Data (aaaammdd): "));
    if (resp == SUCESSO)
        printf("Data removida.\n");
    else if (resp == ERRO_COMPRAS)
        printf("A data possui compras. Remova as compras antes.\n");
    else
        printf("Nao ha compras nessa data.\n");
}

void menu_listar_compras(Cliente *clientes) {
    char cnpj[19];
    Cliente *cli;

    ler_texto("CNPJ do cliente: ", cnpj, sizeof(cnpj));
    cli = buscarCliente(clientes, cnpj);
    if (cli == NULL)
        printf("Cliente nao encontrado.\n");
    else if (cli->raizCompras == NULL)
        printf("Cliente sem compras.\n");
    else
        imprimir_datas(cli->raizCompras);
}

void menu_remover_compra(Cliente *clientes) {
    char cnpj[19];
    Cliente *cli;
    NoData *d;

    ler_texto("CNPJ do cliente: ", cnpj, sizeof(cnpj));
    cli = buscarCliente(clientes, cnpj);
    if (cli == NULL) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    d = buscarData(cli->raizCompras, ler_int("Data (aaaammdd): "));
    if (d == NULL) {
        printf("Nao ha compras nessa data.\n");
        return;
    }

    if (remover_compra(d, ler_int("Codigo da compra: ")) == SUCESSO)
        printf("Compra removida.\n");
    else
        printf("Compra nao encontrada.\n");
}

int main(void) {
    Produto *produtos = NULL;
    Cliente *clientes = NULL;
    int opcao;

    do {
        exibir_menu();
        if (scanf("%d", &opcao) != 1)
            opcao = 0;
        limpar_buffer();

        switch (opcao) {
            case 1:  menu_cadastrar_produto(&produtos);      break;
            case 2:  menu_buscar_produto(produtos);          break;
            case 3:  menu_remover_produto(&produtos);        break;
            case 4:  imprimir_produtos(produtos);            break;
            case 5:  menu_cadastrar_cliente(&clientes);      break;
            case 6:  menu_buscar_cliente(clientes);          break;
            case 7:  menu_remover_cliente(&clientes);        break;
            case 8:  imprimir_clientes(clientes);            break;
            case 9:  menu_registrar_compra(clientes, produtos); break;
            case 10: menu_buscar_data(clientes);             break;
            case 11: menu_remover_data(clientes);            break;
            case 12: menu_listar_compras(clientes);          break;
            case 13: menu_remover_compra(clientes);          break;
            case 0:  break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    liberar_produtos(produtos);
    liberar_clientes(clientes);
    return 0;
}
