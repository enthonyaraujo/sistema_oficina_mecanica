#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/clientes.h"


veiculo *lista_veiculos = NULL;
int total_veiculos = 0;

extern veiculo *lista_veiculos; 
extern int total_veiculos;   

veiculo* buscar_veiculo_por_placa(veiculo *veiculos, int total, const char *placa) {
    for (int i = 0; i < total; i++) {
        if (strcasecmp(veiculos[i].placa, placa) == 0) { 
            return &veiculos[i];
        }
    }
    return NULL;
}

void remover_veiculo(veiculo **veiculos, int *total) {
    if (*veiculos == NULL || *total == 0) {
        printf("Nenhum veículo para remover.\n");
        return;
    }

    char placa[20];
    printf("Digite a placa do veículo para remover: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = 0;

    int pos = -1;
    for (int i = 0; i < *total; i++) {
        if (strcmp((*veiculos)[i].placa, placa) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Veículo não encontrado.\n");
        return;
    }

    for (int i = pos; i < *total - 1; i++) {
        (*veiculos)[i] = (*veiculos)[i + 1];
    }

    (*total)--;

    if (*total > 0) {
        veiculo *temp = realloc(*veiculos, (*total) * sizeof(veiculo));
        if (temp)
            *veiculos = temp;
    } else {
        free(*veiculos);
        *veiculos = NULL;
    }

    salvar_todos_veiculos(*veiculos, *total);
    printf("Veículo removido com sucesso.\n");
}


void atualizar_veiculo(veiculo *veiculos, int totalVeiculos) {
    if (veiculos == NULL || totalVeiculos == 0) {
        printf("Nenhum veículo cadastrado para atualizar.\n");
        return;
    }

    char placa_buscar[20];
    printf("Digite a placa do veículo que deseja atualizar: ");
    fgets(placa_buscar, sizeof(placa_buscar), stdin);
    placa_buscar[strcspn(placa_buscar, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < totalVeiculos; i++) {
        if (strcmp(veiculos[i].placa, placa_buscar) == 0) {
            encontrado = 1;
            printf("Veículo encontrado:\n");
            printf(" Placa: %s\n", veiculos[i].placa);
            printf(" Modelo: %s\n", veiculos[i].modelo);
            printf(" Ano: %d\n", veiculos[i].ano);
            printf(" Cliente: %s\n", veiculos[i].clientePtr->nome);

            printf("Digite o novo modelo (Enter para manter atual): ");
            char novo_modelo[50];
            fgets(novo_modelo, sizeof(novo_modelo), stdin);
            if (novo_modelo[0] != '\n') {
                novo_modelo[strcspn(novo_modelo, "\n")] = 0;
                strcpy(veiculos[i].modelo, novo_modelo);
            }

            printf("Digite o novo ano (0 para manter atual): ");
            int novo_ano;
            scanf("%d", &novo_ano);
            getchar();
            if (novo_ano != 0) {
                veiculos[i].ano = novo_ano;
            }

            printf("Digite o novo CPF do cliente (Enter para manter atual): ");
            char novo_cpf[20];
            fgets(novo_cpf, sizeof(novo_cpf), stdin);
            if (novo_cpf[0] != '\n') {
                novo_cpf[strcspn(novo_cpf, "\n")] = 0;
                cliente *novo_cliente = buscar_cliente_por_cpf(novo_cpf);
                if (novo_cliente) {
                    veiculos[i].clientePtr = novo_cliente;
                } else {
                    printf("Cliente com CPF %s não encontrado. Cliente antigo mantido.\n", novo_cpf);
                }
            }

            salvar_todos_veiculos(veiculos, totalVeiculos);
            printf("Veículo atualizado com sucesso.\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Veículo com placa %s não encontrado.\n", placa_buscar);
    }
}


void imprimir_veiculos(veiculo *veiculos, int total) {
    if (veiculos == NULL || total == 0) {
        printf("Nenhum veículo cadastrado.\n");
        return;
    }

    printf("\n== Lista de Veículos Cadastrados ==\n");
    for (int i = 0; i < total; i++) {
        printf("Veículo %d:\n", i + 1);
        printf(" Placa: %s\n", veiculos[i].placa);
        printf(" Modelo: %s\n", veiculos[i].modelo);
        printf(" Ano: %d\n", veiculos[i].ano);
        printf(" Cliente: %s\n", veiculos[i].clientePtr->nome);
        printf("\n");
    }
}


//cliente* buscar_cliente_por_cpf(const char *cpf_procurado);

void salvar_todos_veiculos(veiculo *veiculos, int total) {
    FILE *arquivo = fopen("data/veiculos.txt", "w");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de veículos");
        return;
    }

    printf("Salvando %d veículos...\n", total);

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "Placa: %s\n", veiculos[i].placa);
        fprintf(arquivo, "Modelo: %s\n", veiculos[i].modelo);
        fprintf(arquivo, "Ano: %d\n", veiculos[i].ano);
        fprintf(arquivo, "CPF do cliente associado: %s\n\n", veiculos[i].clientePtr->cpf);
    }

    fclose(arquivo);
}

veiculo* carregar_veiculos(int *total) {
    FILE *arquivo = fopen("data/veiculos.txt", "r");
    if (!arquivo) return NULL;

    veiculo *veiculos = NULL;
    veiculo temp;
    cliente *cliente_associado;
    char linha[150];
    char cpf_temp[20];
    int leu_placa = 0, leu_modelo = 0, leu_ano = 0, leu_cpf = 0;

    *total = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strncmp(linha, "Placa: ", 7) == 0) {
            strncpy(temp.placa, linha + 7, sizeof(temp.placa));
            temp.placa[strcspn(temp.placa, "\n")] = 0;
            leu_placa = 1;
        } else if (strncmp(linha, "Modelo: ", 8) == 0) {
            strncpy(temp.modelo, linha + 8, sizeof(temp.modelo));
            temp.modelo[strcspn(temp.modelo, "\n")] = 0;
            leu_modelo = 1;
        } else if (strncmp(linha, "Ano: ", 5) == 0) {
            temp.ano = atoi(linha + 5);
            leu_ano = 1;
        } else if (strncmp(linha, "CPF do cliente associado: ", 26) == 0) {
            strncpy(cpf_temp, linha + 26, sizeof(cpf_temp));
            cpf_temp[strcspn(cpf_temp, "\n")] = 0;
            cliente_associado = buscar_cliente_por_cpf(cpf_temp);
            if (!cliente_associado) continue;
            temp.clientePtr = cliente_associado;
            leu_cpf = 1;
        }

        if (leu_placa && leu_modelo && leu_ano && leu_cpf) {
            veiculo *p = realloc(veiculos, (*total + 1) * sizeof(veiculo));
            if (!p) {
                free(veiculos);
                fclose(arquivo);
                return NULL;
            }
            veiculos = p;
            veiculos[*total] = temp;
            (*total)++;
            leu_placa = leu_modelo = leu_ano = leu_cpf = 0;
        }
    }
    fclose(arquivo);
    return veiculos;
}

