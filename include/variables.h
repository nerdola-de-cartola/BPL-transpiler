#ifndef VARIABLES_H
#define VARIABLES_H

#include "common.h"
#include "functions.h"

#define MAX_VARIABLE 5

// Struct Tipo Variable, contendo as informações utilizadas por este tipo
typedef struct
{
   int size;
   int stackPosition;
   int type;
} Variable;

/**
 * @brief Verifica os casos de erro nas declarações das variáveis locais no arquivo bpl
 * 
 * @param qtd
 * @param c1
 * @param c2
 * @param c3
 * @param index
 * @return void
*/
void verifyLocalVariables(int qtd, char c1, char c2, char c3, int index);

/**
 * @brief "Limpa" as informações do Vetor VARIABLES para a próxima declaração
 * 
 * @return void
*/
void clearVARIABLES();

/**
 * @brief Lê a declaração de Variáveis no arquivo BPL, calcula as informações de cada uma
 * 
 * @return lastStackPos -> Posição da última variável declarada, na pilha
*/
int localVariables();

/**
 * @brief "Printa" A posição das variáveis na pilha, em formato de comentário Assembly
 * 
 * @param index
 * @return void
*/
void printLocalVariables(int index);

/**
 * @brief Recebe o índice da variável e retorna o endereço do vetor VARIABLES neste índice
 * 
 * @param index
 * @return &VARIABLES[index - 1];
*/
Variable *getVariable(int index);

#endif