#ifndef QUARTO_H
#define QUARTO_H

#define LIM_QUARTOS 50

int loadQuarto();
int fechaQuarto();

int statusQuarto(int num); //retorna status do quarto
int diariaQuarto(int num,float* diaria); //passa o valor da diaria pelo parametro int* diaria

int consultaQuarto(int num); //imprime as informações do quarto
int cadastraQuarto(int num, int tipo, float valor); //status sempre vazio quando criado
int listaQuartos(int modo); //modo: 0= livres, 1= todos

#endif