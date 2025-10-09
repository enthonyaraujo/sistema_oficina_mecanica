#include <stdio.h>
#include <stdlib.h>
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/clientes.h"

void menuVeiculos() {

    int opcao;

    do {
        printf("\n== Menu de Veiculos ==\n");
        printf("1 - Cadastro de Veiculos\n");
        printf("2 - Atualizar Veiculos\n");
        printf("3 - Listar Veiculos\n");
        printf("4 - Remover Veiculos\n");
        printf("0 - Voltar\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
        case 0:
            printf("Voltando ao menu principal...\n");
            break;
        case 1:
            printf("cadastrar veiculo.\n");
            break;
        case 2:
            printf("Atualizar veiculo.\n");
            break;
        case 3:
            printf("Listar veiculos.\n");
            break;
        case 4:
            printf("Remover veiculo.\n");
            break;
        default:
            printf("Opção inválida!\n");
        }

    } while (opcao != 0);

}
