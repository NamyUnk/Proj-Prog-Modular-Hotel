#ifndef QUARTO_H
#define QUARTO_H

#define LIM_QUARTOS 50

typedef struct quarto{

    int num;
    int tipo; //tipo: 0= suite, 1= solteiro, 2= casal
    float valor;
    int status; //status: 0= vazio, 1= ocupado
    
} quarto;

typedef struct cadeiaQuarto{

    quarto* quarto;
    struct cadeiaQuarto* prox;

} cadeiaQuarto;

cadeiaQuarto* buscaQuarto(int num, cadeiaQuarto* cabeca);
int consultaQuarto(int num, cadeiaQuarto* cabeca);
cadeiaQuarto* cadastraQuarto(int num, int tipo, float valor, cadeiaQuarto* cabeca); //status sempre vazio quando criado
void listaQuartos(int modo, cadeiaQuarto* cabeca); //modo: 0= livres, 1= todos

#endif