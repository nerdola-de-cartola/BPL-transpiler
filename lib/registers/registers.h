#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../type.h"
#include "../common/common.h"

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

void registersInit();

Register *getRegister(char *name64, int type);

void freeRegister(Register **r);

void registerName(int index, char name32[4], char name64[4]);

#endif