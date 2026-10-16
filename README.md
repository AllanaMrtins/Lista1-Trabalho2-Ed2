# Fábrica — Sistema de Gerenciamento em C

Sistema de gerenciamento de produtos, clientes e compras desenvolvido em **linguagem C**, utilizando árvores rubro-negras para organizar e manipular os dados de forma eficiente.

O projeto reúne conceitos de estruturas de dados, alocação dinâmica de memória, árvores de busca binária balanceadas e listas encadeadas, aplicados a um sistema de controle de uma fábrica.

## Sobre o projeto

O sistema permite cadastrar e consultar produtos, gerenciar clientes e registrar compras associadas a datas específicas. Também realiza o controle de estoque e calcula o preço unitário dos produtos de acordo com as faixas de quantidade cadastradas.

Além das operações de gerenciamento, o projeto possui um experimento que analisa o comportamento das árvores rubro-negras durante a inserção e a busca de clientes, utilizando diferentes ordens de inserção de CNPJs.

A organização dos dados utiliza árvores rubro-negras, uma estrutura de busca binária que mantém o balanceamento por meio de rotações e alterações de cores durante as operações de inserção e remoção.

O projeto foi desenvolvido com foco na aplicação prática dos conceitos estudados em **Estruturas de Dados**, especialmente árvores balanceadas, estruturas dinâmicas e análise do comportamento de algoritmos.

## Funcionalidades

### Produtos

- Cadastro de produtos com código, tipo, cabelo, situação, tamanho e estoque.
- Definição de três faixas de preço por quantidade.
- Busca de produtos pelo código.
- Listagem de produtos em ordem crescente de código.
- Remoção de produtos cadastrados.
- Verificação de códigos duplicados.
- Consulta de estoque disponível.
- Cálculo do preço unitário conforme a quantidade solicitada.

### Clientes

- Cadastro de clientes com CNPJ, nome, endereço e contato.
- Busca de clientes pelo CNPJ.
- Listagem de clientes em ordem crescente de CNPJ.
- Remoção de clientes sem datas de compra associadas.
- Verificação de CNPJs duplicados.

### Compras

- Registro de compras vinculadas a um cliente e a uma data.
- Cadastro de itens associados a cada compra.
- Consulta de compras por data.
- Busca de compras pelo código.
- Remoção de compras e dos respectivos itens.
- Impedimento da remoção de datas que ainda possuem compras.
- Impedimento da remoção de clientes que ainda possuem datas de compra.

### Controle de estoque e preços

- Verificação da disponibilidade de estoque antes de adicionar um item.
- Atualização do estoque após a inclusão de um item na compra.
- Cálculo do valor do item com base no preço unitário e na quantidade adquirida.
- Seleção da faixa de preço correspondente à quantidade informada.

### Experimento com CNPJs

- Geração de 100 CNPJs para a realização dos testes.
- Armazenamento dos CNPJs gerados em um vetor.
- Inserção de clientes em quatro cenários diferentes.
- Busca de 10 CNPJs em cada cenário.
- Busca de um CNPJ inexistente para avaliar o comportamento da pesquisa quando o elemento não está cadastrado.
- Contagem dos passos realizados durante as buscas.
- Cálculo da altura da árvore rubro-negra em cada cenário.
- Apresentação dos resultados para comparação entre as diferentes formas de inserção.

## Tecnologias e conceitos utilizados

| Tecnologia ou conceito | Aplicação no projeto |
|---|---|
| Linguagem C | Implementação da lógica do sistema |
| Árvores rubro-negras | Organização de produtos, clientes e datas |
| Listas encadeadas | Armazenamento de compras e itens |
| Alocação dinâmica | Criação e liberação de nós com `malloc` e `free` |
| Ponteiros | Manipulação das estruturas dinâmicas |
| Recursividade | Implementação de operações em árvores |
| Arquivos de cabeçalho | Organização das declarações e estruturas do programa |
| Geração de dados de teste | Criação de CNPJs para o experimento |
| Análise de algoritmos | Comparação dos passos das buscas e da altura das árvores |

## Estrutura de dados

O sistema utiliza três árvores rubro-negras principais e listas encadeadas para representar os relacionamentos entre os dados.

### 1. Árvore de produtos

Cada nó armazena as informações de um produto e utiliza o código como chave de ordenação.

A árvore permite inserir, buscar, listar e remover produtos, mantendo os códigos organizados em ordem crescente durante a listagem.

