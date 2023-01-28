#include "functions.h"

Function FUNCTIONS[MAX_FUNCTION];
int CURRENT_FUNCTION_INDEX;

void functionsInit() {

   int i;

   for(i = 0; i < MAX_FUNCTION; i++) {
      FUNCTIONS[i].parameterCount = 0;
      FUNCTIONS[i].valid = false;
      FUNCTIONS[i].variableCount = 0;
   }

}

void functionDefinition()
{
   char type1, type2, type3;

   int paramLenght = sscanf(BUFFER, "function f%d p%c1 p%c2 p%c3",
                            &CURRENT_FUNCTION_INDEX,
                            &type1,
                            &type2,
                            &type3);

   FUNCTIONS[CURRENT_FUNCTION_INDEX - 1].valid = true;
   Function *function = getFunction(CURRENT_FUNCTION_INDEX);
   function->parameterCount = paramLenght - 1;

   verifyParams(CURRENT_FUNCTION_INDEX, type1, type2, type3, function->parameterCount);

   if (function->parameterCount >= 1)
   {
      function->parameters[0].type = type1 == 'i' ? INT : VET;
      function->parameters[0].reg = getRegister("rdi", CALLER_SAVED);
   }
   if (function->parameterCount >= 2)
   {
      function->parameters[1].type = type2 == 'i' ? INT : VET;
      function->parameters[1].reg = getRegister("rsi", CALLER_SAVED);
   }
   if (function->parameterCount >= 3)
   {
      function->parameters[2].type = type3 == 'i' ? INT : VET;
      function->parameters[2].reg = getRegister("rdx", CALLER_SAVED);
   }

   printFunctionHeader();

   clearVARIABLES();

   readNewLine();

   if (strcmp(BUFFER, "def") != 0)
      error("Missing definition of local variables");

   int stackSize = localVariables();
   stackSize = paramDefinition(stackSize);
   subq(stackSize);

   while (true)
   {

      readNewLine();

      if (strcmp(BUFFER, "end") == 0)
         break;
      else if (charInStr('=', BUFFER))
         assignment(&function);
      else if (strInStr(BUFFER, "index"))
         arrayAccess();
      else if (strInStr(BUFFER, "if"))
         ifStatement();
      else
         error("Invalid instruction in function definition");

      fprintf(F_OUTPUT, "\n");
   }

   // leave ret
   printFunctionEnd();
}

void subq(int lastStackPos)
{
   while (lastStackPos % 16 != 0)
      lastStackPos++;

   fprintf(F_OUTPUT, "subq $%d, %%rsp\n\n", lastStackPos);
}

Function *getFunction(int index) {

   if(index < 1 || index > MAX_FUNCTION || !FUNCTIONS[index - 1].valid)
      error("Invalid function index");

   return &FUNCTIONS[index - 1];
}

void printFunctionHeader()
{
   fprintf(F_OUTPUT, ".globl f%d\n", CURRENT_FUNCTION_INDEX);
   fprintf(F_OUTPUT, "f%d:\n\n", CURRENT_FUNCTION_INDEX);
   fprintf(F_OUTPUT, "pushq %%rbp\n");
   fprintf(F_OUTPUT, "movq %%rsp, %%rbp\n\n");
}

void printFunctionEnd()
{
   int i;
   Function *f = getFunction(CURRENT_FUNCTION_INDEX); 

   for(i = 0; i < f->parameterCount; i++)
      freeRegister(&f->parameters[i].reg);

   fprintf(F_OUTPUT, "leave\nret\n");
}