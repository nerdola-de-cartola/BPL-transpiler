#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <string.h>

#include "common.h"
#include "variables.h"
#include "assignment.h"

extern int IF_INDEX;

/**
 * @brief Imprime o comando if (em assembly)
 */
void ifStatement();

/**
 * @brief Verifica se o comando if é válido
 * @param c1 Tipo da fonte (variável, constante ou parâmetro)
 * @param c2 Tipo do valor (inteiro ou vetor)
 */
void verifyIfStatement(char c1, char c2);

#endif

