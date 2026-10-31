# Fábrica — Sistema de Gerenciamento em C

Sistema de gerenciamento de **produtos, clientes e compras** desenvolvido em **linguagem C**, que organiza os dados em **árvores rubro-negras caídas para a esquerda** (*Left-Leaning Red-Black Trees*, LLRB) e em listas encadeadas.

O projeto também inclui um **experimento** que compara o comportamento da árvore de clientes (chave: CNPJ) com diferentes ordens de inserção.

> Projeto acadêmico de **Estruturas de Dados II**.

## Sumário

- [Funcionalidades](#funcionalidades)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Como compilar e executar](#como-compilar-e-executar)
- [Menu do sistema](#menu-do-sistema)
- [Modelo de dados](#modelo-de-dados)
- [Árvore rubro-negra (LLRB)](#árvore-rubro-negra-llrb)
- [Regras de negócio](#regras-de-negócio)
- [Experimento com CNPJs](#experimento-com-cnpjs)
- [Códigos de retorno](#códigos-de-retorno)
- [Observações e limitações](#observações-e-limitações)

## Funcionalidades

**Produtos**
- Cadastro com código, tipo, cabelo, situação, tamanho, estoque e três faixas de preço por quantidade.
- Busca, remoção e listagem em ordem crescente de código.
- Bloqueio de códigos duplicados.

**Clientes**
- Cadastro com CNPJ, nome, endereço e contato.
- Busca, remoção e listagem em ordem crescente de CNPJ.
- Bloqueio de CNPJs duplicados.
- Remoção bloqueada enquanto o cliente tiver datas de compra.

**Compras**
- Cada cliente possui sua própria árvore de datas (formato `aaaammdd`).
- Cada data guarda uma lista de compras, e cada compra uma lista de itens.
- Registro de compra com vários itens; consulta por data; listagem de todas as compras do cliente.
- Remoção de compra **devolvendo as quantidades ao estoque**.
- Remoção de data bloqueada enquanto houver compras nela.
- Compra finalizada sem nenhum item é descartada automaticamente (e a data é removida se ficar vazia).

## Estrutura do projeto

| Arquivo | Conteúdo |
|---|---|
| `main.c` | Menu interativo, leitura de dados e fluxos de cada operação |
| `Arv-VermelhaPreta.h` | Structs, constantes, códigos de retorno e protótipos |
| `Arv-VermelhaPreta.c` | Implementação da LLRB, produtos, clientes, datas, compras e itens |
| `funcoes_experimento.c` | Geração de CNPJs, ordens de inserção, contagem de passos e relatório |

## Como compilar e executar

Requer um compilador C (por exemplo, `gcc`).

```bash
gcc -Wall -o fabrica main.c Arv-VermelhaPreta.c funcoes_experimento.c
./fabrica
```

No Windows, o executável gerado será `fabrica.exe`.

## Menu do sistema

| Opção | Operação |
|---:|---|
| 1 | Cadastrar produto |
| 2 | Buscar produto |
| 3 | Remover produto |
| 4 | Listar produtos |
| 5 | Cadastrar cliente |
| 6 | Buscar cliente |
| 7 | Remover cliente |
| 8 | Listar clientes |
| 9 | Registrar compra de um cliente |
| 10 | Buscar data de compra de um cliente |
| 11 | Remover data de compra de um cliente |
| 12 | Listar compras de um cliente |
| 13 | Remover compra de um cliente |
| 14 | Executar experimento com CNPJs |
| 0 | Sair |

**Valores aceitos no cadastro de produto**

| Campo | Opções |
|---|---|
| Tipo | 0 shampoo · 1 condicionador · 2 finalizador · 3 máscara · 4 óleo |
| Cabelo | 0 liso · 1 ondulado · 2 crespo |
| Situação | 0 oleoso · 1 normal · 2 seco · 3 pós-química |
| Tamanho | 0 300 ml · 1 500 ml · 2 1 L |

**Fluxo de uma compra (opção 9):** informar o CNPJ do cliente → a data (`aaaammdd`) → o código da compra → os itens (código do produto e quantidade). Digite `0` no código do produto para encerrar.

## Modelo de dados

```
Árvore de Produtos (chave: codigo)
   └── Produto ── faixas de preço [3]

Árvore de Clientes (chave: cnpj, comparação com strcmp)
   └── Cliente
         └── Árvore de Datas (chave: data aaaammdd)
               └── NoData
                     └── Lista de Compras
                           └── Compra
                                 └── Lista de Itens
```

São **três árvores rubro-negras**: produtos, clientes e datas (uma árvore de datas por cliente). Compras e itens são listas encadeadas simples.

Todos os nós de árvore começam com uma struct `Nobase` (ponteiros `esq`/`dir` e `cor`). Isso permite reutilizar as mesmas funções de rotação e balanceamento para produtos, clientes e datas, por meio de *casts* de ponteiro.

## Árvore rubro-negra (LLRB)

A implementação segue a variante **left-leaning**, em que links vermelhos só podem pender para a esquerda. As operações auxiliares são:

| Função | Papel |
|---|---|
| `cor` | Retorna a cor do nó (nó nulo é preto) |
| `trocaCor` | Inverte a cor do nó e de seus filhos |
| `rotacionaEsq` / `rotacionaDir` | Rotações |
| `corrigirInsercao` | Restaura as propriedades após inserir |
| `moverEsqRED` / `moverDirRED` | Empurram um nó vermelho para baixo antes da remoção |
| `balancear` | Restaura as propriedades após remover |
| `procuraMenor` / `removerMenor` | Sucessor usado na remoção de nó com dois filhos |

Após inserções e remoções na raiz, a cor da raiz é redefinida como preta.

## Regras de negócio

- **Preço unitário:** usa a faixa de maior `qtdMIN` que seja menor ou igual à quantidade comprada.
- **Valor do item:** `preço unitário × quantidade`, gravado no item como valor pago.
- **Estoque:** a quantidade deve ser maior que zero e não pode exceder o estoque; ao adicionar o item, o estoque é decrementado.
- **Cancelamento:** ao remover uma compra, o estoque de cada produto dos itens é restituído.
- **Integridade:** não é possível remover cliente com datas, nem data com compras.

## Experimento com CNPJs

Acessível pela **opção 14** do menu. Usa semente fixa (`srand(2026)`), então os resultados são reproduzíveis.

**Procedimento**
1. Gera 100 CNPJs válidos (com dígitos verificadores), sem repetição, ordenados.
2. Sorteia 10 CNPJs para busca e gera 1 CNPJ inexistente.
3. Para cada um dos quatro cenários, insere os 100 clientes em uma árvore nova:

| Caso | Ordem de inserção |
|---:|---|
| 1 | Crescente |
| 2 | Decrescente |
| 3 | CNPJ do meio primeiro, demais em ordem aleatória |
| 4 | Totalmente aleatória |

4. Conta os **passos** (nós visitados) de cada busca e calcula a **altura** da árvore.
5. Exibe a tabela com média e máximo dos 10 CNPJs, os passos da busca pelo CNPJ inexistente e a altura de cada árvore.

**Exemplo de resultado (resumo)**

| Métrica | Crescente | Decrescente | Meio + aleat. | Aleatório |
|---|---:|---:|---:|---:|
| Média dos 10 | 5,5 | 6,0 | 5,5 | 6,0 |
| Máximo dos 10 | 7 | 7 | 8 | 7 |
| CNPJ inexistente | 7 | 8 | 7 | 6 |
| Altura da árvore | 7 | 9 | 9 | 9 |

Mesmo com inserção em ordem totalmente crescente ou decrescente (o pior caso de uma árvore de busca comum, que degeneraria em lista com altura 100), a altura fica próxima de log₂(100) ≈ 6,6, e as buscas visitam poucos nós. Isso mostra o efeito do balanceamento da árvore.

## Códigos de retorno

| Código | Valor | Significado |
|---|---:|---|
| `SUCESSO` | 1 | Operação realizada |
| `ERRO_MEMORIA` | 0 | Falha de alocação |
| `ERRO_REPETIDO` | 5 | Elemento já cadastrado |
| `ERRO_INEXISTENTE` | 6 | Elemento não encontrado |
| `ERRO_ESTOQUE` | 7 | Reservado para estoque (declarado, ainda não utilizado; a checagem é feita por `tem_estoque`) |
| `ERRO_COMPRAS` | 8 | Remoção impedida por haver compras associadas |

## Observações e limitações

- Os dados ficam apenas em memória; ao encerrar o programa, nada é salvo em arquivo.
- A entrada de dados é validada de forma básica (opções de menu e estoque); campos como CNPJ e data não têm validação de formato no cadastro manual.
- Os nomes de arquivo e de campos usam o português, sem acentos nas mensagens do terminal.

---

**Projeto acadêmico desenvolvido em C para a prática de Estruturas de Dados (árvores balanceadas, listas encadeadas e alocação dinâmica).**
