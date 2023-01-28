#include "assignment.h"

void simpleAssignment(int index_destiny, int index_source, char type_destiny, char type_source)
{

   Variable *source_v = NULL;
   Parameter *source_p = NULL;
   Register *tmp = NULL;

   if(type_destiny == 'v') {

      Variable *destiny = getVariable(index_destiny);

      switch (type_source)
      {
      case 'c':
         fprintf(F_OUTPUT, "movl $%d, -%d(%%rbp)\n", index_source, destiny->stackPosition);
         break;
      
      case 'v':
         tmp = getRegister(NULL, CALLER_SAVED);
         source_v = getVariable(index_source);

         fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", source_v->stackPosition, tmp->name32);

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", tmp->name32, destiny->stackPosition);

         freeRegister(&tmp);

         break;

      case 'p':
         source_p = getParameter(index_source);

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", source_p->reg->name32 , destiny->stackPosition);

         break;

      }

      return;
   } else {

      Parameter *destiny = getParameter(index_destiny);

      switch (type_source)
      {
      case 'c':
         fprintf(F_OUTPUT, "movl $%d, %%%s\n", index_source, destiny->reg->name32);
         break;
      
      case 'v':
         source_v = getVariable(index_source);

         fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", source_v->stackPosition, destiny->reg->name32);

         break;

      case 'p':
         source_p = getParameter(index_source);

         fprintf(F_OUTPUT, "movl %%%s, %%%s\n", source_p->reg->name32 , destiny->reg->name32);

         break;

      }

   }

}

bool isOperator(char op) {

   if(op != '+' && op != '-' && op != '*' && op != '/') return false;
   return true;

}

void verifyAssignment(
   int qtd_args,
   char op,
   char type_destiny,
   char type_source1,
   char type_source2)
{

   // Verifica a quantidade de parâmetros encontrados
   if(qtd_args != 4 && qtd_args != 7)
      error("Invalid assignment type");

   // Verifica o tipo do destino
   if(type_destiny != 'v' && type_destiny != 'p')
      error("Invalid destiny in assignment");

   // Verifica a primeira fonte
   if(type_source1 != 'v' && type_source1 != 'p' && type_source1 != 'c')
      error("Invalid first member in operation");


   // Se for uma operação
   if(qtd_args == 7) {

      // Verifica o operador
      if(!isOperator(op))
         error("Invalid operator");

      // Verifica a segunda fonte
      if(type_source2 != 'v' && type_source2 != 'p' && type_source2 != 'c')
         error("Invalid first member in operation");

   }

}

void assignment()
{
   int r;
   char type_destiny, type_source1, type_source2;
   int index_destiny, index_source1, index_source2;
   char operand;

   r = sscanf(
       BUFFER,
       "%ci%d = %ci%i %c %ci%d",
       &type_destiny,
       &index_destiny,
       &type_source1,
       &index_source1,
       &operand,
       &type_source2,
       &index_source2);

   
   verifyAssignment(
      r,
      operand,
      type_destiny,
      type_source1,
      type_source2
   );

   if (r == 4)
   {
      simpleAssignment(index_destiny, index_source1, type_destiny, type_source1);
      return;
   }
   
   Register *reg;
   Variable *destiny = getVariable(index_destiny);

   reg = operation(operand, type_source1, index_source1, type_source2, index_source2);

   fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, destiny->stackPosition);

   freeRegister(&reg);

}


Register *operation(char op, char type1, int index1, char type2, int index2)
{
   Register *r = NULL;
   Variable *v = NULL;
   Parameter *p = NULL;

   char operation[6];
   setOperation(op, operation);

   if(op == '/') {
      r = divi(type1, index1, type2, index2);
      return r;
   }

   r = getRegister(NULL, CALLER_SAVED);

   switch (type1)
   {
   case 'c':
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, r->name32);
      break;

   case 'v':
      v = getVariable(index1);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", v->stackPosition, r->name32);
      break;

   case 'p':
      p = getParameter(index1);
      fprintf(F_OUTPUT, "movl %%%s, %%%s\n", p->reg->name32, r->name32);
      break;
   }

   switch (type2)
   {
   case 'c':
      fprintf(F_OUTPUT, "%s $%d, %%%s\n", operation, index2, r->name32);
      break;
   
   case 'v':
      v = getVariable(index2);
      fprintf(F_OUTPUT, "%s -%d(%%rbp), %%%s\n", operation, v->stackPosition, r->name32);
      break;
   
   case 'p':
      p = getParameter(index2);
      fprintf(F_OUTPUT, "%s %%%s, %%%s\n", operation, p->reg->name32, r->name32);
      break;
   
   }

   return r;
}


Register *divi(char type1, int index1, char type2, int index2)
{
   Register *rax = getRegister("rax", CALLER_SAVED);
   Register *r_tmp = getRegister(NULL, CALLER_SAVED);
   Variable *v = NULL;
   Parameter *p = NULL;

   switch (type1)
   {
   case 'c':
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, rax->name32);
      break;
   
   case 'v':
      v = getVariable(index1);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", v->stackPosition, rax->name32);

   case 'p':
      p = getParameter(index1);
      fprintf(F_OUTPUT, "movl %%%s, %%%s\n", p->reg->name32, rax->name32);

   }

   
   switch (type2)
   {
   case 'c':
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index2, r_tmp->name32);
      break;
   
   case 'v':
      v = getVariable(index2);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", v->stackPosition, r_tmp->name32);
      break;

   case 'p':
      p = getParameter(index2);
      fprintf(F_OUTPUT, "movl %%%s, %%%s\n", p->reg->name32, r_tmp->name32);
      break;

   }

   fprintf(F_OUTPUT, "cltd\n");
   fprintf(F_OUTPUT, "idiv %%%s\n", r_tmp->name32);

   freeRegister(&r_tmp);

   return rax;
}

void setOperation(char op, char *operation) {
   switch (op)
   {
   case '+':
      strcpy(operation, "addl");
      break;

   case '-':
      strcpy(operation, "subl");
      break;

   case '*':
      strcpy(operation, "imull");
      break;

   case '/':
      strcpy(operation, "idivl");
      break;
   
   }
}