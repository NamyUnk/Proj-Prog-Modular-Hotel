#ifndef HOSPEDAGEM_H
#define HOSPEDAGEM_H

#define LIM_HOSPEDAGENS 500

#include "hospede.h"
#include "quarto.h"

typedef struct data{

    int dia;
    int mes;
    int ano;

} data;

typedef struct horario{

    int hora;
    int minuto;

} horario;

typedef struct hospedagem{

    int codigo;
    int status; //0= ativa, 1= encerrada
    int hospede; //int hospede: código
    int quarto;
    data* dataEntrada;
    horario* horaEntrada;
    data* dataSaida;
    horario* horaSaida;
    int diarias_reservadas;
    int diarias_efetivas;

} hospedagem;

typedef struct cadeiaHospedagem{

    hospedagem* hospedagem;
    struct cadeiaHospedagem* prox;

} cadeiaHospedagem;

void listaHospedagens(int status);
int checkIn(int hospede, int quarto, int data, int hora, int diarias);
int checkOut(int hospede, int quarto, int data, int hora);

#endif