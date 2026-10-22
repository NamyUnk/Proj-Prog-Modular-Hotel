#include <stdio.h>
#include <stdlib.h>

#include "quarto.h"

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

cadeiaQuarto* quartos = NULL;

int loadQuarto() {

    /* retornos:
    0 = sucesso
    1 = arquivo nulo
    2 = erro alocação de memória
    3 = erro de leitura
    */

    FILE* arquivo = fopen("quarto.bin", "rb");

    if (!arquivo) {

        printf("\nErro ao abrir o arquivo para leitura!\n");

        return 1;
    }

    quarto q;
    cadeiaQuarto* novo;
    cadeiaQuarto* atual = NULL;

    while (fread(&q, sizeof(quarto), 1, arquivo) == 1) {

        novo = malloc(sizeof(cadeiaQuarto));

        if (!novo) {

            printf("\nErro ao alocar memória para o nó!\n");
            fclose(arquivo);

            return 2;
        }

        novo->quarto = malloc(sizeof(quarto));

        if (!novo->quarto) {

            free(novo);
            printf("\nErro ao alocar memória para o quarto!\n");
            fclose(arquivo);

            return 2;
        }

        *novo->quarto = q;
        novo->prox = NULL;

        if (quartos == NULL) quartos = novo;
        else atual->prox = novo;
    
        atual = novo;
    }

    if (ferror(arquivo)) {
        printf("\nErro ao ler o arquivo!\n");
        fclose(arquivo);
        return 3;
    }

    fclose(arquivo);

    printf("\nQuartos carregados com sucesso!\n");

    return 0;
}

int fechaQuarto() {

    /* retornos:
    0 = sucesso
    1 = arquivo não abriu
    2 = erro ao salvar
    */

    FILE* arquivo = fopen("quarto.bin", "wb");

    if (!arquivo) {

        printf("\nErro ao abrir o arquivo para escrita!\n");

        return 1;
    }

    cadeiaQuarto* atual = quartos;

    while (atual != NULL) {

        if (fwrite(atual->quarto, sizeof(quarto), 1, arquivo) != 1) {

            printf("\nErro ao salvar os quartos no arquivo!\n");
            fclose(arquivo);

            return 2;
        }

        atual = atual->prox;
    }

    fclose(arquivo);

    printf("\nQuartos salvos com sucesso!\n");

    return 0;
}

int buscaQuarto(int num,int* status,float* diaria,cadeiaQuarto** quarto){ //func. auxiliar

    /* retornos:
    0 = sucesso
    1 = lista inexistente
    2 = quarto inexistente
    */

    cadeiaQuarto* pAux = quartos;

    if(!pAux){
        printf("\nNão foi possível encontrar a lista de quartos registrados.\n");
        return 1;
    }

    while(pAux){

        if(pAux->quarto && pAux->quarto->num == num){

            if(status) *status = pAux->quarto->status;
            if(diaria) *diaria = pAux->quarto->valor;
            if(quarto) *quarto = pAux;

            return 0;
        }

        pAux = pAux->prox;
    }

    printf("\nNão foi possível encontrar o quarto.\n");
    return 2;
}

int statusQuarto(int num){

    /* retornos:
    0 = livre
    1 = ocupado
    2 = erro nas infos do quarto
    3 = quarto inexistente
    */

    int status;

    int verifica = buscaQuarto(num,&status,NULL,NULL);
    if(verifica!=0) return 3; //mensagem de erro já printada no buscar

    if (status!=0 && status!=1){

        printf("\nStatus do quarto encontrado inválido!\n");
        return 2;
    }

    return status;
}

int diariaQuarto(int num,float* diaria){

    /* retornos:
    0 = sucesso
    1 = erro parâmetro
    2 = quarto inexistente
    3 = erro ao resgatar a diária
    */

    if(!diaria){

        printf("\nEndereço para registrar a diária não encontrado!\n");
        return 1;
    }

    int verifica = buscaQuarto(num,NULL,diaria,NULL);
    if(verifica!=0) return 2; //mensagem de erro já printada no buscar

    if(*diaria == 0){

        printf("\nErro ao resgatar o valor da diária no sitema!\n");
        return 3;
    }

    return 0;
}

int consultaQuarto(int num){

    /* retornos:
    0 = sucesso
    1 = quarto inexistente
    2 = erro nas infos do quarto
    */

    cadeiaQuarto* pAux = NULL;
    int verifica = buscaQuarto(num, NULL, NULL, &pAux);
    if(verifica!=0) return 1;

    int status = statusQuarto(num);

    char *tipo;
    char *status_s;

    switch (pAux->quarto->tipo){

        case 0: 
        tipo = "Suíte";
        break;

        case 1:
        tipo = "Solteiro";
        break;

        case 2:
        tipo = "Casal";
        break;

        default:
        printf("\nTipo de quarto encontrado inválido!\n");
        return 2;
    }

    switch(status){

        case 0: 
        status_s = "Livre";
        break;

        case 1:
        status_s = "Ocupado";
        break;

        default:
        return 2;
    }

    printf("\nNúmero: %d\tTipo: %s\tValor: R$ %.2f\tStatus: %s\n",num,tipo,pAux->quarto->valor,status_s);

    return 0;
}

