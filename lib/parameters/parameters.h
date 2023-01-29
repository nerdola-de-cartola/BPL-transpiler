#ifndef PARAMETERS_H
#define PARAMETERS_H

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

void saveParameters();

void restoreParameters();

void passParameters(int index_function, char category[3], char type[3], int index[3]);

#endif
