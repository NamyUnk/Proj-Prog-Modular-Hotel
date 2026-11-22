#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospedagem.h"
#include "hospede.h"
#include "quarto.h"

typedef struct hospedagem{

    int codigo;
    HP_STATUS status;
    int hospede; 
    int quarto;
    horario_S entrada;
    horario_S saida;
    int diarias_reservadas;
    int diarias_efetivas;
    float valorTotal;

} hospedagem_S;

typedef struct cadeiaHospedagem{
    hospedagem_S* hospedagem;
    struct cadeiaHospedagem* prox;
} cadeiaHospedagem_S;
//amem ao static
static cadeiaHospedagem_S* cabeca = NULL;
static int totalHospedagens = 0;


static void imprimeHorario(horario_S h)
{
    //n ia fazer uma só pra isso mas usei tanto que achei melhor
    printf("%02d/%02d/%04d %02d:%02d", h.dia, h.mes, h.ano, h.hora, h.minuto);
}
static void imprimeHospedagem(const hospedagem_S* h){
   char nome[100];
 
    printf("Hospedagem %d | Hospede: ", h->codigo);
    if (obtemNomeHospede(h->hospede, nome, sizeof(nome)) == 0)
        printf("%s", nome);
    else
        printf("(codigo %d)", h->hospede);
 
    printf(" | Quarto: %d | Entrada: ", h->quarto);
    imprimeHorario(h->entrada);
 
    if (h->status == HP_ENCERRADA) {
        printf(" | Saida: ");
        imprimeHorario(h->saida);
        printf(" | Diarias: %d | Total: R$ %.2f", h->diarias_efetivas, h->valorTotal);} 
    else {printf(" | Diarias previstas: %d", h->diarias_reservadas);}
 
    printf("\n");
}
static int ehBissexto(int ano){
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}
static int horarioValido(horario_S h){
    //fevereiro so tem 29 dias em ano bissexto blz?
    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (h.mes == 2 && ehBissexto(h.ano))
        diasNoMes[1] = 29;
    if (h.ano < 1900 || h.mes < 1 || h.mes > 12||h.dia < 1 || h.dia > diasNoMes[h.mes - 1])
        return 0;
    if(h.hora >= 0 && h.hora < 24 && h.minuto >= 0 && h.minuto < 60)
    return 1;
    return 0;
}
/* Converte a data no numero de dias desde um ponto fixo do calendario
 * (ignora hora e minuto). A diferenca entre duas conversoes e a quantidade
 * de dias entre as datas. O "ano" comeca em marco, entao fevereiro vira o
 * ultimo mes e o dia extra do bissexto cai no fim. */
