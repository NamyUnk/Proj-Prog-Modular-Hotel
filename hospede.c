#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospede.h"

/*
codigos de retorno das funcoes PRA USAR NA MAIN QUANDO ESTIVER PRONTA:
0 = sucesso na operacao
1 = codigo invalido (precisa ser inteiro positivo maior que zero)
2 = hospede duplicado (codigo ou documento ja cadastrado no sistema)
3 = limite atingido (atingiu a capacidade maxima de lim_hospedes)
4 = erro de alocacao de memoria (malloc falhou ao tentar alocar)
*/

extern cadeiaHospede* hospedes; //variavel global pra hospedes (será criada na main)

int cadastraHospede(int codigo, char* nome, int documento, int telefone) {
    if (codigo <= 0) {
        printf("\nO código do hóspede precisa de ser um inteiro positivo.\n");
        return 1;
    }
                
cadeiaHospede* atual = hospedes; // verifica hospedes duplicados e o limite max
int total = 0;

    while (atual) { //enquanto houverem hospedes:
        if (atual->hospede) {
            if (atual->hospede->codigo == codigo) {
                printf("\njá existe um hóspede registado com esse código %d.\n", codigo);
                return 2;
            }
            if (atual->hospede->documento == documento) {
                printf("\nJá existe um hóspede registado com o documento %d.\n", documento);
                return 2;
            }
        }
        total++;
        atual = atual->prox;
    }

    if (total >= LIM_HOSPEDES) {
        printf("\nLimite máximo de hóspedes atingido (%d).\n", LIM_HOSPEDES);
        return 3;
    }

    cadeiaHospede* novoNo = (cadeiaHospede*)malloc(sizeof(cadeiaHospede)); // alocando o nó da lista
    if (!novoNo) {
        printf("\nErro ao alocar memória para o nó do hóspede.\n");
        return 4;
    }

    novoNo->hospede = (hospede*)malloc(sizeof(hospede)); // alocando a struct do hospede
    if (!novoNo->hospede) {
        free(novoNo);
        printf("\nErro ao alocar memória para os dados do hóspede.\n");
        return 4;
    }

    //valores:
    novoNo->hospede->codigo = codigo;
    novoNo->hospede->documento = documento;
    novoNo->hospede->telefone = telefone;

    if (nome) {
        novoNo->hospede->nome = (char*)malloc(strlen(nome) + 1);
        if (novoNo->hospede->nome) {
            strcpy(novoNo->hospede->nome, nome);
        } else {
            novoNo->hospede->nome = NULL;
        }
    } else {
        novoNo->hospede->nome = NULL;
    }

    novoNo->prox = hospedes; //insere no inicio da lista
    hospedes = novoNo;

    printf("\nHóspede %s registado com sucesso!\n", nome ? nome : ""); //exibição do nome e mensagem de sucesso
    return 0;
}

int consultaHospede(int id, int tipo) {
    
    if (tipo != 1 && tipo != 2) {
        printf("\nTipo de consulta inválido! Use 1 para Código ou 2 para Documento.\n");
        return 2;
    }

    cadeiaHospede* atual = hospedes;

    while (atual) { //percorre a lista de hospedes 
        if (atual->hospede) {
            int encontrado = 0;
            if (tipo == 1 && atual->hospede->codigo == id) encontrado = 1;
            if (tipo == 2 && atual->hospede->documento == id) encontrado = 1;

            if (encontrado) { //exibe os dados do hospede pedido
                printf("\n--- Dados do Hóspede ---\n");
                printf("Código: %d\n", atual->hospede->codigo);
                printf("Nome: %s\n", atual->hospede->nome ? atual->hospede->nome : "N/D");
                printf("Documento: %d\n", atual->hospede->documento);
                printf("Telefone: %d\n", atual->hospede->telefone);
                return 0;
            }
        }
        atual = atual->prox;
    }

    printf("\nHóspede não encontrado.\n");
    return 1;
}

void listaHospedes() {
    cadeiaHospede* atual = hospedes;

    if (!atual) {
        printf("\nNão existem hóspedes registados no sistema.\n");
        return;
    }

    printf("\n--- Lista de Hóspedes ---\n\n");
    while (atual) {
        if (atual->hospede) {
            printf("Código: %d\tNome: %s\tDocumento: %d\tTelefone: %d\n",
                   atual->hospede->codigo,
                   atual->hospede->nome ? atual->hospede->nome : "N/D",
                   atual->hospede->documento,
                   atual->hospede->telefone);
        }
        atual = atual->prox;
    }
}

int alteraHospede(int id, int tipo) {
    if (tipo != 1 && tipo != 2) {
        printf("\nTipo inválido! Use 1 para Código ou 2 para Documento.\n");
        return 2;
    }

    cadeiaHospede* atual = hospedes;

    while (atual) {
        if (atual->hospede) {
            int encontrado = 0;
            if (tipo == 1 && atual->hospede->codigo == id) encontrado = 1;
            if (tipo == 2 && atual->hospede->documento == id) encontrado = 1;

            if (encontrado) {
                char novoNome[100];
                int novoTelefone;

                printf("\nHóspede encontrado: %s (Código: %d)\n", 
                       atual->hospede->nome ? atual->hospede->nome : "N/D", 
                       atual->hospede->codigo);

                printf("Introduza o novo nome: ");
                scanf(" %[^\n]", novoNome);

                printf("Introduza o novo telefone: ");
                scanf("%d", &novoTelefone);

                if (atual->hospede->nome) { //libera a memoria do nome antigo alocado dinamicamente
                    free(atual->hospede->nome);
                }
                atual->hospede->nome = (char*)malloc(strlen(novoNome) + 1); //aloca um novo
                if (atual->hospede->nome) {
                    strcpy(atual->hospede->nome, novoNome);
                }

                atual->hospede->telefone = novoTelefone;

                printf("\nDados do hóspede alterados com sucesso!\n");
                return 0;
            }
        }
        atual = atual->prox;
    }

    printf("\nHóspede não encontrado para alteração.\n");
    return 1;


}
