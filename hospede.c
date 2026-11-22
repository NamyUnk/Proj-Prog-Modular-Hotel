#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hospede.h"

/*
codigos de retorno das funcoes PRA USAR NA MAIN QUANDO ESTIVER PRONTA:
0 = sucesso na operacao
1 = codigo invalido (precisa ser inteiro positivo maior que zero)
2 = hospede duplicado (codigo ou documento ja cadastrado no sistema)
3 = limite atingido (atingiu a capacidade maxima de lim_hospedes)
4 = erro de alocacao de memoria (malloc falhou ao tentar alocar)
*/

typedef struct hospede{

    int codigo;
    char* nome;
    int documento;
    int telefone;

} hospede;

typedef struct cadeiaHospede{

    hospede* hospede;
    struct cadeiaHospede* prox;

} cadeiaHospede;

cadeiaHospede* hospedes = NULL;

int cadastraHospede(int codigo, char* nome, int documento, int telefone) {
    if (codigo <= 0) {
        printf("\nO codigo do hospede precisa de ser um inteiro positivo.\n");
        return 1;
    }
                
cadeiaHospede* atual = hospedes; // verifica hospedes duplicados e o limite max
int total = 0;

    while (atual) { //enquanto houverem hospedes:
        if (atual->hospede) {
            if (atual->hospede->codigo == codigo) {
                printf("\njá existe um hospede registado com esse codigo %d.\n", codigo);
                return 2;
            }
            if (atual->hospede->documento == documento) {
                printf("\nJá existe um hospede registado com o documento %d.\n", documento);
                return 2;
            }
        }
        total++;
        atual = atual->prox;
    }

    if (total >= LIM_HOSPEDES) {
        printf("\nLimite máximo de hospedes atingido (%d).\n", LIM_HOSPEDES);
        return 3;
    }

    cadeiaHospede* novoNo = (cadeiaHospede*)malloc(sizeof(cadeiaHospede)); // alocando o nó da lista
    if (!novoNo) {
        printf("\nErro ao alocar memoria para o no do hospede.\n");
        return 4;
    }

    novoNo->hospede = (hospede*)malloc(sizeof(hospede)); // alocando a struct do hospede
    if (!novoNo->hospede) {
        free(novoNo);
        printf("\nErro ao alocar memoria para os dados do hospede.\n");
        return 4;
    }

    //valores:
    novoNo->hospede->codigo = codigo;
    novoNo->hospede->documento = documento;
    novoNo->hospede->telefone = telefone;

    if (nome) {
        novoNo->hospede->nome = (char*)malloc(strlen(nome) + 1);
        if (novoNo->hospede->nome) {
            strcpy(novoNo->hospede->nome, nome);
        } else {
            novoNo->hospede->nome = NULL;
        }
    } else {
        novoNo->hospede->nome = NULL;
    }

    novoNo->prox = hospedes; //insere no inicio da lista
    hospedes = novoNo;

    printf("\nHospede %s registado com sucesso!\n", nome ? nome : ""); //exibição do nome e mensagem de sucesso
    return 0;
}

int consultaHospede(int id, int tipo) {
    
    if (tipo != 1 && tipo != 2) {
        printf("\nTipo de consulta invalido! Use 1 para Codigo ou 2 para Documento.\n");
        return 2;
    }

    cadeiaHospede* atual = hospedes;

    while (atual) { //percorre a lista de hospedes 
        if (atual->hospede) {
            int encontrado = 0;
            if (tipo == 1 && atual->hospede->codigo == id) encontrado = 1;
            if (tipo == 2 && atual->hospede->documento == id) encontrado = 1;

            if (encontrado) { //exibe os dados do hospede pedido
                printf("\n--- Dados do Hospede ---\n");
                printf("Codigo: %d\n", atual->hospede->codigo);
                printf("Nome: %s\n", atual->hospede->nome ? atual->hospede->nome : "N/D");
                printf("Documento: %d\n", atual->hospede->documento);
                printf("Telefone: %d\n", atual->hospede->telefone);
                return 0;
            }
        }
        atual = atual->prox;
    }

    printf("\nHospede nao encontrado.\n");
    return 1;
}

int listaHospedes() {
    /* retornos:
    0 = sucesso (pelo menos um hospede listado)
    1 = lista vazia
    */
    cadeiaHospede* atual = hospedes;
    int encontrados = 0;

    printf("\n--- Lista de Hospedes ---\n\n");
    while (atual) {
        if (atual->hospede) {
            printf("Codigo: %d\tNome: %s\tDocumento: %d\tTelefone: %d\n",
                   atual->hospede->codigo,
                   atual->hospede->nome ? atual->hospede->nome : "N/D",
                   atual->hospede->documento,
                   atual->hospede->telefone);
            encontrados++;
        }
        atual = atual->prox;
    }

    if (encontrados == 0) {
        printf("Nao existem hospedes registados no sistema.\n");
        return 1;
    }

    return 0;
}