static long diaAbsoluto(horario_S h){
    long a = h.ano;
    long m = h.mes;
    if (m <= 2) {
        a--;
        m += 12;
    }
    return 365 * a + a / 4 - a / 100 + a / 400 + (153 * (m - 3) + 2) / 5 + h.dia;
}
/* Mesma ideia, em minutos: permite comparar entrada e saida com a hora */
static long minutoAbsoluto(horario_S h){
    return diaAbsoluto(h) * 1440L + h.hora * 60L + h.minuto;
}
/* Devolve a hospedagem ATIVA do hospede, ou NULL se nao houver */
static hospedagem_S* buscaAtivaPorHospede(int codHospede){
    cadeiaHospedagem_S* p;
    for (p = cabeca; p != NULL; p = p->prox)
        if (p->hospedagem->status == HP_ATIVA && p->hospedagem->hospede == codHospede)
            return p->hospedagem;
    return NULL;
}




 
HP_RET checkIn(int nHospede, int nQuarto, horario_S entrada, int diarias){
    //funçao da checagem desesperada o incio
    hospedagem_S* h;
    cadeiaHospedagem_S* no;
    float valorDiaria = 0.0f;
    //ve se o hospede tá de boa
    if(verificaHospede(nHospede) != 0)
        return HP_HOSPEDE_INEXISTENTE;
    if (buscaAtivaPorHospede(nHospede) != NULL)
        return HP_HOSPEDE_JA_HOSPEDADO; 

    int disponibilidade= verificaDisponibilidade(nQuarto);
    //vê se o quarto ta de boa
    if (disponibilidade == 1)
        return HP_QUARTO_INEXISTENTE;
    if (disponibilidade != 0)
        return HP_QUARTO_OCUPADO;    
    if (obtemValorDiaria(nQuarto, &valorDiaria) != 0)
        return HP_ERRO_QUARTO;
    //vê se as diarias tao de boa
    if (diarias <= 0)
        return HP_DIARIAS_INVALIDAS;
    //vê se o horario ta boa (meu deus)
    if (!horarioValido(entrada))
        return HP_DATA_INVALIDA;
    //ve se tem espaço no hotel
    if (totalHospedagens >= LIM_HOSPEDAGENS)
        return HP_LIMITE_ATINGIDO;

    h  = malloc(sizeof(hospedagem_S));
    no = malloc(sizeof(cadeiaHospedagem_S));
    if (h == NULL || no == NULL) {
        free(h);                                       
        free(no);
        return HP_ERRO_MEMORIA;
    }
 
    if (ocupaQuarto(nQuarto) != 0) {
        free(h);
        free(no);
        return HP_ERRO_QUARTO;
    }
    //preenchendo de fato
    h->codigo             = totalHospedagens+1;
    h->status             = HP_ATIVA;
    h->hospede            = nHospede;
    h->quarto             = nQuarto;
    h->entrada            = entrada;                
    h->diarias_reservadas = diarias;
    h->diarias_efetivas   = 0;
    h->valorTotal         = diarias*valorDiaria;
 
    //colocando na lista
    no->hospedagem = h;
    no->prox = cabeca;
    cabeca = no;
    totalHospedagens++;
 
    return HP_OK;
}

HP_RET checkOut(int hospede, int quarto, horario_S hora, float* valorTotal){
    hospedagem_S* h;
    float valorDiaria;
    long diarias;
    //essa segue o mesmo principio da anterior
    if (verificaHospede(hospede) != 0)
        return HP_HOSPEDE_INEXISTENTE;
    if (verificaDisponibilidade(quarto) == 1)
        return HP_QUARTO_INEXISTENTE;

    h = buscaAtivaPorHospede(hospede);
    if (h == NULL || h->quarto != quarto)
        return HP_HOSPEDAGEM_NAO_ENCONTRADA;
    if (!horarioValido(hora))
        return HP_DATA_INVALIDA;
    if (minutoAbsoluto(hora) < minutoAbsoluto(h->entrada))
        return HP_SAIDA_ANTES_DA_ENTRADA;

    if (obtemValorDiaria(quarto, &valorDiaria) != 0)
        return HP_ERRO_QUARTO;

    
    diarias = diaAbsoluto(hora) - diaAbsoluto(h->entrada);
    if (diarias < 1)
        diarias = 1;     // minimo de 1 diaria 

    // Libera o quarto antes de encerrar 
    if (liberaQuarto(quarto) != 0)
        return HP_ERRO_QUARTO;

    
    h->saida            = hora;
    h->diarias_efetivas = (int) diarias;
    h->valorTotal       = (float) diarias * valorDiaria;
    h->status           = HP_ENCERRADA;
    if (valorTotal != NULL)
        *valorTotal = h->valorTotal;
    return HP_OK;
}

