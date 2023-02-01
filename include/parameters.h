#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "common.h"
#include "registers.h"
#include "type.h"

typedef struct {
   Type type;
   Register *reg;
   int stackPosition;
} Parameter;

#include "functions.h"

/**
 * @brief Verifica os parâmetros para criação de uma função
 * 
 * @param order Índice da função
 * @param type1 Tipo do primeiro parâmetro
 * @param type2 Tipo do segundo parâmetro
 * @param type3 Tipo do terceiro parâmetro
 * @param paramQtd Quantidade de parâmetros lido (sscanf)
 */
void verifyParams(int order, char type1, char type2, char type3, int paramQtd);

/**
 * @brief Imprime comentários no código assembly para mostrar o registrador
 * e a posição na pilha onde o parâmetro vai ser guardado
 * 
 * @param p Ponteiro para o parâmetro
 * @param index Índice do parâmetro (1-based)
 */
void printParam(Parameter *p, int index);

/**
 * @brief Separa espaços na pilha para salvar cada um dos parâmetros,
 * apesar desse espaço ser separado previamente ele será utilizado 
 * somente se necessário
 * 
 * @param stackSize Tamanho atual da pilha 
 * @return int Novo tamanho da pilha após inserir os parâmetros
 */
int paramDefinition(int stackSize);

/**
 * @brief Função para acessar os parâmetros da função atual
 * 
 * @param index Índice do parâmetro desejado (1-based)
 * @return Parameter* Ponteiro para o parâmetro desejado
 */
Parameter *getParameter(int index);

/**
 * @brief Salva os parâmetros da função (assembly)
*/
void saveParameters();

/**
 * @brief Restaura os parâmetros da função (assembly)
 * @return void
*/
void restoreParameters();

/**
 * @brief Passa os parâmetros para a função
 * @param qtd Quantidade de parâmetros
 * @param category Categoria do parâmetro (constante, variável ou parâmetro)
 * @param type Tipo do valor na categoria (inteiro ou vetor)
 * @param index Índice do parâmetro
*/
void passParameters(int qtd, char category[3], char type[3], int index[3]);

#endif
