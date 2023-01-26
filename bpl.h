#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_SIZE 256

#define CALLER_SAVED 0
#define CALLEE_SAVED 1

#define MAX_REGISTER 16
#define MAX_VARIABLE 5
#define MAX_FUNCTION 100

#define INT 0
#define VET 1

typedef int Type;

typedef struct
{
   char name32[4];
   char name64[4];
   bool free;
   Type type;
} Register;

typedef struct {
   Type type;
   Register *reg;
   int stackPosition;
} Parameter;

typedef struct
{
   int size;
   int stackPosition;
   int type;
} Variable;

typedef struct {
   int parameterCount;
   Parameter parameters[3];
   int variableCount;
   int order;
} Function;

void beginFile();

bool charInStr(const char c, const char *str);

void assignment();

void error(const char *error_type);

Register *add(char type1, int index1, char type2, int index2);

Register *sub(char type1, int index1, char type2, int index2);

Register *mul(char type1, int index1, char type2, int index2);

Register *divi(char type1, int index1, char type2, int index2);

Register *getRegister(char *name64, int type);

void freeRegister(Register **r);

void registersInit();

int localVariables();

void printLocalVariables(int index);

void subq(int lastStackPos);

void remove_newline(char *ptr);

Variable *getVariable(int index);

void ifStatement();

bool strInStr(char *string, char*substring);

void verifyIfStatement(char c1, char c2, int index);

char *readNewLine();

void verifyArrayAccess(int r, char c1, int id1, int index, char c2, char c3, int id2);

void arrayAccess();

void arrayAccessGet(Register *r, char type, int index);

void arrayAccessSet(Register *r, char type, int index);

bool strInStr(char *string, char*substring);

void functionDefinition();

void printFunctionHeader(Function *function);

void printFunctionEnd();

void verifyParams(int order, char type1, char type2, char type3, int paramQtd);