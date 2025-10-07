#ifndef CLIENTES_H
#define CLIENTES_H
#include "structs.h"

void MenuClientes();

void salvarCliente(clientes novoCliente);

void CadastrarCliente(clientes *listaClientes, int *qtdClientes);

void ListarClientes();


#endif 

