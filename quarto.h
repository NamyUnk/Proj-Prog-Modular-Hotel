#ifndef QUARTO_H
#define QUARTO_H

#define LIM_QUARTOS = 50

typedef struct quarto{

    int num;
    int tipo; //tipo: 0= suite, 1= solteiro, 2= casal
    int valor;
    int status; //status: 0= vazio, 1= ocupado
    
} quarto;

int consultaQuarto(int num);
int cadastraQuarto(int num, int tipo, int valor); //status sempre vazio quando criado
void listaQuartos(int modo); //modo: 0= livres, 1= todos
