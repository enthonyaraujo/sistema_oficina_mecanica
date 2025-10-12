#include <stdio.h>
#include <string.h>
#include "../include/clientes.h"

 // link do algoritmo: https://www.macoratti.net/alg_cpf.htm

int validarcpf(int cpf[11]){ 
    int primeiro_digito, segundo_digito, resto1, resto2;
    int todos_iguais = 1;
    // verificar primeiro se sao todos iguais
    for (int i = 1; i < 11; i++) {
        if (cpf[i] != cpf[0]) {
            todos_iguais = 0;
            break;
        }
    }            
    if (todos_iguais) {
        printf("CPF invalido (todos os digitos iguais)\n");
        return 0;
    }
    else{
         //calculo do primeiro digito
    primeiro_digito = cpf[0]*10 + cpf[1]*9 +cpf[2]*8 + cpf[3]*7 + cpf[4]*6 + cpf[5]*5+ cpf[6]*4 + cpf[7]*3 + cpf[8]*2;  
    resto1 = (primeiro_digito) % 11;
    
    /*- Se o resto da divisão for menor que 2, então o dígito é igual a 0 (Zero).
-   Se o resto da divisão for maior ou igual a 2, então o dígito verificador é igual a 
    11 menos o resto da divisão (11 - resto). */

    if (resto1 < 2)
    {
        primeiro_digito = 0;
    }
    else{
        primeiro_digito = 11 - resto1;
    }

    // calculo do segundo digito
    segundo_digito = cpf[0]*11 + cpf[1]*10 +cpf[2]*9 + cpf[3]*8 + cpf[4]*7 + cpf[5]*6+ cpf[6]*5 + cpf[7]*4 + cpf[8]*3 + cpf[9]*2; 
    resto2 = segundo_digito % 11;

    if (resto2 < 2)
    {
        segundo_digito = 0;
    }
    else{
        segundo_digito = 11 - resto2;
    }

    if (primeiro_digito == cpf[9] && segundo_digito == cpf[10])
    {
        printf("CPF Validado\n");
        return 1;
    }
    else
    {
        printf("CPF Invalidado\n");
        return 0;
    }

    }
   
}

int ler_cpf(char *entrada) {
    int cpf[11];

    if (strlen(entrada) != 11) {
        return 0;
    }
    for (int i = 0; i < 11; i++) {
        if (entrada[i] < '0' || entrada[i] > '9') return 0;
        cpf[i] = entrada[i] - '0';
    }
    return validarcpf(cpf); // validarcpf também retorna 1/0 como sugerido antes
}
