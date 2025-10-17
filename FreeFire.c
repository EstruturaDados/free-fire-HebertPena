#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// --- ESTRUTURAS E ENUMS ---

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[50];
    char tipo[50];
    int quantidade;
    int prioridade;
} Item;

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
typedef enum {
    NOME,
    TIPO,
    PRIORIDADE
} CriterioOrdenacao;


// --- VARIÁVEIS GLOBAIS ---

// Vetor mochila:
// Armazena até 10 itens coletados.
Item mochila[10];
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
int numItens = 0;
long long int comparacoes = 0;
bool ordenadaPorNome = false;


// --- PROTÓTIPOS DAS FUNÇÕES ---

void limparTela();
void pressioneEnterParaContinuar();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();


// --- FUNÇÃO PRINCIPAL ---

int main() {
    int opcao;

    do {
        limparTela();
        exibirMenu();
        printf("Escolha uma opcao: ");
        
        // Validação de entrada para garantir que seja um número
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de erro
            while(getchar() != '\n'); 
            opcao = -1; // Atribui um valor inválido para repetir o menu
        }

        // A estrutura switch trata cada opção chamando a função correspondente.
        // A ordenação e busca binária exigem que os dados estejam bem organizados.
        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("\nSaindo do programa. Ate a proxima, sobrevivente!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        if (opcao != 0) {
            pressioneEnterParaContinuar();
        }

    } while (opcao != 0);

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    // Tenta usar o comando específico do sistema operacional para uma limpeza mais eficaz
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Pausa a execução até o usuário pressionar Enter
void pressioneEnterParaContinuar() {
    printf("\nPressione Enter para continuar...");
    // Limpa o buffer de entrada antes de esperar por um novo caractere
    while(getchar() != '\n');
    getchar(); // Espera pelo Enter
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu() {
    printf("========================================\n");
    printf("      MOCHILA DE SOBREVIVENCIA\n");
    printf("========================================\n");
    printf("Itens na mochila: %d/10\n", numItens);
    // Informa o status da ordenação para a busca binária
    if (ordenadaPorNome) {
        printf("Status: Itens ordenados por nome.\n");
    } else {
        printf("Status: Itens nao ordenados por nome.\n");
    }
    printf("----------------------------------------\n");
    printf("1. Adicionar um item\n");
    printf("2. Remover um item\n");
    printf("3. Listar todos os itens\n");
    printf("4. Ordenar os itens\n");
    printf("5. Buscar item por nome (Busca Binaria)\n");
    printf("0. Sair\n");
    printf("----------------------------------------\n");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    limparTela();
    printf("--- ADICIONAR NOVO ITEM ---\n");
    if (numItens >= 10) {
        printf("\nA mochila esta cheia! Remova um item antes de adicionar outro.\n");
        return;
    }

    Item novoItem;
    printf("Nome do item: ");
    // Limpa o buffer antes de ler a string
    while(getchar() != '\n'); 
    fgets(novoItem.nome, 50, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; // Remove o \n

    printf("Tipo do item (Kit Medico, Municao, Arma, etc.): ");
    fgets(novoItem.tipo, 50, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    do {
        printf("Prioridade (1-5, onde 5 e a mais alta): ");
        scanf("%d", &novoItem.prioridade);
        if (novoItem.prioridade < 1 || novoItem.prioridade > 5) {
            printf("Prioridade invalida! Insira um valor entre 1 e 5.\n");
        }
    } while (novoItem.prioridade < 1 || novoItem.prioridade > 5);

    mochila[numItens] = novoItem;
    numItens++;
    ordenadaPorNome = false; // A inserção desordena a lista

    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    limparTela();
    printf("--- REMOVER ITEM ---\n");
    if (numItens == 0) {
        printf("\nA mochila esta vazia. Nao ha itens para remover.\n");
        return;
    }

    char nomeParaRemover[50];
    printf("Digite o nome do item a ser removido: ");
    while(getchar() != '\n');
    fgets(nomeParaRemover, 50, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = 0;

    int indice = -1;
    for (int i = 0; i < numItens; i++) {
        // strcasecmp ignora maiúsculas/minúsculas
        if (strcasecmp(mochila[i].nome, nomeParaRemover) == 0) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        printf("\nItem '%s' encontrado. Removendo...\n", mochila[indice].nome);
        // Desloca os itens para preencher o espaço vazio
        for (int i = indice; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;
        ordenadaPorNome = false; // A remoção pode desordenar a lista
        printf("Item removido com sucesso!\n");
    } else {
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeParaRemover);
    }
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens() {
    limparTela();
    printf("--- ITENS NA MOCHILA ---\n");
    if (numItens == 0) {
        printf("\nA mochila esta vazia.\n");
        return;
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("| %-25s | %-20s | %-10s | %-10s |\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < numItens; i++) {
        printf("| %-25s | %-20s | %-10d | %-10d |\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    printf("--------------------------------------------------------------------------------\n");
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).
void menuDeOrdenacao() {
    limparTela();
    printf("--- ORDENAR ITENS ---\n");
    if (numItens < 2) {
        printf("\nE necessario ter pelo menos 2 itens para ordenar.\n");
        return;
    }
    
    int criterio;
    printf("Escolha o criterio de ordenacao:\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo (A-Z)\n");
    printf("3. Por Prioridade (Mais alta primeiro)\n");
    printf("0. Cancelar\n");
    printf("Opcao: ");
    scanf("%d", &criterio);

    switch (criterio) {
        case 1:
            insertionSort(NOME);
            printf("\nItens ordenados por nome.\n");
            break;
        case 2:
            insertionSort(TIPO);
            printf("\nItens ordenados por tipo.\n");
            break;
        case 3:
            insertionSort(PRIORIDADE);
            printf("\nItens ordenados por prioridade.\n");
            break;
        case 0:
            printf("\nOperacao de ordenacao cancelada.\n");
            return;
        default:
            printf("\nCriterio invalido!\n");
            return;
    }
    printf("Analise de Desempenho: Foram realizadas %lld comparacoes.\n", comparacoes);
}

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)
void insertionSort(CriterioOrdenacao criterio) {
    comparacoes = 0;
    int i, j;
    Item chave;

    for (i = 1; i < numItens; i++) {
        chave = mochila[i];
        j = i - 1;
        
        bool deveMover = false;
        // O loop while move os elementos maiores que a chave para a direita
        while (j >= 0) {
            comparacoes++; // Incrementa a cada comparação
            switch (criterio) {
                case NOME:
                    deveMover = (strcasecmp(mochila[j].nome, chave.nome) > 0);
                    break;
                case TIPO:
                    deveMover = (strcasecmp(mochila[j].tipo, chave.tipo) > 0);
                    break;
                case PRIORIDADE:
                    // Ordena da maior para a menor prioridade
                    deveMover = (mochila[j].prioridade < chave.prioridade);
                    break;
            }

            if(deveMover){
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break;
            }
        }
        mochila[j + 1] = chave;
    }

    // Atualiza o status de ordenação para a busca binária
    if (criterio == NOME) {
        ordenadaPorNome = true;
    } else {
        ordenadaPorNome = false;
    }
}

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscaBinariaPorNome() {
    limparTela();
    printf("--- BUSCA BINARIA POR NOME ---\n");
    if (!ordenadaPorNome) {
        printf("\nERRO: A mochila precisa estar ordenada por nome para usar a busca binaria.\n");
        printf("Use a opcao 4 no menu principal para ordenar.\n");
        return;
    }
    if (numItens == 0) {
        printf("\nA mochila esta vazia. Nao ha o que buscar.\n");
        return;
    }

    char nomeProcurado[50];
    printf("Digite o nome do item que deseja buscar: ");
    while(getchar() != '\n');
    fgets(nomeProcurado, 50, stdin);
    nomeProcurado[strcspn(nomeProcurado, "\n")] = 0;

    int inicio = 0;
    int fim = numItens - 1;
    int meio;
    long long int comparacoesBusca = 0;

    while (inicio <= fim) {
        comparacoesBusca++;
        meio = inicio + (fim - inicio) / 2; // Evita overflow para listas muito grandes
        int res = strcasecmp(mochila[meio].nome, nomeProcurado);

        if (res == 0) {
            printf("\n--- Item Encontrado! ---\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            printf("\nBusca realizada com %lld comparacoes.\n", comparacoesBusca);
            return;
        }
        if (res < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    printf("\nItem '%s' nao foi encontrado na mochila.\n", nomeProcurado);
    printf("Busca finalizada com %lld comparacoes.\n", comparacoesBusca);
}
