#include <string.h>
#include "hospedagem.h"
#include "hospede.h"
#include "quarto.h"

typedef struct hospedagem{

    int codigo;
    HP_STATUS status;
    int hospede; 
    int quarto;
    data_S dataEntrada;
    horario_S horaEntrada;
    data_S dataSaida;
    Hhrario_S horaSaida;
    int diarias_reservadas;
    int diarias_efetivas;
    float valorTotal;

} hospedagem_S;

typedef struct cadeiaHospedagem{
    Hospedagem* hospedagem;
    struct cadeiaHospedagem* prox;
} cadeiaHospedagem_S;
//amem ao static
static cadeiaHospedagem* cabeca = NULL;
static int totalHospedagens = 0;


static void imprimeHorario(horario_S h)
{
    //n ia fazer uma só pra isso mas usei tanto que achei melhor
    printf("%02d/%02d/%04d %02d:%02d", h.dia, h.mes, h.ano, h.hora, h.minuto);
}
static void imprimeHospedagem(const hospedagem* h){
   char nome[100];
 
    printf("Hospedagem %d | Hospede: ", h->codigo);
    if (obtemNomeHospede(h->hospede, nome, sizeof(nome)) == HOS_OK)
        printf("%s", nome);
    else
        printf("(codigo %d)", h->hospede);
 
    printf(" | Quarto: %d | Entrada: ", h->quarto);
    imprimeHorario(h->entrada);
 
    if (h->status == HP_ENCERRADA) {
        printf(" | Saida: ");
        imprimeHorario(h->saida);
        printf(" | Diarias: %d | Total: R$ %.2f", h->diariasEfetivas, h->valorTotal);} 
    else {printf(" | Diarias previstas: %d", h->diariasReservadas);}
 
    printf("\n");
}
static int horarioValido(horario_S h){
    //fevereiro NAO PODE ter 28 dias blz?
    int diasNoMes[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (h.mes < 1 || h.mes > 12||h.dia < 1 || h.dia > diasNoMes[h.mes - 1])
        return 0;
    if(h.hora >= 0 && h.hora < 24 && h.minuto >= 0 && h.minuto < 60)
    return 1;
}






 
HP_RET filtraHospedagens(HP_STATUS status){
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
    printf("Foram encrontrados %d quartos ativos.");
    if(status==HP_ENCERRADA)
    printf("Foram encrontrados %d quartos encerrados.");

     return HP_OK;
}
HP_RET checkIn(int nHospede, int nQuarto, horario_S entrada, int diarias){
    //funçao da checagem desesperada o incio
    hospedagem_S* h;
    cadeiaHospedagem_S* no;
    //ve se o hospede tá de boa
    if(verificaHospede(nHospede) != H_OK)
        return HP_HOSPEDE_INEXISTENTE;
    if (buscaAtivaPorHospede(codHospede) != NULL)
        return HP_HOSPEDE_JA_HOSPEDADO; 

    QUA_RET disponibilidade= statusQuarto(nQuarto);
    //vê se o quarto ta de boa
    if (disponibilidade == QUA_INEXISTENTE)
        return HP_QUARTO_INEXISTENTE;
    if (disponibilidade != QUA_OK)
        return HP_QUARTO_OCUPADO;    
    //vê se as diarias tao de boa
    if (diarias <= 0)
        return HP_DIARIAS_INVALIDAS;
    //vê se o horario ta boa (meu deus)
    if (!horarioValido(entrada))
        return HP_DATA_INVALIDA;
    //ve se tem espaço no hotel
    if (totalHospedagens >= LIM_HOSPEDAGENS)
        return HP_LIMITE_ATINGIDO;

    h  = malloc(sizeof(hospedagem));
    no = malloc(sizeof(cadeiaHospedagem));
    if (h == NULL || no == NULL) {
        free(h);                                       
        free(no);
        return HP_ERRO_MEMORIA;
    }
 
    if (ocupaQuarto(numQuarto) != QUA_OK) {
        free(h);
        free(no);
        return HP_ERRO_QUARTO;
    }
    int valorDiaria=0;
    diariaQuarto(numQuarto, &valorDiaria);
    //preenchendo de fato
    h->codigo            = totalHospedagens+1;
    h->status            = HP_ATIVA;
    h->hospede           = codHospede;
    h->quarto            = numQuarto;
    h->entrada           = entrada;                
    h->diariasReservadas = diarias;
    h->diariasEfetivas   = 0;
    h->valorTotal        = diarias*valorDiaria;
 
    //colocando na lista
    no->hospedagem = h;
    no->prox = cabeca;
    cabeca = no;
    totalHospedagens++;
 
    return HP_OK
}

int checkOut(int hospede, int quarto, Horario hora);
