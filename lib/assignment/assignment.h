#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "../registers/registers.h"
#include "../variables/variables.h"
#include "../parameters/parameters.h"
#include "../common/common.h"

#pragma once

#include <string.h>

void simpleAssignment(int index_destiny, int index_source, char type_destiny, char type_source);

bool isOperator(char op);

void verifyAssignment(
   int qtd_args,
   char op,
   char type_destiny,
   char type_source1,
   char type_source2);

void assignment();

Register *operation(char op, char type1, int index1, char type2, int index2);

Register *divi(char type1, int index1, char type2, int index2);

void setOperation(char op, char *operation);

#endif