#ifndef PARAMETERS_H
#define PARAMETERS_H

#pragma once

#include "../common/common.h"
#include "../registers/registers.h"
#include "../type.h"

typedef struct {
   Type type;
   Register *reg;
   int stackPosition;
} Parameter;

#include "../functions/functions.h"

void verifyParams(int order, char type1, char type2, char type3, int paramQtd);

void printParam(Parameter *p, int index);

int paramDefinition(int stackSize);

Parameter *getParameter(int index);

#endif
