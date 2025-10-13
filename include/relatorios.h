#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "structs.h"
#include "veiculos.h"
#include "ordens.h"

void relatorio_historico_veiculo(const ordemServico *ordens, int totalOrdens, const char *placa);

void relatorio_ordens_por_dia(const ordemServico *ordens, int totalOrdens, const char *data);

void relatorio_veiculos_por_cliente(const veiculo *veiculos, int totalVeiculos, const char *cpf);

void relatorio_ordens_por_status(const ordemServico *ordens, int totalOrdens);

void relatorio_clientes_recorrentes(const ordemServico *ordens, int totalOrdens);

void menuRelatorios(ordemServico *ordens, int totalOrdens, veiculo *veiculos, int totalVeiculos);

#endif

