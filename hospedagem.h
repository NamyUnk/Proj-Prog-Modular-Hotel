#ifndef HOSPEDAGEM_H
#define HOSPEDAGEM_H

#define LIM_HOSPEDAGENS 500

#include "hospede.h"
#include "quarto.h"

typedef struct horario{int dia;int mes;int ano;int hora;int minuto;} horario_S;
// dicionarios de enum assustadores a seguir
typedef enum {
    HP_ATIVA = 0,
    HP_ENCERRADA = 1
} HP_STATUS;
typedef enum {
    HP_OK = 0,                     /* operacao concluida */
    HP_HOSPEDE_INEXISTENTE,        /* codigo de hospede nao cadastrado */
    HP_HOSPEDE_JA_HOSPEDADO,       /* hospede ja tem hospedagem ativa (RN02) */
    HP_QUARTO_INEXISTENTE,         /* numero de quarto nao cadastrado */
    HP_QUARTO_OCUPADO,             /* quarto ja tem hospedagem ativa (RN01) */
    HP_DIARIAS_INVALIDAS,          /* diarias previstas <= 0 */
    HP_DATA_INVALIDA,              /* data ou horario fora do formato */
    HP_SAIDA_ANTES_DA_ENTRADA,     /* check-out anterior ao check-in */
    HP_HOSPEDAGEM_NAO_ENCONTRADA,  /* nao ha hospedagem ativa para o par hospede/quarto */
    HP_STATUS_INVALIDO,            /* filtro diferente de HP_ATIVA e HP_ENCERRADA */
    HP_LISTA_VAZIA,                /* nenhuma hospedagem na situacao pedida */
    HP_LIMITE_ATINGIDO,            /* LIM_HOSPEDAGENS alcancado */
    HP_ERRO_MEMORIA,               /* falha de alocacao */
    HP_ERRO_QUARTO,                /* modulo Quarto recusou a operacao */
    HP_ERRO_ARQUIVO,               /* nao foi possivel abrir/gravar o arquivo */
    HP_ARQUIVO_INVALIDO            /* arquivo com linha incompleta ou dados invalidos */
} HP_RET;


int checkIn(int hospede, int quarto, horario_S hora, int diarias);
/***************************************************************************
checkOut
Encerra a hospedagem ativa do hospede no quarto informado, calcula as
diarias efetivas (min 1) e o valor total, e libera o quarto (RF15 a RF19, RN03, RN04).
valorTotal: parametro de saida; recebe o valor da estadia quando o
retorno for HP_OK. Pode ser NULL se o valor nao interessar.
Retornos: HP_OK, HP_HOSPEDE_INEXISTENTE, HP_QUARTO_INEXISTENTE,
HP_HOSPEDAGEM_NAO_ENCONTRADA, HP_DATA_INVALIDA,
HP_SAIDA_ANTES_DA_ENTRADA, HP_ERRO_QUARTO
 ***************************************************************************/
int checkOut(int hospede, int quarto, horario_S hora);
/***************************************************************************
listaHospedagens
Exibe na tela as hospedagens na situacao informada (RF20, RF21).
status: HP_ATIVA ou HP_ENCERRADA (qualquer outro valor e recusado)
Retornos: HP_OK, HP_STATUS_INVALIDO, HP_LISTA_VAZIA
 ***************************************************************************/
HP_RET listaHospedagens(HP_STATUS status);


/***************************************************************************
 salvaHospedagens
 Grava todas as hospedagens (ativas e encerradas) no arquivo de uma por linha.
 Retornos: HP_OK, HP_ERRO_ARQUIVO
 ***************************************************************************/
 HP_RET salvaHospedagens(const char* nomeArquivo);
/***************************************************************************
carregaHospedagens
Le o arquivo gravado por salvaHospedagens e substitui a lista atual
pelo conteudo dele. 
Retornos: HP_OK, HP_ERRO_ARQUIVO (ex.: arquivo ainda nao existe),
            HP_ARQUIVO_INVALIDO, HP_ERRO_MEMORIA
 ***************************************************************************/
HP_RET carregaHospedagens(const char* nomeArquivo);


#endif