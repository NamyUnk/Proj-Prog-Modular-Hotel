/*
COMO RODAR (dentro da pasta testes):
    gcc test_hospedagem.c teste.c ..\hospedagem.c ..\quarto.c ..\hospede.c -o test_hospedagem
    .\test_hospedagem
    
Resultado esperado: === HOSPEDAGEM: 39 testes, 0 falhas ===
Obs: compila junto com quarto.c e hospede.c porque o Hospedagem usa os dois.
*/

#include <stdio.h>

#include "teste.h"
#include "../quarto.h"
#include "../hospede.h"
#include "../hospedagem.h"

int main(void){

    FILE* arquivo;
    float valor = 0;

    int diaEntrada = 20;
    int mesEntrada = 11;
    int anoEntrada = 2026;
    int horaEntrada = 14;
    int minutoEntrada = 0; //20/11/2026 14:00

    int diaSaida = 22;
    int mesSaida = 11;
    int anoSaida = 2026;
    int horaSaida = 11;
    int minutoSaida = 43; //22/11/2026 11:00 (2 diarias)

    int diaMesmoDia = 20;
    int mesMesmoDia = 11;
    int anoMesmoDia = 2026;
    int horaMesmoDia = 18;
    int minutoMesmoDia = 55; //20/11/2026 18:55 (menos de 1 dia)

    int diaAntes = 19;
    int mesAntes = 11;
    int anoAntes = 2026;
    int horaAntes = 10;
    int minutoAntes = 12; //antes da entrada

    int diaDiaInvalido = 31;
    int mesDiaInvalido = 2;
    int anoDiaInvalido = 2026;
    int horaDiaInvalido = 18;
    int minutoDiaInvalido = 24; //31 de fevereiro

    int diaHoraInvalida = 20;
    int mesHoraInvalida = 11;
    int anoHoraInvalida = 2026;
    int horaHoraInvalida = 25;
    int minutoHoraInvalida = 0; //25 horas

    int quantidade = 5;

    remove("hospedagem.bin"); //apaga o arquivo de testes anteriores, para comecar do zero

    //PREPARACAO: quartos e hospedes usados nos testes

    cadastraQuarto(301, 2, 200); //casal, R$ 200,00
    cadastraQuarto(302, 0, 350); //suite, R$ 350,00
    cadastraQuarto(303, 1, 150); //solteiro, R$ 150,00

    cadastraHospede(1, "Ana", 111, 2199);
    cadastraHospede(2, "Bruno", 222, 2198);
    cadastraHospede(3, "Carla", 333, 2197);

    //LISTA VAZIA:

    printf("\n--- LISTA VAZIA ---\n\n");

    confereTeste("Listar ativas sem nenhuma hospedagem", listaHospedagens(HP_ATIVA), HP_LISTA_VAZIA);
    confereTeste("Carregar sem arquivo existente", loadHospedagem(), HP_ERRO_ARQUIVO);

    //CHECK-IN INVALIDO:

    printf("\n--- CHECK-IN INVALIDO ---\n\n");

    confereTeste("Check-in de hospede inexistente (99)", checkIn(99, 301, diaEntrada, mesEntrada, anoEntrada, horaEntrada, minutoEntrada, 2), HP_HOSPEDE_INEXISTENTE);
    confereTeste("Check-in em quarto inexistente (999)", checkIn(1, 999, diaEntrada, mesEntrada, anoEntrada, horaEntrada, minutoEntrada, 2), HP_QUARTO_INEXISTENTE);
    confereTeste("Check-in com 0 diarias", checkIn(1, 301, diaEntrada, mesEntrada, anoEntrada, horaEntrada, minutoEntrada, 0), HP_DIARIAS_INVALIDAS);
    confereTeste("Check-in em 31 de fevereiro", checkIn(1, 301, diaDiaInvalido, mesDiaInvalido, anoDiaInvalido, horaDiaInvalido, minutoDiaInvalido, 2), HP_DATA_INVALIDA);
    confereTeste("Check-in as 25 horas", checkIn(1, 301, diaHoraInvalida, mesHoraInvalida, anoHoraInvalida, horaHoraInvalida, minutoHoraInvalida, 2), HP_DATA_INVALIDA);
    confereTeste("Quarto 301 continua livre", statusQuarto(301), 0);

    //CHECK-IN VALIDO:

    printf("\n--- CHECK-IN VALIDO ---\n\n");

    confereTeste("Check-in da Ana (1) no quarto 301", checkIn(1, 301, diaEntrada, mesEntrada, anoEntrada, horaEntrada, minutoEntrada, 2), HP_OK);
    confereTeste("Quarto 301 ficou ocupado", statusQuarto(301), 1);
    confereTeste("Check-in do Bruno (2) no quarto 302", checkIn(2, 302, diaEntrada, mesEntrada, anoEntrada, horaEntrada, minutoEntrada, 3), HP_OK);

    //CHECK-IN REPETIDO:

    printf("\n--- CHECK-IN REPETIDO ---\n\n");

    confereTeste("Check-in da Carla no 301 ocupado (RN01)", checkIn(3, 301, diaEntrada, mesEntrada, anoEntrada, horaEntrada, minutoEntrada, 1), HP_QUARTO_OCUPADO);
    confereTeste("Check-in da Ana ja hospedada (RN02)", checkIn(1, 303, diaEntrada, mesEntrada, anoEntrada, horaEntrada, minutoEntrada, 1), HP_HOSPEDE_JA_HOSPEDADO);

    //LISTAGEM:

    printf("\n--- LISTAGEM ---\n\n");

    confereTeste("Listar hospedagens ativas", listaHospedagens(HP_ATIVA), HP_OK);
    confereTeste("Listar encerradas (nenhuma ainda)", listaHospedagens(HP_ENCERRADA), HP_LISTA_VAZIA);
    confereTeste("Listar com status invalido", listaHospedagens((HP_STATUS) 5), HP_STATUS_INVALIDO);

    //GRAVACAO NO ARQUIVO (com 2 hospedagens ativas):

    printf("\n--- GRAVACAO ---\n\n");

    confereTeste("Salvar hospedagens no arquivo", fechaHospedagem(), HP_OK);

    //CHECK-OUT INVALIDO:

    printf("\n--- CHECK-OUT INVALIDO ---\n\n");

    confereTeste("Check-out de hospede inexistente (99)", checkOut(99, 301, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, &valor), HP_HOSPEDE_INEXISTENTE);
    confereTeste("Check-out em quarto inexistente (999)", checkOut(1, 999, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, &valor), HP_QUARTO_INEXISTENTE);
    confereTeste("Check-out da Carla, que nao esta hospedada", checkOut(3, 301, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, &valor), HP_HOSPEDAGEM_NAO_ENCONTRADA);
    confereTeste("Check-out da Ana no quarto errado (302)", checkOut(1, 302, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, &valor), HP_HOSPEDAGEM_NAO_ENCONTRADA);
    confereTeste("Check-out em 31 de fevereiro", checkOut(1, 301, diaDiaInvalido, mesDiaInvalido, anoDiaInvalido, horaDiaInvalido, minutoDiaInvalido, &valor), HP_DATA_INVALIDA);
    confereTeste("Check-out antes da entrada", checkOut(1, 301, diaAntes, mesAntes, anoAntes, horaAntes, minutoAntes, &valor), HP_SAIDA_ANTES_DA_ENTRADA);
    confereTeste("Quarto 301 continua ocupado", statusQuarto(301), 1);

    //CHECK-OUT VALIDO:

    printf("\n--- CHECK-OUT VALIDO ---\n\n");

    confereTeste("Check-out da Ana no 301", checkOut(1, 301, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, &valor), HP_OK);
    confereTeste("Valor total = 2 diarias x R$ 200 = R$ 400 (RN03)", valor == 400, 1);
    confereTeste("Quarto 301 voltou a ficar livre", statusQuarto(301), 0);
    confereTeste("Check-out da Ana de novo", checkOut(1, 301, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, &valor), HP_HOSPEDAGEM_NAO_ENCONTRADA);

    confereTeste("Check-out do Bruno no mesmo dia", checkOut(2, 302, diaMesmoDia, mesMesmoDia, anoMesmoDia, horaMesmoDia, minutoMesmoDia, &valor), HP_OK);
    confereTeste("Cobra no minimo 1 diaria: R$ 350", valor == 350, 1);

    confereTeste("Listar hospedagens encerradas", listaHospedagens(HP_ENCERRADA), HP_OK);
    confereTeste("Listar ativas (todas encerradas)", listaHospedagens(HP_ATIVA), HP_LISTA_VAZIA);

    //QUARTO E HOSPEDE LIBERADOS:

    printf("\n--- NOVA HOSPEDAGEM DEPOIS DO CHECK-OUT ---\n\n");

    confereTeste("Check-in da Carla no 301 liberado", checkIn(3, 301, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, 1), HP_OK);
    confereTeste("Check-in da Ana de novo (no 303)", checkIn(1, 303, diaSaida, mesSaida, anoSaida, horaSaida, minutoSaida, 1), HP_OK);

    //CARREGAR DO ARQUIVO:

    printf("\n--- CARREGAR DO ARQUIVO ---\n\n");

    //o arquivo foi salvo quando havia 2 hospedagens ativas e nenhuma encerrada
    confereTeste("Carregar hospedagens do arquivo", loadHospedagem(), HP_OK);
    confereTeste("Hospedagens ativas foram recuperadas", listaHospedagens(HP_ATIVA), HP_OK);
    confereTeste("Lista atual foi trocada pela do arquivo", listaHospedagens(HP_ENCERRADA), HP_LISTA_VAZIA);

    //arquivo que promete 5 hospedagens mas nao tem nenhuma
    arquivo = fopen("hospedagem.bin", "wb");
    fwrite(&quantidade, sizeof(int), 1, arquivo);
    fclose(arquivo);

    confereTeste("Carregar arquivo invalido", loadHospedagem(), HP_ARQUIVO_INVALIDO);
    confereTeste("Lista nao mudou depois do arquivo invalido", listaHospedagens(HP_ATIVA), HP_OK);

    remove("hospedagem.bin"); //apaga o arquivo criado pelo teste

    //RESULTADO:

    return resultadoTestes("HOSPEDAGEM");
}