#include <stdio.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/ordens.h"

int main(){
    veiculo *lista_veiculos = NULL;
    int total_veiculos = 0;

    int opcao;

    do
    {
        printf("=== Sistema de Gerenciamento para Oficina Mecânica ===\n=== Menu ===\n");
        printf("1 - Clientes\n2 - Veículos\n3 - Ordens de Serviço\n4 - Relatórios\n0 - Sair\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
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
            total_veiculos = 0;
            lista_veiculos = carregar_veiculos(&total_veiculos);
            menuOrdens();
            break;
        case 4:
            break;

        default:
            printf("Opcao Invalida\n");
            printf("\n");
            break;
        }
    } while (opcao != 0);
    

}