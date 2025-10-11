#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/ordens.h"
#include "../include/veiculos.h"
#include "../include/structs.h"

// função para cadastrar uma nova ordem
void cadastrar_ordem(ordemServico **ordens, int *total) {
    ordemServico nova;
    nova.idOrdem = (*total) + 1;
    
    char placa_input[20];
    printf("Digite a placa do veículo para a ordem: ");
    fgets(placa_input, sizeof(placa_input), stdin);
    placa_input[strcspn(placa_input, "\n")] = 0;

    for (int i = 0; placa_input[i]; i++) {
        placa_input[i] = toupper((unsigned char)placa_input[i]);
    }

    veiculo *veiculo_associado = buscar_veiculo_por_placa(placa_input);
    if (!veiculo_associado) {
        printf("Veículo não encontrado.\n");
        return;
    }

    nova.veiculo = veiculo_associado;

    printf("Digite a data de entrada (dd/mm/aaaa): ");
    fgets(nova.dataEntrada, sizeof(nova.dataEntrada), stdin);
    nova.dataEntrada[strcspn(nova.dataEntrada, "\n")] = 0;

    printf("Descreva o problema: ");
    fgets(nova.descricaoProblema, sizeof(nova.descricaoProblema), stdin);
    nova.descricaoProblema[strcspn(nova.descricaoProblema, "\n")] = 0;

    nova.status = AGUARDANDO_AVALIACAO;

    ordemServico *p = realloc(*ordens, (*total + 1) * sizeof(ordemServico));
    if (!p) {
        printf("Erro de memória.\n");
        return;
    }
    *ordens = p;
    (*ordens)[*total] = nova;
    (*total)++;

    printf("Ordem de serviço cadastrada com ID: %d\n", nova.idOrdem);
}

// Lista todas as ordens
void listar_ordens(const ordemServico *ordens, int total) {
    if (total == 0) {
        printf("Nenhuma ordem cadastrada.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("ID: %d\n", ordens[i].idOrdem);
        printf("Veículo (placa): %s\n", ordens[i].veiculo->placa);
        printf("Data de entrada: %s\n", ordens[i].dataEntrada);
        printf("Descrição: %s\n", ordens[i].descricaoProblema);
        printf("Status: ");
        switch (ordens[i].status) {
            case AGUARDANDO_AVALIACAO: printf("Aguardando Avaliação\n"); break;
            case EM_REPARO: printf("Em Reparo\n"); break;
            case FINALIZADO: printf("Finalizado\n"); break;
            case ENTREGUE: printf("Entregue\n"); break;
        }
        printf("-------------------------\n");
    }
}

// Atualiza status da ordem
void atualizar_status_ordem(ordemServico *ordens, int total) {
    if (total == 0) {
        printf("Nenhuma ordem para atualizar.\n");
        return;
    }
    int id, nova_status;
    printf("Digite o ID da ordem que deseja atualizar: ");
    scanf("%d", &id);
    getchar();

    int pos = -1;
    for (int i = 0; i < total; i++) {
        if (ordens[i].idOrdem == id) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("Ordem não encontrada.\n");
        return;
    }

    printf("Escolha o novo status:\n");
    printf("0 - Aguardando Avaliação\n1 - Em Reparo\n2 - Finalizado\n3 - Entregue\n");
    scanf("%d", &nova_status);
    getchar();

    if (nova_status < 0 || nova_status > 3) {
        printf("Status inválido.\n");
        return;
    }

    ordens[pos].status = (statusOrdem)nova_status;
    printf("Status atualizado com sucesso.\n");
}

// alvar ordens em arquivo
void salvar_ordens_em_arquivo(const ordemServico *ordens, int total, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar ordens.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "ID:%d\n", ordens[i].idOrdem);
        fprintf(arquivo, "Placa:%s\n", ordens[i].veiculo->placa);
        fprintf(arquivo, "DataEntrada:%s\n", ordens[i].dataEntrada);
        fprintf(arquivo, "Descricao:%s\n", ordens[i].descricaoProblema);
        fprintf(arquivo, "Status:%d\n\n", ordens[i].status);
    }

    fclose(arquivo);
}

