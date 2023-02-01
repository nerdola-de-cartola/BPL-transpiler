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

void verifyParams(int order, char type1, char type2, char type3, int paramQtd);

void printParam(Parameter *p, int index);

int paramDefinition(int stackSize);

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