HP_RET listaHospedagens(HP_STATUS status){
    if (status != HP_ATIVA && status != HP_ENCERRADA)
    return HP_STATUS_INVALIDO;

    cadeiaHospedagem_S* p;
    int encontradas = 0;
 
    for(p = cabeca; p != NULL; p = p->prox) {
        if(p->hospedagem->status == status) {
            imprimeHospedagem(p->hospedagem);
            encontradas++;
        }
    }
    if(encontradas == 0)
    return HP_LISTA_VAZIA;
    if(status==HP_ATIVA)
    printf("Foram encontradas %d hospedagens ativas.\n", encontradas);
    if(status==HP_ENCERRADA)
    printf("Foram encontradas %d hospedagens encerradas.\n", encontradas);

     return HP_OK;
}

HP_RET salvaHospedagens(const char* nomeArquivo){
    FILE* arq;
    cadeiaHospedagem_S* p;
    hospedagem_S* h;

    arq = fopen(nomeArquivo, "w");
    if (arq == NULL)return HP_ERRO_ARQUIVO;
    for (p = cabeca; p != NULL; p = p->prox) {
        h = p->hospedagem;
        fprintf(arq, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %.2f\n",
                h->codigo, (int) h->status, h->hospede, h->quarto,
                h->entrada.dia, h->entrada.mes, h->entrada.ano, h->entrada.hora, h->entrada.minuto,
                h->saida.dia, h->saida.mes, h->saida.ano, h->saida.hora, h->saida.minuto,
                h->diarias_reservadas, h->diarias_efetivas, h->valorTotal);
    }

    if (fclose(arq) != 0)
        return HP_ERRO_ARQUIVO;
    return HP_OK;
}

HP_RET carregaHospedagens(const char* nomeArquivo){
    FILE* arq;
    cadeiaHospedagem_S* novaCabeca = NULL;   
    cadeiaHospedagem_S* fim = NULL;          
    cadeiaHospedagem_S* no;
    hospedagem_S lida;
    hospedagem_S* h;
    int status, lidos, total = 0;

    arq = fopen(nomeArquivo, "r");
    if (arq == NULL)return HP_ERRO_ARQUIVO;

    /* fscanf devolve qts leu: 17 = linha completa */
    while ((lidos = fscanf(arq, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f",
                           &lida.codigo, &status, &lida.hospede, &lida.quarto,
                           &lida.entrada.dia, &lida.entrada.mes, &lida.entrada.ano,
                           &lida.entrada.hora, &lida.entrada.minuto,
                           &lida.saida.dia, &lida.saida.mes, &lida.saida.ano,
                           &lida.saida.hora, &lida.saida.minuto,
                           &lida.diarias_reservadas, &lida.diarias_efetivas,
                           &lida.valorTotal)) == 17) {

        /* confere se a linha faz sentido antes de aceitar */
        if ((status != HP_ATIVA && status != HP_ENCERRADA)
            || !horarioValido(lida.entrada)
            || (status == HP_ENCERRADA && !horarioValido(lida.saida))
            || total >= LIM_HOSPEDAGENS) {
            lidos = 0;                       /* marca como invalido e para */
            break;
        }
        lida.status = (HP_STATUS) status;

        h  = malloc(sizeof(hospedagem_S));
        no = malloc(sizeof(cadeiaHospedagem_S));
        if (h == NULL || no == NULL) {
            free(h);
            free(no);
            liberaLista(novaCabeca);
            fclose(arq);
            return HP_ERRO_MEMORIA;
        }

        *h = lida;                           /* copia a struct inteira */
        no->hospedagem = h;
        no->prox = NULL;
        if (fim == NULL)
            novaCabeca = no;
        else
            fim->prox = no;
        fim = no;
        total++;
    }

    fclose(arq);

    /* EOF = leu ate o fim sem problemas; qualquer outro valor = parou numa linha ruim */
    if (lidos != EOF) {
        liberaLista(novaCabeca);             /* descarta o que foi lido; a lista atual fica intacta */
        return HP_ARQUIVO_INVALIDO;
    }

    /* deu tudo certo: troca a lista antiga pela nova */
    liberaLista(cabeca);
    cabeca = novaCabeca;
    totalHospedagens = total;
    return HP_OK;
}