cliente* buscar_cliente_por_cpf(const char *cpf_procurado) {
    FILE *arquivo = fopen("data/clientes.txt", "r");
    if (!arquivo) return NULL;

    cliente *cli = malloc(sizeof(cliente));
    if (!cli) {
        fclose(arquivo);
        return NULL;
    }

    char linha[150];
    int leu_nome = 0, leu_cpf = 0, leu_telefone = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strncmp(linha, "Nome: ", 6) == 0) {
            strncpy(cli->nome, linha + 6, sizeof(cli->nome));
            cli->nome[strcspn(cli->nome, "\n")] = 0;
            leu_nome = 1;
        } else if (strncmp(linha, "CPF: ", 5) == 0) {
            strncpy(cli->cpf, linha + 5, sizeof(cli->cpf));
            cli->cpf[strcspn(cli->cpf, "\n")] = 0;
            leu_cpf = 1;
        } else if (strncmp(linha, "Telefone: ", 9) == 0) {
            strncpy(cli->telefone, linha + 9, sizeof(cli->telefone));
            cli->telefone[strcspn(cli->telefone, "\n")] = 0;
            leu_telefone = 1;
        }
        if (leu_nome && leu_cpf && leu_telefone) {
            if (strcmp(cli->cpf, cpf_procurado) == 0) {
                fclose(arquivo);
                return cli;
            }
            leu_nome = leu_cpf = leu_telefone = 0;
        }
    }
    fclose(arquivo);
    free(cli);
    return NULL;
}

