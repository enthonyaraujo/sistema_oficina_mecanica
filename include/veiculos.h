#ifndef VEICULOS_H
#define VEICULOS_H
#include "structs.h"

veiculo* carregar_veiculos(int *total);
void salvar_todos_veiculos(veiculo *veiculos, int total);
void cadastro_veiculos(veiculo **veiculos, int *total);
void imprimir_veiculos(veiculo *veiculos, int total);
veiculo* buscar_veiculo_por_placa(const char *placa);

void remover_veiculo(veiculo **veiculos, int *total);
void atualizar_veiculo(veiculo *veiculos, int total);

void menuVeiculos();

#endif 