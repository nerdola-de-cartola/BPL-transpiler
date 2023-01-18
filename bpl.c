#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_SIZE 256

#define CALLER_SAVED 0
#define CALLEE_SAVED 1

#define MAX_REGISTER 16
#define MAX_VARIABLE 5

#define INT 0
#define VET_INT 1

typedef struct
{
   char name32[4];
   char name64[4];
   bool free;
   int type;
} Register;

// Mine
typedef struct
{
   int size;
   int stackPosition;
   int type;
} Variable;
//

//==============================================================================
// Variáveis Globais
//==============================================================================

Register REGISTERS[MAX_REGISTER];

// Mine
Variable VARIABLES[MAX_VARIABLE];

FILE *F_SOURCE;
FILE *F_OUTPUT;
int LINE_COUNT;
char BUFFER[MAX_LINE_SIZE];

//==============================================================================
// Declaração de Funções "Auxiliares"
//==============================================================================

void writeMain();

bool charInStr(const char c, const char *str);

void assignment();

void error(const char *error_type);

Register *add(char type1, int index1, char type2, int index2);

Register *sub(char type1, int index1, char type2, int index2);

Register *mul(char type1, int index1, char type2, int index2);

Register *divi(char type1, int index1, char type2, int index2);

Register *getRegister(char *name64, int type);

void freeRegister(Register **r);

void registersInit();

void localVariables();

void printLocalVariables(int index);

void subq(int lastStackPos);

void remove_newline(char *ptr);

void printLocalVariables(int index);

void subq(int lastStackPos);

//==============================================================================
// Função Main
//==============================================================================

int main(int argc, char **argv)
{

   if (argc != 3)
   {
      printf("Quantidade de argumentos inválida\n");
      printf("Modo de uso: bpl.exe <seu_códio.bpl> <codigo_traduzido.s>\n");
      // return 0;
   }

   F_SOURCE = fopen(argv[1], "rt");
   if(F_SOURCE == NULL) 
   {
      printf("impossible to read file\n");
      return 1;
   }

   F_OUTPUT = fopen(argv[2], "wt");
   if(F_SOURCE == NULL) 
   {
      printf("impossible to write file\n");
      return 1;
   }

   LINE_COUNT = 1;
   registersInit();

   while (fgets(BUFFER, MAX_LINE_SIZE, F_SOURCE) != NULL)
   {

      remove_newline(BUFFER);

      if (strcmp(BUFFER, "def") == 0)
         localVariables();
      else if (charInStr('=', BUFFER))
         assignment();

      fprintf(F_OUTPUT, "\n");
      LINE_COUNT++;
   }

   writeMain();

   fclose(F_SOURCE);
   fclose(F_OUTPUT);

   return 0;
}

//==============================================================================
// Código das Funções Auxiliares
//==============================================================================

// Remove o '\n' e o \r do fim da linha
void remove_newline(char *ptr)
{
  while (*ptr) {
    if (*ptr == '\n' || *ptr == '\r')
      *ptr = 0;
    else
      ptr++;
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
      fgets(BUFFER, MAX_LINE_SIZE, F_SOURCE);
      r = sscanf(
          BUFFER,
          "v%c%c v%c%d size ci%d",
          &varORvet,
          &filler1,
          &filler2,
          &index,
          &vetSize); // se r = 4 variável (int) se r = 5 variável (int vetor)

      // Condições de erro de "escrita" no arquivo .bpl
      if ((varORvet != 'a') && (varORvet != 'e'))
         error("invalid type in localVariables");

      if ((filler1 != 'r') && (filler1 != 't'))
         error("invalid type in localVariables");

      if ((filler2 != 'a') && (filler2 != 'i'))
         error("invalid type in localVariables");

      if (index > 5 || index < 1)
         error("invalid type in localVariables");

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
         break;
      }

      lastStackPos = VARIABLES[index - 1].stackPosition;

      printLocalVariables(index);
   }

   subq(lastStackPos);
}

void printLocalVariables(int index)
{
   if (VARIABLES[index - 1].type == INT)
   { /* Se for Variável inteira */
      fprintf(F_OUTPUT, "# vi%d: -%d\n", index, VARIABLES[index - 1].stackPosition);
   }
   else if (VARIABLES[index - 1].type == VET_INT)
   { /* Se for Vetor de inteiros */
      fprintf(F_OUTPUT, "# va%d: -%d\n", index, VARIABLES[index - 1].stackPosition);
   }
}

void subq(int lastStackPos)
{
   while(lastStackPos % 16 != 0)
      lastStackPos++;

   fprintf(F_OUTPUT, "subq $%d, %%rsp\n", lastStackPos);
}

