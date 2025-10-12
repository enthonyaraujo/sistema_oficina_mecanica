#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/structs.h"
#include "../include/validar_cpf.h"



int cpf_duplicado(const char *cpf, const cliente *lista, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            return 1; // CPF já existe
        }
    }
    return 0; // CPF não encontrado
}

void imprimir_clientes(const cliente *lista, int count) {
    if (count == 0) {
        printf("\nNenhum cliente cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Clientes Cadastrados ---\n");
    for (int i = 0; i < count; i++) {
        printf("Nome:     %s\n", lista[i].nome);
        printf("CPF:      %s\n", lista[i].cpf);
        printf("Telefone: %s\n", lista[i].telefone);
    }
}

cliente* buscar_cliente_na_lista(const char* cpf, const cliente* lista, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
          
            return (cliente*)&lista[i]; 
        }
    }
    return NULL; 
}

void adiciona_cliente(cliente **lista_ptr, int *count_ptr, const cliente *novoCliente) {
    *lista_ptr = realloc(*lista_ptr, sizeof(cliente) * (*count_ptr + 1));
    if (*lista_ptr == NULL) {
        perror("Falha ao alocar memoria para novo cliente");
        exit(1);
    }
    (*lista_ptr)[*count_ptr] = *novoCliente;
    (*count_ptr)++;
}

void remover_cliente_por_cpf(cliente **lista_ptr, int *count_ptr) {
    if (*count_ptr == 0) {
        printf("Nenhum cliente cadastrado para remover.\n");
        return;
    }

    char cpf_busca[20];
    printf("Digite o CPF do cliente que deseja remover: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;

    int pos = -1;
    for (int i = 0; i < *count_ptr; i++) {
        if (strcmp((*lista_ptr)[i].cpf, cpf_busca) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Cliente com CPF %s nao encontrado.\n", cpf_busca);
        return;
    }

   
    for (int i = pos; i < *count_ptr - 1; i++) {
        (*lista_ptr)[i] = (*lista_ptr)[i + 1];
    }
    (*count_ptr)--;

    if (*count_ptr > 0) {
        cliente *temp = realloc(*lista_ptr, sizeof(cliente) * (*count_ptr));
        if (temp != NULL) {
            *lista_ptr = temp;
        }
    } else {
        free(*lista_ptr);
        *lista_ptr = NULL;
    }
    printf("Cliente removido com sucesso.\n");
}

void atualizar_cliente(cliente *lista, int count) {
    if (count == 0) {
        printf("Nenhum cliente cadastrado para atualizar.\n");
        return;
    }
    char cpf_busca[20];
    printf("Digite o CPF do cliente que deseja atualizar: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf_busca) == 0) {
            printf("Cliente encontrado: %s\n", lista[i].nome);
            printf("Digite o novo nome (ou Enter para manter): ");
            char novo_nome[100];
            fgets(novo_nome, sizeof(novo_nome), stdin);
            if (novo_nome[0] != '\n') {
                novo_nome[strcspn(novo_nome, "\n")] = 0;
                strcpy(lista[i].nome, novo_nome);
            }
            
            printf("Cliente atualizado com sucesso.\n");
            return;
        }
    }
    printf("Cliente com CPF %s nao encontrado.\n", cpf_busca);
}


cliente* carregar_clientes_do_arquivo(int *count_ptr, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        *count_ptr = 0;
        return NULL;
    }

    cliente *lista = NULL;
    *count_ptr = 0;
    
    char linha[256];
    cliente temp;
    int step = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
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
            adiciona_cliente(&lista, count_ptr, &temp);
            step = 0;
        }
    }
    fclose(arquivo);
    return lista;
}

void salvar_clientes_no_arquivo(const cliente *lista, int count, const char *nome_arquivo) {
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



void cadastro_clientes(cliente **lista_ptr, int *count_ptr) {
    cliente novoCliente;
    int opcao, ch;
    do {
        printf("\n== Cadastro de Novo Cliente ==\n");
        printf("Digite o nome: ");
        fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
        novoCliente.nome[strcspn(novoCliente.nome, "\n")] = 0;

        while (1) {
            printf("Digite o CPF (11 digitos, sem pontos): ");
            fgets(novoCliente.cpf, sizeof(novoCliente.cpf), stdin);
            novoCliente.cpf[strcspn(novoCliente.cpf, "\n")] = 0;
            
            if (strlen(novoCliente.cpf) != 11) {
                printf("Erro: O CPF deve conter exatamente 11 digitos.\n");
                continue;
            }
            
            if (cpf_duplicado(novoCliente.cpf, *lista_ptr, *count_ptr)) {
                printf("Erro: CPF ja cadastrado.\n");
            } else {
                break;
            }
        }
        
        ler_cpf(novoCliente.cpf); // Validação do CPF

        printf("Digite o telefone: ");
        fgets(novoCliente.telefone, sizeof(novoCliente.telefone), stdin);
        novoCliente.telefone[strcspn(novoCliente.telefone, "\n")] = 0;

        
        adiciona_cliente(lista_ptr, count_ptr, &novoCliente);
        printf("Cliente '%s' cadastrado com sucesso!\n", novoCliente.nome);

        printf("\nDeseja cadastrar outro cliente? (1 - Sim / 0 - Nao): ");
        scanf("%d", &opcao);
        while ((ch = getchar()) != '\n' && ch != EOF);

    } while (opcao == 1);
}

void menuClientes(cliente **lista_ptr, int *count_ptr) {
    int opcao;
    do {
        printf("\n=== MENU DE CLIENTES ===\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Atualizar Cliente\n");
        printf("3 - Listar Clientes\n");
        printf("4 - Remover Cliente\n");
        printf("0 - Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: > ");

        scanf("%d", &opcao);
        while (getchar() != '\n'); 
        switch (opcao) {
            case 0:
                printf("\nVoltando ao menu principal...\n");
                break;
            case 1:
                cadastro_clientes(lista_ptr, count_ptr);
                break;
            case 2:
                atualizar_cliente(*lista_ptr, *count_ptr);
                break;
            case 3:
                imprimir_clientes(*lista_ptr, *count_ptr);
                break;
            case 4:
                remover_cliente_por_cpf(lista_ptr, count_ptr);
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
}