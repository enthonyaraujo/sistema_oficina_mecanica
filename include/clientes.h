#ifndef CLIENTES_H
#define CLIENTES_H
#include "structs.h"

cliente* buscar_cliente_por_cpf(const char *cpf_procurado);

void menuClientes();
void adiciona_cliente(const char *nome, const char *cpf, const char *telefone);
void atualizar_cliente();
void remover_cliente_por_cpf();
int cpf_duplicado(const char *cpf);
void imprimir_lista();
void carregar_clientes_do_arquivo(const char *nome_arquivo);
void salvar_clientes_no_arquivo(const char *nome_arquivo);


#endif 

