#ifndef ORDENS_H
#define ORDENS_H

#include "structs.h"

ordemServico *carregar_ordens_de_arquivo(int *total, const char *arquivo, veiculo *veiculos, int totalVeiculos);

void salvar_ordens_em_arquivo(const ordemServico *ordens, int total, const char *arquivo);

void abrir_ordem(ordemServico **ordens, int *total, veiculo *veiculos, int totalVeiculos);
void atualizar_ordem(ordemServico *ordens, int total);
void encerrar_ordem(ordemServico *ordens, int total);

void listar_todas(const ordemServico *ordens, int total);

void listar_por_cliente(const ordemServico *ordens, int total, const char *cpf);
void listar_por_veiculo(const ordemServico *ordens, int total, const char *placa);
void listar_por_status(const ordemServico *ordens, int total, statusOrdem status);

void menuOrdens();

#endif
