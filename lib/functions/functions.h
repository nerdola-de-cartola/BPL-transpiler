#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>
#include <string.h>

#include "../parameters/parameters.h"

typedef struct {
   int parameterCount;
   Parameter parameters[3];
   int variableCount;
   bool valid;
} Function;

#include "../common/common.h"
#include "../registers/registers.h"
#include "../conditions/conditions.h"
#include "../array/array.h"
#include "../assignment/assignment.h"
#include "../variables/variables.h"

#define MAX_FUNCTION 100
#define INT 0
#define VET 1

extern Function FUNCTIONS[MAX_FUNCTION];
extern int CURRENT_FUNCTION_INDEX;

void functionsInit();

void functionDefinition();

void verifyCallFunction(int qtd, char type_destiny, int index_function, char category[3], char type[3]);

void callFunction();

void verifyReturn(char type);

void returnFunction();

void subq(int lastStackPos);

Function *getFunction(int index);

void printFunctionHeader();

void printFunctionEnd();

#endif