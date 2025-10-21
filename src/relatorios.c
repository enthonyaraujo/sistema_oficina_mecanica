#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/structs.h"
#include "../include/relatorios.h"
#include "../include/ordens.h"

void relatorio_historico_veiculo(const ordemServico *ordens, int totalOrdens, const char *placa) {
    printf("\n=== HISTORICO DO VEICULO %s ===\n", placa);
    int encontrou = 0;

    // Percorre todas as ordens para encontrar as que pertencem ao veículo de placa informada
    for (int i = 0; i < totalOrdens; i++) {
        if (ordens[i].veiculo && strcmp(ordens[i].veiculo->placa, placa) == 0) {
            printf("ID: %d | Data: %s | Descricao: %s | Status: %s\n",
                   ordens[i].idOrdem,
                   ordens[i].dataEntrada,
                   ordens[i].descricaoProblema,
                   status_para_string(ordens[i].status));
            encontrou = 1;
        }
    }

    // Caso nenhuma ordem seja encontrada, informa ao usuário
    if (!encontrou)
        printf("Nenhuma ordem encontrada para este veiculo.\n");
}

void relatorio_ordens_por_dia(const ordemServico *ordens, int totalOrdens, const char *data) {
    printf("\n=== ORDENS ABERTAS EM %s ===\n", data);
    int encontrou = 0;

    // Percorre todas as ordens para listar as abertas na data informada
    for (int i = 0; i < totalOrdens; i++) {
        if (strcmp(ordens[i].dataEntrada, data) == 0) {
            printf("ID: %d | Veiculo: %s | Status: %s\n",
                   ordens[i].idOrdem,
                   ordens[i].veiculo ? ordens[i].veiculo->placa : "Desconhecido",
                   status_para_string(ordens[i].status));
            encontrou = 1;
        }
    }

    // Caso não exista ordem para essa data, exibe mensagem
    if (!encontrou)
        printf("Nenhuma ordem aberta nesta data.\n");
}

void relatorio_veiculos_por_cliente(const veiculo *veiculos, int totalVeiculos, const char *cpf) {
    printf("\n=== VEICULOS DO CLIENTE CPF %s ===\n", cpf);
    int encontrou = 0;

    // Percorre todos os veículos e mostra os que pertencem ao cliente informado, via CPF
    for (int i = 0; i < totalVeiculos; i++) {
        if (veiculos[i].clientePtr && strcmp(veiculos[i].clientePtr->cpf, cpf) == 0) {
            printf("Placa: %s | Modelo: %s\n",
                   veiculos[i].placa,
                   veiculos[i].modelo);
            encontrou = 1;
        }
    }

    // Caso não encontre veículos para o cliente, exibe mensagem
    if (!encontrou)
        printf("Nenhum veiculo encontrado para este cliente.\n");
}

void relatorio_ordens_por_status(const ordemServico *ordens, int totalOrdens) {
    int contA = 0, contR = 0, contF = 0, contE = 0;

    // Conta quantas ordens há em cada status
    for (int i = 0; i < totalOrdens; i++) {
        switch (ordens[i].status) {
            case AGUARDANDO_AVALIACAO: contA++; break;
            case EM_REPARO: contR++; break;
            case FINALIZADO: contF++; break;
            case ENTREGUE: contE++; break;
        }
    }

    // Mostra relatório resumido das quantidades por status
    printf("\n=== RELATORIO DE ORDENS POR STATUS ===\n");
    printf("Aguardando avaliacao: %d\n", contA);
    printf("Em reparo: %d\n", contR);
    printf("Finalizado: %d\n", contF);
    printf("Entregue: %d\n", contE);
}

void relatorio_clientes_recorrentes(const ordemServico *ordens, int totalOrdens) {
    typedef struct {
        char nome[100];
        char cpf[20];
        int qtd;
    } ClienteContador;

    ClienteContador lista[100];
    int total = 0;

    // Soma a quantidade de ordens por cliente de acordo com o CPF
    for (int i = 0; i < totalOrdens; i++) {
        if (!ordens[i].veiculo || !ordens[i].veiculo->clientePtr)
            continue;

        const char *cpf = ordens[i].veiculo->clientePtr->cpf;
        const char *nome = ordens[i].veiculo->clientePtr->nome;

        int achou = 0;
        for (int j = 0; j < total; j++) {
            if (strcmp(lista[j].cpf, cpf) == 0) {
                lista[j].qtd++;
                achou = 1;
                break;
            }
        }

        // Se o cliente ainda não está na lista, adiciona
        if (!achou) {
            strcpy(lista[total].cpf, cpf);
            strcpy(lista[total].nome, nome);
            lista[total].qtd = 1;
            total++;
        }
    }

    printf("\n=== CLIENTES MAIS RECORRENTES ===\n");
    if (total == 0) {
        printf("Nenhum cliente encontrado.\n");
        return;
    }

    // Exibe todos os clientes recorrentes e o total de ordens de cada
    for (int i = 0; i < total; i++) {
        printf("%s (CPF: %s) — %d ordens\n",
               lista[i].nome, lista[i].cpf, lista[i].qtd);
    }
}

void menuRelatorios(ordemServico *ordens, int totalOrdens, veiculo *veiculos, int totalVeiculos) {
    int opc;
    do {
        printf("\n===== MENU DE RELATORIOS =====\n");
        printf("[1] Historico de servicos de um veiculo\n");
        printf("[2] Ordens abertas em um dia especifico\n");
        printf("[3] Listar veiculos de um cliente\n");
        printf("[4] Ordens por status\n");
        printf("[5] Clientes mais recorrentes\n");
        printf("[0] Voltar ao menu Principal\n> ");
        scanf("%d", &opc);
        getchar();

        // Direciona cada opção do menu para o relatório correspondente
        switch (opc) {
            case 1: {
                char placa[10];
                printf("Placa: ");
                scanf(" %9[^\n]", placa);
                relatorio_historico_veiculo(ordens, totalOrdens, placa);
                break;
            }
            case 2: {
                char data[20];
                printf("Data (dd/mm/aaaa): ");
                scanf(" %19[^\n]", data);
                relatorio_ordens_por_dia(ordens, totalOrdens, data);
                break;
            }
            case 3: {
                char cpf[20];
                printf("CPF: ");
                scanf(" %19[^\n]", cpf);
                relatorio_veiculos_por_cliente(veiculos, totalVeiculos, cpf);
                break;
            }
            case 4:
                relatorio_ordens_por_status(ordens, totalOrdens);
                break;
            case 5:
                relatorio_clientes_recorrentes(ordens, totalOrdens);
                break;
        }
    } while (opc != 0);
}
