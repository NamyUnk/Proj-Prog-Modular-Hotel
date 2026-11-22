#ifndef TESTE_H
#define TESTE_H

/* confereTeste: compara o retorno obtido com o esperado e conta o caso de teste.
   Imprime "[OK] descricao" ou "[FALHA] descricao --- esperado: X, obtido: Y". */
void confereTeste(char* descricao, int obtido, int esperado);

/* resultadoTestes: imprime o total de testes e de falhas do modulo.
   Retorna o numero de falhas (0 = todos passaram). */
int resultadoTestes(char* modulo);

#endif