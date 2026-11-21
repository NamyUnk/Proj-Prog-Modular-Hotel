#ifndef HOSPEDE_H
#define HOSPEDE_H

#define LIM_HOSPEDES 100

#include <string.h>

int consultaHospede(int id, int tipo); //id: código ou documento - tipo: 1= código, 2= documento
int cadastraHospede(int codigo, char* nome, int documento, int telefone);
void listaHospedes();
int alteraHospede(int id, int tipo);

int loadHospede();
int fechaHospede();

#endif
