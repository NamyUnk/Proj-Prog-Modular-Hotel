#ifndef HOSPEDE_H
#define HOSPEDE_H

#define LIM_HOSPEDES = 100

#include <string.h>

typedef char[100] str;

int consultaHospede(int id, int tipo); //id: código ou documento - tipo: 1= código, 2= documento
int cadastraHospede(int codigo, str nome, int documento, int telefone);
void listaHospedes();
int alteraHospede(int id, int tipo);