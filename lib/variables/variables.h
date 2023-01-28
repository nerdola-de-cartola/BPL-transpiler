#include "../common/common.h"
#include "../functions/functions.h"

#ifndef VARIABLES_H
#define VARIABLES_H

#pragma once

#define MAX_VARIABLE 5

typedef struct
{
   int size;
   int stackPosition;
   int type;
} Variable;

void verifyLocalVariables(char c1, char c2, char c3, int index);

void clearVARIABLES();

int localVariables();

void printLocalVariables(int index);

Variable *getVariable(int index);

#endif