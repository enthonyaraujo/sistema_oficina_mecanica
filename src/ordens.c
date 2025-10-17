#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "../include/ordens.h"
#include "../include/structs.h"
#include "../include/veiculos.h"
#include "../include/clientes.h"


int ano_bissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int validar_data(const char *data_str) {
    int dia, mes, ano;

    if (sscanf(data_str, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0;
    }

    if (ano < 1900 || ano > 2100 || mes < 1 || mes > 12 || dia < 1) {
        return 0;
    }

    int dias_no_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (ano_bissexto(ano)) {
        dias_no_mes[2] = 29;
    }

    if (dia > dias_no_mes[mes]) {
        return 0;
    }

    return 1;
}


// Converte enum statusOrdem para string correspondente
const char* status_para_string(statusOrdem s) {
    switch (s) {
        case AGUARDANDO_AVALIACAO: return "Aguardando Avaliacao";
        case EM_REPARO: return "Em Reparo";
        case FINALIZADO: return "Finalizado";
        case ENTREGUE: return "Entregue";
        default: return "Desconhecido";
    }
}

// Converte string de status para enum statusOrdem
statusOrdem string_para_status(const char *str) {
    if (strcmp(str, "Aguardando Avaliacao") == 0) return AGUARDANDO_AVALIACAO;
    if (strcmp(str, "Em Reparo") == 0) return EM_REPARO;
    if (strcmp(str, "Finalizado") == 0) return FINALIZADO;
    if (strcmp(str, "Entregue") == 0) return ENTREGUE;
    return AGUARDANDO_AVALIACAO;
}

// Imprime a lista completa de ordens de serviço
void listar_todas(const ordemServico *ordens, int total) {
    if (total == 0) {
        printf("\nNenhuma ordem de servico cadastrada.\n");
        return;
    }
    printf("\n--- Lista de Todas as Ordens de Servico ---\n");
    for (int i = 0; i < total; i++) {
        if (ordens[i].veiculo) {
            printf("ID: %-4d | Placa: %-8s | Status: %s\n",
                   ordens[i].idOrdem,
                   ordens[i].veiculo->placa,
                   status_para_string(ordens[i].status));
        }
    }
}

// Carrega ordens de serviço do arquivo, associando-as a veículos existentes
ordemServico *carregar_ordens_de_arquivo(int *total_ptr, const char *arquivo, veiculo *lista_veiculos, int total_veiculos) {
    FILE *file = fopen(arquivo, "r");
    if (!file) {
        *total_ptr = 0;
        return NULL;
    }

    ordemServico *ordens = NULL;
    *total_ptr = 0;
    ordemServico temp;
    char placaTmp[10], statusTmp[50];

    // Loop para ler cada ordem do arquivo, campo a campo
    while (fscanf(file, "%d;%9[^;];%19[^;];%255[^;];%49[^\n]\n",
                  &temp.idOrdem, placaTmp, temp.dataEntrada, temp.descricaoProblema, statusTmp) == 5) {

        temp.veiculo = NULL;
        // Procura veículo correspondente pela placa
        for (int j = 0; j < total_veiculos; j++) {
            if (strcmp(lista_veiculos[j].placa, placaTmp) == 0) {
                temp.veiculo = &lista_veiculos[j];
                break;
            }
        }

        // Se não encontrou veículo, exibe aviso e descarta a ordem lida
        if (!temp.veiculo) {
            printf("Aviso: Veiculo com placa %s nao encontrado. Ordem de servico ID %d nao pode ser carregada corretamente.\n", placaTmp, temp.idOrdem);
            continue;
        }

        temp.status = string_para_status(statusTmp); // Converte status lido para enum
        
        // Realoca vetor e adiciona a ordem carregada
        (*total_ptr)++;
        ordens = realloc(ordens, (*total_ptr) * sizeof(ordemServico));
        ordens[*total_ptr - 1] = temp;
    }
    fclose(file);
    return ordens;
}

// Salva todas as ordens de serviço no arquivo texto, uma por linha
void salvar_ordens_em_arquivo(const ordemServico *ordens, int total, const char *arquivo) {
    FILE *file = fopen(arquivo, "w");
    if (!file) {
        printf("Erro ao salvar arquivo de ordens.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        if (ordens[i].veiculo) {
            fprintf(file, "%d;%s;%s;%s;%s\n",
                    ordens[i].idOrdem,
                    ordens[i].veiculo->placa,
                    ordens[i].dataEntrada,
                    ordens[i].descricaoProblema,
                    status_para_string(ordens[i].status));
        }
    }
    fclose(file);
}

// Abre (registra) uma nova ordem de serviço para um veículo existente
void abrir_ordem(ordemServico **ordens_ptr, int *total_ptr, veiculo *lista_veiculos, int total_veiculos) {
    if (total_veiculos == 0) {
        printf("\nErro: Nao ha veiculos cadastrados. Cadastre um veiculo primeiro.\n");
        return;
    }

    ordemServico nova;
    // Geração do ID automático (incrementa último ID)
    nova.idOrdem = (*total_ptr == 0) ? 1 : (*ordens_ptr)[*total_ptr - 1].idOrdem + 1;

    char placaBusca[10];
    printf("\n=== ABRIR NOVA ORDEM DE SERVICO ===\n");
    printf("Digite a placa do veiculo: ");
    fgets(placaBusca, sizeof(placaBusca), stdin);
    placaBusca[strcspn(placaBusca, "\n")] = 0;

    // Procura veículo informado
    veiculo *encontrado = NULL;
    for (int i = 0; i < total_veiculos; i++) {
        if (strcasecmp(lista_veiculos[i].placa, placaBusca) == 0) {
            encontrado = &lista_veiculos[i];
            break;
        }
    }

    if (!encontrado) {
        printf("Erro: Veiculo com placa %s nao encontrado.\n", placaBusca);
        return;
    }

    nova.veiculo = encontrado;

    //verificação da data
    while (1) {
        printf("Data de entrada (dd/mm/aaaa): ");
        fgets(nova.dataEntrada, sizeof(nova.dataEntrada), stdin);
        nova.dataEntrada[strcspn(nova.dataEntrada, "\n")] = 0;

        if (validar_data(nova.dataEntrada)) {
            break;
        } else {
            printf("Data invalida! Por favor, insira uma data valida.\n");
        }
    }

    printf("Descricao do problema: ");
    fgets(nova.descricaoProblema, sizeof(nova.descricaoProblema), stdin);
    nova.descricaoProblema[strcspn(nova.descricaoProblema, "\n")] = 0;
    
    nova.status = AGUARDANDO_AVALIACAO; // Status inicial padrão

    // Adiciona a ordem na lista (realoca e incrementa o ponteiro)
    (*total_ptr)++;
    *ordens_ptr = realloc(*ordens_ptr, (*total_ptr) * sizeof(ordemServico));
    (*ordens_ptr)[*total_ptr - 1] = nova;

    printf("Ordem de servico aberta com sucesso (ID %d) para o veiculo %s.\n", nova.idOrdem, nova.veiculo->placa);
}

// Atualiza o status de uma ordem de serviço existente pelo ID
void atualizar_ordem(ordemServico *ordens, int total) {
    if (total == 0) {
        printf("Nenhuma ordem cadastrada.\n");
        return;
    }
    int id, opc;
    printf("\nDigite o ID da ordem para atualizar: ");
    scanf("%d", &id);
    getchar(); // Limpa o buffer de entrada

    for (int i = 0; i < total; i++) {
        if (ordens[i].idOrdem == id) {
            printf("Status atual: %s\n", status_para_string(ordens[i].status));
            printf("Selecione novo status:\n");
            printf("1. Aguardando Avaliacao\n2. Em Reparo\n3. Finalizado\n4. Entregue\n> ");
            scanf("%d", &opc);
            getchar();

            if (opc >= 1 && opc <= 4) {
                ordens[i].status = (statusOrdem)(opc - 1); // Atualiza status
                printf("Ordem atualizada!\n");
            } else {
                printf("Opcao de status invalida.\n");
            }
            return;
        }
    }
    printf("Ordem com ID %d nao encontrada.\n", id);
}

// Menu principal das operações relacionadas a ordens de serviço
void menuOrdens(ordemServico **ordens_ptr, int *totalOrdens_ptr, veiculo *lista_veiculos, int total_veiculos) {
    int opc;
    do {
        printf("\n===== MENU DE ORDENS DE SERVICO =====\n");
        printf("1. Abrir nova ordem\n");
        printf("2. Atualizar status da ordem\n");
        printf("3. Listar todas as ordens\n");
        printf("0. Voltar ao Menu Principal\n> ");
        scanf("%d", &opc);
        getchar(); // Limpa buffer

        switch (opc) {
            case 1:
                abrir_ordem(ordens_ptr, totalOrdens_ptr, lista_veiculos, total_veiculos);
                break;
            case 2:
                atualizar_ordem(*ordens_ptr, *totalOrdens_ptr);
                break;
            case 3:
                listar_todas(*ordens_ptr, *totalOrdens_ptr);
                break;
            case 0:
                printf("\nVoltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opc != 0);
}