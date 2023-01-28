#include "conditions.h"

int IF_INDEX;

void ifStatement()
{

   int index;
   char c1, c2;

   sscanf(BUFFER, "if %c%c%d", &c1, &c2, &index);

   verifyIfStatement(c1, c2, index);

   if (c1 == 'v')
   { // variável local
      Variable *var = getVariable(index);
      fprintf(F_OUTPUT, "cmpl $0, -%d(%%rbp)\n", var->stackPosition);
   }
   else if (c1 == 'c')
   { // constante
      fprintf(F_OUTPUT, "cmpl $0, $%d\n", index);
   }
   else
   { // parâmetro
   }

   fprintf(F_OUTPUT, "je .if%d\n", IF_INDEX);

   readNewLine();

   if (charInStr('=', BUFFER)) // atribuição
      assignment();
   // acesso a array
   // retorno de função

   readNewLine();

   if (strcmp(BUFFER, "endif") != 0)
      error("Missing endif statement");

   fprintf(F_OUTPUT, ".if%d:\n", IF_INDEX);
   IF_INDEX++;
}



void verifyIfStatement(char c1, char c2, int index)
{

   if (c2 != 'i')
      error("Invalid type in if statement");

   if (c1 == 'v')
   { // if vi1
      if (index < 1 || index > 5)
         error("Invalid type in if statement");
   }
   else if (c1 == 'p')
   { // if pi1
      if (index < 1 || index > 3)
         error("Invalid type in if statement");
   }
   else
   {
      error("Invalid type in if statement");
   }
}