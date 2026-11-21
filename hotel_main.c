//Trabalho Hotel - Projeto Programação Modular

/*
Fabiano Carvalho Costa Júnior -- 2520144
João Vitor Lopes da Costa -- 2520345 
Rafaela Gomes de Souza Sant’Anna -- 2310112
Vinicius Silberman Pereira -- 2510569
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

//#include "hospedagem.h"
#include "hospede.h"
#include "quarto.h"


void limparEntrada(void){
    int caractere;
    while((caractere = getchar()) != '\n' && caractere != EOF);
}

int main(void){

    int check = 0;
    int numQuarto = 0;
    int tipoQuarto = 0;
    float valorQuarto = 0;
    char resposta;
    int modo;

    //inicialização:
    printf("\n---INICIALIZANDO SISTEMA---\n\n");

    printf("\nQUARTOS:");
    check = loadQuarto();
    check = loadHospede(); 

    printf("\nBem vindo ao sistema de gerenciamento de hoteis.\nSelecione qual acaoo gostaria de realizar:\n\n"
        "\tExemplo: ACAO --- BOTAO\n\n"
        "\tCLIENTES / HOSPEDES\n\n\t\tCadastrar novo hospede --- a\n\t\tAlterar dados de hospede --- b\n"
        "\t\tConsultar dados de hospede --- c\n\t\tListar todos os hospedes --- d\n\n"
        "\tQUARTOS\n\n\t\tCadastrar novo quarto --- e\n\t\tConsultar dados de quarto --- f\n\t\tListar quartos --- g\n\n"
        "\tHOSPEDAGENS / RESERVAS\n\n\t\tRealizar check-in --- h\n\t\tRealizar check-out --- i\n\t\tListar hospedagens --- j\n\n"
        "\tSAIR DO PROGRAMA --- x\n");

    char opcao = ' ';

    do {
        printf("\nDigite a opcao desejada: ");
        scanf(" %c", &opcao);
        limparEntrada();

        switch(opcao){

            // case 'a':
            //     printf("\nCadastrar novo hospede\n");
            //     break;

            // case 'b':
            //     printf("\nAlterar dados de hospede\n");
            //     break;

            // case 'c':
            //     printf("\nConsultar dados de hospede\n");
            //     break;

            // case 'd':
            //     printf("\nListar todos os hospedes\n");
            //     break;

            case 'e':

                do{
                    printf("\nDigite o numero do quarto: ");
                    scanf(" %d", &numQuarto);
                    limparEntrada();
                    printf("Digite o tipo do quarto (0= suite, 1= solteiro, 2= casal): ");
                    scanf(" %d", &tipoQuarto);
                    limparEntrada();
                    printf("Digite o valor da diaria do quarto: ");
                    scanf(" %f", &valorQuarto);
                    limparEntrada();
                    check = cadastraQuarto(numQuarto,tipoQuarto,valorQuarto);

                    printf("\nGostaria de cadastrar outro quarto? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando cadastro de quartos.\n");
                        check = 0;
                    }

                } while(check!=0);

                break;

            case 'f':

                do{
                    printf("\nDigite o numero do quarto: ");
                    scanf(" %d", &numQuarto);
                    limparEntrada();
                    check = consultaQuarto(numQuarto);

                    printf("\nGostaria de consultar outro quarto? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando consulta de quartos.\n");
                        check = 0;
                    }

                } while(check!=0);

                break;

            case 'g':

                do{
                    printf("\nListar quartos livres (0) ou todos (1)? ");
                    scanf(" %d", &modo);
                    limparEntrada();
                    check = listaQuartos(modo);

                    printf("\nGostaria de listar novamente? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando listagem de quartos.\n");
                        check = 0;
                    }

                } while(check!=0);

                break;

            // case 'h':
            //     printf("\nRealizar check-in\n");
            //     break;

            // case 'i':
            //     printf("\nRealizar check-out\n");
            //     break;

            // case 'j':
            //     printf("\nListar hospedagens\n");
            //     break;

            case 'x':
                printf("\nSaindo do programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }
        
    } while(opcao != 'x');

    //finalização:

    printf("\n---FINALIZANDO SISTEMA---\n");

    check = fechaQuarto();
    check = fechaHospede(); 
    return 0;
}