int validar_placa(char *placa) {
    int contLetra = 0;
    int contNum = 0;
    if (strlen(placa) != 7) return 0;
    for (int i = 0; i < 7; i++) {
        char c = placa[i];
        if (isalpha((unsigned char)c)) {
            if (i == 0 || i == 1 || i == 2 || i == 4) {
                contLetra++;
            } else {
                return 0; 
            }
        } else if (isdigit((unsigned char)c)) {
            if (i == 3 || i == 5 || i == 6) {
                contNum++;
            } else if (i == 4 && contLetra == 3) {
                contNum++;
            } else {
                return 0; 
            }
        } else {
            return 0; 
        }
    }
    if ((contLetra == 3 && contNum == 4) || (contLetra == 4 && contNum == 3)) {
        return 1; 
    }
    return 0;
}

int ano_valido(int ano) {
    if (ano < 1900) return 0;

    // pega o ano atual do sistema
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int ano_atual = tm_info->tm_year + 1900;

    if (ano > ano_atual) return 0;

    return 1;
}

void cadastro_veiculos(veiculo **veiculos, int *totalVeiculos) {
    int opcao, ch;
    do
    {
        veiculo novoVeiculo;
        char cpf_input[20];

        printf("Digite a placa do veículo: ");
        fgets(novoVeiculo.placa, sizeof(novoVeiculo.placa), stdin);
        novoVeiculo.placa[strcspn(novoVeiculo.placa, "\n")] = 0;

        if (!validar_placa(novoVeiculo.placa)) {
            printf("Placa inválida. Tente novamente.\n");
            continue;
        }

        printf("Digite o modelo do veículo: ");
        fgets(novoVeiculo.modelo, sizeof(novoVeiculo.modelo), stdin);
        novoVeiculo.modelo[strcspn(novoVeiculo.modelo, "\n")] = 0;

        printf("Digite o ano do veículo: ");
        scanf("%d", &novoVeiculo.ano);
        getchar();
        if (!ano_valido(novoVeiculo.ano)) {
            printf("Ano inválido. Tente novamente.\n");
            continue;
        }

        printf("Digite o CPF do Cliente associado: ");
        fgets(cpf_input, sizeof(cpf_input), stdin);
        cpf_input[strcspn(cpf_input, "\n")] = 0;

        novoVeiculo.clientePtr = buscar_cliente_por_cpf(cpf_input);
        if (!novoVeiculo.clientePtr) {
            printf("Cliente não encontrado.\n");
            continue;
        }

        veiculo *p = realloc(*veiculos, (*totalVeiculos + 1) * sizeof(veiculo));
        if (!p) {
            printf("Erro de memória.\n");
            break;
        }
        *veiculos = p;
        (*veiculos)[*totalVeiculos] = novoVeiculo;
        (*totalVeiculos)++;

        salvar_todos_veiculos(*veiculos, *totalVeiculos);

        printf("Veículo cadastrado com sucesso.\n");

        printf("Deseja cadastrar outro veículo? (1 - Sim / 0 - Não): "); 
        scanf("%d", &opcao);
        while ((ch = getchar()) != '\n' && ch != EOF);
    } while (opcao == 1);
    
}


void menuVeiculos() {
    int totalVeiculos = 0;
    veiculo *lista_veiculos = carregar_veiculos(&totalVeiculos);

    int opcao;
    do {
        printf("\n== MENU DE VEICULOS ==\n");
        printf("1 - Cadastro de Veiculos\n");
        printf("2 - Atualizar Veiculos\n");
        printf("3 - Listar Veiculos\n");
        printf("4 - Remover Veiculos\n");
        printf("0 - Voltar\n> ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                cadastro_veiculos(&lista_veiculos, &totalVeiculos);
                break;
            case 2:
                atualizar_veiculo(lista_veiculos, totalVeiculos);
                break;
            case 3:
                imprimir_veiculos(lista_veiculos, totalVeiculos);
                break;
            case 4:
                remover_veiculo(&lista_veiculos, &totalVeiculos);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    salvar_todos_veiculos(lista_veiculos, totalVeiculos);
    free(lista_veiculos);
}
