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
   { 
      Parameter *p = getParameter(index);
      fprintf(F_OUTPUT, "cmpl $0, %%%s\n", p->reg->name32);
   }

   fprintf(F_OUTPUT, "je .if%d\n", IF_INDEX);

   readNewLine();

   if (charInStr('=', BUFFER)) // atribuição
      assignment();
   else if (strInStr(BUFFER, "index")) // acesso a array
      arrayAccess();
   else if(strInStr(BUFFER, "return")) // retorno de função
      returnFunction();
   else
      error("Invalid instruction in if statement");

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

   if(c1 != 'c' && c1 != 'v' && c1 != 'p')
      error("Invalid type in if statement");

}