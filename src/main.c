#include <stdio.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/ordens.h"
#include "../include/relatorios.h"

int main() {
    veiculo *lista_veiculos = NULL;
    int total_veiculos = 0;

    ordemServico *ordens = NULL;
    int total_ordens = 0;

    
    lista_veiculos = carregar_veiculos(&total_veiculos);
    ordens = carregar_ordens_de_arquivo(&total_ordens, "data/ordens.txt", lista_veiculos, total_veiculos);

    int opcao;
    do {
        printf("\n=== SISTEMA DE GERENCIAMENTE PARA OFICINA MECANICA ===\n");
        printf("=== MENU PRINCIAL ===\n");
        printf("1 - Clientes\n");
        printf("2 - Veículos\n");
        printf("3 - Ordens de Serviço\n");
        printf("4 - Relatórios\n");
        printf("0 - Sair\n> ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 0:
                printf("Fechando programa...\n");
                break;

            case 1:
                menuClientes();
                break;

            case 2:
                menuVeiculos();
                break;

            case 3:
                menuOrdens();
                break;

            case 4:
                // 🔹 Passa as listas já carregadas
                menuRelatorios(ordens, total_ordens, lista_veiculos, total_veiculos);
                break;

            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 0);

    
    salvar_ordens_em_arquivo(ordens, total_ordens, "data/ordens.txt");

    
    free(ordens);
    free(lista_veiculos);

    return 0;
}
