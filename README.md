# Atividade Prática 1º Unidade - Oficina Mecânica - Algoritmos e Estrutura de Dados I

Sistema de Gerenciamento para Oficina Mecânica

Desenvolver um sistema que simula o funcionamento básico de uma oficina mecânica,
permitindo o cadastro de clientes, veículos, abertura e gerenciamento de ordens de ser-
viço, além da geração de relatórios. O sistema deve armazenar os dados em arquivos,
garantindo persistência.

## Como compilar: 

```bash
    
    gcc -Iinclude -o SistemaOficinaMecanica src/*.c
    ./SistemaOficinaMecanica

```

## Descrição Geral

O sistema simulará o funcionamento de uma oficina, onde:

• O registro de clientes conterá: Nome, CPF e Telefone. 

• O registro de veículos conterá: Placa (identificador único), Modelo, Ano e ponteiro
para o cliente (dono).

• As ordens de serviço conterão: ID da Ordem, ponteiro para o veículo, Data de En-
trada, Descrição do Problema e Status (usar enum: AGUARDANDO_AVALIACAO,
EM_REPARO, FINALIZADO, ENTREGUE).

• O gerenciamento dos dados deverá permitir inclusão, remoção e atualização de in-
formações.

• A criação de relatórios deve ser feita por meio do preenchimento de arquivos de
texto, contendo histórico de serviços de um veículo, ordens de serviço abertas em
um dia específico ou todos os veículos de um cliente.

# Funcionalidades Obrigatórias

## Cadastro de Clientes

• Atributos: Nome, CPF, Telefone.

• Operações: Inserção, remoção e atualização.

• Persistência: Salvamento/recuperação em arquivo.

## Cadastro de Veículos

• Atributos: Placa (identificador único), Modelo, Ano, Cliente (ponteiro).

• Operações: Inserção, remoção e atualização.

• Persistência: Salvamento/recuperação em arquivo.

## Gerenciamento de Ordens de Serviço

• Atributos: ID da Ordem, Veículo, Data de Entrada, Descrição do Problema, Status.
• Operações: Abrir, atualizar, encerrar ordens de serviço.

• Relatórios: Listar ordens por cliente, veículo ou status.

• Persistência: Salvamento/recuperação em arquivo.

## Relatórios

• Histórico de serviços de um veículo.

• Ordens de serviço abertas em um dia específico.

• Listar todos os veículos cadastrados de um cliente.

• Relatório de ordens de serviço por status.

• Relatório de clientes mais recorrentes.

## Especificações Técnicas

• Uso de ponteiros para manipulação de clientes, veículos e ordens.

• Manipulação de strings com ponteiros (cópia, concatenação, comparação).

• Alocação dinâmica de memória com malloc e free.

• Vetores dinâmicos para armazenar registros.

• Uso de structs e enums para representar dados.

• Manipulação de arquivos para leitura e escrita de clientes, veículos, ordens de serviço
e relatórios.

• Validação de dados de entrada (ex.: evitar CPFs inválidos ou placas duplicadas).

• Implementação de busca eficiente (ex.: localizar veículo pela placa ou cliente pelo
CPF).

• Histórico do cliente (consultar todas as ordens e veículos associados a um cliente)