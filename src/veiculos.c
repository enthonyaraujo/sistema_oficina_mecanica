// Arquivo: veiculos.c (VERSÃO FINAL E CORRIGIDA)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../include/veiculos.h"
#include "../include/structs.h"
#include "../include/clientes.h"

int validar_placa(const char *placa) {
    int len = strlen(placa);
    if (len != 7) return 0; // A placa deve ter exatamente 7 caracteres

    // Verifica padrão Mercosul (3 letras, 1 número, 1 letra, 2 números)
    if (isalpha(placa[0]) && isalpha(placa[1]) && isalpha(placa[2]) && isdigit(placa[3]) && isalpha(placa[4]) && isdigit(placa[5]) && isdigit(placa[6])) {
        return 1;
    }
    // Verifica padrão antigo (3 letras seguidas por 4 números)
    if (isalpha(placa[0]) && isalpha(placa[1]) && isalpha(placa[2]) && isdigit(placa[3]) && isdigit(placa[4]) && isdigit(placa[5]) && isdigit(placa[6])) {
        return 1;
    }
    return 0; // Retorna 0 para formato inválido
}

int ano_valido(int ano) {
    if (ano < 1900) return 0; // Ano não pode ser anterior a 1900

    // Recupera o ano atual do sistema
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    int ano_atual = tm_info.tm_year + 1900;

    // Verifica se o ano informado não está muito à frente do atual
    if (ano > ano_atual + 1) return 0;
    return 1;
}

void imprimir_veiculos(const veiculo *veiculos, int total) {
    if (total == 0) {
        printf("\nNenhum veiculo cadastrado.\n");
        return;
    }
    printf("\n--- Lista de Veiculos Cadastrados ---\n");
    // Mostra todas as informações de cada veículo
    for (int i = 0; i < total; i++) {
        printf("Placa:    %s\n", veiculos[i].placa);
        printf("Modelo:   %s\n", veiculos[i].modelo);
        printf("Ano:      %d\n", veiculos[i].ano);
        if (veiculos[i].clientePtr) {
            printf("Cliente:  %s (CPF: %s)\n", veiculos[i].clientePtr->nome, veiculos[i].clientePtr->cpf);
        } else {
            printf("Cliente:  Nao associado\n");
        }
    }
}

void remover_veiculo(veiculo **veiculos_ptr, int *total_ptr) {
    if (*total_ptr == 0) {
        printf("Nenhum veiculo para remover.\n");
        return;
    }
    char placa[20];
    printf("Digite a placa do veiculo para remover: ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = 0;

    // Procura a posição do veículo na lista
    int pos = -1;
    for (int i = 0; i < *total_ptr; i++) {
        if (strcasecmp((*veiculos_ptr)[i].placa, placa) == 0) {
            pos = i;
            break;
        }
    }

    // Se o veículo não for encontrado, exibe mensagem de erro
    if (pos == -1) {
        printf("Veiculo com placa %s nao encontrado.\n", placa);
        return;
    }

    // Reorganiza o vetor, removendo o veículo
    for (int i = pos; i < *total_ptr - 1; i++) {
        (*veiculos_ptr)[i] = (*veiculos_ptr)[i + 1];
    }
    (*total_ptr)--;

    // Realoca a memória do vetor ou libera tudo caso fique vazio
    if (*total_ptr > 0) {
        veiculo *temp = realloc(*veiculos_ptr, (*total_ptr) * sizeof(veiculo));
        if (temp) *veiculos_ptr = temp;
    } else {
        free(*veiculos_ptr);
        *veiculos_ptr = NULL;
    }
    printf("Veiculo removido com sucesso.\n");
}

void atualizar_veiculo(veiculo *veiculos, int totalVeiculos, cliente *lista_clientes, int total_clientes) {
    if (totalVeiculos == 0) {
        printf("Nenhum veiculo para atualizar.\n");
        return;
    }
    char placa_buscar[20];
    printf("Digite a placa do veiculo que deseja atualizar: ");
    fgets(placa_buscar, sizeof(placa_buscar), stdin);
    placa_buscar[strcspn(placa_buscar, "\n")] = 0;

    // Procura o veículo a ser atualizado
    for (int i = 0; i < totalVeiculos; i++) {
        if (strcasecmp(veiculos[i].placa, placa_buscar) == 0) {
            printf("Digite o novo CPF do cliente (Enter para manter o atual): ");
            char novo_cpf[20];
            fgets(novo_cpf, sizeof(novo_cpf), stdin);
            if (novo_cpf[0] != '\n') {
                novo_cpf[strcspn(novo_cpf, "\n")] = 0;
                cliente *novo_cliente = buscar_cliente_na_lista(novo_cpf, lista_clientes, total_clientes);
                if (novo_cliente) {
                    veiculos[i].clientePtr = novo_cliente;
                    printf("Proprietario do veiculo atualizado para: %s\n", novo_cliente->nome);
                } else {
                    printf("Cliente com CPF %s nao encontrado. Proprietario anterior mantido.\n", novo_cpf);
                }
            }
            printf("Veiculo atualizado com sucesso.\n");
            return;
        }
    }
    printf("Veiculo com placa %s nao encontrado.\n", placa_buscar);
}

veiculo* carregar_veiculos(int *total_ptr, cliente *lista_clientes, int total_clientes) {
    FILE *arquivo = fopen("data/veiculos.txt", "r");
    if (!arquivo) {
        *total_ptr = 0;
        return NULL;
    }

    veiculo *lista_veiculos = NULL;
    *total_ptr = 0;
    veiculo temp;
    char cpf_temp[20];
    
    // Lê os dados de cada veículo do arquivo e associa ao cliente correto
    while (fscanf(arquivo, "Placa: %s\nModelo: %[^\n]\nAno: %d\nCPF do cliente associado: %s\n\n",
                  temp.placa, temp.modelo, &temp.ano, cpf_temp) == 4)
    {
        temp.clientePtr = buscar_cliente_na_lista(cpf_temp, lista_clientes, total_clientes);
        if (temp.clientePtr == NULL) {
            printf("Aviso: Cliente com CPF %s (do arquivo de veiculos) nao foi encontrado. Veiculo de placa %s nao sera carregado.\n", cpf_temp, temp.placa);
            continue; // Ignora veículos cujo cliente não existe
        }

        (*total_ptr)++;
        lista_veiculos = realloc(lista_veiculos, (*total_ptr) * sizeof(veiculo));
        lista_veiculos[*total_ptr - 1] = temp;
    }
    
    fclose(arquivo);
    return lista_veiculos;
}

void salvar_todos_veiculos(const veiculo *veiculos, int total, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de veiculos para salvar");
        return;
    }
    // Salva todos os veículos com informações do proprietário associado
    for (int i = 0; i < total; i++) {
        if (veiculos[i].clientePtr) {
            fprintf(arquivo, "Placa: %s\n", veiculos[i].placa);
            fprintf(arquivo, "Modelo: %s\n", veiculos[i].modelo);
            fprintf(arquivo, "Ano: %d\n", veiculos[i].ano);
            fprintf(arquivo, "CPF do cliente associado: %s\n\n", veiculos[i].clientePtr->cpf);
        }
    }
    fclose(arquivo);
}

