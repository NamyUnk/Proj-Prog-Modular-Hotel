/*
COMO RODAR (dentro da pasta testes):
    gcc test_quarto.c teste.c ..\quarto.c -o test_quarto
    .\test_quarto

Resultado esperado: === QUARTO: 38 testes, 0 falhas ===
*/

#include <stdio.h>

#include "teste.h"
#include "../quarto.h"

int main(void){

    float diaria = 0;
    FILE* arquivo;

    remove("quarto.bin"); //apaga o arquivo de testes anteriores, para comecar do zero

    //LISTA VAZIA:

    printf("\n--- LISTA VAZIA ---\n\n");

    confereTeste("Carregar sem arquivo existente", loadQuarto(), 1);
    confereTeste("Consultar quarto com lista vazia", consultaQuarto(112), 1);
    confereTeste("Listar quartos com lista vazia", listaQuartos(1), 1);

    //CADASTRO COM VALORES INVALIDOS:

    printf("\n--- CADASTRO INVALIDO ---\n\n");

    confereTeste("Cadastrar quarto numero 0", cadastraQuarto(0,0,250), 1);
    confereTeste("Cadastrar quarto numero negativo", cadastraQuarto(-5,0,250), 1);
    confereTeste("Cadastrar quarto com diaria 0", cadastraQuarto(101,0,0), 1);
    confereTeste("Cadastrar quarto com diaria negativa", cadastraQuarto(101,0,-10), 1);
    confereTeste("Cadastrar quarto com tipo invalido (3)", cadastraQuarto(101,3,250), 1);
    confereTeste("Quarto invalido nao entrou na lista", consultaQuarto(101), 1);

    //CADASTRO COM VALORES VALIDOS:

    printf("\n--- CADASTRO VALIDO ---\n\n");

    confereTeste("Cadastrar quarto 112 (nova lista)", cadastraQuarto(112,0,250), 0);
    confereTeste("Cadastrar quarto 120 (fim da lista)", cadastraQuarto(120,2,300), 0);
    confereTeste("Cadastrar quarto 105 (comeco da lista)", cadastraQuarto(105,1,180), 0);
    confereTeste("Cadastrar quarto 115 (meio da lista)", cadastraQuarto(115,2,250), 0);

    //CADASTRO REPETIDO:

    printf("\n--- CADASTRO REPETIDO ---\n\n");

    confereTeste("Cadastrar 112 de novo", cadastraQuarto(112,1,100), 2);
    confereTeste("Cadastrar 105 de novo (primeiro da lista)", cadastraQuarto(105,1,100), 2);
    confereTeste("Cadastrar 120 de novo (ultimo da lista)", cadastraQuarto(120,1,100), 2);

    //CONSULTA:

    printf("\n--- CONSULTA ---\n\n");

    confereTeste("Consultar quarto 112 existente", consultaQuarto(112), 0);
    confereTeste("Consultar quarto 999 inexistente", consultaQuarto(999), 1);

    //STATUS:

    printf("\n--- STATUS ---\n\n");

    confereTeste("Status do quarto 112 (livre)", statusQuarto(112), 0);
    confereTeste("Status do quarto 999 inexistente", statusQuarto(999), 3);

    //DIARIA:

    printf("\n--- DIARIA ---\n\n");

    confereTeste("Resgatar diaria do quarto 112", diariaQuarto(112,&diaria), 0);
    confereTeste("Diaria do quarto 112 e 250", diaria == 250, 1);
    confereTeste("Resgatar diaria com ponteiro nulo", diariaQuarto(112,NULL), 1);
    confereTeste("Resgatar diaria do quarto 999 inexistente", diariaQuarto(999,&diaria), 2);

    //LISTAGEM:

    printf("\n--- LISTAGEM ---\n\n");

    confereTeste("Listar quartos livres (modo 0)", listaQuartos(0), 0);
    confereTeste("Listar todos os quartos (modo 1)", listaQuartos(1), 0);
    confereTeste("Listar com modo invalido (2)", listaQuartos(2), 2);

    //OCUPAR QUARTO:

    printf("\n--- OCUPAR QUARTO ---\n\n");

    confereTeste("Ocupar o quarto 112", ocupaQuarto(112), 0);
    confereTeste("Status do quarto 112 agora e ocupado (1)", statusQuarto(112), 1);
    confereTeste("Ocupar o 112 de novo (ja ocupado)", ocupaQuarto(112), 2);
    confereTeste("Ocupar o quarto 999 inexistente", ocupaQuarto(999), 1);
    confereTeste("Listar livres com o 112 ocupado", listaQuartos(0), 0);

    //LIBERAR QUARTO:

    printf("\n--- LIBERAR QUARTO ---\n\n");

    confereTeste("Liberar o quarto 112", liberaQuarto(112), 0);
    confereTeste("Status do quarto 112 voltou a livre (0)", statusQuarto(112), 0);
    confereTeste("Liberar o 112 de novo (ja livre)", liberaQuarto(112), 2);
    confereTeste("Liberar o quarto 999 inexistente", liberaQuarto(999), 1);

    //GRAVACAO NO ARQUIVO:

    printf("\n--- GRAVACAO ---\n\n");

    confereTeste("Salvar quartos no arquivo", fechaQuarto(), 0);

    arquivo = fopen("quarto.bin", "rb");
    confereTeste("Arquivo quarto.bin foi criado", arquivo != NULL, 1);
    if(arquivo) fclose(arquivo);

    remove("quarto.bin"); //apaga o arquivo criado pelo teste

    //RESULTADO:

    return resultadoTestes("QUARTO");
}