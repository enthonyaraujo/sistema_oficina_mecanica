#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ordens.h"
#include "../include/structs.h"
#include "../include/veiculos.h"

const char* status_para_string(statusOrdem s) {
    switch (s) {
        case AGUARDANDO_AVALIACAO: return "Aguardando avaliacao";
        case EM_REPARO: return "Em reparo";
        case FINALIZADO: return "Finalizado";
        case ENTREGUE: return "Entregue";
        default: return "Desconhecido";
    }
}

statusOrdem string_para_status(const char *str) {
    if (strcmp(str, "Aguardando avaliacao") == 0) return AGUARDANDO_AVALIACAO;
    if (strcmp(str, "Em reparo") == 0) return EM_REPARO;
    if (strcmp(str, "Finalizado") == 0) return FINALIZADO;
    if (strcmp(str, "Entregue") == 0) return ENTREGUE;
    return AGUARDANDO_AVALIACAO;
}

void listar_todas(const ordemServico *ordens, int total) {
    if (total == 0) {
        printf("\nNenhuma ordem cadastrada.\n");
        return;
    }

    printf("\n=== LISTA DE TODAS AS ORDENS ===\n");
    for (int i = 0; i < total; i++) {
        printf("ID: %d | Placa: %s | Status: %s\n",
               ordens[i].idOrdem,
               ordens[i].veiculo->placa,
               status_para_string(ordens[i].status));
    }
}

void listar_por_cliente(const ordemServico *ordens, int total, const char *cpf) {
    printf("\n=== ORDENS DO CLIENTE CPF %s ===\n", cpf);
    int encontrou = 0;

    for (int i = 0; i < total; i++) {
        if (ordens[i].veiculo && ordens[i].veiculo->clientePtr &&
            strcmp(ordens[i].veiculo->clientePtr->cpf, cpf) == 0) {
            
            printf("ID: %d | Veiculo: %s | Status: %s\n",
                   ordens[i].idOrdem,
                   ordens[i].veiculo->placa,
                   status_para_string(ordens[i].status));
            encontrou = 1;
        }
    }

    if (!encontrou)
        printf("Nenhuma ordem encontrada para esse cliente.\n");
}

void listar_por_veiculo(const ordemServico *ordens, int total, const char *placa) {
    printf("\n=== ORDENS DO VEICULO %s ===\n", placa);
    int encontrou = 0;

    for (int i = 0; i < total; i++) {
        if (ordens[i].veiculo && strcmp(ordens[i].veiculo->placa, placa) == 0) {
            printf("ID: %d | Cliente: %s | Status: %s\n",
                   ordens[i].idOrdem,
                   ordens[i].veiculo->clientePtr ? ordens[i].veiculo->clientePtr->nome : "Desconhecido",
                   status_para_string(ordens[i].status));
            encontrou = 1;
        }
    }

    if (!encontrou)
        printf("Nenhuma ordem encontrada para esse veiculo.\n");
}

void listar_por_status(const ordemServico *ordens, int total, statusOrdem status) {
    printf("\n=== ORDENS COM STATUS: %s ===\n", status_para_string(status));
    int encontrou = 0;

    for (int i = 0; i < total; i++) {
        if (ordens[i].status == status) {
            printf("ID: %d | Placa: %s | Cliente: %s\n",
                   ordens[i].idOrdem,
                   ordens[i].veiculo ? ordens[i].veiculo->placa : "Sem veiculo",
                   ordens[i].veiculo && ordens[i].veiculo->clientePtr
                       ? ordens[i].veiculo->clientePtr->nome
                       : "Desconhecido");
            encontrou = 1;
        }
    }

    if (!encontrou)
        printf("Nenhuma ordem com esse status.\n");
}


ordemServico *carregar_ordens_de_arquivo(int *total, const char *arquivo, veiculo *veiculos, int totalVeiculos) {
    FILE *file = fopen(arquivo, "r");
    if (!file) {
        *total = 0;
        return NULL;
    }

    ordemServico temp;
    ordemServico *ordens = NULL;
    *total = 0;

    char placaTmp[10];
    char statusTmp[50];

    while (fscanf(file, "%d;%9[^;];%19[^;];%255[^;];%49[^\n]\n",
                  &temp.idOrdem,
                  placaTmp,
                  temp.dataEntrada,
                  temp.descricaoProblema,
                  statusTmp) == 5) {

        // Encontra o veículo correspondente pela placa
        temp.veiculo = NULL;
        for (int j = 0; j < totalVeiculos; j++) {
            if (strcmp(veiculos[j].placa, placaTmp) == 0) {
                temp.veiculo = &veiculos[j];
                break;
            }
        }

        temp.status = string_para_status(statusTmp);

        (*total)++;
        ordens = realloc(ordens, (*total) * sizeof(ordemServico));
        ordens[*total - 1] = temp;
    }

    fclose(file);
    return ordens;
}


void salvar_ordens_em_arquivo(const ordemServico *ordens, int total, const char *arquivo) {
    FILE *file = fopen(arquivo, "w");
    if (!file) {
        printf("Erro ao salvar arquivo de ordens.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(file, "%d;%s;%s;%s;%s\n",
                ordens[i].idOrdem,
                ordens[i].veiculo->placa,
                ordens[i].dataEntrada,
                ordens[i].descricaoProblema,
                status_para_string(ordens[i].status));
    }

    fclose(file);
}