int cadastraQuarto(int num, int tipo, float valor){

    /* retornos:
    0 = sucesso
    1 = valores invalidos
    2 = quarto já registrado
    3 = erro de memória
    4 = erro percorrendo lista
    */

    cadeiaQuarto* pAux = quartos;

    if(num<=0 || valor<= 0){

        printf("\nO número e valor da diária do quarto precisam ser positivos.\n");
        return 1; 
    }

    if(tipo!=0 && tipo!=1 && tipo!=2){

        printf("\nTipo de quarto inválido!\n");
        return 1;
    }

    int confereRepetido = buscaQuarto(num,NULL,NULL,NULL);

    if(confereRepetido==0){

        printf("\nO número do quarto já está registrado no sistema.\n");
        return 2;
    }
    else printf("Ou seja, o número está disponível para a criação do novo quarto!\n");

    cadeiaQuarto* quartonovo = malloc(sizeof(cadeiaQuarto));

    if (!quartonovo) {
        printf("\nErro ao alocar memória para o nó.\n");
        return 3;
    }

    quartonovo->quarto = malloc(sizeof(quarto));

    if (!quartonovo->quarto) {

        free(quartonovo);
        printf("\nErro ao alocar memória para o quarto.\n");
        return 3;
    }

    quartonovo->quarto->num = num;
    quartonovo->quarto->tipo = tipo;
    quartonovo->quarto->valor = valor;
    quartonovo->quarto->status = 0;
    quartonovo->prox = NULL;

    if(pAux){

        while(pAux){

            if(pAux->quarto && pAux->quarto->num < num){ //se o atual é menor

                if(pAux->prox){

                    if (pAux->prox->quarto->num > num){ //se o atual é menor e o próximo é maior

                        quartonovo->prox = pAux->prox;
                        pAux->prox = quartonovo;
                        printf("\nQuarto registrado com sucesso no meio da lista!\n");
                        return 0;
                    }
                    pAux = pAux->prox; //se o atual é menor e o próximo também é menor
                }
                else{ //se nao existe proximo

                    pAux->prox = quartonovo;
                    printf("\nQuarto registrado com sucesso no fim da lista!\n");
                    return 0;
                }
            }
            else{ //se o atual é maior

                quartonovo->prox = pAux;
                printf("\nQuarto registrado com sucesso no começo da lista!\n");
                quartos = quartonovo;
                return 0;
            }
        }
        printf("\nErro ao percorrer a lista de quartos para registrar o quarto novo!\n");
        free(quartonovo->quarto);
        free(quartonovo);
        return 4;
    }
  
    printf("\nQuarto registrado com sucesso como nova lista!\tret: 0\n");
    quartos = quartonovo;
    return 0;
}

int listaQuartos(int modo){

    /* retornos:
    0 = sucesso
    1 = lista não encontrada
    2 = modo invalido
    3 = valor invalido
    4 = erro percorrendo lista
    */

    cadeiaQuarto* pAux = quartos;

    if(!pAux){
        printf("\nNão foi possível encontrar a lista de quartos registrados.\n");
        return 1;
    }

    if(modo != 1 && modo != 0){

        printf("\nModo de listagem inválido! Por favor, solicitar a listagem novamente.\n");
        return 2;
    }

    if(modo == 0){

        printf("\nLista de quartos disponíveis:\n\n");

        while(pAux){

            if(pAux->quarto->status == 0){

                char *tipo;
                char *status;

                switch (pAux->quarto->tipo){

                    case 0: 
                    tipo = "Suíte";
                    break;

                    case 1:
                    tipo = "Solteiro";
                    break;

                    case 2:
                    tipo = "Casal";
                    break;

                    default:
                    printf("\nTipo de quarto encontrado inválido!\n");
                    return 3;
                }

                printf("\nNúmero: %d\tTipo: %s\tValor: R$ %.2f\tStatus: Livre\n",pAux->quarto->num,tipo,pAux->quarto->valor);
                pAux = pAux->prox;
            }
            else pAux = pAux->prox;
        }
        return 0;
    }

    if(modo == 1){

        printf("\nLista de quartos:\n\n");

        while(pAux){

            char *tipo;
            char *status;

            switch (pAux->quarto->tipo){

                case 0: 
                tipo = "Suíte";
                break;

                case 1:
                tipo = "Solteiro";
                break;

                case 2:
                tipo = "Casal";
                break;

                default:
                printf("\nTipo de quarto encontrado inválido!\n");
                return 3;
                }

            switch(pAux->quarto->status){

                case 0: 
                status = "Livre";
                break;

                case 1:
                status = "Ocupado";
                break;

                default:
                printf("\nStatus do quarto encontrado inválido!\n");
                return 3;
                }

            printf("\nNúmero: %d\tTipo: %s\tValor: R$ %.2f\tStatus: %s\n",pAux->quarto->num,tipo,pAux->quarto->valor,status);
            pAux = pAux->prox;
        }
        return 0;
    }
}