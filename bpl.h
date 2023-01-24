#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_SIZE 256
#define MAX_REGISTER 16
#define MIN_VARIABLE 1
#define MAX_VARIABLE 5
#define MIN_PARAMETERS 1
#define MAX_PARAMETERS 3
#define MAX_FUNCTIONS 1000

typedef enum {
   CALLER_SAVED,
   CALLEE_SAVED
} RegisterType;

typedef enum {
   INT,
   VET
} VarTypes;

typedef struct
{
   char name32[4];
   char name64[4];
   bool free;
   RegisterType type;
} Register;

// Save parameters on callee saved registers
typedef struct {
   VarTypes type;
   Register *reg;
} Parameter;

typedef struct
{
   int size;
   int stackPosition;
   VarTypes type;
} Variable;

typedef struct {
   int parameterCount;
   Parameter parameters[MAX_PARAMETERS];
   int variableCount;
   Variable variables[MAX_VARIABLE];
   int order;
} Function;

bool charInStr(const char c, const char *str);

void assignment();

void error(const char *error_type);

Register *add(char type1, int index1, char type2, int index2);

Register *sub(char type1, int index1, char type2, int index2);

Register *mul(char type1, int index1, char type2, int index2);

Register *divi(char type1, int index1, char type2, int index2);

Register *getRegister(char *name64, RegisterType type);

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

void verifyLocalVariables(char c1, char c2, char c3, int index);

char *readNewLine();

void functionDefinition();