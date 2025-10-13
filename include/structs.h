#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

typedef struct 
{
    char nome[100];
    char cpf[20]; // deve conter 11 numeros
    char telefone[50]; // deve conter 12 numeros
} cliente;

typedef struct 
{
    char placa[10];
    char modelo[50];
    int ano;
    cliente *clientePtr;
} veiculo;

typedef enum {
    AGUARDANDO_AVALIACAO,
    EM_REPARO,
    FINALIZADO,
    ENTREGUE
} statusOrdem;

typedef struct ordemServico {
    int idOrdem;
    veiculo *veiculo;
    char dataEntrada[20];
    char descricaoProblema[256];
    statusOrdem status;
} ordemServico;       


#endif

