//Trabalho Hotel - Projeto Programação Modular

/*
Fabiano Carvalho Costa Júnior -- 2520144
João Vitor Lopes da Costa -- 2520345 
Rafaela Gomes de Souza Sant’Anna -- 2310112
Vinicius Silberman Pereira -- 2510569
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "hospedagem.h"
#include "hospede.h"
#include "quarto.h"

cadeiaQuarto** pQuartos = NULL;
cadeiaHospede* hospedes = NULL;
cadeiaHospedagem* hospedagens = NULL;

int main(void){

    printf("Bem vindo ao sistema de gerenciamento de hotéis.\nSelecione qual ação gostaria de realizar:\n\n"
        "\tExemplo: AÇÃO --- BOTÃO\n\n"
        "\tCLIENTES / HÓSPEDES\n\n\t\tCadastrar novo hóspede --- a\n\t\tAlterar dados de hóspede --- b\n"
        "\t\tConsultar dados de hóspede --- c\n\t\tListar todos os hóspedes --- d\n\n"
        "\tQUARTOS\n\n\t\tCadastrar novo quarto --- e\n\t\tConsultar dados de quarto --- f\n\t\tListar quartos --- g\n\n"
        "\tHOSPEDAGENS / RESERVAS\n\n\t\tRealizar check-in --- h\n\t\tRealizar check-out --- i\n\t\tListar hospedagens --- j\n\n"
        "\tSAIR DO PROGRAMA --- x\n");

    return 0;
}