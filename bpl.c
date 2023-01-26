#include "bpl.h"

//==============================================================================
// Variáveis Globais
//==============================================================================

Register REGISTERS[MAX_REGISTER];
Variable VARIABLES[MAX_VARIABLE];
FILE *F_SOURCE;
FILE *F_OUTPUT;
int LINE_COUNT, IF_INDEX;
char BUFFER[MAX_LINE_SIZE];

//==============================================================================
// Função Main
//==============================================================================

int main(int argc, char **argv)
{

   if (argc != 3)
   {
      printf("Quantidade de argumentos inválida\n");
      printf("Modo de uso: bpl.exe <seu_códio.bpl> <codigo_traduzido.s>\n");
      return 0;
   }

   F_SOURCE = fopen(argv[1], "rt");
   if (F_SOURCE == NULL)
   {
      printf("impossible to read file\n");
      return 1;
   }

   F_OUTPUT = fopen(argv[2], "wt");
   if (F_SOURCE == NULL)
   {
      printf("impossible to write file\n");
      return 1;
   }

   LINE_COUNT = 0;
   IF_INDEX = 0;
   registersInit();

   while (readNewLine() != NULL)
   {

      if (strcmp(BUFFER, "def") == 0)
         localVariables();
      else if (charInStr('=', BUFFER))
         assignment();
      else if (strInStr(BUFFER, "index"))
         arrayAccess();
      else if (strInStr(BUFFER, "if"))
         ifStatement();

      fprintf(F_OUTPUT, "\n");
   }

   writeMain();

   fclose(F_SOURCE);
   fclose(F_OUTPUT);

   return 0;
}

//==============================================================================
// Código das Funções Auxiliares
//==============================================================================

void remove_newline(char *ptr)
{
   while (*ptr)
   {
      if (*ptr == '\n' || *ptr == '\r')
         *ptr = 0;
      else
         ptr++;
   }
}

bool strInStr(char *string, char *substring)
{
   int i, j;

   for (i = 0, j = 0; string[i] != '\0'; i++)
   {

      if (string[i] == substring[j])
         j++;

      if (substring[j] == '\0')
         return true;
   }

   return false;
}

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

void localVariables()
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
         VARIABLES[index - 1].type = VET_INT;

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

   subq(lastStackPos);
}

void printLocalVariables(int index)
{

   Variable *var = getVariable(index);

   if (var->type == INT)
   { /* Se for Variável inteira */
      fprintf(F_OUTPUT, "# vi%d: -%d\n", index, var->stackPosition);
   }
   else if (var->type == VET_INT)
   { /* Se for Vetor de inteiros */
      fprintf(F_OUTPUT, "# va%d: -%d\n", index, var->stackPosition);
   }
}

void subq(int lastStackPos)
{
   while (lastStackPos % 16 != 0)
      lastStackPos++;

   fprintf(F_OUTPUT, "subq $%d, %%rsp\n", lastStackPos);
}

void writeMain()
{
   fprintf(F_OUTPUT,
           ".text\n"
           "call .f1\n");
}

bool charInStr(const char c, const char *str)
{
   while (*str)
   {
      if (c == *str)
         return true;
      str++;
   }

   return false;
}

