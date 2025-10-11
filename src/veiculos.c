#include <stdio.h>
#include <stdlib.h>
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/clientes.h"

void cadastro_veiculos(){
    char nome_arquivo = "data/veiculos.txt";
    int opcao;
    char cpf_busca;

    veiculo novoVeiculo;
    veiculo *novoVeiculoPtr = &novoVeiculo;


    do
    {
        
        printf("Digite a placa do veiculo: \n");
        fgets(novoVeiculoPtr->placa, sizeof(novoVeiculoPtr), stdin);

        printf("Digite o modelo do veiculo: \n");
        fgets(novoVeiculoPtr->modelo, sizeof(novoVeiculoPtr->modelo), stdin);

        printf("Digite o ano do veiculo: \n");
        fgets(novoVeiculoPtr->ano, sizeof(novoVeiculoPtr->ano), stdin);

        printf("Digite o CPF do Cliente: \n");
        fgets(novoVeiculoPtr->clientePtr, sizeof(novoVeiculoPtr->clientePtr), stdin); //quero associar a um cliente ja cadastrado via cpf

        
        printf("Deseja salvar o carro? (1 - Sim / 2 - Nao)");
    } while (opcao == 1);
    
}

int main() { // void menu_veiculos

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
