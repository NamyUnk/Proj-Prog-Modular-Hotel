#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospedagem.h"
#include "hospede.h"
#include "quarto.h"
#define ARQ_HOSPEDAGENS "hospedagem.bin"

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
/* Libera todos os nos de uma lista e as hospedagens que eles guardam */
static void liberaLista(cadeiaHospedagem_S* lista){
    cadeiaHospedagem_S* prox;
    while (lista != NULL) {
        prox = lista->prox;
        free(lista->hospedagem);
        free(lista);
        lista = prox;
    }
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

    int disponibilidade= statusQuarto(nQuarto);
    //vê se o quarto ta de boa
    if (disponibilidade == 3)
        return HP_QUARTO_INEXISTENTE;
    if (disponibilidade != 0)
        return HP_QUARTO_OCUPADO;    
    if (diariaQuarto(nQuarto, &valorDiaria) != 0)
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

    h  = calloc(1, sizeof(hospedagem_S));
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
    if (statusQuarto(quarto) == 3)
        return HP_QUARTO_INEXISTENTE;

    h = buscaAtivaPorHospede(hospede);
    if (h == NULL || h->quarto != quarto)
        return HP_HOSPEDAGEM_NAO_ENCONTRADA;
    if (!horarioValido(hora))
        return HP_DATA_INVALIDA;
    if (minutoAbsoluto(hora) < minutoAbsoluto(h->entrada))
        return HP_SAIDA_ANTES_DA_ENTRADA;

    if (diariaQuarto(quarto, &valorDiaria) != 0)
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
#define ARQ_HOSPEDAGENS "hospedagem.bin"
#define N_CAMPOS_INT 16

/* Copia os campos inteiros da hospedagem para um vetor, na ordem do arquivo */
static void empacota(const hospedagem_S* h, int campos[]){
    campos[0]  = h->codigo;
    campos[1]  = (int) h->status;
    campos[2]  = h->hospede;
    campos[3]  = h->quarto;
    campos[4]  = h->entrada.dia;
    campos[5]  = h->entrada.mes;
    campos[6]  = h->entrada.ano;
    campos[7]  = h->entrada.hora;
    campos[8]  = h->entrada.minuto;
    campos[9]  = h->saida.dia;
    campos[10] = h->saida.mes;
    campos[11] = h->saida.ano;
    campos[12] = h->saida.hora;
    campos[13] = h->saida.minuto;
    campos[14] = h->diarias_reservadas;
    campos[15] = h->diarias_efetivas;
}

/* Caminho inverso: monta a hospedagem a partir do vetor lido do arquivo */
static void desempacota(const int campos[], hospedagem_S* h){
    h->codigo             = campos[0];
    h->status             = (HP_STATUS) campos[1];
    h->hospede            = campos[2];
    h->quarto             = campos[3];
    h->entrada.dia        = campos[4];
    h->entrada.mes        = campos[5];
    h->entrada.ano        = campos[6];
    h->entrada.hora       = campos[7];
    h->entrada.minuto     = campos[8];
    h->saida.dia          = campos[9];
    h->saida.mes          = campos[10];
    h->saida.ano          = campos[11];
    h->saida.hora         = campos[12];
    h->saida.minuto       = campos[13];
    h->diarias_reservadas = campos[14];
    h->diarias_efetivas   = campos[15];
}


HP_RET fechaHospedagem(void){
  
    FILE* arq;
    cadeiaHospedagem_S* p;
    int campos[N_CAMPOS_INT];

    /* "wb": o b (binario) e essencial no Windows, que em modo texto troca
     * todo byte 10 (\n) por 13 10 (\r\n) e estragaria os numeros gravados */
    arq = fopen(ARQ_HOSPEDAGENS, "wb");
    if (arq == NULL)return HP_ERRO_ARQUIVO;

    /* primeiro a quantidade, para a leitura saber quantas hospedagens esperar */
    if (fwrite(&totalHospedagens, sizeof(int), 1, arq) != 1) {
        fclose(arq);
        return HP_ERRO_ARQUIVO;
    }

    for (p = cabeca; p != NULL; p = p->prox) {
        empacota(p->hospedagem, campos);
        if (fwrite(campos, sizeof(int), N_CAMPOS_INT, arq) != N_CAMPOS_INT
            || fwrite(&p->hospedagem->valorTotal, sizeof(float), 1, arq) != 1) {
            fclose(arq);
            return HP_ERRO_ARQUIVO;
        }
    }

    if (fclose(arq) != 0)
        return HP_ERRO_ARQUIVO;
    return HP_OK;
}


HP_RET loadHospedagem(void){

    FILE* arq;
    cadeiaHospedagem_S* novaCabeca = NULL;   
    cadeiaHospedagem_S* fim = NULL;          
    cadeiaHospedagem_S* no;
    hospedagem_S lida;
    hospedagem_S* h;
    int campos[N_CAMPOS_INT];
    int quantidade, i;

    arq = fopen(ARQ_HOSPEDAGENS, "rb");
    if (arq == NULL)return HP_ERRO_ARQUIVO;

    /* fread devolve quantos itens leu: se for menos que o pedido, o arquivo acabou antes */
    if (fread(&quantidade, sizeof(int), 1, arq) != 1
        || quantidade < 0 || quantidade > LIM_HOSPEDAGENS) {
        fclose(arq);
        return HP_ARQUIVO_INVALIDO;
    }

    for (i = 0; i < quantidade; i++) {
        if (fread(campos, sizeof(int), N_CAMPOS_INT, arq) != N_CAMPOS_INT
            || fread(&lida.valorTotal, sizeof(float), 1, arq) != 1)
            break;                           /* arquivo cortado no meio */

        /* confere se a hospedagem faz sentido antes de aceitar */
        if (campos[1] != HP_ATIVA && campos[1] != HP_ENCERRADA)
            break;
        desempacota(campos, &lida);
        if (!horarioValido(lida.entrada)
            || (lida.status == HP_ENCERRADA && !horarioValido(lida.saida)))
            break;

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
    }

    /* tem que ter lido todas as hospedagens prometidas, e depois delas o arquivo tem que acabar */
    if (i < quantidade || fgetc(arq) != EOF) {
        fclose(arq);
        liberaLista(novaCabeca);             /* descarta o que foi lido; a lista atual fica intacta */
        return HP_ARQUIVO_INVALIDO;
    }
    fclose(arq);

    /* deu tudo certo: troca a lista antiga pela nova */
    liberaLista(cabeca);
    cabeca = novaCabeca;
    totalHospedagens = quantidade;
    return HP_OK;
}