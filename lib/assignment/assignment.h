#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "registers.h"
#include "variables.h"
#include "parameters.h"
#include "common.h"

#include <string.h>

/// @brief Realiza a atribuição (mov_) simples, ou seja, sem operações como adição, divisão, etc. 
/// @param index_destiny
/// @param index_source
/// @param type_destiny
/// @param type_source
/// @return void
void simpleAssignment(int index_destiny, int index_source, char type_destiny, char type_source);


/// @brief Verifica se char recebido é um tipo operador definido
/// @param op
/// @return bool (true ou false) 
bool isOperator(char op);


/// @brief Verifica os casos de erro no arquivo bpl (Atribuição)
/// @param qtd_args -> r
/// @param op -> operand
/// @param type_destiny -> type_destiny
/// @param type_source1 -> type_source1
/// @param type_source2 -> type_source2
/// @return void
void verifyAssignment(
   int qtd_args,
   char op,
   char type_destiny,
   char type_source1,
   char type_source2);


/// @brief Lê o comando e realiza a parte inicial de "atribuição" (main)
/// @param op
/// @return void
void assignment();


/// @brief Realiza o "print" da atribuição já em assembly e em caso de divisão, chama função "divi"
/// @param op
/// @param type1
/// @param index1
/// @param type2
/// @param index2
/// @return Tipo Registrador -> r
Register *operation(char op, char type1, int index1, char type2, int index2);


/// @brief Realiza "print" da operação bpl com operador de divisão, já na linguagem Assembly 
/// @param qtd_args -> r
/// @param op -> operand
/// @param type_destiny -> type_destiny
/// @param type_source1 -> type_source1
/// @return Tipo Registrador -> rax
Register *divi(char type1, int index1, char type2, int index2);


/// @brief Atribui o comando em Assembly ao parâmetro char *operation dado o tipo de operação (char op) recebido
/// @param op
/// @param operation
/// @return void
void setOperation(char op, char *operation);


/// @brief "Printa" o retorno do Valor final à pilha
/// @param type
/// @param index
/// @return void
void assignmentFromReturn(char type, int index);


#endif
