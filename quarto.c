#include <stdio.h>
#include <stdlib.h>

#include "quarto.h"

cadeiaQuarto* buscaQuarto(int num, cadeiaQuarto* cabeca){

    if(!cabeca){
        printf("\nNão foi possível encontrar a lista de quartos registrados.\n");
        return NULL;
    }

    while(cabeca){

        if(cabeca->quarto && cabeca->quarto->num == num) return cabeca;

        cabeca = cabeca->prox;
    }

    printf("\nNão foi possível encontrar o quarto.\n");
    return NULL;
}

int consultaQuarto(int num, cadeiaQuarto* cabeca){

    /* retornos:
    0 = sucesso
    1 = quarto inexistente
    2 = erro nas infos do quarto
    */

    cabeca = buscaQuarto(num,cabeca);

    if(!cabeca) return 1;

    char *tipo;
    char *status;

    switch (cabeca->quarto->tipo){

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

    switch(cabeca->quarto->status){

        case 0: 
        status = "Livre";
        break;

        case 1:
        status = "Ocupado";
        break;

        default:
        printf("\nStatus do quarto encontrado inválido!\n");
        return 2;
    }

    printf("\nNúmero: %d\tTipo: %s\tValor: R$ %.2f\tStatus: %s\n",cabeca->quarto->num,tipo,cabeca->quarto->valor,status);

    return 0;
}

cadeiaQuarto* cadastraQuarto(int num, int tipo, float valor, cadeiaQuarto* cabeca){

    /* retornos:
    0 = sucesso
    1 = valores invalidos
    2 = quarto já registrado
    3 = erro de memória
    4 = erro percorrendo lista
    */

    if(num<=0 || valor<= 0){

        printf("\nO número do quarto precisa ser um inteiro positivo.\tret: 1\n");
        return cabeca; 
    }

    cadeiaQuarto* confereRepetido = buscaQuarto(num, cabeca);

    if(confereRepetido){

        printf("\nO número do quarto já está registrado no sistema.\tret: 2\n");
        return cabeca;
    }
    else printf("Ou seja, o número está disponível para a criação do novo quarto!\n");

    cadeiaQuarto* quartonovo = malloc(sizeof(cadeiaQuarto));

    if (!quartonovo) {
        printf("\nErro ao alocar memória para o nó.\tret: 3\n");
        return cabeca;
    }

    quartonovo->quarto = malloc(sizeof(quarto));

    if (!quartonovo->quarto) {
        free(quartonovo);
        printf("\nErro ao alocar memória para o quarto.\tret: 3\n");
        return cabeca;
    }

    quartonovo->quarto->num = num;
    quartonovo->quarto->tipo = tipo;
    quartonovo->quarto->valor = valor;
    quartonovo->quarto->status = 0;
    quartonovo->prox = NULL;

    if(cabeca){

        while(cabeca){

            if(cabeca->quarto && cabeca->quarto->num < num){ //se o atual é menor

                if(cabeca->prox){

                    if (cabeca->prox->quarto->num > num){ //se o atual é menor e o próximo é maior

                        quartonovo->prox = cabeca->prox;
                        cabeca->prox = quartonovo;
                        printf("\nQuarto registrado com sucesso no meio da lista!\tret: 0\n");
                        return cabeca;
                    }
                    cabeca = cabeca->prox; //se o atual é menor e o próximo também é menor
                }
                else{ //se nao existe proximo

                    cabeca->prox = quartonovo;
                    printf("\nQuarto registrado com sucesso no fim da lista!\tret: 0\n");
                    return cabeca;
                }
            }
            else{ //se o atual é maior

                quartonovo->prox = cabeca;
                cabeca = quartonovo;
                printf("\nQuarto registrado com sucesso no começo da lista!\tret: 0\n");
                return cabeca;
            }
        }
        printf("\nErro ao percorrer a lista de quartos para registrar o quarto novo!\tret: 4\n");
        return cabeca;
    }
    cabeca = quartonovo;
    printf("\nQuarto registrado com sucesso como nova lista!\tret: 0\n");
    return cabeca;
}

void listaQuartos(int modo, cadeiaQuarto* cabeca){

    if(!cabeca){

        printf("\nNão foi possível encontrar a lista de quartos registrados.\n");
        return;
    }

    if(modo != 1 && modo != 0){

        printf("\nModo de listagem inválido! Por favor, solicitar a listagem novamente.\n");
        return;
    }

    if(modo == 0){

        printf("\nLista de quartos disponíveis:\n\n");

        while(cabeca){

            if(cabeca->quarto->status == 0){

                char *tipo;
                char *status;

                switch (cabeca->quarto->tipo){

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
                    return;
                }

                printf("\nNúmero: %d\tTipo: %s\tValor: R$ %.2f\tStatus: Livre\n",cabeca->quarto->num,tipo,cabeca->quarto->valor);
                cabeca = cabeca->prox;
            }
            else cabeca = cabeca->prox;
        }
        return;
    }

    if(modo == 1){

        printf("\nLista de quartos:\n\n");

        while(cabeca){

            char *tipo;
            char *status;

            switch (cabeca->quarto->tipo){

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
                return;
                }

            switch(cabeca->quarto->status){

                case 0: 
                status = "Livre";
                break;

                case 1:
                status = "Ocupado";
                break;

                default:
                printf("\nStatus do quarto encontrado inválido!\n");
                return;
                }

            printf("\nNúmero: %d\tTipo: %s\tValor: R$ %.2f\tStatus: %s\n",cabeca->quarto->num,tipo,cabeca->quarto->valor,status);
            cabeca = cabeca->prox;
        }
        return;
    }
}