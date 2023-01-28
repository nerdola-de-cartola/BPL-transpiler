#include "variables.h"

Variable VARIABLES[MAX_VARIABLE];

extern FILE *F_OUTPUT;
extern char BUFFER[MAX_LINE_SIZE];

void verifyLocalVariables(int qtd, char c1, char c2, char c3, int index)
{

   if(qtd != 4 && qtd != 5)
      error("Invalid read in local variables");

   if (index > 5 || index < 1)
      error("invalid type in localVariables");

   if (c1 == 'a') // var vi1
   {
      if (c2 != 'r' || c3 != 'i')
         error("invalid type in localVariables");
   }
   else if (c1 == 'e') // vet va1
   {
      if (c2 != 't' || c3 != 'a' || !strInStr(BUFFER, "size ci"))
         error("invalid type in localVariables");
   }
}

void clearVARIABLES()
{
   int i;
   Variable *var;

   for (i = 0; i < MAX_VARIABLE; i++)
   {
      var = &VARIABLES[i];

      var->size = 0;
      var->stackPosition = 0;
      var->type = -1;
   }
}



int localVariables()
{

   Function *f = getFunction(CURRENT_FUNCTION_INDEX);
   Variable *var = NULL;

   int r;
   char varORvet, filler1, filler2;
   int vetSize;
   int lastStackPos = 0;
   int index;

   while (true)
   {
      readNewLine();

      r = sscanf(
          BUFFER,
          "v%c%c v%c%d size ci%d",
          &varORvet,
          &filler1,
          &filler2,
          &index,
          &vetSize); // se r = 4 variável (int) se r = 5 variável (int vetor)

      if (strcmp(BUFFER, "enddef") == 0)
         break;

      verifyLocalVariables(r, varORvet, filler1, filler2, index);

      f->variableCount++;

      var = getVariable(index);

      if (r == 4) /* Se for Variável inteira */
      {
         var->size = 4;
         var->type = INT;

         while (lastStackPos % 4 != 0)
            lastStackPos++;

         var->stackPosition = lastStackPos + 4;
      }
      else if (r == 5) /* Se for Vetor de inteiros */
      {
         var->size = 4 * vetSize;
         var->type = VET;

         while (lastStackPos % 4 != 0)
            lastStackPos++;

         var->stackPosition = lastStackPos + var->size;
      }

      lastStackPos = var->stackPosition;

      printLocalVariables(index);
   }

   return lastStackPos;
}

void printLocalVariables(int index)
{

   Variable *var = getVariable(index);

   if (var->type == INT)
   { /* Se for Variável inteira */
      fprintf(F_OUTPUT, "# vi%d: -%d\n", index, var->stackPosition);
   }
   else if (var->type == VET)
   { /* Se for Vetor de inteiros */
      fprintf(F_OUTPUT, "# va%d: -%d\n", index, var->stackPosition);
   }
}

Variable *getVariable(int index)
{

   Function *f = getFunction(CURRENT_FUNCTION_INDEX);

   if (index < 1 || index > f->variableCount)
      error("Invalid index of variable");

   return &VARIABLES[index - 1];
}
