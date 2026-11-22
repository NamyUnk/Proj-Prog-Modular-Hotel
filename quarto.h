#ifndef QUARTO_H
#define QUARTO_H

/*
MÓDULO QUARTO

Carrega os quartos de um arquivo binário, armazenando em uma lista encadeada temporária durante
o uso, ordenando por número do quarto. 
A lista fica encapsulada no quarto.c, não sendo possível acessá-la diretamente de outros módulos,
apenas utilizando funções de interface.

Dados de cada quarto:

- Número do quarto (int)
- Tipo do quarto (int) 0=Suíte, 1=Solteiro, 2=Casal
- Valor da diária (float)
- Status do quarto (int) 0=livre, 1=ocupado
*/

#define LIM_QUARTOS 50

/*
FUNÇÕES DE INICIALIZAÇÃO E FINALIZAÇÃO

--- loadQuarto

Carrega os quartos do arquivo binário "quarto.bin" para a lista encadeada temporária.



*/
int loadQuarto(void);
int fechaQuarto(void);

int statusQuarto(int num); //retorna status do quarto
int diariaQuarto(int num,float* diaria); //passa o valor da diaria pelo parametro int* diaria

int consultaQuarto(int num); //imprime as informações do quarto
int cadastraQuarto(int num, int tipo, float valor); //status sempre vazio quando criado
int listaQuartos(int modo); //modo: 0= livres, 1= todos

#endif