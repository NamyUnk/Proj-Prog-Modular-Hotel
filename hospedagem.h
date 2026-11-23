#ifndef HOSPEDAGEM_H
#define HOSPEDAGEM_H

/*
MÓDULO HOSPEDAGEM
Autor do módulo: Vinicius Silberman Pereira 2510569

Registra os check-ins e check-outs dos hóspedes, calcula o valor das estadias e lista as
hospedagens ativas e encerradas. As hospedagens ficam em uma lista encadeada temporária
durante o uso, encapsulada no hospedagem.c, não sendo possível acessá-la diretamente de
outros módulos, apenas utilizando funções de interface.

É cliente dos módulos Hóspede e Quarto: consulta se o hóspede existe, se o quarto está
livre e qual o valor da diária, e pede ao módulo Quarto para ocupar e liberar quartos.

Dados de cada hospedagem:
- Código de identificação (int)
- Situação: ativa ou encerrada (HP_STATUS)
- Código do hóspede (int)
- Número do quarto (int)
- Data e horário de entrada (horario_S)
- Data e horário de saída (horario_S)
- Diárias reservadas (int)
- Diárias efetivas (int)
- Valor total (float)
*/

#define LIM_HOSPEDAGENS 500

/*
TIPOS DO MÓDULO

--- horario_S

Data e horário juntos, usados para informar a entrada e a saída de uma hospedagem.

--- HP_STATUS

Situação de uma hospedagem: HP_ATIVA (0) ou HP_ENCERRADA (1).

--- HP_RET

Condições de retorno de todas as funções do módulo.
*/

typedef struct horario {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} horario_S;

typedef enum {
    HP_ATIVA = 0,
    HP_ENCERRADA = 1
} HP_STATUS;

typedef enum {
    HP_OK = 0,                     /* operação concluída */
    HP_HOSPEDE_INEXISTENTE,        /* código de hóspede não cadastrado */
    HP_HOSPEDE_JA_HOSPEDADO,       /* hóspede já tem hospedagem ativa (RN02) */
    HP_QUARTO_INEXISTENTE,         /* número de quarto não cadastrado */
    HP_QUARTO_OCUPADO,             /* quarto já tem hospedagem ativa (RN01) */
    HP_DIARIAS_INVALIDAS,          /* diárias previstas <= 0 */
    HP_DATA_INVALIDA,              /* data ou horário fora do formato */
    HP_SAIDA_ANTES_DA_ENTRADA,     /* check-out anterior ao check-in */
    HP_HOSPEDAGEM_NAO_ENCONTRADA,  /* não há hospedagem ativa para o par hóspede/quarto */
    HP_STATUS_INVALIDO,            /* filtro diferente de HP_ATIVA e HP_ENCERRADA */
    HP_LISTA_VAZIA,                /* nenhuma hospedagem na situação pedida */
    HP_LIMITE_ATINGIDO,            /* LIM_HOSPEDAGENS alcançado */
    HP_ERRO_MEMORIA,               /* falha de alocação */
    HP_ERRO_QUARTO,                /* módulo Quarto recusou a operação */
    HP_ERRO_ARQUIVO,               /* não foi possível abrir ou gravar o arquivo */
    HP_ARQUIVO_INVALIDO            /* arquivo com linha incompleta ou dados inválidos */
} HP_RET;

/*
FUNÇÕES DE INICIALIZAÇÃO E FINALIZAÇÃO

--- carregaHospedagens

Lê o arquivo de texto gravado por salvaHospedagens e substitui a lista encadeada temporária
de hospedagens pelo conteúdo dele, na mesma ordem em que foi salvo.

Parametros: const char* nomeArquivo (nome do arquivo a ser lido)

Retorno: HP_RET
HP_OK = sucesso
HP_ERRO_ARQUIVO = arquivo não pôde ser aberto (por exemplo, ainda não existe)
HP_ARQUIVO_INVALIDO = arquivo com linha incompleta ou dados inválidos
HP_ERRO_MEMORIA = erro de alocação de memória

Assertivas de entrada: nomeArquivo não é nulo. O módulo Quarto foi carregado a partir do
arquivo salvo junto com este, para que os quartos das hospedagens ativas já estejam ocupados.

Assertivas de saída: em caso de sucesso, a lista encadeada temporária contém exatamente as
hospedagens do arquivo e o contador de hospedagens foi atualizado, de modo que o próximo
check-in recebe o código seguinte ao último lido. Em caso de erro, a lista anterior permanece
intacta. O status dos quartos não é alterado.


--- salvaHospedagens

Grava todas as hospedagens da lista encadeada temporária (ativas e encerradas) no arquivo de
texto informado, uma por linha. Se o arquivo já existir, é sobrescrito.

Parametros: const char* nomeArquivo (nome do arquivo a ser gravado)

Retorno: HP_RET
HP_OK = sucesso
HP_ERRO_ARQUIVO = arquivo não pôde ser aberto ou gravado

Assertivas de entrada: nomeArquivo não é nulo.

Assertivas de saída: em caso de sucesso, o arquivo contém todas as hospedagens da lista, com as
hospedagens ativas gravadas com a saída zerada. A lista encadeada temporária não é alterada.
*/

