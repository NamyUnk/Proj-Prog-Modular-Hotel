//Trabalho Hotel - Projeto Programação Modular

/*
Fabiano Carvalho Costa Júnior -- 2520144
João Vitor Lopes da Costa -- 2520345 
Rafaela Gomes de Souza Sant’Anna -- 2310112
Vinicius Silberman Pereira -- 2510569
*/

/*
MAIN
Autora do módulo: Rafaela Gomes 2310112

Módulo principal do sistema de gerenciamento de hotel, responsável por interagir com o usuário e chamar as funções dos módulos hospede, quarto e hospedagem.

Para compilar: gcc -Wall quarto.c hospede.c hospedagem.c hotel_main.c -o hotel_main
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "hospedagem.h"
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
    char nomeHospede[100];
    ull documento;
    int telefone;
    int codigo;
    int dia,mes,ano,hora,minuto,diarias = 0;
    float valorTotal = 0.0f;

    //inicialização:
    printf("\n---INICIALIZANDO SISTEMA---\n\n");

    check = loadQuarto();
    check = loadHospede(); 
    check = loadHospedagem();

    char opcao = ' ';

    do {
        printf("\nBem vindo ao sistema de gerenciamento de hoteis.\nSelecione qual acaoo gostaria de realizar:\n\n"
        "\tExemplo: ACAO --- BOTAO\n\n"
        "\tCLIENTES / HOSPEDES\n\n\t\tCadastrar novo hospede --- a\n\t\tAlterar dados de hospede --- b\n"
        "\t\tConsultar dados de hospede --- c\n\t\tListar todos os hospedes --- d\n\n"
        "\tQUARTOS\n\n\t\tCadastrar novo quarto --- e\n\t\tConsultar dados de quarto --- f\n\t\tListar quartos --- g\n\n"
        "\tHOSPEDAGENS / RESERVAS\n\n\t\tRealizar check-in --- h\n\t\tRealizar check-out --- i\n\t\tListar hospedagens --- j\n\n"
        "\tSAIR DO PROGRAMA --- x\n");

        printf("\nDigite a opcao desejada: ");
        scanf(" %c", &opcao);
        limparEntrada();

        switch(opcao){

            case 'a':

                do{
                    printf("\nDigite o nome do hospede: ");
                    scanf(" %[^\n]", nomeHospede);
                    limparEntrada();
                    printf("\nDigite o numero do documento do hospede: ");
                    scanf(" %llu", &documento);
                    limparEntrada();
                    printf("\nDigite o telefone do hospede: ");
                    scanf(" %d", &telefone);
                    limparEntrada();
                    printf("\nDigite o codigo de identificacao para o novo hospede: ");
                    scanf(" %d", &codigo);
                    limparEntrada();
                    check = cadastraHospede(codigo, nomeHospede, documento, telefone);

                    printf("\nGostaria de cadastrar outro hospede? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando cadastro de hospedes.\n");
                        check = 0;
                    }

                } while(check!=0);

                break;

            case 'b':

                do{
                    printf("\nDeseja informar o codigo (1) ou o documento (2)? ");
                    scanf(" %d", &modo);
                    limparEntrada();
                    printf("\nInforme o numero: ");
                    scanf(" %llu", &documento);
                    limparEntrada();
                    check = alteraHospede(documento, modo);

                    printf("\nGostaria de alterar outro hospede? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando alteracao de hospedes.\n");
                        check = 0;
                    }

                } while(check!=0);

                break;

            case 'c':

                do{
                    printf("\nDeseja informar o codigo (1) ou o documento (2)? ");
                    scanf(" %d", &modo);
                    limparEntrada();
                    printf("\nInforme o numero: ");
                    scanf(" %llu", &documento);
                    limparEntrada();
                    check = consultaHospede(documento, modo);

                    printf("\nGostaria de consultar outro hospede? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando consulta de hospedes.\n");
                        check = 0;
                    }

                } while(check!=0);

                break;

            case 'd':
                listaHospedes();
                break;

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

            case 'h':

                do{
                    printf("\nInforme o codigo do hospede: ");
                    scanf(" %d", &codigo);
                    limparEntrada();
                    printf("\nInforme o numero do quarto: ");
                    scanf(" %d", &numQuarto);
                    limparEntrada();
                    printf("\nInforme a data de entrada (dd/mm/aaaa hh:mm): ");
                    scanf(" %d/%d/%d %d:%d", &dia, &mes, &ano, &hora, &minuto);
                    limparEntrada();
                    printf("\nInforme o numero de diarias: ");
                    scanf(" %d", &diarias);
                    limparEntrada();
                    check = checkIn(codigo, numQuarto, dia, mes, ano, hora, minuto, diarias);

                    if(check == HP_OK)
                        printf("\nCheck-in realizado com sucesso!\n");
                    else if(check == HP_HOSPEDE_INEXISTENTE)
                        printf("\nNao foi encontrado um hospede com o codigo %d.\n", codigo);
                    else if(check == HP_HOSPEDE_JA_HOSPEDADO)
                        printf("\nO hospede %d ja possui uma hospedagem ativa.\n", codigo);
                    else if(check == HP_QUARTO_INEXISTENTE)
                        printf("\nNao foi encontrado um quarto com o numero %d.\n", numQuarto);
                    else if(check == HP_QUARTO_OCUPADO)
                        printf("\nO quarto %d ja esta ocupado.\n", numQuarto);
                    else if(check == HP_DIARIAS_INVALIDAS)
                        printf("\nO numero de diarias informado e invalido.\n");
                    else if(check == HP_DATA_INVALIDA)
                        printf("\nA data de entrada informada e invalida.\n");
                    else if(check == HP_LIMITE_ATINGIDO)
                        printf("\nO limite de hospedagens ativas foi atingido. Nao e possivel realizar check-in.\n");
                    else
                        printf("\nOcorreu um erro inesperado. Codigo de retorno: %d\n", check);

                    printf("\nGostaria de realizar outro check-in? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando check-in.\n");
                        check = 0;
                    }

                } while(check!=0);

                break;

            case 'i':

                do{
                    printf("\nInforme o codigo do hospede: ");
                    scanf(" %d", &codigo);
                    limparEntrada();
                    printf("\nInforme o numero do quarto: ");
                    scanf(" %d", &numQuarto);
                    limparEntrada();
                    printf("\nInforme a data de saida (dd/mm/aaaa hh:mm): ");
                    scanf(" %d/%d/%d %d:%d", &dia, &mes, &ano, &hora, &minuto);
                    limparEntrada();
                    check = checkOut(codigo, numQuarto, dia, mes, ano, hora, minuto, &valorTotal);

                    if(check == HP_OK)
                        printf("\nCheck-out realizado com sucesso! Valor total da hospedagem: R$ %.2f\n", valorTotal);
                    else if(check == HP_HOSPEDAGEM_NAO_ENCONTRADA)
                        printf("\nNao foi encontrada uma hospedagem ativa para o hospede %d no quarto %d.\n", codigo, numQuarto);
                    else if(check == HP_SAIDA_ANTES_DA_ENTRADA)
                        printf("\nData de saida informada e anterior a data de entrada da hospedagem.\n");
                    else if(check == HP_ERRO_QUARTO)
                        printf("\nErro ao liberar o quarto. Verifique se o quarto esta cadastrado e tente novamente.\n");
                    else
                        printf("\nOcorreu um erro inesperado. Codigo de retorno: %d\n", check);

                    printf("\nGostaria de realizar outro check-out? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando check-out.\n");
                        check = 0;
                    }

                } while(check!=0);
                
                break;

            case 'j':

                do{
                    printf("\nListar hospedagens ativas (0) ou encerradas (1)? ");
                    scanf(" %d", &modo);
                    limparEntrada();
                    check = listaHospedagens(modo);

                    printf("\nGostaria de listar novamente? (s/n): ");
                    scanf(" %c", &resposta);
                    limparEntrada();

                    if(resposta == 's' || resposta == 'S') check = 1; 
                    else if(resposta == 'n' || resposta == 'N') check = 0;
                    else{
                        printf("\nResposta invalida! Encerrando listagem de hospedagens.\n");
                        check = 0;
                    }
                    
                } while(check!=0);

                break;

            case 'x':
                printf("\nSaindo do programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }
        
    } while(opcao != 'x');

    //Finalização:

    printf("\n---FINALIZANDO SISTEMA---\n");

    check = fechaQuarto();
    check = fechaHospede(); 
    check = fechaHospedagem();
    return 0;
}
