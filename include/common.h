#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 256

// Declaração de Variáveis Globais (externas)
extern FILE *F_SOURCE;
extern FILE *F_OUTPUT;
extern char BUFFER[MAX_LINE_SIZE];
extern int LINE_COUNT;

/**
 * @brief Remove comando de nova linha em File
 * 
 * @param ptr
 * @return void
*/
void remove_newline(char *ptr);

/**
 * @brief Verifica ocorrência de substring em string maior
 * 
 * @param string
 * @param substring
 * @return bool (true ou false)
*/
bool strInStr(char *string, char *substring);

/**
 * @brief Verifica ocorrência de caractere (const char c) em string (const char *str)
 * 
 * @param c
 * @param str
 * @return bool (true ou false)
*/
bool charInStr(const char c, const char *str);

/**
 * @brief "Printa" mensagem de erro desejada, linha em que ocorreu o erro, e encerra o programa
 * 
 * @param error_type
 * @return void
*/
void error(const char *error_type);

/**
 * @brief Atualiza a linha a ser lida no instante
 * 
 * @return ptr
*/
char *readNewLine();

/**
 * @brief "Printa" .text em arquivo output, simbolizando início do código assembly 
 * 
 * @return void
*/
void beginFile();

#endif