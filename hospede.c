#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospede.h"

/* copia a string pra memoria dinamica, devolve NULL se falhar */
static char* copiaTexto(const char* origem) {
    char* copia = (char*)malloc(strlen(origem) + 1);
    if (copia) strcpy(copia, origem);
    return copia;
}

/* procura um hospede por codigo (tipo 1) ou documento (tipo 2) */
static hospede* buscaHospede(cadeiaHospede* lista, int id, int tipo) {
    while (lista) {
        if (lista->hospede) {
            if (tipo == 1 && lista->hospede->codigo == id) return lista->hospede;
            if (tipo == 2 && lista->hospede->documento == id) return lista->hospede;
        }
        lista = lista->prox;
    }
    return NULL;
}

int cadastraHospede(cadeiaHospede** lista, int codigo, char* nome, int documento, int telefone) {
    if (codigo <= 0) return 1;

    if (buscaHospede(*lista, codigo, 1) || buscaHospede(*lista, documento, 2)) return 2;

    int total = 0;
    for (cadeiaHospede* p = *lista; p; p = p->prox) total++;
    if (total >= LIM_HOSPEDES) return 3;

    cadeiaHospede* novoNo = (cadeiaHospede*)malloc(sizeof(cadeiaHospede));
    if (!novoNo) return 4;

    novoNo->hospede = (hospede*)malloc(sizeof(hospede));
    if (!novoNo->hospede) {
        free(novoNo);
        return 4;
    }

    novoNo->hospede->nome = NULL;
    if (nome) {
        novoNo->hospede->nome = copiaTexto(nome);
        if (!novoNo->hospede->nome) {
            free(novoNo->hospede);
            free(novoNo);
            return 4;
        }
    }

    novoNo->hospede->codigo = codigo;
    novoNo->hospede->documento = documento;
    novoNo->hospede->telefone = telefone;

    novoNo->prox = *lista; /* insere no inicio */
    *lista = novoNo;
    return 0;
}

int consultaHospede(cadeiaHospede* lista, int id, int tipo) {
    if (tipo != 1 && tipo != 2) return 2;

    hospede* h = buscaHospede(lista, id, tipo);
    if (!h) return 1;

    printf("\n--- dados do hospede ---\n");
    printf("codigo: %d\n", h->codigo);
    printf("nome: %s\n", h->nome ? h->nome : "N/D");
    printf("documento: %d\n", h->documento);
    printf("telefone: %d\n", h->telefone);
    return 0;
}

void listaHospedes(cadeiaHospede* lista) {
    if (!lista) {
        printf("\nnao existem hospedes registados\n");
        return;
    }

    printf("\n--- lista de hospedes ---\n\n");
    while (lista) {
        if (lista->hospede) {
            printf("codigo: %d\tnome: %s\tdocumento: %d\ttelefone: %d\n",
                   lista->hospede->codigo,
                   lista->hospede->nome ? lista->hospede->nome : "N/D",
                   lista->hospede->documento,
                   lista->hospede->telefone);
        }
        lista = lista->prox;
    }
}

int alteraHospede(cadeiaHospede* lista, int id, int tipo, char* novoNome, int novoTelefone) {
    if (tipo != 1 && tipo != 2) return 2;

    hospede* h = buscaHospede(lista, id, tipo);
    if (!h) return 1;

    if (novoNome) {
        char* copia = copiaTexto(novoNome);
        if (!copia) return 4; /* mantem o nome antigo se falhar */
        free(h->nome);
        h->nome = copia;
    }

    h->telefone = novoTelefone;
    return 0;
}

void liberaHospedes(cadeiaHospede** lista) {
    cadeiaHospede* atual = *lista;
    while (atual) {
        cadeiaHospede* prox = atual->prox;
        if (atual->hospede) {
            free(atual->hospede->nome);
            free(atual->hospede);
        }
        free(atual);
        atual = prox;
    }
    *lista = NULL;
}