void printLocalVariables(int index)
{
   if (VARIABLES[index - 1].type == INT)
   { /* Se for Variável inteira */
      fprintf(F_OUTPUT, "# vi%d: -%d\n", index, VARIABLES[index - 1].stackPosition);
   }
   else if (VARIABLES[index - 1].type == VET_INT)
   { /* Se for Vetor de inteiros */
      fprintf(F_OUTPUT, "# va%d: -%d\n", index, VARIABLES[index - 1].stackPosition);
   }
}

void subq(int lastStackPos)
{
   while(lastStackPos % 16 != 0)
      lastStackPos++;

   fprintf(F_OUTPUT, "subq $%d, %%rsp\n", lastStackPos);
}

//

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

void simpleAssignment(int index_destiny, int index_source, char type_source) {

   if(type_source == 'c') 
      fprintf(
         F_OUTPUT,
         "movl $%d, -%d(%%rbp)\n",
         index_source,
         VARIABLES[index_destiny-1].stackPosition
      );
   else
      fprintf(
         F_OUTPUT,
         "movl -%d(%%rbp), -%d(%%rbp)\n",
         VARIABLES[index_source-1].stackPosition,
         VARIABLES[index_destiny-1].stackPosition
      );
   
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
       &index_source2
      );

   if(r != 4 && r != 7)
      error("invalid assignment");

   if (r == 4) {

      if (
          (type_destiny != 'v') ||
          (type_source1 != 'v' && type_source1 != 'c')
      ) error("invalid type in assignment");

      simpleAssignment(index_destiny, index_source1, type_source1);

   }
   else
   {
      Register *reg;

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

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, VARIABLES[index_destiny-1].stackPosition);
         freeRegister(&reg);
         break;

      case '-':
         reg = sub(type_source1, index_source1, type_source2, index_source2);

         if (reg == NULL)
            error("no register available");

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, VARIABLES[index_destiny-1].stackPosition);
         freeRegister(&reg);
         break;

      case '*':
         reg = mul(type_source1, index_source1, type_source2, index_source2);

         if (reg == NULL)
            error("no register available");

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, VARIABLES[index_destiny-1].stackPosition);
         freeRegister(&reg);
         break;

      case '/':
         reg = divi(type_source1, index_source1, type_source2, index_source2);

         if (reg == NULL)
            error("no register available");

         fprintf(F_OUTPUT, "movl %%%s, -%d(%%rbp)\n", reg->name32, VARIABLES[index_destiny-1].stackPosition);
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

   if(type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, r->name32);
   else 
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", VARIABLES[index1-1].stackPosition, r->name32);

   if(type2 == 'c')
      fprintf(F_OUTPUT, "addl $%d, %%%s\n", index2, r->name32);
   else
      fprintf(F_OUTPUT, "addl -%d(%%rbp), %%%s\n", VARIABLES[index2-1].stackPosition, r->name32);

   return r;
}

Register *sub(char type1, int index1, char type2, int index2)
{
   Register *r = getRegister(NULL, CALLER_SAVED);

   if(type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, r->name32);
   else 
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", VARIABLES[index1-1].stackPosition, r->name32);

   if(type2 == 'c')
      fprintf(F_OUTPUT, "subl $%d, %%%s\n", index2, r->name32);
   else
      fprintf(F_OUTPUT, "subl -%d(%%rbp), %%%s\n", VARIABLES[index2-1].stackPosition, r->name32);

   return r;
}

Register *mul(char type1, int index1, char type2, int index2)
{
   Register *r = getRegister(NULL, CALLER_SAVED);

   if(type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, r->name32);
   else 
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", VARIABLES[index1-1].stackPosition, r->name32);

   if(type2 == 'c')
      fprintf(F_OUTPUT, "imull $%d, %%%s\n", index2, r->name32);
   else
      fprintf(F_OUTPUT, "imull -%d(%%rbp), %%%s\n", VARIABLES[index2-1].stackPosition, r->name32);

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

   if(type1 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index1, rax->name32);
   else
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", VARIABLES[index1-1].stackPosition, rax->name32);


   if(type2 == 'c')
      fprintf(F_OUTPUT, "movl $%d, %%%s\n", index2, r_tmp->name32);
   else
      fprintf(F_OUTPUT, "movl -%d(%%rbp), %%%s\n", VARIABLES[index2-1].stackPosition, r_tmp->name32);


   fprintf(F_OUTPUT, "cltd\n");
   fprintf(F_OUTPUT, "idiv %%%s\n", r_tmp->name32);

   freeRegister(&r_tmp);

   return rax;
}
