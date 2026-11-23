/*
COMO RODAR (dentro da pasta testes):
    gcc test_hospede.c teste.c ..\hospede.c -o test_hospede
    .\test_hospede

Resultado esperado: === HOSPEDE: 34 testes, 0 falhas ===
*/

#include <stdio.h>
#include <string.h>

#include "teste.h"
#include "../hospede.h"

int main(void){

    FILE* arquivo;
    char nome[100];
    char curto[4];
    int i;
    int cadastrados = 0;

    remove("hospede.bin"); //apaga o arquivo de testes anteriores, para comecar do zero

    //LISTA VAZIA:

    printf("\n--- LISTA VAZIA ---\n\n");

    confereTeste("Carregar sem arquivo existente", loadHospede(), 1);
    confereTeste("Consultar hospede com lista vazia", consultaHospede(1,1), 1);
    confereTeste("Listar hospedes com lista vazia", listaHospedes(), 1);

    //CADASTRO COM VALORES INVALIDOS:

    printf("\n--- CADASTRO INVALIDO ---\n\n");

    confereTeste("Cadastrar hospede com codigo 0", cadastraHospede(0,"Ana",111,2199), 1);
    confereTeste("Cadastrar hospede com codigo negativo", cadastraHospede(-3,"Ana",111,2199), 1);
    confereTeste("Hospede invalido nao entrou na lista", consultaHospede(111,2), 1);

    //CADASTRO COM VALORES VALIDOS:

    printf("\n--- CADASTRO VALIDO ---\n\n");

    confereTeste("Cadastrar hospede 1 (Ana)", cadastraHospede(1,"Ana",111,2199), 0);
    confereTeste("Cadastrar hospede 2 (Bruno)", cadastraHospede(2,"Bruno",222,2198), 0);
    confereTeste("Cadastrar hospede 3 (sem nome)", cadastraHospede(3,NULL,333,2197), 0);

    confereTeste("Listar hospedes cadastrados", listaHospedes(), 0);

    //CADASTRO REPETIDO:

    printf("\n--- CADASTRO REPETIDO ---\n\n");

    confereTeste("Cadastrar codigo 1 de novo", cadastraHospede(1,"Carla",444,2196), 2);
    confereTeste("Cadastrar documento 222 de novo", cadastraHospede(4,"Carla",222,2196), 2);

    //CONSULTA:

    printf("\n--- CONSULTA ---\n\n");

    confereTeste("Consultar hospede 1 pelo codigo", consultaHospede(1,1), 0);
    confereTeste("Consultar hospede pelo documento 222", consultaHospede(222,2), 0);
    confereTeste("Consultar codigo 99 inexistente", consultaHospede(99,1), 1);
    confereTeste("Consultar documento 999 inexistente", consultaHospede(999,2), 1);
    confereTeste("Consultar com tipo invalido (3)", consultaHospede(1,3), 2);

    //VERIFICAR HOSPEDE:

    printf("\n--- VERIFICAR HOSPEDE ---\n\n");

    confereTeste("Verificar hospede 1 existente", verificaHospede(1), 0);
    confereTeste("Verificar hospede 99 inexistente", verificaHospede(99), 1);

    //OBTER NOME:

    printf("\n--- OBTER NOME ---\n\n");

    confereTeste("Obter nome do hospede 2", obtemNomeHospede(2, nome, sizeof(nome)), 0);
    confereTeste("Nome obtido e Bruno", strcmp(nome, "Bruno") == 0, 1);
    confereTeste("Obter nome do hospede 3 (sem nome)", obtemNomeHospede(3, nome, sizeof(nome)), 0);
    confereTeste("Hospede sem nome devolve N/D", strcmp(nome, "N/D") == 0, 1);
    confereTeste("Obter nome do hospede 99 inexistente", obtemNomeHospede(99, nome, sizeof(nome)), 1);
    confereTeste("Obter nome numa variavel pequena", obtemNomeHospede(2, curto, sizeof(curto)), 0);
    confereTeste("Nome cortado no tamanho da variavel", strcmp(curto, "Bru") == 0, 1);

    //ALTERACAO:

    printf("\n--- ALTERACAO ---\n\n");

    confereTeste("Alterar com tipo invalido (3)", alteraHospede(1,3), 2);
    confereTeste("Alterar hospede 99 inexistente", alteraHospede(99,1), 1);

    //alteraHospede le o novo nome e telefone do teclado (scanf).
    //Para o teste rodar sozinho, o teclado e trocado por um arquivo com as respostas.
    arquivo = fopen("entrada_teste.txt", "w");
    fprintf(arquivo, "Ana Maria\n2195\n");
    fclose(arquivo);
    freopen("entrada_teste.txt", "r", stdin);

    confereTeste("Alterar hospede 1 (novo nome e telefone)", alteraHospede(1,1), 0);
    remove("entrada_teste.txt");

    //LIMITE DE HOSPEDES:

    printf("\n--- LIMITE DE HOSPEDES (%d) ---\n\n", LIM_HOSPEDES);

    //ja existem 3 hospedes; cadastra mais ate chegar no limite
    for(i = 1; i <= LIM_HOSPEDES - 3; i++){
        if(cadastraHospede(1000 + i, "Teste", 1000 + i, 2100) == 0) cadastrados++;
    }

    confereTeste("Cadastrar hospedes ate o limite", cadastrados, LIM_HOSPEDES - 3);
    confereTeste("Cadastrar alem do limite", cadastraHospede(5000,"Extra",5000,2100), 3);

    //GRAVACAO NO ARQUIVO (fechaHospede salva e libera a memoria):

    printf("\n--- GRAVACAO E LIBERACAO ---\n\n");

    confereTeste("Salvar hospedes no arquivo", fechaHospede(), 0);

    arquivo = fopen("hospede.bin", "rb");
    confereTeste("Arquivo hospede.bin foi criado", arquivo != NULL, 1);
    if(arquivo) fclose(arquivo);

    confereTeste("Lista foi liberada depois de fechar", verificaHospede(1), 1);

    remove("hospede.bin"); //apaga o arquivo criado pelo teste

    //RESULTADO:

    return resultadoTestes("HOSPEDE");
}