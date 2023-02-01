#include "../include/array.h"

//==============================================================================
// Acesso a Array - Início
//==============================================================================

void verifyArrayAccess(int r, char c1, char c2, char c3)
{
   if (r != 6)
      error("function 'sscanf' didn't perform as expected in ArrayAccess Functions");

   if(c1 != 'v' && c1 != 'p')
      error("Invalid array");
   
   if(c2 != 'v' && c2 != 'p' && c2 != 'c')
      error("Invalid assignment with array");

   if(c3 != 'i')
      error("Invalid assignment with array");

   if(strInStr(BUFFER, "get") && c2 == 'c')
      error("Invalid assignment to const in array access");
}


void arrayAccess()
{
   int r;
   char parORvet, parORvarORconst, filler;
   int identifier1, identifier2, index;

   // Leitura do Comando e obtenção das Variáveis
   r = sscanf(
       BUFFER,
       "%*s %ca%d index ci%d %*s %c%c%d",
       &parORvet,
       &identifier1,
       &index,
       &parORvarORconst,
       &filler,
       &identifier2);

   verifyArrayAccess(r, parORvet, parORvarORconst, filler);

   // Declaração dos registradores que serão utilizados no código Assembly
   Register *r1 = getRegister(NULL, CALLER_SAVED);

   Register *r2 = getRegister(NULL, CALLER_SAVED);

   if (parORvet == 'v') // Vetor de Inteiros
   {
      Variable *vet = getVariable(identifier1);

      if(vet->type != VET)
         error("Invalid variable for array access");

      fprintf(F_OUTPUT, "leaq -%d(%%rbp), %%%s\n", vet->stackPosition, r1->name64);
   }
   else if(parORvet == 'p') // Parâmetro Array
   {
      Parameter *parA = getParameter(identifier1);

      if(parA->type != VET)
         error("Invalid variable for array access");

      fprintf(F_OUTPUT, "leaq (%%%s), %%%s\n", parA->reg->name64, r1->name64);
   }

   fprintf(F_OUTPUT, "movq $%d, %%%s\n", index, r2->name64);

   fprintf(F_OUTPUT, "imulq $4, %%%s\n", r2->name64);

   fprintf(F_OUTPUT, "addq %%%s, %%%s\n", r1->name64, r2->name64);

   if (strInStr(BUFFER, "get")){ // Em caso de Comando "get"
      arrayAccessGet(r2, parORvarORconst, identifier2);
   }
   else if (strInStr(BUFFER, "set")){ // Em caso de Comando "set"
      arrayAccessSet(r2, parORvarORconst, identifier2);
   }
   else error("Invalid array access"); 

   freeRegister(&r1);
   freeRegister(&r2);
}


void arrayAccessGet(Register *r, char type, int index)
{
   if (type == 'v') // Tipo Variável (int)
   {
      Variable *var = getVariable(index);

      fprintf(F_OUTPUT, "movl (%%%s), %%%s\n", r->name64, r->name32);
      fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", r->name32, var->stackPosition);
   }
   else if(type == 'p'){ // Tipo Parâmetro (int)
      Parameter *parI = getParameter(index);

      fprintf(F_OUTPUT, "movl (%%%s), %%%s\n", r->name64, parI->reg->name32);
   }
   else
   {
      error("Invalid type in array get");
   }
}


void arrayAccessSet(Register *r, char type, int index)
{
   if (type == 'v') // Tipo Variável (int)
   {
      Variable *var = getVariable(index);
      Register *tmp = getRegister(NULL, CALLER_SAVED);

      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", var->stackPosition, tmp->name32);
      fprintf(F_OUTPUT, "movl %%%s, (%%%s)\n", tmp->name32, r->name64);
   }
   else if (type == 'c') // Tipo Constante 
   {
      fprintf(F_OUTPUT, "movl $%d, (%%%s)\n", index, r->name64);
   }
   else if(type == 'p') // Tipo Parâmetro (int)
   {
      Parameter *parI = getParameter(index);

      fprintf(F_OUTPUT, "movl %%%s, (%%%s)\n", parI->reg->name32, r->name64);
   }
   else
   {
      error("Invalid type in array set");
   }
}

//==============================================================================
// Acesso a Array - Fim
//==============================================================================