void simpleAssignment(int index_destiny, int index_source, char type_source)
{

   Variable *destiny = getVariable(index_destiny);

   if (type_source == 'c')
      fprintf(
          F_OUTPUT,
          "movl $%d, -%d(%%rbp)\n",
          index_source,
          destiny->stackPosition);
   else
   {

      Variable *source = getVariable(index_source);

      fprintf(
          F_OUTPUT,
          "movl -%d(%%rbp), -%d(%%rbp)\n",
          source->stackPosition,
          destiny->stackPosition);
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

   if (r != 4 && r != 7)
      error("invalid assignment");

   if (r == 4)
   {

      if (
          (type_destiny != 'v') ||
          (type_source1 != 'v' && type_source1 != 'c'))
         error("invalid type in assignment");

      simpleAssignment(index_destiny, index_source1, type_source1);
   }
   else
   {
      Register *reg;
      Variable *destiny = getVariable(index_destiny);

      if (
          (type_destiny != 'v') ||
          (type_source1 != 'v' && type_source1 != 'c') ||
          (type_source2 != 'v' && type_source2 != 'c'))
         error("invalid type in assignment");

      switch (operand)
      {
      case '+':
         reg = add(type_source1, index_source1, type_source2, index_source2);

         if (reg == NULL)
            error("no register available");

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, destiny->stackPosition);
         freeRegister(&reg);
         break;

      case '-':
         reg = sub(type_source1, index_source1, type_source2, index_source2);

         if (reg == NULL)
            error("no register available");

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, destiny->stackPosition);
         freeRegister(&reg);
         break;

      case '*':
         reg = mul(type_source1, index_source1, type_source2, index_source2);

         if (reg == NULL)
            error("no register available");

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, destiny->stackPosition);
         freeRegister(&reg);
         break;

      case '/':
         reg = divi(type_source1, index_source1, type_source2, index_source2);

         if (reg == NULL)
            error("no register available");

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, destiny->stackPosition);
         freeRegister(&reg);
         break;
      default:
         error("invalid operand in assignment");
         break;
      }
   }
}

void error(const char *error_type)
{
   printf(
       "ERROR\n"
       "Line %d\n"
       "Type: %s\n",
       LINE_COUNT,
       error_type);

   fclose(F_SOURCE);
   fclose(F_OUTPUT);
   exit(1);
}

void registersInit()
{
   REGISTERS[0].free = true;
   REGISTERS[0].type = CALLER_SAVED;
   strcpy(REGISTERS[0].name32, "eax");
   strcpy(REGISTERS[0].name64, "rax");

   REGISTERS[1].free = true;
   REGISTERS[1].type = CALLEE_SAVED;
   strcpy(REGISTERS[1].name32, "ebx");
   strcpy(REGISTERS[1].name64, "rbx");

   REGISTERS[2].free = true;
   REGISTERS[2].type = CALLER_SAVED;
   strcpy(REGISTERS[2].name32, "ecx");
   strcpy(REGISTERS[2].name64, "rcx");

   REGISTERS[3].free = true;
   REGISTERS[3].type = CALLER_SAVED;
   strcpy(REGISTERS[3].name32, "edx");
   strcpy(REGISTERS[3].name64, "rdx");

   REGISTERS[4].free = true;
   REGISTERS[4].type = CALLER_SAVED;
   strcpy(REGISTERS[4].name32, "esi");
   strcpy(REGISTERS[4].name64, "rsi");

   REGISTERS[5].free = true;
   REGISTERS[5].type = CALLER_SAVED;
   strcpy(REGISTERS[5].name32, "edi");
   strcpy(REGISTERS[5].name64, "rdi");

   REGISTERS[6].free = false;
   REGISTERS[6].type = CALLER_SAVED;
   strcpy(REGISTERS[6].name32, "ebp");
   strcpy(REGISTERS[6].name64, "rbp");

   REGISTERS[7].free = false;
   REGISTERS[7].type = CALLER_SAVED;
   strcpy(REGISTERS[7].name32, "esp");
   strcpy(REGISTERS[7].name64, "rsp");

   REGISTERS[8].free = true;
   REGISTERS[8].type = CALLER_SAVED;
   strcpy(REGISTERS[8].name32, "r8d");
   strcpy(REGISTERS[8].name64, "r9");

   REGISTERS[9].free = true;
   REGISTERS[9].type = CALLER_SAVED;
   strcpy(REGISTERS[9].name32, "r9d");
   strcpy(REGISTERS[9].name64, "r9");

   REGISTERS[10].free = true;
   REGISTERS[10].type = CALLER_SAVED;
   strcpy(REGISTERS[10].name32, "r10d");
   strcpy(REGISTERS[10].name64, "r10");

   REGISTERS[11].free = true;
   REGISTERS[11].type = CALLER_SAVED;
   strcpy(REGISTERS[11].name32, "r11d");
   strcpy(REGISTERS[11].name64, "r11");

   REGISTERS[12].free = true;
   REGISTERS[12].type = CALLEE_SAVED;
   strcpy(REGISTERS[12].name32, "r12d");
   strcpy(REGISTERS[12].name64, "r12");

   REGISTERS[13].free = true;
   REGISTERS[13].type = CALLEE_SAVED;
   strcpy(REGISTERS[13].name32, "r13d");
   strcpy(REGISTERS[13].name64, "r13");

   REGISTERS[14].free = true;
   REGISTERS[14].type = CALLEE_SAVED;
   strcpy(REGISTERS[14].name32, "r14d");
   strcpy(REGISTERS[14].name64, "r14");

   REGISTERS[15].free = true;
   REGISTERS[15].type = CALLEE_SAVED;
   strcpy(REGISTERS[15].name32, "r15d");
   strcpy(REGISTERS[15].name64, "r15");
}