int alteraHospede(int id, int tipo) {
    if (tipo != 1 && tipo != 2) {
        printf("\nTipo invalido! Use 1 para Codigo ou 2 para Documento.\n");
        return 2;
    }

    cadeiaHospede* atual = hospedes;

    while (atual) {
        if (atual->hospede) {
            int encontrado = 0;
            if (tipo == 1 && atual->hospede->codigo == id) encontrado = 1;
            if (tipo == 2 && atual->hospede->documento == id) encontrado = 1;

            if (encontrado) {
                char novoNome[100];
                int novoTelefone;

                printf("\nHospede encontrado: %s (Codigo: %d)\n", 
                       atual->hospede->nome ? atual->hospede->nome : "N/D", 
                       atual->hospede->codigo);

                printf("Introduza o novo nome: ");
                scanf(" %[^\n]", novoNome);

                printf("Introduza o novo telefone: ");
                scanf("%d", &novoTelefone);

                if (atual->hospede->nome) { //libera a memoria do nome antigo alocado dinamicamente
                    free(atual->hospede->nome);
                }
                atual->hospede->nome = (char*)malloc(strlen(novoNome) + 1); //aloca um novo
                if (atual->hospede->nome) {
                    strcpy(atual->hospede->nome, novoNome);
                }

                atual->hospede->telefone = novoTelefone;

                printf("\nDados do hospede alterados com sucesso!\n");
                return 0;
            }
        }
        atual = atual->prox;
    }

    printf("\nHospede nao encontrado para alteracao.\n");
    return 1;


}

int loadHospede() {

    /* retornos:
    0 = sucesso
    1 = arquivo nulo
    2 = erro alocacao de memoria
    3 = erro de leitura
    */

    FILE* arquivo = fopen("hospede.bin", "rb");

    if (!arquivo) {

        printf("\nErro ao abrir o arquivo para leitura!\n");

        return 1;
    }

    int codigo, documento, telefone, tamanho;
    cadeiaHospede* novo;
    cadeiaHospede* atual = hospedes;

    while (atual && atual->prox) atual = atual->prox; //vai ate o fim da lista

    while (fread(&codigo, sizeof(int), 1, arquivo) == 1) {

        //o nome e um ponteiro, entao e salvo separado: tamanho (com o \0) e depois os caracteres
        if (fread(&documento, sizeof(int), 1, arquivo) != 1 ||
            fread(&telefone, sizeof(int), 1, arquivo) != 1 ||
            fread(&tamanho, sizeof(int), 1, arquivo) != 1 || tamanho < 0) {

            printf("\nErro ao ler o arquivo!\n");
            fclose(arquivo);

            return 3;
        }

        novo = malloc(sizeof(cadeiaHospede));

        if (!novo) {

            printf("\nErro ao alocar memoria para o no!\n");
            fclose(arquivo);

            return 2;
        }

        novo->hospede = malloc(sizeof(hospede));

        if (!novo->hospede) {

            free(novo);
            printf("\nErro ao alocar memoria para o hospede!\n");
            fclose(arquivo);

            return 2;
        }

        novo->hospede->nome = NULL; //tamanho 0 = hospede sem nome

        if (tamanho > 0) {

            novo->hospede->nome = malloc(tamanho);

            if (!novo->hospede->nome) {

                free(novo->hospede);
                free(novo);
                printf("\nErro ao alocar memoria para o nome!\n");
                fclose(arquivo);

                return 2;
            }

            if (fread(novo->hospede->nome, 1, tamanho, arquivo) != (size_t)tamanho) {

                free(novo->hospede->nome);
                free(novo->hospede);
                free(novo);
                printf("\nErro ao ler o arquivo!\n");
                fclose(arquivo);

                return 3;
            }

            novo->hospede->nome[tamanho - 1] = '\0';
        }

        novo->hospede->codigo = codigo;
        novo->hospede->documento = documento;
        novo->hospede->telefone = telefone;
        novo->prox = NULL;

        if (hospedes == NULL) hospedes = novo;
        else atual->prox = novo;

        atual = novo;
    }

    if (ferror(arquivo)) {
        printf("\nErro ao ler o arquivo!\n");
        fclose(arquivo);
        return 3;
    }

    fclose(arquivo);

    printf("\nHospedes carregados com sucesso!\n");

    return 0;
}

int fechaHospede() {

    /* retornos:
    0 = sucesso
    1 = arquivo nao abriu
    2 = erro ao salvar
    */

    FILE* arquivo = fopen("hospede.bin", "wb");

    if (!arquivo) {

        printf("\nErro ao abrir o arquivo para escrita!\n");

        return 1;
    }

    cadeiaHospede* atual = hospedes;

    while (atual != NULL) {

        if (atual->hospede) {

            int tamanho = atual->hospede->nome ? (int)strlen(atual->hospede->nome) + 1 : 0; //0 = sem nome

            if (fwrite(&atual->hospede->codigo, sizeof(int), 1, arquivo) != 1 ||
                fwrite(&atual->hospede->documento, sizeof(int), 1, arquivo) != 1 ||
                fwrite(&atual->hospede->telefone, sizeof(int), 1, arquivo) != 1 ||
                fwrite(&tamanho, sizeof(int), 1, arquivo) != 1 ||
                (tamanho > 0 && fwrite(atual->hospede->nome, 1, tamanho, arquivo) != (size_t)tamanho)) {

                printf("\nErro ao salvar os hospedes no arquivo!\n");
                fclose(arquivo);

                return 2;
            }
        }

        atual = atual->prox;
    }

    fclose(arquivo);

    printf("\nHospedes salvos com sucesso!\n");

    return 0;
}

int obtemNomeHospede(int codigo, char* destino, size_t tamanho) {

    /* retornos:
    0 = achou e copiou o nome
    1 = hospede nao encontrado
    */

    cadeiaHospede* atual = hospedes;

    while (atual) {
        if (atual->hospede && atual->hospede->codigo == codigo) {
            if (destino && tamanho > 0) { 
                snprintf(destino, tamanho, "%s", atual->hospede->nome ? atual->hospede->nome : "N/D");
            }
            return 0;
        }
        atual = atual->prox;
    }

    return 1;
}
