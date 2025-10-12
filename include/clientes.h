#ifndef CLIENTES_H
#define CLIENTES_H
#include "structs.h"

void menuClientes(cliente **lista_ptr, int *count_ptr);
cliente* buscar_cliente_na_lista(const char* cpf, const cliente* lista, int count);

cliente* carregar_clientes_do_arquivo(int *count_ptr, const char *nome_arquivo);
void salvar_clientes_no_arquivo(const cliente *lista, int count, const char *nome_arquivo);


void adiciona_cliente(cliente **lista_ptr, int *count_ptr, const cliente *novoCliente);
void cadastro_clientes(cliente **lista_ptr, int *count_ptr);
void imprimir_clientes(const cliente *lista, int count);
void atualizar_cliente(cliente *lista, int count);
void remover_cliente_por_cpf(cliente **lista_ptr, int *count_ptr);

int cpf_duplicado(const char *cpf, const cliente *lista, int count);
cliente* buscar_cliente_na_lista(const char* cpf, const cliente* lista, int count);

#endif
