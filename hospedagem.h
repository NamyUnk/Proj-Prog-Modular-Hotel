#ifndef HOSPEDAGEM_H
#define HOSPEDAGEM_H

#define LIM_HOSPEDAGENS 500

#include "hospede.h"
#include "quarto.h"

typedef struct horario{int dia;int mes;int ano;int hora;int minuto;} horario_S;
// dicionarios de enum assustadores a seguir
typedef enum {
    HP_ATIVA = 0,
    HP_ENCERRADA = 1
} HP_STATUS;
typedef enum {
    HP_OK = 0,                     /* tudo ok */
    HP_HOSPEDE_INEXISTENTE,        /* codigo de hospede nao cadastrado */
    HP_HOSPEDE_JA_HOSPEDADO,       /* hospede ja tem hospedagem ativa (RN02) */
    HP_QUARTO_INEXISTENTE,         /* numero de quarto nao cadastrado */
    HP_QUARTO_OCUPADO,             /* quarto ja tem hospedagem ativa (RN01) */
    HP_DIARIAS_INVALIDAS,          /* diarias previstas <= 0 */
    HP_DATA_INVALIDA,              /* data ou horario fora do formato */
    HP_SAIDA_ANTES_DA_ENTRADA,     /* check-out antes de um check-in */
    HP_HOSPEDAGEM_NAO_ENCONTRADA,  /* nao ha hospedagem ativa para o par hospede/quarto */
    HP_STATUS_INVALIDO,            /* filtro diferente de HP_ATIVA e HP_ENCERRADA */
    HP_LISTA_VAZIA,                /* nenhuma hospedagem na situacao pedida */
    HP_LIMITE_ATINGIDO,            /* LIM_HOSPEDAGENS alcancado */
    HP_ERRO_MEMORIA,               /* falha de alocacao */
    HP_ERRO_QUARTO                 /* modulo Quarto deu erro */
} HP_RET;



int listaHospedagens(int status);
int checkIn(int hospede, int quarto, horario_S hora, int diarias);
int checkOut(int hospede, int quarto, horario_S hora);

#endif