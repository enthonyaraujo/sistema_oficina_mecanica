#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include "../include/clientes.h"
#include "../include/structs.h"
#include "../include/validar_cpf.h"


// Função para checar se um CPF já existe na lista de clientes
int cpf_duplicado(const char *cpf, const cliente *lista, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            return 1; // CPF já existe
        }
    }
    return 0; // CPF não encontrado
}


// Função para imprimir todos os clientes da lista
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

// Função para buscar um cliente pelo CPF na lista
cliente* buscar_cliente_na_lista(const char* cpf, const cliente* lista, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            return (cliente*)&lista[i]; // Retorna ponteiro para o cliente encontrado
        }
    }
    return NULL; 
}


// Função para adicionar um novo cliente à lista
void adiciona_cliente(cliente **lista_ptr, int *count_ptr, const cliente *novoCliente) {
    // Realoca a lista para o novo tamanho
    *lista_ptr = realloc(*lista_ptr, sizeof(cliente) * (*count_ptr + 1));
    if (*lista_ptr == NULL) {
        perror("Falha ao alocar memoria para novo cliente"); // Erro de alocação
        exit(1);
    }
    (*lista_ptr)[*count_ptr] = *novoCliente; // Adiciona novo cliente
    (*count_ptr)++;
}


// Função para remover um cliente da lista usando CPF
void remover_cliente_por_cpf(cliente **lista_ptr, int *count_ptr) {
    if (*count_ptr == 0) {
        printf("Nenhum cliente cadastrado para remover.\n");
        return;
    }

    char cpf_busca[20];
    printf("Digite o CPF do cliente que deseja remover: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin); // Recebe CPF para busca
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;    // Remove o '\n' do final

    // Procura o cliente pelo CPF
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

    // Remove o cliente deslocando os próximos para trás
    for (int i = pos; i < *count_ptr - 1; i++) {
        (*lista_ptr)[i] = (*lista_ptr)[i + 1];
    }
    (*count_ptr)--;

    // Reduz memória alocada ou libera se não sobrar nenhum cliente
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


// Função para atualizar nome do cliente por CPF
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
            printf("\nCliente encontrado!\n");
            printf("Nome atual: %s\n", lista[i].nome);
            printf("CPF atual: %s\n", lista[i].cpf);
            printf("Telefone atual: %s\n\n", lista[i].telefone);

            // Atualizar nome
            printf("Digite o novo nome (ou Enter para manter): ");
            char novo_nome[100];
            fgets(novo_nome, sizeof(novo_nome), stdin);
            if (novo_nome[0] != '\n') {
                novo_nome[strcspn(novo_nome, "\n")] = 0;
                strcpy(lista[i].nome, novo_nome);
            }

            // Atualizar CPF
            while (1) {
                printf("Digite o novo CPF (11 dígitos, ou Enter para manter): ");
                char novo_cpf[20];
                fgets(novo_cpf, sizeof(novo_cpf), stdin);

                if (novo_cpf[0] == '\n') break; // mantem o atual
                novo_cpf[strcspn(novo_cpf, "\n")] = 0;

                int valido = 1;
                if (strlen(novo_cpf) != 11) valido = 0;
                for (int j = 0; j < strlen(novo_cpf); j++) {
                    if (!isdigit((unsigned char)novo_cpf[j])) {
                        valido = 0;
                        break;
                    }
                }

                if (valido) {
                    strcpy(lista[i].cpf, novo_cpf);
                    break;
                } else {
                    printf("CPF inválido! Deve conter exatamente 11 números.\n");
                }
            }

            // Atualizar telefone
            while (1) {
                printf("Digite o novo numero de telefone (DD + numero com o primeiro 9, ou Enter para manter): ");
                char novo_tel[50];
                fgets(novo_tel, sizeof(novo_tel), stdin);

                if (novo_tel[0] == '\n') break; // mantem o atual
                novo_tel[strcspn(novo_tel, "\n")] = 0;

                int valido = 1;
                if (strlen(novo_tel) != 11) valido = 0;
                for (int j = 0; j < strlen(novo_tel); j++) {
                    if (!isdigit((unsigned char)novo_tel[j])) {
                        valido = 0;
                        break;
                    }
                }

                if (valido) {
                    strcpy(lista[i].telefone, novo_tel);
                    break;
                } else {
                    printf("Numero invalido, forneca um numero com 11 digitos Ex:23123456789.\n");
                }
            }

            printf("\nCliente atualizado com sucesso!\n");
            return;
        }
    }

    printf("Cliente com CPF %s não encontrado.\n", cpf_busca);
}


// Função para carregar clientes do arquivo
cliente* carregar_clientes_do_arquivo(int *count_ptr, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r"); // Abre arquivo para leitura
    if (!arquivo) {
        *count_ptr = 0;
        return NULL;
    }

    cliente *lista = NULL;
    *count_ptr = 0;
    char linha[256];
    cliente temp;
    int step = 0;

    // Lê linha por linha e monta cada cliente a partir de 3 linhas (nome, CPF, telefone)
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
    fclose(arquivo); // Fecha arquivo após leitura
    return lista;
}