void cadastro_veiculos(veiculo **veiculos_ptr, int *total_ptr, cliente *lista_clientes, int total_clientes) {
    veiculo novoVeiculo;
    char cpf_input[20];

    printf("\n== Cadastro de Novo Veiculo ==\n");

    // Entrada e validação da placa
    printf("Digite a placa do veiculo (ex: BRA2E19 ou ABC1234): ");
    fgets(novoVeiculo.placa, sizeof(novoVeiculo.placa), stdin);
    novoVeiculo.placa[strcspn(novoVeiculo.placa, "\n")] = 0;
    if (!validar_placa(novoVeiculo.placa)) {
        printf("Erro: Formato de placa invalido.\n");
        return;
    }

    // Entrada do modelo do veículo
    printf("Digite o modelo do veiculo: ");
    fgets(novoVeiculo.modelo, sizeof(novoVeiculo.modelo), stdin);
    novoVeiculo.modelo[strcspn(novoVeiculo.modelo, "\n")] = 0;

    // Entrada e validação do ano
    printf("Digite o ano do veiculo: ");
    scanf("%d", &novoVeiculo.ano);
    getchar();
    if (!ano_valido(novoVeiculo.ano)) {
        printf("Erro: Ano invalido.\n");
        return;
    }
    
    // Entrada do CPF do cliente proprietário
    printf("Digite o CPF do Cliente proprietario: ");
    fgets(cpf_input, sizeof(cpf_input), stdin);
    cpf_input[strcspn(cpf_input, "\n")] = 0;

    novoVeiculo.clientePtr = buscar_cliente_na_lista(cpf_input, lista_clientes, total_clientes);
    if (!novoVeiculo.clientePtr) {
        printf("Erro: Cliente com CPF %s nao encontrado. Cadastre o cliente primeiro.\n", cpf_input);
        return;
    }

    // Adiciona o novo veículo ao vetor dinâmico
    (*total_ptr)++;
    *veiculos_ptr = realloc(*veiculos_ptr, (*total_ptr) * sizeof(veiculo));
    (*veiculos_ptr)[*total_ptr - 1] = novoVeiculo;

    printf("Veiculo de placa %s cadastrado com sucesso para o cliente %s.\n", novoVeiculo.placa, novoVeiculo.clientePtr->nome);
}

void menuVeiculos(veiculo **lista_veiculos_ptr, int *total_veiculos_ptr, cliente *lista_clientes, int total_clientes) {
    int opcao;
    do {
        printf("\n== MENU DE VEICULOS ==\n");
        printf("1 - Cadastrar Veiculo\n");
        printf("2 - Atualizar Veiculo\n");
        printf("3 - Listar Veiculos\n");
        printf("4 - Remover Veiculo\n");
        printf("0 - Voltar ao Menu Principal\n> ");
        scanf("%d", &opcao);
        getchar();

        // Menu principal para ações sobre veículos
        switch(opcao) {
            case 1:
                cadastro_veiculos(lista_veiculos_ptr, total_veiculos_ptr, lista_clientes, total_clientes);
                break;
            case 2:
                atualizar_veiculo(*lista_veiculos_ptr, *total_veiculos_ptr, lista_clientes, total_clientes);
                break;
            case 3:
                imprimir_veiculos(*lista_veiculos_ptr, *total_veiculos_ptr);
                break;
            case 4:
                remover_veiculo(lista_veiculos_ptr, total_veiculos_ptr);
                break;
        }
    } while(opcao != 0);
}
