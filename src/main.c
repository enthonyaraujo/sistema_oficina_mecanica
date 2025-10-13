#include <stdio.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/ordens.h"
#include "../include/relatorios.h"

int main() {

    // Ponteiro para lista de clientes e variável para total de clientes
    cliente *lista_clientes = NULL;
    int total_clientes = 0;

    // Ponteiro para lista de veículos e variável para total de veículos
    veiculo *lista_veiculos = NULL;
    int total_veiculos = 0;

    // Ponteiro para lista de ordens de serviço e variável para total de ordens
    ordemServico *ordens = NULL;
    int total_ordens = 0;

    // Carrega os dados de clientes do arquivo para memória
    lista_clientes = carregar_clientes_do_arquivo(&total_clientes, "data/clientes.txt");

    // Carrega os dados de veículos do arquivo, associando aos clientes
    lista_veiculos = carregar_veiculos(&total_veiculos, lista_clientes, total_clientes);

    // Carrega ordens de serviço, associando aos veículos
    ordens = carregar_ordens_de_arquivo(&total_ordens, "data/ordens.txt", lista_veiculos, total_veiculos);

    int opcao;
    do {
        // Exibe menu principal do sistema para o usuário
        printf("\n=== SISTEMA DE GERENCIAMENTO PARA OFICINA MECANICA ===\n");
        printf("=== MENU PRINCIPAL ===\n");
        printf("1 - Clientes\n");
        printf("2 - Veiculos\n");
        printf("3 - Ordens de Servico\n");
        printf("4 - Relatorios\n");
        printf("0 - Sair\n> ");
        scanf("%d", &opcao);  // Recebe opção escolhida
        getchar();            // Limpa buffer do teclado

        // Estrutura de decisão para escolher o menu correspondente
        switch (opcao) {
            case 0:
                printf("Fechando programa...\n");
                break;

            case 1:
                menuClientes(&lista_clientes, &total_clientes);  // Menu de operações para clientes
                break;

            case 2:
                menuVeiculos(&lista_veiculos, &total_veiculos, lista_clientes, total_clientes); // Menu de operações para veículos
                break;

            case 3:
                menuOrdens(&ordens, &total_ordens, lista_veiculos, total_veiculos); // Menu de operações para ordens de serviço
                break;

            case 4:
                menuRelatorios(ordens, total_ordens, lista_veiculos, total_veiculos); // Menu de relatórios
                break;

            default:
                printf("Opcao invalida.\n"); 
                break;
        }
    } while (opcao != 0);    // Repete o menu correspoe até o usuário escolher sair

    // Salva os dados atualizados nos arquivos correspondentes antes de finalizar
    salvar_clientes_no_arquivo(lista_clientes, total_clientes, "data/clientes.txt");
    salvar_todos_veiculos(lista_veiculos, total_veiculos, "data/veiculos.txt");
    salvar_ordens_em_arquivo(ordens, total_ordens, "data/ordens.txt");

    // Libera a memória alocada para as listas antes de encerrar
    free(lista_clientes);
    free(ordens);
    free(lista_veiculos);

    return 0; // Finaliza o programa
}
