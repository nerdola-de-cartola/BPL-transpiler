#include "variables.h"

Variable VARIABLES[MAX_VARIABLE];

extern FILE *F_OUTPUT;
extern char BUFFER[MAX_LINE_SIZE];

void verifyLocalVariables(char c1, char c2, char c3, int index)
{

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

   for (i = 1; i <= MAX_VARIABLE; i++)
   {
      var = getVariable(i);

      var->size = 0;
      var->stackPosition = 0;
      var->type = -1;
   }
}



int localVariables()
{

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

      verifyLocalVariables(varORvet, filler1, filler2, index);

      if (r == 4) /* Se for Variável inteira */
      {
         VARIABLES[index - 1].size = 4;
         VARIABLES[index - 1].type = INT;

         while (lastStackPos % 4 != 0)
            lastStackPos++;

         VARIABLES[index - 1].stackPosition = lastStackPos + 4;
      }
      else if (r == 5) /* Se for Vetor de inteiros */
      {
         VARIABLES[index - 1].size = 4 * vetSize;
         VARIABLES[index - 1].type = VET;

         while (lastStackPos % 4 != 0)
            lastStackPos++;

         VARIABLES[index - 1].stackPosition = lastStackPos + VARIABLES[index - 1].size;
      }
      else
      {
         error("Invalid local variable");
      }

      lastStackPos = VARIABLES[index - 1].stackPosition;

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
   if (index < 1 || index > 5)
      error("Invalid index of variable");

   return &VARIABLES[index - 1];
}
