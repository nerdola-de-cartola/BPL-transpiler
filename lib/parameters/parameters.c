#include "parameters.h"

void verifyParams(int order, char type1, char type2, char type3, int paramQtd)
{
   if (order < 1 || order > MAX_FUNCTION)
      error("Invalid function index");

   if (paramQtd >= 1)
      if (type1 != 'i' && type1 != 'a')
         error("Invalid param type");

   if (paramQtd >= 2)
      if (type2 != 'i' && type2 != 'a')
         error("Invalid param type");

   if (paramQtd == 3)
      if (type3 != 'i' && type3 != 'a')
         error("Invalid param type");
}


void printParam(Parameter *p, int index)
{

   if (p->type == INT)
      fprintf(F_OUTPUT, "# pi%d -> %%%s | -%d(%%rbp)\n", index, p->reg->name32, p->stackPosition);
   else
      fprintf(F_OUTPUT, "# pa%d -> %%%s | -%d(%%rbp)\n", index, p->reg->name64, p->stackPosition);
}

int paramDefinition(int stackSize)
{

   Function *f = getFunction(CURRENT_FUNCTION_INDEX); 
   int i;

   for (i = 0; i < f->parameterCount; i++)
   {
      if (f->parameters[i].type == INT)
      {
         stackSize += 4;
         while (stackSize % 4 != 0)
            stackSize++;
      }
      else if (f->parameters[i].type == VET)
      {
         stackSize += 8;
         while (stackSize % 8 != 0)
            stackSize++;
      }

      f->parameters[i].stackPosition = stackSize;

      printParam(&f->parameters[i], i + 1);
   }

   return stackSize;
}


Parameter *getParameter(int index) {

   Function *f = getFunction(CURRENT_FUNCTION_INDEX); 

   if(index < 1 || index > f->parameterCount)
      error("Invalid index of parameter");

   return &f->parameters[index - 1];
}

