#ifndef VEICULOS_H
#define VEICULOS_H
#include "structs.h"


void menuVeiculos(veiculo **lista_veiculos_ptr, int *total_veiculos_ptr, cliente *lista_clientes, int total_clientes);

veiculo* carregar_veiculos(int *total, cliente *lista_clientes, int total_clientes);

void salvar_todos_veiculos(const veiculo *veiculos, int total, const char *nome_arquivo);

void cadastro_veiculos(veiculo **veiculos, int *totalVeiculos, cliente *lista_clientes, int total_clientes);

void atualizar_veiculo(veiculo *veiculos, int totalVeiculos, cliente *lista_clientes, int total_clientes);

void remover_veiculo(veiculo **veiculos, int *total);

void imprimir_veiculos(const veiculo *veiculos, int total);

#endif