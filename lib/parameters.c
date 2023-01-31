#include "../include/parameters.h"

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

void saveParameters() {

   Function *f = getFunction(CURRENT_FUNCTION_INDEX);
   Parameter *p = NULL;
   int i;

   if(f->parameterCount > 0)
      fprintf(F_OUTPUT, "# salvando registradores caller saved\n");

   for(i = 1; i <= f->parameterCount; i++) {

      p = getParameter(i);

      if(p->type == INT)
         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", p->reg->name32, p->stackPosition);
      else
         fprintf(F_OUTPUT, "movq %%%s, -%d(%%rbp)\n", p->reg->name64, p->stackPosition);

      freeRegister(&p->reg);

   }

}

void restoreParameters() {
   Function *f = getFunction(CURRENT_FUNCTION_INDEX);
   Parameter *p = NULL;
   int i;

   if(f->parameterCount > 0)
      fprintf(F_OUTPUT, "# recuperando registradores caller saved\n");

   for(i = 1; i <= f->parameterCount; i++) {

      p = getParameter(i);
      p->reg = getRegister(NULL, CALLER_SAVED);

      printParam(p, i);

      if(p->type == INT) {
         fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", p->stackPosition, p->reg->name32);
      } else {
         fprintf(F_OUTPUT, "movq -%d(%%rbp), %%%s\n", p->stackPosition, p->reg->name64);
      }

   }

}

void passParameters(int qtd, char category[3], char type[3], int index[3]) {

   Variable *v = NULL;
   Parameter *p = NULL;
   char reg_name32[4];
   char reg_name64[4];
   int i;

   if(qtd > 0)
      fprintf(F_OUTPUT, "# passando parâmetros para função\n");

   for(i = 0; i < qtd; i++) {

      registerName(i+1, reg_name32, reg_name64);

      if(category[i] == 'c') {
         fprintf(F_OUTPUT, "movl $%d, %%%s\n", index[i], reg_name32);
      }
      else if(category[i] == 'v') {

         v = getVariable(index[i]);

         if(type[i] == 'i')
            fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", v->stackPosition, reg_name32);
         else
            fprintf(F_OUTPUT, "leaq -%d(%%rbp), %%%s\n", v->stackPosition, reg_name64);

      }
      else {

         p = getParameter(index[i]);

         if(type[i] == 'i')
            fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", p->stackPosition, reg_name32);
         else
            fprintf(F_OUTPUT, "leaq -%d(%%rbp), %%%s\n", p->stackPosition, reg_name64);

      }

   }


}