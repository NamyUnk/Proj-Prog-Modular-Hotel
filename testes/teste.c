#include <stdio.h>

#include "teste.h"

static int totalTestes = 0; //contadores encapsulados: so este modulo acessa
static int totalFalhas = 0;

void confereTeste(char* descricao, int obtido, int esperado){

    totalTestes++;

    if(obtido == esperado){

        printf("[OK] %s\n", descricao);
    }
    else{

        totalFalhas++;
        printf("[FALHA] %s --- esperado: %d, obtido: %d\n", descricao, esperado, obtido);
    }
}

int resultadoTestes(char* modulo){

    printf("\n=== %s: %d testes, %d falhas ===\n", modulo, totalTestes, totalFalhas);

    return totalFalhas;
}
