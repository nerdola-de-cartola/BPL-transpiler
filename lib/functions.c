#include "../include/functions.h"

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
      else if(strInStr(BUFFER, "call"))
         callFunction();
      else if (charInStr('=', BUFFER))
         assignment(&function);
      else if (strInStr(BUFFER, "index"))
         arrayAccess();
      else if (strInStr(BUFFER, "if"))
         ifStatement();
      else if(strInStr(BUFFER, "return"))
         returnFunction();
      else
         error("Invalid instruction in function definition");

      fprintf(F_OUTPUT, "\n");
   }

   // leave ret
   printFunctionEnd();
}

void verifyCallFunction(int qtd, char type_destiny, int index_function, char category[3], char type[3]) {

   if(qtd != 3 && qtd != 6 && qtd != 9 && qtd != 12)
      error("Invalid function call");

   if(type_destiny != 'v' && type_destiny != 'p')
      error("Invalid destiny in function call");

   int i;
   const int qtd_params = (qtd - 3) / 3;

   Function *f = getFunction(index_function);

   if(qtd_params != f->parameterCount)
      error("Invalid parameters amount");

   for(i = 0; i < qtd_params; i++) {

      if(category[i] != 'c' && category[i] != 'v' && category[i] != 'p')
         error("Invalid category in function call");

      if(category[i] == 'c' && type[i] != 'i')
         error("Invalid const type in function call");

      if(type[i] != 'i' && type[i] != 'a')
         error("Invalid type in function call");

      if(f->parameters[i].type == INT && type[i] != 'i')
         error("Incompatible parameter");

      if(f->parameters[i].type == VET && (type[i] != 'a' || category[i] == 'c'))
         error("Incompatible parameter");

   }

}

void callFunction() {

   char type_destiny = '\0';
   int index_destiny, index_function;

   char category[3];
   char type[3];
   int index[3];

   int r = sscanf(
      BUFFER,
      "%ci%d = call f%d %c%c%d %c%c%d %c%c%d",
      &type_destiny,
      &index_destiny,
      &index_function,
      &category[0],
      &type[0],
      &index[0],
      &category[1],
      &type[1],
      &index[1],
      &category[2],
      &type[2],
      &index[2]
   );

   verifyCallFunction(r, type_destiny, index_function, category, type);

   saveParameters();
   fprintf(F_OUTPUT, "\n");

   passParameters(index_function, category, type, index);
   fprintf(F_OUTPUT, "\n");

   fprintf(F_OUTPUT, "call f%d\n\n", index_function);

   restoreParameters();
   fprintf(F_OUTPUT, "\n");

   assignmentFromReturn(type_destiny, index_destiny);

}



void verifyReturn(char type) {

   if(type != 'c' && type != 'v' && type != 'p')
      error("Invalid type in return function");

}

void returnFunction() {

   char type;
   int index;

   sscanf(BUFFER, "return %ci%d", &type, &index);

   verifyReturn(type);

   Register *rax = getRegister("rax", CALLER_SAVED);
   Variable *v = NULL;
   Parameter *p = NULL;

   switch (type)
   {
   case 'c':
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index, rax->name32);
      break;

   case 'v':
      v = getVariable(index);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", v->stackPosition, rax->name32);
      break;

   case 'p':
      p = getParameter(index);
      fprintf(F_OUTPUT, "movl %%%s, %%%s\n", p->reg->name32, rax->name32);
      break;
   
   }

   freeRegister(&rax);

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