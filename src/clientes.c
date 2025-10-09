#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/structs.h"
#include "../include/validar_cpf.h"

cliente *lista = NULL; //ponteiro para lista, inicialmente sem memoria alocada
int count = 0; // contador 

void adiciona_cliente(const char *nome, const char *cpf, const char *telefone) { // funcao para adicionar cliente, recebe strings para adicao na lista de clientes
    lista = realloc(lista, sizeof(cliente) * (count + 1));  // count + 1 clientes (para nao precisar definir um valor fixo do tamanho da lista)
    strcpy(lista[count].nome, nome); 
    strcpy(lista[count].cpf, cpf);
    strcpy(lista[count].telefone, telefone);
    count++; // incrementa o contador
}

void carregar_clientes_do_arquivo(const char *nome_arquivo) { // funcao para leitura de clientes (precisamos fazer assim pois nao é possivel usar o append "a")
    FILE *arquivo = fopen(nome_arquivo, "r"); // abre o arquivo para leitura
    if (!arquivo) return; // se o comando acima falhar ele retorna

    char linha[256];
    cliente temp; 
    int step = 0;

    while (fgets(linha, sizeof(linha), arquivo)) { // loop para ler o arquivo
        linha[strcspn(linha, "\n")] = 0;
        if (strlen(linha) == 0) continue;

        if (strncmp(linha, "Nome: ", 6) == 0) {
            strcpy(temp.nome, linha + 6);
            step = 1;
        } else if (strncmp(linha, "CPF: ", 5) == 0 && step == 1) {
            strcpy(temp.cpf, linha + 5);
            step = 2;
        } else if (strncmp(linha, "Telefone: ", 10) == 0 && step == 2) {
            strcpy(temp.telefone, linha + 10);
            adiciona_cliente(temp.nome, temp.cpf, temp.telefone);
            step = 0;
        }
    }
    fclose(arquivo);
}

void salvar_clientes_no_arquivo(const char *nome_arquivo) { // funcao para escrever o nomes no arquivo
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(arquivo, "Nome: %s\n", lista[i].nome);
        fprintf(arquivo, "CPF: %s\n", lista[i].cpf);
        fprintf(arquivo, "Telefone: %s\n\n", lista[i].telefone);
    }
    fclose(arquivo);
}

int cadastroClientes() { // funcao para cadastrar os clientes
    char nome_arquivo[] = "data/clientes.txt"; // nome do arquivo

    carregar_clientes_do_arquivo(nome_arquivo);

    char nome[100], cpf[20], telefone[50];
    int opcao, ch;

    do {
        printf("\n== Cadastro de Clientes ==\n");

        printf("Digite o nome: \n");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = 0;

        printf("Digite o cpf: \n");
        fgets(cpf, sizeof(cpf), stdin);
        cpf[strcspn(cpf, "\n")] = 0;

        printf("Digite o número de telefone: \n");
        fgets(telefone, sizeof(telefone), stdin);
        telefone[strcspn(telefone, "\n")] = 0;

        adiciona_cliente(nome, cpf, telefone);

        printf("Deseja salvar outro cliente? (1 - sim / 2 - nao)\n");
        scanf("%d", &opcao);
        while ((ch = getchar()) != '\n' && ch != EOF); 

    } while (opcao == 1);

    salvar_clientes_no_arquivo(nome_arquivo);

    free(lista);
    return 0;
}

void menuClientes() { //funcao menu no arquivo
    int opcao;

    do {
        printf("\n=== Menu de Clientes ===\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Atualizar Cliente\n");
        printf("3 - Listar Clientes\n");
        printf("4 - Remover Cliente\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            case 1:
                cadastroClientes();
                break;
            case 2:
                printf("Função de atualizar cliente.\n");
                break;
            case 3:
                printf("Função de listar cliente.\n");
                break;
            case 4:
                printf("Função de remover cliente.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);
}
