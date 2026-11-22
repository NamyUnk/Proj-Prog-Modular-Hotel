#ifndef QUARTO_H
#define QUARTO_H

/*
MÓDULO QUARTO
Autora do módulo: Rafaela Gomes 2310112

Carrega os quartos de um arquivo binário, armazenando em uma lista encadeada temporária durante
o uso, ordenando por número do quarto. 
A lista fica encapsulada no quarto.c, não sendo possível acessá-la diretamente de outros módulos,
apenas utilizando funções de interface.

Dados de cada quarto:

- Número do quarto (int)
- Tipo do quarto (int) 0=Suíte, 1=Solteiro, 2=Casal
- Valor da diária (float)
- Status do quarto (int) 0=livre, 1=ocupado
*/

#define LIM_QUARTOS 50

/*
FUNÇÕES DE INICIALIZAÇÃO E FINALIZAÇÃO

--- loadQuarto

Carrega os quartos do arquivo binário "quarto.bin" para a lista encadeada temporária.

Parametros: nenhum

Retorno: int
0 = sucesso
1 = arquivo nulo
2 = erro alocação de memória
3 = erro de leitura

Assertivas de entrada: nenhuma. Se o arquivo não existir, a lista encadeada temporária de quartos será inicializada 
como vazia.

Assertivas de saída: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.


--- fechaQuarto

Salva os quartos da lista encadeada temporária no arquivo binário "quarto.bin".

Parametros: nenhum

Retorno: int
0 = sucesso
1 = arquivo não abriu
2 = erro ao salvar

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: os dados da lista encadeada temporária de quartos foram salvos no arquivo binário.
*/

int loadQuarto(void);
int fechaQuarto(void);

/*
FUNÇÕES DE INTERFACE PARA O HOSPEDAGEM.C

---statusQuarto

Retorna o status do quarto (livre ou ocupado)

Parametros: int num (número do quarto)

Retorno: int
0 = livre
1 = ocupado
2 = erro nas informações do quarto
3 = quarto inexistente

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: o status do quarto foi retornado, ou uma mensagem de erro foi exibida.


---diariaQuarto

Retorna o valor da diária do quarto

Parametros: int num (número do quarto), float* diaria (ponteiro para armazenar o valor da diária)

Retorno: int
0 = sucesso
1 = erro parâmetro
2 = quarto inexistente
3 = erro ao resgatar a diária

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: o valor da diária do quarto foi retornado, ou uma mensagem de erro foi exibida.


---liberaQuarto

Libera o quarto, colocando-o no estado livre.

Parametros: int num (número do quarto)

Retorno: int
0 = sucesso
1 = quarto inexistente
2 = erro ao liberar o quarto

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: o quarto foi liberado, ou uma mensagem de erro foi exibida.


---ocupaQuarto

Ocupar o quarto, colocando-o no estado ocupado.

Parametros: int num (número do quarto)

Retorno: int
0 = sucesso
1 = quarto inexistente
2 = erro ao ocupar o quarto

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: o quarto foi ocupado, ou uma mensagem de erro foi exibida.
*/

int statusQuarto(int num); //retorna status do quarto
int diariaQuarto(int num,float* diaria); //passa o valor da diaria pelo parametro int* diaria
int liberaQuarto(int num); //libera o quarto, status=livre
int ocupaQuarto(int num); //ocupa o quarto, status=ocupado

/*
FUNÇÕES DE INTERFACE PARA O HOTEL_MAIN.C

---consultaQuarto

Imprime as informações do quarto (número, tipo, valor da diária e status)

Parametros: int num (número do quarto)

Retorno: int
0 = sucesso
1 = quarto inexistente
2 = erro ao consultar o quarto

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: as informações do quarto foram exibidas, ou uma mensagem de erro foi exibida.


---cadastraQuarto

Cadastra um novo quarto na lista encadeada temporária de quartos.

Parametros: int num (número do quarto), int tipo (tipo do quarto), float valor (valor da diária)

Retorno: int
0 = sucesso
1 = valores inválidos
2 = quarto já registrado
3 = erro de memória
4 = erro percorrendo lista

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: o quarto foi cadastrado na lista encadeada temporária de quartos, ou uma mensagem de erro foi exibida.


---listaQuartos

Lista os quartos da lista encadeada temporária de quartos, de acordo com o modo especificado.

Parametros: int modo (0 = listar apenas quartos livres, 1 = listar todos os quartos)

Retorno: int
0 = sucesso
1 = erro ao listar os quartos
2 = modo inválido

Assertivas de entrada: a lista encadeada temporária de quartos está carregada com os dados do arquivo binário.

Assertivas de saída: os quartos foram listados de acordo com o modo especificado, ou uma mensagem de erro foi exibida.
*/

int consultaQuarto(int num); //imprime as informações do quarto
int cadastraQuarto(int num, int tipo, float valor); //status sempre vazio quando criado
int listaQuartos(int modo); //modo: 0= livres, 1= todos


#endif