### 2. Árvore de clientes

Os clientes são organizados pelo CNPJ, utilizado como chave de busca. Cada cliente possui uma árvore própria para armazenar as datas associadas às suas compras.

### 3. Árvore de datas

Cada cliente possui uma árvore de datas. Cada nó representa uma data e mantém uma lista de compras realizadas naquela data.

### 4. Listas de compras e itens

Cada data pode possuir uma lista de compras, e cada compra pode conter uma lista de itens. Essa organização permite registrar vários produtos em uma mesma compra e várias compras em uma mesma data.

## Como funcionam as árvores rubro-negras?

As árvores rubro-negras são árvores de busca binária autobalanceadas. Cada nó possui uma cor, vermelha ou preta, utilizada para preservar propriedades que limitam o crescimento da árvore.

Neste projeto, são utilizadas operações auxiliares para manter o balanceamento:

- **Rotações à esquerda:** reorganizam os nós quando necessário.
- **Rotações à direita:** corrigem determinadas configurações da árvore.
- **Troca de cores:** ajusta as cores dos nós durante o balanceamento.
- **Correção após inserção:** restaura as propriedades da árvore após inserir um elemento.
- **Balanceamento após remoção:** reorganiza a árvore durante a exclusão de elementos.
- **Busca pelo menor elemento:** auxilia na remoção de nós que possuem dois filhos.

Essas operações permitem que a estrutura permaneça organizada e balanceada, evitando que as operações de busca dependam de uma árvore completamente degenerada em uma lista.

## Experimento de inserção e busca

O experimento tem como objetivo comparar o comportamento da árvore rubro-negra ao receber os mesmos dados em diferentes ordens de inserção.

São utilizados quatro cenários:

1. **CNPJs em ordem crescente:** os clientes são inseridos do menor para o maior CNPJ.
2. **CNPJs em ordem decrescente:** os clientes são inseridos do maior para o menor CNPJ.
3. **CNPJ do meio inserido primeiro:** o CNPJ da posição central da sequência ordenada é inserido primeiro, seguido pelos demais em ordem aleatória.
4. **CNPJs em ordem totalmente aleatória:** os clientes são inseridos em uma sequência aleatória.

Em cada cenário, o experimento realiza buscas de CNPJs cadastrados e de um CNPJ inexistente. Durante os testes, são contabilizados os passos das buscas e calculada a altura da árvore.

Os resultados permitem comparar o número de passos necessários para localizar os elementos e a altura alcançada pela árvore em cada cenário. Dessa forma, é possível observar o comportamento do balanceamento da árvore rubro-negra diante de diferentes sequências de inserção.

O experimento pode ser executado diretamente pelo menu principal, selecionando a opção 14.

## Menu do sistema

Ao executar o programa, o usuário encontra um menu interativo com as seguintes opções:

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
| 0 | Encerrar o sistema |

## Tratamento de operações

O sistema utiliza códigos de retorno para identificar os resultados das operações:

| Código | Significado |
|---|---|
| `SUCESSO` | Operação realizada com sucesso |
| `ERRO_MEMORIA` | Falha na alocação de memória |
| `ERRO_REPETIDO` | Elemento já cadastrado |
| `ERRO_INEXISTENTE` | Elemento não encontrado |
| `ERRO_ESTOQUE` | Código reservado para erros relacionados ao estoque |
| `ERRO_COMPRAS` | Operação impedida devido à existência de compras |

Esses códigos permitem distinguir situações de sucesso e falha durante o uso das funções do sistema.

## Objetivos de aprendizagem

O desenvolvimento deste projeto permite praticar:

- Implementação e manutenção de árvores rubro-negras.
- Manipulação de ponteiros e estruturas dinâmicas em C.
- Operações de inserção, busca e remoção em árvores balanceadas.
- Uso de listas encadeadas para representar relacionamentos entre entidades.
- Gerenciamento de memória dinâmica.
- Organização de código em arquivos `.c` e `.h`.
- Aplicação de estruturas de dados em um sistema de gerenciamento.
- Comparação de algoritmos por meio de experimentos.
- Análise do número de passos das buscas e da altura das árvores.

## Status do projeto

Em desenvolvimento, com foco na implementação e validação das operações de gerenciamento, no balanceamento das árvores, na integração entre produtos, clientes e compras e na análise experimental das operações de inserção e busca.

---

**Projeto acadêmico desenvolvido em linguagem C para aplicação prática de conceitos de Estruturas de Dados.**