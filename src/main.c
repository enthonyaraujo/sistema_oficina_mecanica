#include <stdio.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/ordens.h"
#include "../include/relatorios.h"

int main() {

    cliente *lista_clientes = NULL;
    int total_clientes = 0;

    veiculo *lista_veiculos = NULL;
    int total_veiculos = 0;

    ordemServico *ordens = NULL;
    int total_ordens = 0;

    lista_clientes = carregar_clientes_do_arquivo(&total_clientes, "data/clientes.txt");
    lista_veiculos = carregar_veiculos(&total_veiculos, lista_clientes, total_clientes);
    ordens = carregar_ordens_de_arquivo(&total_ordens, "data/ordens.txt", lista_veiculos, total_veiculos);

    int opcao;
    do {
        printf("\n=== SISTEMA DE GERENCIAMENTE PARA OFICINA MECANICA ===\n");
        printf("=== MENU PRINCIAL ===\n");
        printf("1 - Clientes\n");
        printf("2 - Veiculos\n");
        printf("3 - Ordens de Servico\n");
        printf("4 - Relatorios\n");
        printf("0 - Sair\n> ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 0:
                printf("Fechando programa...\n");
                break;

            case 1:
                menuClientes(&lista_clientes, &total_clientes);
                break;

            case 2:
                menuVeiculos(&lista_veiculos, &total_veiculos, lista_clientes,total_clientes);
                break;

            case 3:
                menuOrdens(&ordens, &total_ordens, lista_veiculos, total_veiculos);
                break;

            case 4:
                //  Passa as listas já carregadas
                menuRelatorios(ordens, total_ordens, lista_veiculos, total_veiculos);
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    salvar_clientes_no_arquivo(lista_clientes, total_clientes, "data/clientes.txt");
    salvar_todos_veiculos(lista_veiculos, total_veiculos, "data/veiculos.txt");
    salvar_ordens_em_arquivo(ordens, total_ordens, "data/ordens.txt");

    free(lista_clientes);
    free(ordens);
    free(lista_veiculos);

    return 0;
}