Register *add(char type1, int index1, char type2, int index2)
{
   Register *r = getRegister(NULL, CALLER_SAVED);

   if (type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, r->name32);
   else
   {
      Variable *var1 = getVariable(index1);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", var1->stackPosition, r->name32);
   }

   if (type2 == 'c')
      fprintf(F_OUTPUT, "addl $%d, %%%s\n", index2, r->name32);
   else
   {
      Variable *var2 = getVariable(index2);
      fprintf(F_OUTPUT, "addl -%d(%%rbp), %%%s\n", var2->stackPosition, r->name32);
   }

   return r;
}

Register *sub(char type1, int index1, char type2, int index2)
{
   Register *r = getRegister(NULL, CALLER_SAVED);

   if (type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, r->name32);
   else
   {
      Variable *var1 = getVariable(index1);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", var1->stackPosition, r->name32);
   }

   if (type2 == 'c')
      fprintf(F_OUTPUT, "subl $%d, %%%s\n", index2, r->name32);
   else
   {
      Variable *var2 = getVariable(index2);
      fprintf(F_OUTPUT, "subl -%d(%%rbp), %%%s\n", var2->stackPosition, r->name32);
   }

   return r;
}

Register *mul(char type1, int index1, char type2, int index2)
{
   Register *r = getRegister(NULL, CALLER_SAVED);

   if (type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, r->name32);
   else
   {
      Variable *var1 = getVariable(index1);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", var1->stackPosition, r->name32);
   }

   if (type2 == 'c')
      fprintf(F_OUTPUT, "imull $%d, %%%s\n", index2, r->name32);
   else
   {
      Variable *var2 = getVariable(index2);
      fprintf(F_OUTPUT, "imull -%d(%%rbp), %%%s\n", var2->stackPosition, r->name32);
   }

   return r;
}

Register *getRegister(char *name64, int type)
{
   int i;

   for (i = 0; i < MAX_REGISTER; i++)
   {
      if (REGISTERS[i].free && REGISTERS[i].type == type)
      {
         if (
             name64 != NULL &&
             strcmp(REGISTERS[i].name64, name64) != 0)
            continue;

         REGISTERS[i].free = false;
         return &REGISTERS[i];
      }
   }

   return NULL;
}

void freeRegister(Register **r)
{
   (*r)->free = true;
   *r = NULL;
}

Register *divi(char type1, int index1, char type2, int index2)
{
   Register *rax = getRegister("rax", CALLER_SAVED);
   Register *r_tmp = getRegister(NULL, CALLER_SAVED);

   if (type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, rax->name32);
   else
   {
      Variable *var1 = getVariable(index1);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", var1->stackPosition, rax->name32);
   }

   if (type2 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index2, r_tmp->name32);
   else
   {
      Variable *var2 = getVariable(index2);
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", var2->stackPosition, r_tmp->name32);
   }

   fprintf(F_OUTPUT, "cltd\n");
   fprintf(F_OUTPUT, "idiv %%%s\n", r_tmp->name32);

   freeRegister(&r_tmp);

   return rax;
}

Variable *getVariable(int index)
{
   if (index < 1 || index > 5)
      error("Invalid index of variable");

   return &VARIABLES[index - 1];
}

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

char *readNewLine()
{
   char *ptr = fgets(BUFFER, MAX_LINE_SIZE, F_SOURCE);

   LINE_COUNT++;
   remove_newline(BUFFER);

   return ptr;
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
   else if (c1 == 'c')
   { // if ci1
      if (index < 0)
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