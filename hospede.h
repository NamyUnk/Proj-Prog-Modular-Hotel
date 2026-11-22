#ifndef HOSPEDE_H
#define HOSPEDE_H

/*
MÓDULO HOSPEDE
Autor do módulo: Fabiano Júnior 2520144

Carrega os hóspedes de um arquivo binário, armazenando em uma lista encadeada temporária durante
o uso. A lista fica encapsulada no hospede.c, não sendo possível acessá-la diretamente de outros módulos, apenas 
utilizando funções de interface.

Dados de cada hóspede:
- Código de identificação (int)
- Nome (string)
- Documento (int)
- Telefone (int)
*/

#define LIM_HOSPEDES 100

#include <string.h>

/*
FUNÇÕES DE INICIALIZAÇÃO E FINALIZAÇÃO

--- loadHospede

Carrega os hóspedes do arquivo binário "hospede.bin" para a lista encadeada temporária.

Parametros: nenhum

Retorno: int
0 = sucesso
1 = arquivo nulo
2 = erro alocação de memória
3 = erro de leitura

Assertivas de entrada: nenhuma. Se o arquivo não existir, a lista encadeada temporária de hóspedes será inicializada 
como vazia.

Assertivas de saída: a lista encadeada temporária de hóspedes está carregada com os dados do arquivo binário.


--- fechaHospede

Salva os hóspedes da lista encadeada temporária no arquivo binário "hospede.bin".

Parametros: nenhum

Retorno: int
0 = sucesso
1 = arquivo nulo
2 = erro alocação de memória
3 = erro de escrita

Assertivas de entrada: a lista encadeada temporária de hóspedes está carregada com os dados do arquivo binário.

Assertivas de saída: os dados da lista encadeada temporária de hóspedes foram salvos no arquivo binário.
*/

int loadHospede();
int fechaHospede();

/*
FUNÇÕES DE INTERFACE

---consultaHospede

Retorna os dados do hóspede, dado o código ou o documento.

Parametros: int id (código ou documento do hóspede), int tipo (1=código, 2=documento)

Retorno: int
0 = sucesso
1 = hóspede não encontrado
2 = tipo inválido

Assertivas de entrada: a lista encadeada temporária de hóspedes está carregada com os dados do arquivo binário.

Assertivas de saída: os dados do hóspede foram exibidos, ou uma mensagem de erro foi exibida.


---cadastraHospede

Cadastra um novo hóspede na lista encadeada temporária de hóspedes.

Parametros: int codigo (código do hóspede), char* nome (nome do hóspede), int documento (documento do hóspede), int telefone (telefone do hóspede)

Retorno: int
0 = sucesso
1 = código inválido
2 = hóspede já registrado
3 = limite máximo de hóspedes atingido
4 = erro de memória

Assertivas de entrada: a lista encadeada temporária de hóspedes está carregada com os dados do arquivo binário.

Assertivas de saída: o hóspede foi cadastrado na lista encadeada temporária de hóspedes, ou uma mensagem de erro foi exibida.


---listaHospedes

Exibe a lista de hóspedes cadastrados na lista encadeada temporária de hóspedes.

Parametros: nenhum

Retorno: int
0 = sucesso (pelo menos um hóspede listado)
1 = lista vazia

Assertivas de entrada: a lista encadeada temporária de hóspedes está carregada com os dados do arquivo binário.

Assertivas de saída: a lista de hóspedes foi exibida, ou uma mensagem de erro foi exibida.


---alteraHospede

Altera os dados de um hóspede na lista encadeada temporária de hóspedes, dado o código ou o documento.

Parametros: int id (código ou documento do hóspede), int tipo (1=código, 2=documento)

Retorno: int
0 = sucesso
1 = hóspede não encontrado
2 = tipo inválido

Assertivas de entrada: a lista encadeada temporária de hóspedes está carregada com os dados do arquivo binário.

Assertivas de saída: os dados do hóspede foram alterados na lista encadeada temporária de hóspedes, ou uma mensagem de erro foi exibida.


---obtemNomeHospede

Obtém o nome de um hóspede dado o código.

Parametros: int codigo (código do hóspede), char* destino (buffer para armazenar o nome), size_t tamanho (tamanho do buffer)

Retorno: int
0 = achou e copiou o nome
1 = hóspede não encontrado

Assertivas de entrada: a lista encadeada temporária de hóspedes está carregada com os dados do arquivo binário.

Assertivas de saída: o nome do hóspede foi copiado para o buffer destino, ou uma mensagem de erro foi exibida.
*/

int consultaHospede(int id, int tipo); //id: código ou documento - tipo: 1= código, 2= documento
int cadastraHospede(int codigo, char* nome, int documento, int telefone);
int listaHospedes();
int alteraHospede(int id, int tipo);
int obtemNomeHospede(int codigo, char* destino, size_t tamanho);

#endif