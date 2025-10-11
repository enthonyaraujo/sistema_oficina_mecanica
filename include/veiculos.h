#ifndef VEICULOS_H
#define VEICULOS_H
#include "structs.h"


extern veiculo *lista_veiculos;
extern int total_veiculos;


veiculo* carregar_veiculos_de_arquivo(int *total, const char *nome_arquivo);
veiculo* buscar_veiculo_por_placa(veiculo *veiculos, int total, const char *placa);

void salvar_todos_veiculos(veiculo *veiculos, int total);
void cadastro_veiculos(veiculo **veiculos, int *total);
void imprimir_veiculos(veiculo *veiculos, int total);

void remover_veiculo(veiculo **veiculos, int *total);
void atualizar_veiculo(veiculo *veiculos, int total);

void menuVeiculos();

#endif 