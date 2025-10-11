#ifndef ORDENS_H
#define ORDENS_H
#include "structs.h"
#include "veiculos.h" 


void cadastrar_ordem(ordemServico **ordens, int *total);
void listar_ordens(const ordemServico *ordens, int total);
void atualizar_status_ordem(ordemServico *ordens, int total);
void salvar_ordens_em_arquivo(const ordemServico *ordens, int total, const char *nome_arquivo);
ordemServico* carregar_ordens_de_arquivo(int *total, const char *nome_arquivo);
void listar_ordens_por_cliente(const ordemServico *ordens, int total, const char *cpf_cliente);
void listar_ordens_por_veiculo(const ordemServico *ordens, int total, const char *placa);
void listar_ordens_por_status(const ordemServico *ordens, int total, statusOrdem status);
void menuOrdens();


#endif