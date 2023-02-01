#ifndef ARRAY_H
#define ARRAY_H

#include "common.h"
#include "variables.h"
#include "registers.h"
#include "functions.h"

/**
  * @brief Verifica se o acesso ao array é válido
  * @param r Número de parâmetros lidos (sscanf)
  * @param c1 primeiro  parametro ou vetor
  * @param c2 parametro, vetor ou variavel
  * @param c3 valor a ser atribuido
  */
void verifyArrayAccess(int r, char c1, char c2, char c3);

void arrayAccess();

/**
  * @brief Realiza o acesso ao array
  * @param r Ponteiro para o registrador que será utilizado
  * @param type Se vai enviar para uma variavel ou para um parametro
  * @param index Índice a ser acessado
  */
void arrayAccessGet(Register *r, char type, int index);

/**
  * @brief Realiza a atribuição ao array
  * @param r Ponteiro para o registrador que será utilizado
  * @param type Se vai enviar para uma variavel ou para um parametro
  * @param index Índice a ser definido
  */
void arrayAccessSet(Register *r, char type, int index);

#endif