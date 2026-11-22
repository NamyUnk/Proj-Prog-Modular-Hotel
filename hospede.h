#ifndef HOSPEDE_H
#define HOSPEDE_H

/*
MÓDULO HOSPEDE

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
*/

int consultaHospede(int id, int tipo); //id: código ou documento - tipo: 1= código, 2= documento
int cadastraHospede(int codigo, char* nome, int documento, int telefone);
void listaHospedes();
int alteraHospede(int id, int tipo);
int obtemNomeHospede(int codigo, char* destino, size_t tamanho);

#endif