// Função para salvar lista de clientes em arquivo
void salvar_clientes_no_arquivo(const cliente *lista, int count, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        // Escreve os dados de cada cliente formatados linha a linha
        fprintf(arquivo, "Nome: %s\n", lista[i].nome);
        fprintf(arquivo, "CPF: %s\n", lista[i].cpf);
        fprintf(arquivo, "Telefone: %s\n\n", lista[i].telefone);
    }
    fclose(arquivo); // Fecha o arquivo após escrita
}


// Função para cadastrar clientes pelo usuário
void cadastro_clientes(cliente **lista_ptr, int *count_ptr) {
    cliente novoCliente;
    int opcao, ch;

    do {
        printf("\n== Cadastro de Novo Cliente ==\n");

        // Solicita informações do cliente
        printf("Digite o nome: ");
        fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
        novoCliente.nome[strcspn(novoCliente.nome, "\n")] = 0;

        // Solicita CPF e valida se já existe
        while (1) {
            int cpf_valido = 1;

            printf("Digite o CPF (11 digitos, sem pontos): ");
            fgets(novoCliente.cpf, sizeof(novoCliente.cpf), stdin);
            novoCliente.cpf[strcspn(novoCliente.cpf, "\n")] = 0;

            // Verifica se tem 11 dígitos
            if (strlen(novoCliente.cpf) != 11) {
                printf("Erro: O CPF deve conter exatamente 11 digitos.\n");
                continue;
            }

            // verifica se contem apenas números
            for (int i = 0; i < 11; i++) {
                if (!isdigit((unsigned char)novoCliente.cpf[i])) {
                    cpf_valido = 0;
                    break;
                }
            }

            if (!cpf_valido) {
                printf("Erro: O CPF deve conter apenas números.\n");
                continue;
            }

            // Verifica duplicação
            if (cpf_duplicado(novoCliente.cpf, *lista_ptr, *count_ptr)) {
                printf("Erro: CPF ja cadastrado.\n");
            } else {
                break; // CPF válido e não duplicado
            }
        }

        ler_cpf(novoCliente.cpf); // Chamada à função para validação do CPF

        // Solicita telefone e valida se contém apenas números
        while (1) {
            int telefone_valido = 1;

            printf("Digite o telefone (DD + numero com o primeiro 9, somente numeros): ");
            fgets(novoCliente.telefone, sizeof(novoCliente.telefone), stdin);
            novoCliente.telefone[strcspn(novoCliente.telefone, "\n")] = 0;

            // garante que terá 11 digitos
            if (strlen(novoCliente.telefone) != 11) {
                printf("Numero invalido, forneca um numero com 11 digitos Ex:23123456789");
                continue;
            }

            // Verifica se está vazio
            if (strlen(novoCliente.telefone) == 0) {
                printf("Erro: O telefone nao pode estar vazio.\n");
                continue;
            }

            // verifica se contem apenas números
            for (size_t i = 0; i < strlen(novoCliente.telefone); i++) {
                if (!isdigit((unsigned char)novoCliente.telefone[i])) {
                    telefone_valido = 0;
                    break;
                }
            }

            if (!telefone_valido) {
                printf("Erro: O telefone deve conter apenas numeros.\n");
                continue;
            }

            break; // Telefone válido
        }

        // Adiciona cliente à lista
        adiciona_cliente(lista_ptr, count_ptr, &novoCliente);
        printf("Cliente '%s' cadastrado com sucesso!\n", novoCliente.nome);

        // Pergunta se deseja cadastrar mais um cliente
        printf("\nDeseja cadastrar outro cliente? (1 - Sim / 0 - Nao): ");
        scanf("%d", &opcao);
        while ((ch = getchar()) != '\n' && ch != EOF);

    } while (opcao == 1);
}


// Menu de operações para clientes
void menuClientes(cliente **lista_ptr, int *count_ptr) {
    int opcao;
    do {
        printf("\n===== MENU DE CLIENTES =====\n");
        printf("[1] Cadastrar Cliente\n");
        printf("[2] Atualizar Cliente\n");
        printf("[3] Listar Clientes\n");
        printf("[4] Remover Cliente\n");
        printf("[0] Voltar ao Menu Principal\n");
        printf("> ");

        scanf("%d", &opcao);
        while (getchar() != '\n');
        switch (opcao) {
            case 0:
                printf("\nVoltando ao menu principal...\n");
                break;
            case 1:
                cadastro_clientes(lista_ptr, count_ptr);        // Cadastro de novo cliente
                break;
            case 2:
                atualizar_cliente(*lista_ptr, *count_ptr);     // Atualização de cliente existente
                break;
            case 3:
                imprimir_clientes(*lista_ptr, *count_ptr);     // Listagem completa dos clientes
                break;
            case 4:
                remover_cliente_por_cpf(lista_ptr, count_ptr); // Remoção de cliente por CPF
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");  
                break;
        }
    } while (opcao != 0);
}
