#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>
#include <string.h>

#include "parameters.h"

typedef struct {
   int parameterCount;
   Parameter parameters[3];
   int variableCount;

} Function;

#include "common.h"
#include "registers.h"
#include "conditions.h"
#include "array.h"
#include "assignment.h"
#include "variables.h"

#define MAX_FUNCTION 100
#define INT 0
#define VET 1

extern Function FUNCTIONS[MAX_FUNCTION];
extern int CURRENT_FUNCTION_INDEX;

/**
 * @brief Inicializa o vetor de funções
*/
void functionsInit();

/**
 * @brief Cria uma função por completo
*/
void functionDefinition();

/**
 * @brief Verifica se os parâmetros passados para uma chamada de função são válidos
 * @param index Índice da função
 * @param qtd Quantidade de parâmetros
 * @param type_destiny Tipo do destino do retorno da função (variável ou parâmetro)
 * @param category Categoria do destino do retorno da função (constante, variavel ou parâmetro)
 * @param type Tipo do valor na categoria (inteiro ou vetor)
*/
int verifyCallFunction(int qtd, char type_destiny, char category[3], char type[3]);

/**
 * @brief Realiza a chamada de uma função
*/
void callFunction();

/**
 * @brief Verifica se a função retorna um tipo valido
*/
void verifyReturn(char type);

/**
 * @brief Move o valor de retorno para os registradores (em assembly)
*/
void returnFunction();

/**
 * @brief imprime a subtração da pilha de uma função (em assembly)
*/
void subq(int lastStackPos);

/**
 * @brief Função para acessar os dados de uma determinada função
 * 
 * @param index Índice da função desejada (1-based)
 * @return Function * Um ponteiro para a função desejada
 */
Function *getFunction(int index);

/**
 * @brief Imprime o cabeçalho de uma função (em assembly)
*/
void printFunctionHeader();

/**
 * @brief Imprime a finalização de uma função (em assembly)
*/
void functionEnd();

#endif