void abrir_ordem(ordemServico **ordens, int *total, veiculo *veiculos, int totalVeiculos) {
    if (totalVeiculos == 0) {
        printf("Nenhum veiculo cadastrado!\n");
        return;
    }

    ordemServico nova;
    nova.idOrdem = (*total == 0) ? 1 : (*ordens)[*total - 1].idOrdem + 1;

    char placaBusca[10];
    printf("\n=== ABRIR NOVA ORDEM ===\n");
    printf("Placa do veiculo: ");
    scanf(" %9[^\n]", placaBusca);

    veiculo *encontrado = NULL;
    for (int i = 0; i < totalVeiculos; i++) {
        if (strcmp(veiculos[i].placa, placaBusca) == 0) {
            encontrado = &veiculos[i];
            break;
        }
    }

    if (!encontrado) {
        printf("Veiculo nao encontrado.\n");
        return;
    }

    nova.veiculo = encontrado;

    printf("Data de entrada (dd/mm/aaaa): ");
    scanf(" %19[^\n]", nova.dataEntrada);
    printf("Descricao do problema: ");
    scanf(" %255[^\n]", nova.descricaoProblema);
    nova.status = AGUARDANDO_AVALIACAO;

    (*total)++;
    *ordens = realloc(*ordens, (*total) * sizeof(ordemServico));
    (*ordens)[*total - 1] = nova;

    salvar_ordens_em_arquivo(*ordens, *total, "data/ordens.txt");
    printf("Ordem aberta com sucesso (ID %d)\n", nova.idOrdem);
}


void atualizar_ordem(ordemServico *ordens, int total) {
    if (total == 0) {
        printf("Nenhuma ordem cadastrada.\n");
        return;
    }

    int id;
    printf("\nDigite o ID da ordem: ");
    scanf("%d", &id);

    for (int i = 0; i < total; i++) {
        if (ordens[i].idOrdem == id) {
            printf("Status atual: %s\n", status_para_string(ordens[i].status));
            printf("Selecione novo status:\n");
            printf("1. Aguardando avaliacao\n2. Em reparo\n3. Finalizado\n4. Entregue\n> ");

            int opc;
            scanf("%d", &opc);
            ordens[i].status = (opc - 1);
            salvar_ordens_em_arquivo(ordens, total, "data/ordens.txt");
            printf("Ordem atualizada!\n");
            return;
        }
    }
    printf("Ordem nao encontrada.\n");
}

void encerrar_ordem(ordemServico *ordens, int total) {
    if (total == 0) {
        printf("Nenhuma ordem cadastrada.\n");
        return;
    }

    int id;
    printf("Digite o ID da ordem para encerrar: ");
    scanf("%d", &id);

    for (int i = 0; i < total; i++) {
        if (ordens[i].idOrdem == id) {
            ordens[i].status = ENTREGUE;
            salvar_ordens_em_arquivo(ordens, total, "data/ordens.txt");
            printf("Ordem encerrada com sucesso!\n");
            return;
        }
    }
    printf("Ordem nao encontrada.\n");
}


void menuOrdens() {
    ordemServico *ordens = NULL;
    int totalOrdens = 0;
    veiculo *veiculos = NULL; 
    int totalVeiculos = 0;

    veiculos = carregar_veiculos(&totalVeiculos);
    ordens = carregar_ordens_de_arquivo(&totalOrdens, "data/ordens.txt", veiculos, totalVeiculos);

    int opc;
    do {
        printf("\n===== MENU DE ORDENS =====\n");
        printf("1. Abrir nova ordem\n");
        printf("2. Atualizar ordem\n");
        printf("3. Encerrar ordem\n");
        printf("4. Listar todas\n");
        printf("5. Listar por cliente\n");
        printf("6. Listar por veiculo\n");
        printf("7. Listar por status\n");
        printf("0. Voltar\n> ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 1:
                abrir_ordem(&ordens, &totalOrdens, veiculos, totalVeiculos);
                break;

            case 2:
                atualizar_ordem(ordens, totalOrdens);
                break;

            case 3:
                encerrar_ordem(ordens, totalOrdens);
                break;

            case 4:
                listar_todas(ordens, totalOrdens);
                break;

            case 5: {
                char cpf[20];
                printf("CPF do cliente: ");
                scanf(" %19[^\n]", cpf);
                listar_por_cliente(ordens, totalOrdens, cpf);
                break;
            }

            case 6: {
                char placa[10];
                printf("Placa do veiculo: ");
                scanf(" %9[^\n]", placa);
                listar_por_veiculo(ordens, totalOrdens, placa);
                break;
            }

            case 7: {
                int st;
                printf("1. Aguardando avaliacao\n2. Em reparo\n3. Finalizado\n4. Entregue\n> ");
                scanf("%d", &st);
                listar_por_status(ordens, totalOrdens, st - 1);
                break;
            }
        }
    } while (opc != 0);

    salvar_ordens_em_arquivo(ordens, totalOrdens, "data/ordens.txt");
    free(ordens);
}