// ordens do arquivo
ordemServico* carregar_ordens_de_arquivo(int *total, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) return NULL;

    ordemServico *ordens = NULL;
    ordemServico temp;
    char linha[300];
    char placa[20];
    int linha_campo = 0;

    *total = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha_campo++;
        if (linha_campo == 1) { // ID
            sscanf(linha, "ID:%d", &temp.idOrdem);
        } else if (linha_campo == 2) { // Placa
            sscanf(linha, "Placa:%s", placa);
            temp.veiculo = buscar_veiculo_por_placa(placa);
            if (!temp.veiculo) {
                // Veículo não encontrado, pular essa ordem
                while (fgets(linha, sizeof(linha), arquivo) && strcmp(linha, "\n") != 0);
                linha_campo = 0;
                continue;
            }
        } else if (linha_campo == 3) { // Data Entrada
            sscanf(linha, "DataEntrada:%10s", temp.dataEntrada);
        } else if (linha_campo == 4) { // Descrição
            sscanf(linha, "Descricao:%255[^\n]", temp.descricaoProblema);
        } else if (linha_campo == 5) { // Status
            int statusInt;
            sscanf(linha, "Status:%d", &statusInt);
            temp.status = (statusOrdem)statusInt;

            ordemServico *p = realloc(ordens, (*total + 1) * sizeof(ordemServico));
            if (!p) {
                free(ordens);
                fclose(arquivo);
                return NULL;
            }
            ordens = p;
            ordens[*total] = temp;
            (*total)++;
            linha_campo = 0;
        }
    }
    fclose(arquivo);
    return ordens;
}


// listar ordens por veículo utilizando a placa
void listar_ordens_por_veiculo(const ordemServico *ordens, int total, const char *placa) {
    int achou = 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(ordens[i].veiculo->placa, placa) == 0) {
            achou = 1;
            printf("ID: %d | Cliente: %s | Data: %s | Status: %d | Descrição: %s\n",
                   ordens[i].idOrdem, ordens[i].veiculo->clientePtr->nome, ordens[i].dataEntrada,
                   ordens[i].status, ordens[i].descricaoProblema);
        }
    }
    if (!achou) printf("Nenhuma ordem encontrada para a placa %s.\n", placa);
}


void menuOrdens() {

    ordemServico *ordens = NULL;
    int totalOrdens = 0;
    const char *arquivo_ordens = "data/ordens.txt";

    ordens = carregar_ordens_de_arquivo(&totalOrdens, arquivo_ordens);
    if (ordens == NULL) totalOrdens = 0;

    int opcao = 0;
    do {
        printf("\n== Menu de Ordens de Serviço ==\n");
        printf("1 - Cadastrar ordem\n");
        printf("2 - Listar todas as ordens\n");
        printf("3 - Atualizar status de ordem\n");
        printf("4 - Listar ordens por veículo (placa)\n");
        printf("0 - Voltar\n");
        printf("Escolha a opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                cadastrar_ordem(&ordens, &totalOrdens);
                salvar_ordens_em_arquivo(ordens, totalOrdens, arquivo_ordens);
                break;
            case 2:
                listar_ordens(ordens, totalOrdens);
                break;
            case 3:
                atualizar_status_ordem(ordens, totalOrdens);
                salvar_ordens_em_arquivo(ordens, totalOrdens, arquivo_ordens);
                break;
            case 4: {
                char placa[20];
                printf("Digite a placa do veículo: ");
                fgets(placa, sizeof(placa), stdin);
                placa[strcspn(placa, "\n")] = 0;
                listar_ordens_por_veiculo(ordens, totalOrdens, placa);
                break;
            }
            case 0:
                printf("Saindo do menu de ordens...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    free(ordens);
}

