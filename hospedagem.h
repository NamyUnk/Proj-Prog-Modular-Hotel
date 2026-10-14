#ifndef HOSPEDAGEM_H
#define HOSPEDAGEM_H

#define LIM_HOSPEDAGENS = 500

#include "hospede.h"
#include "quarto.h"

typedef struct hospedagem{

    int codigo;
    int status; //0= ativa, 1= encerrada
    int hospede; //int hospede: pode ser código ou documento
    int quarto;
    int data_entrada;
    int hora_entrada;
    int data_saida;
    int hora_saida;
    int diarias_reservadas;
    int diarias_efetivas;

} hospedagem;

void listaHospedagens(int status);
int checkIn(int hospede, int quarto, int data, int hora, int diarias);
int checkOut(int hospede, int quarto, int data, int hora);

