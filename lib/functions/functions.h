#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#pragma once

#include <stdbool.h>
#include <string.h>

#include "../parameters/parameters.h"
#include "../common/common.h"
#include "../registers/registers.h"
#include "../conditions/conditions.h"
#include "../array/array.h"
#include "../assignment/assignment.h"
#include "../variables/variables.h"

#define MAX_FUNCTION 100
#define INT 0
#define VET 1

typedef struct {
   int parameterCount;
   Parameter parameters[3];
   int variableCount;
   bool valid;
} Function;

extern Function FUNCTIONS[MAX_FUNCTION];
extern int CURRENT_FUNCTION_INDEX;

void functionsInit();

void functionDefinition();

void verifyReturn(char type);

void returnFunction();

void subq(int lastStackPos);

Function *getFunction(int index);

void printFunctionHeader();

void printFunctionEnd();

#endif