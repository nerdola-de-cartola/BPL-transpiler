#include "array.h"

//==============================================================================
// Acesso a Array - Início
//==============================================================================

void verifyArrayAccess(int r, char c1, int id1, int index, char c2, char c3, int id2)
{
   if (index < 0)
      error("invalid type in ArrayAccess Functions");

   if (r != 6)
      error("function 'sscanf' didn't perform as expected in ArrayAccess Functions");

   if (c1 == 'v')
   {
      if (id1 < 1 || id1 > 5)
         error("invalid type in ArrayAccess Functions");
   }
   else if (c1 == 'p')
   {
      // Parte referente a parâmetros
   }
   else
      error("invalid type in ArrayAccess Functions");

   if (c2 == 'v')
   {
      if (c3 != 'i' && c3 != 'a')
         error("invalid type in ArrayAccess Functions");
      if (id2 > 5 || id2 < 1)
         error("invalid type in ArrayAccess Functions");
   }
   else if (c2 == 'c')
   {
      if (strInStr(BUFFER, "get") || id2 < 0)
         error("invalid type in ArrayAccess Functions");
   }
   else if (c2 == 'p')
   {
      // Parte referente a parâmetros
   }
   else
      error("invalid type in ArrayAccess Functions");
}

void arrayAccess()
{
   int r;
   char parORvet, parORvarORconst, filler;
   int identifier1, identifier2, index;

   r = sscanf(
       BUFFER,
       "%*s %ca%d index ci%d %*s %c%c%d",
       &parORvet,
       &identifier1,
       &index,
       &parORvarORconst,
       &filler,
       &identifier2);

   verifyArrayAccess(r, parORvet, identifier1, index, parORvarORconst, filler, identifier2);

   if (parORvet == 'v') // Vetor de Inteiros
   {
      Variable *vet = getVariable(identifier1);

      Register *r1 = getRegister(NULL, CALLER_SAVED);

      Register *r2 = getRegister(NULL, CALLER_SAVED);

      fprintf(F_OUTPUT, "leaq -%d(%%rbp), %%%s\n", vet->stackPosition, r1->name64);

      fprintf(F_OUTPUT, "movq $%d, %%%s\n", index, r2->name64);

      fprintf(F_OUTPUT, "imulq $4, %%%s\n", r2->name64);

      fprintf(F_OUTPUT, "addq %%%s, %%%s\n", r1->name64, r2->name64);

      if (strInStr(BUFFER, "get"))
      {
         arrayAccessGet(r2, parORvarORconst, identifier2);
      }
      else if (strInStr(BUFFER, "set"))
      {
         arrayAccessSet(r2, parORvarORconst, identifier2);
      }
      else
      {
         error("Invalid array access");
      }

      freeRegister(&r1);
      freeRegister(&r2);
   }
   /*else if(parORvet == 'p') // Parâmetro Array
   {
       // Quando a parte de parâmetros estiver pronta;
   }*/
}

void arrayAccessGet(Register *r, char type, int index)
{
   if (type == 'v')
   {
      Variable *var = getVariable(index);

      fprintf(F_OUTPUT, "movl (%%%s), %%%s\n", r->name64, r->name32);
      fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", r->name32, var->stackPosition);
   }
   else
   {
      error("Invalid type in array get");
   }
   /*else if(type == 'p'){
   // Quando a parte de parâmetros estiver pronta;
   }*/
}

void arrayAccessSet(Register *r, char type, int index)
{
   if (type == 'v')
   {
      Variable *var = getVariable(index);
      Register *tmp = getRegister(NULL, CALLER_SAVED);

      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", var->stackPosition, tmp->name32);
      fprintf(F_OUTPUT, "movl %%%s, (%%%s)\n", tmp->name32, r->name64);
   }
   else if (type == 'c')
   {
      fprintf(F_OUTPUT, "movl $%d, (%%%s)\n", index, r->name64);
   }
   else
   {
      error("Invalid type in array set");
   }
   /*else if(type == 'p'){
   // Quando a parte de parâmetros estiver pronta;
   }*/
}

//==============================================================================
// Acesso a Array - Fim
//==============================================================================