HP_RET loadHospedagem(const char* nomeArquivo);
HP_RET fechaHospedagem(const char* nomeArquivo);

/*
FUNÇÕES DE INTERFACE

--- checkIn

Registra a entrada de um hóspede em um quarto, criando uma hospedagem ativa (RF10 a RF14).

Parametros: int codHospede (código do hóspede), int numQuarto (número do quarto),
horario_S entrada (data e horário de entrada), int diarias (quantidade de diárias reservadas)

Retorno: HP_RET
HP_OK = sucesso
HP_HOSPEDE_INEXISTENTE = hóspede não encontrado
HP_HOSPEDE_JA_HOSPEDADO = hóspede já possui hospedagem ativa
HP_QUARTO_INEXISTENTE = quarto não encontrado
HP_QUARTO_OCUPADO = quarto já ocupado
HP_DIARIAS_INVALIDAS = quantidade de diárias menor ou igual a zero
HP_DATA_INVALIDA = data ou horário de entrada inválido
HP_LIMITE_ATINGIDO = limite máximo de hospedagens atingido
HP_ERRO_MEMORIA = erro de alocação de memória
HP_ERRO_QUARTO = o módulo Quarto recusou a ocupação ou o valor da diária

Assertivas de entrada: os módulos Hóspede e Quarto estão carregados. A lista encadeada temporária
de hospedagens foi carregada por carregaHospedagens ou está vazia.

Assertivas de saída: em caso de sucesso, uma nova hospedagem ativa foi inserida no início da lista,
com código sequencial, data e horário de entrada, diárias reservadas e valor previsto registrados,
e o quarto foi marcado como ocupado no módulo Quarto. Em caso de erro, a lista e o status do quarto
permanecem inalterados.


--- checkOut

Registra a saída de um hóspede, encerrando sua hospedagem ativa e liberando o quarto (RF15 a RF19).
As diárias efetivas são os dias entre entrada e saída, com mínimo de 1 (RN03), e o valor total é
diárias efetivas x valor da diária do quarto (RN04).

Parametros: int codHospede (código do hóspede), int numQuarto (número do quarto),
horario_S saida (data e horário de saída), float* valorTotal (parâmetro de saída que recebe o
valor da estadia; pode ser NULL se o valor não interessar)

Retorno: HP_RET
HP_OK = sucesso
HP_HOSPEDE_INEXISTENTE = hóspede não encontrado
HP_QUARTO_INEXISTENTE = quarto não encontrado
HP_HOSPEDAGEM_NAO_ENCONTRADA = não há hospedagem ativa desse hóspede nesse quarto
HP_DATA_INVALIDA = data ou horário de saída inválido
HP_SAIDA_ANTES_DA_ENTRADA = saída anterior à entrada
HP_ERRO_QUARTO = o módulo Quarto recusou a liberação ou o valor da diária

Assertivas de entrada: os módulos Hóspede e Quarto estão carregados. A lista encadeada temporária
de hospedagens foi carregada por carregaHospedagens ou está vazia.

Assertivas de saída: em caso de sucesso, a hospedagem foi marcada como encerrada, com data e horário
de saída, diárias efetivas e valor total registrados, o quarto foi liberado no módulo Quarto e, se
valorTotal não for NULL, *valorTotal contém o valor da estadia. Em caso de erro, a hospedagem e o
status do quarto permanecem inalterados e *valorTotal não é modificado.


--- listaHospedagens

Exibe as hospedagens da lista encadeada temporária que estejam na situação informada, seguidas
da quantidade encontrada (RF20, RF21).

Parametros: HP_STATUS status (HP_ATIVA ou HP_ENCERRADA)

Retorno: HP_RET
HP_OK = sucesso (pelo menos uma hospedagem listada)
HP_STATUS_INVALIDO = status diferente de HP_ATIVA e HP_ENCERRADA
HP_LISTA_VAZIA = nenhuma hospedagem na situação informada

Assertivas de entrada: a lista encadeada temporária de hospedagens foi carregada por
carregaHospedagens ou está vazia.

Assertivas de saída: as hospedagens na situação informada foram exibidas, ou nada foi exibido e o
código de erro foi retornado para que o chamador exiba a mensagem. A lista não é alterada.
*/

HP_RET checkIn(int codHospede, int numQuarto, horario_S entrada, int diarias);
HP_RET checkOut(int codHospede, int numQuarto, horario_S saida, float* valorTotal);
HP_RET listaHospedagens(HP_STATUS status);

#endif