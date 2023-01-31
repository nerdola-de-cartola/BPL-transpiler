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

void functionsInit();

void functionDefinition();

int verifyCallFunction(int qtd, char type_destiny, char category[3], char type[3]);

void callFunction();

void verifyReturn(char type);

void returnFunction();

void subq(int lastStackPos);

Function *getFunction(int index);

void printFunctionHeader();

void functionEnd();

#endif