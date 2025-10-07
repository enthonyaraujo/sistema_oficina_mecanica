#include <stdio.h>
#include <string.h>
#include "../include/clientes.h"
#include "../include/structs.h"
#include "../include/validar_cpf.h"

#define MAX_CLIENTES 100

void SalvarClientes(const clientes *listaClientes, int qtdClientes) {
    FILE *arquivo = fopen("data/clientes.txt", "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo clientes.txt");
        return;
    }

    for (int i = 0; i < qtdClientes; i++) {
        fprintf(arquivo, "%s,%s,%s\n",
                listaClientes[i].nome,
                listaClientes[i].cpf,
                listaClientes[i].telefone);
    }

    fclose(arquivo);
}

void ListarClientes() {
    FILE *arquivo = fopen("data/clientes.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo clientes.txt");
        return;
    }

    char linha[256];
    clientes cliente;

    printf("\n=== Lista de Clientes ===\n");

    while (fgets(linha, sizeof(linha), arquivo)) {
        // remove o '\n' do final
        linha[strcspn(linha, "\n")] = '\0';

        // separa por vírgulas
        char *token = strtok(linha, ",");
        if (token != NULL) strcpy(cliente.nome, token);

        token = strtok(NULL, ",");
        if (token != NULL) strcpy(cliente.cpf, token);

        token = strtok(NULL, ",");
        if (token != NULL) strcpy(cliente.telefone, token);

        // exibe formatado
        printf("NOME: %s\n", cliente.nome);
        printf("CPF: %s\n", cliente.cpf);
        printf("TELEFONE: %s\n\n", cliente.telefone);
    }

    fclose(arquivo);
}

void CadastrarCliente(clientes *listaClientes, int *qtdClientes) {
    int c, opcao_salvamento;
    clientes novoCliente;

    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("\n=== Cadastro de Cliente ===\n");

    printf("Digite o nome do cliente: ");
    fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
    novoCliente.nome[strcspn(novoCliente.nome, "\n")] = '\0';

    //printf("Digite o CPF (somente números): ");
    //fgets(novoCliente.cpf, sizeof(novoCliente.cpf), stdin);
    //ler_cpf(novoCliente.cpf);
    // verificar se o CPF deu como válido
    int cpf_valido = 0;
    do {
        printf("Digite o CPF (somente números): ");
        fgets(novoCliente.cpf, sizeof(novoCliente.cpf), stdin);
        novoCliente.cpf[strcspn(novoCliente.cpf, "\n")] = '\0';

        cpf_valido = ler_cpf(novoCliente.cpf); // agora ler_cpf retorna 1 se válido, 0 se inválido
        if (!cpf_valido) {
            printf("CPF inválido. Tente novamente.\n");
        }
    } while (!cpf_valido);

    printf("Digite o telefone (somente números): ");
    fgets(novoCliente.telefone, sizeof(novoCliente.telefone), stdin);
    novoCliente.telefone[strcspn(novoCliente.telefone, "\n")] = '\0';

    printf("\nCliente cadastrado com sucesso!\n");
    printf("Nome: %s\nCPF: %s\nTelefone: %s\n",
           novoCliente.nome, novoCliente.cpf, novoCliente.telefone);

    printf("Deseja salvar?\n1 - Sim\n2 - Não\n");
    scanf("%d", &opcao_salvamento);

    if (opcao_salvamento == 1) {
        // adiciona o cliente ao vetor
        listaClientes[*qtdClientes] = novoCliente;
        (*qtdClientes)++;

        // regrava todo o arquivo
        SalvarClientes(listaClientes, *qtdClientes);
    } else {
        printf("Cliente não salvo.\n");
    }
}

void MenuClientes() {
    int opcao;
    clientes listaClientes[MAX_CLIENTES];
    int qtdClientes = 0;

    do {
        printf("\n=== Menu de Clientes ===\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Atualizar Cliente\n");
        printf("3 - Listar Clientes\n");
        printf("4 - Remover Cliente\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida. Tente novamente.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        switch (opcao) {
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            case 1:
                CadastrarCliente(listaClientes, &qtdClientes);
                break;
            case 2:
                printf("Função de atualizar cliente.\n");
                break;
            case 3:
                ListarClientes();
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
