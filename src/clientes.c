#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/clientes.h"
#include "../include/structs.h"
#include "../include/validar_cpf.h"

cliente *lista = NULL; //ponteiro para lista, inicialmente sem memoria alocada
int count = 0; // contador 

int cpf_duplicado(const char *cpf) {
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            return 1; // CPF já existe
        }
    }
    return 0; // CPF não encontrado
}

void imprimir_clientes(){
    if (count == 0){
        printf("Nenhum cliente cadastrado\n");
        return;
    }
    printf("Lista de clientes cadastrados: \n");
    for (int i = 0; i < count; i++)
    {
        printf("Nome: %s \n",lista[i].nome);
        printf("CPF: %s \n",lista[i].cpf);
        printf("Telefone: %s \n",lista[i].telefone);
        printf("\n");
    }
    
}

void remover_cliente_por_cpf() {
    if (count == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    char cpf_busca[20];
    printf("Digite o CPF do cliente que deseja remover: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;
    
    if (cpf_duplicado(cpf_busca))
    {
        printf("\n");
    }


    int pos = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf_busca) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Cliente com CPF %s não encontrado.\n", cpf_busca);
        return;
    }

    for (int i = pos; i < count -1; i++) {
        lista[i] = lista[i + 1];
    }
    count--;

    if (count > 0) {
        cliente *temp = realloc(lista, sizeof(cliente) * count);
        if (temp != NULL) lista = temp;
    } else {
        free(lista);
        lista = NULL;
    }

    printf("Cliente removido com sucesso.\n");
}

void atualizar_cliente() {
    if (count == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    char cpf_busca[20];
    printf("Digite o CPF do cliente que deseja atualizar: ");
    fgets(cpf_busca, sizeof(cpf_busca), stdin);
    if (strlen(cpf_busca) != 11)
    {
        return;    
    }
    
    cpf_busca[strcspn(cpf_busca, "\n")] = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(lista[i].cpf, cpf_busca) == 0) {
            printf("Cliente encontrado\n" ); //lista[i].nome
            printf("Digite o novo nome: ");
            char novo_nome[100];
            fgets(novo_nome, sizeof(novo_nome), stdin);
            novo_nome[strcspn(novo_nome, "\n")] = 0;

            strcpy(lista[i].nome, novo_nome);
            printf("Nome atualizado com sucesso.\n");

            return;
        }
    }

    printf("Cliente com CPF %s não encontrado.\n", cpf_busca);
}

void adiciona_cliente(const char *nome, const char *cpf, const char *telefone) { // funcao para adicionar cliente, recebe strings para adicao na lista de clientes
    lista = realloc(lista, sizeof(cliente) * (count + 1));  // count + 1 clientes (para nao precisar definir um valor fixo do tamanho da lista)
    strcpy(lista[count].nome, nome); 
    strcpy(lista[count].cpf, cpf);
    strcpy(lista[count].telefone, telefone);
    count++; // incrementa o contador
}

void carregar_clientes_do_arquivo(const char *nome_arquivo) { // funcao para leitura de clientes (precisamos fazer assim pois nao é possivel usar o append "a")
    FILE *arquivo = fopen(nome_arquivo, "r"); // abre o arquivo para leitura
    if (!arquivo) return; // se o comando acima falhar ele retorna

    char linha[256];
    cliente temp; 
    int step = 0;

    while (fgets(linha, sizeof(linha), arquivo)) { // loop para ler o arquivo
        linha[strcspn(linha, "\n")] = 0;
        if (strlen(linha) == 0) continue;

        if (strncmp(linha, "Nome: ", 6) == 0) {
            strcpy(temp.nome, linha + 6);
            step = 1;
        } else if (strncmp(linha, "CPF: ", 5) == 0 && step == 1) {
            strcpy(temp.cpf, linha + 5);
            step = 2;
        } else if (strncmp(linha, "Telefone: ", 10) == 0 && step == 2) {
            strcpy(temp.telefone, linha + 10);
            adiciona_cliente(temp.nome, temp.cpf, temp.telefone);
            step = 0;
        }
    }
    fclose(arquivo);
}

void salvar_clientes_no_arquivo(const char *nome_arquivo) { // funcao para escrever o nomes no arquivo
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo para salvar\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(arquivo, "Nome: %s\n", lista[i].nome);
        fprintf(arquivo, "CPF: %s\n", lista[i].cpf);
        fprintf(arquivo, "Telefone: %s\n\n", lista[i].telefone);
    }
    fclose(arquivo);
}

int cadastro_clientes() { // funcao para cadastrar os clientes
    char nome_arquivo[] = "data/clientes.txt"; // nome do arquivo

    carregar_clientes_do_arquivo(nome_arquivo);
    cliente novoCliente;
    cliente *novoClientePtr = &novoCliente;
    
    int opcao, ch;

    do {
        printf("\n== Cadastro de Clientes ==\n");

        printf("Digite o nome: \n");
        fgets(novoClientePtr->nome, sizeof(novoClientePtr->nome), stdin);
        novoClientePtr->nome[strcspn(novoClientePtr->nome, "\n")] = 0;

        while (1) {
            printf("Digite o CPF: \n");
            fgets(novoClientePtr->cpf, sizeof(novoClientePtr->cpf), stdin);
            novoClientePtr->cpf[strcspn(novoClientePtr->cpf, "\n")] = 0;
            if (strlen(novoClientePtr->cpf) != 11)
            {
                printf("O CPF deve contem 11 digitos\n");
                continue;   
            }
    

            if (cpf_duplicado(novoClientePtr->cpf)) {
                printf("CPF já cadastrado. Digite outro CPF.\n");
            } else {
                break;
            }
        }

        ler_cpf(novoClientePtr->cpf);

        printf("Digite o número de telefone: \n");
        fgets(novoClientePtr->telefone, sizeof(novoClientePtr->telefone), stdin);
        novoClientePtr->telefone[strcspn(novoClientePtr->telefone, "\n")] = 0;

        adiciona_cliente(novoClientePtr->nome, novoClientePtr->cpf, novoClientePtr->telefone);

        printf("Deseja salvar outro cliente? (1 - sim / 0 - nao)\n");
        scanf("%d", &opcao);
        while ((ch = getchar()) != '\n' && ch != EOF); 

    } while (opcao == 1);

    salvar_clientes_no_arquivo(nome_arquivo);

    free(lista);
    return 0;
}

void menuClientes() { //funcao menu no arquivo
    carregar_clientes_do_arquivo("data/clientes.txt");
    int opcao;

    do {
        printf("\n=== Menu de Clientes ===\n");
        printf("1 - Cadastrar Cliente\n");
        printf("2 - Atualizar Cliente\n");
        printf("3 - Listar Clientes\n");
        printf("4 - Remover Cliente\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {
            case 0:
                printf("\nVoltando ao menu principal...\n");
                break;
            case 1:
                cadastro_clientes();
                break;
            case 2:
                atualizar_cliente();
                salvar_clientes_no_arquivo("data/clientes.txt");
                break;
            case 3:
                imprimir_clientes();
                break;
            case 4:
                remover_cliente_por_cpf();
                salvar_clientes_no_arquivo("data/clientes.txt");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);
    
}
