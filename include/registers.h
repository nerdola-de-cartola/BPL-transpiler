#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "type.h"
#include "common.h"

#define MAX_REGISTER 16
#define CALLER_SAVED 0
#define CALLEE_SAVED 1

typedef struct
{
   char name32[4];
   char name64[4];
   bool free;
   Type type;
} Register;

/**
 * @brief Inicializa o vetor de registradores
 */
void registersInit();

/**
 * @brief Retorna um registrador e o marca como ocupado
 */
Register *getRegister(char *name64, int type);

/**
 * @brief Libera um registrador para uso
 */
void freeRegister(Register **r);

/**
 * @brief Função para descobrir o nomo do correto do
 * registrador de um parâmetro.
 * exemplo
 * 1º parâmetro = %rdi
 * 2º parâmetro = %rsi
 * 3º parâmetro = %rdx
 * 
 * @param index Índice do parâmetro desejado
 * @param name32 Nome do registrador de 32 bits
 * @param name64 Nome do registrador de 64 bits
 */
void registerName(int index, char name32[4], char name64[4]);

#endif