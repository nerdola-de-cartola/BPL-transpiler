#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_SIZE 256

#define CALLER_SAVED 0
#define CALLEE_SAVED 1

#define MAX_REGISTER 16

typedef struct {
   char name32[4];
   char name64[4];
   bool free;
   int type;
} Register;


Register REGISTERS[MAX_REGISTER];

FILE *F_SOURCE;
FILE *F_OUTPUT;
int LINE_COUNT;
char BUFFER[MAX_LINE_SIZE];

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

int main(int argc, char **argv) {
   int r;

   if(argc != 3) {
      printf("Quantidade de argumentos inválida\n");
      printf("Modo de uso: bpl.exe <seu_códio.bpl> <codigo_traduzido.s>\n");
   }

   F_SOURCE = fopen(argv[1], "rt");
   if(F_SOURCE == NULL) error("impossible to read file");

   F_OUTPUT = fopen(argv[2], "wt");
   if(F_SOURCE == NULL) error("impossible to write file");

   LINE_COUNT = 1;
   registersInit();

   while(fgets(BUFFER, MAX_LINE_SIZE, F_SOURCE) != NULL) {

      if(charInStr('=', BUFFER)) 
         assignment();

      fprintf(F_OUTPUT, "\n");
      LINE_COUNT++;
   }
   

   writeMain();

   fclose(F_SOURCE);
   fclose(F_OUTPUT);

   return 0;
}

void writeMain() {
   fprintf(F_OUTPUT, 
      ".text\n"\
      "call .f1\n"
   );
}


bool charInStr(const char c, const char *str) {
   while(str) {
      if(c == *str) return true;
      str++;
   }

   return false;
}

void assignment() {
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

   if(r == 4) {
      if(
         (type_destiny != 'v') ||
         (type_source1 != 'v' && type_source1 != 'c')
      ) error("invalid type in assignment");

      fprintf(F_OUTPUT, "movl %ci%d, %ci%d\n", type_source1, index_source1, type_destiny, index_destiny);

   } else if(r == 7) {
      Register *reg;

      if(
         (type_destiny != 'v') ||
         (type_source1 != 'v' && type_source1 != 'c') ||
         (type_source2 != 'v' && type_source2 != 'c')
      ) error("invalid type in assignment");
      
      switch(operand) {
            case '+':
               reg = add(type_source1, index_source1, type_source2, index_source2);
               if(reg == NULL) error("no register available");
               fprintf(F_OUTPUT, "movl %%%s, %ci%d\n", reg->name32, type_destiny, index_destiny);
               freeRegister(&reg);
               break;

            case '-':
               reg = sub(type_source1, index_source1, type_source2, index_source2);
               if(reg == NULL) error("no register available");
               fprintf(F_OUTPUT, "movl %%%s, %ci%d\n", reg->name32, type_destiny, index_destiny);
               freeRegister(&reg);
               break;

            case '*':
               reg = mul(type_source1, index_source1, type_source2, index_source2);
               if(reg == NULL) error("no register available");
               fprintf(F_OUTPUT, "movl %%%s, %ci%d\n", reg->name32, type_destiny, index_destiny);
               freeRegister(&reg);
               break;

            case '/':
               reg = divi(type_source1, index_source1, type_source2, index_source2);
               if(reg == NULL) error("no register available");
               fprintf(F_OUTPUT, "movl %%%s, %ci%d\n", reg->name32, type_destiny, index_destiny);
               freeRegister(&reg);
               break;
            
            default:
               error("invalid operand in assignment");
               break;
      }

   } else {
      error("invalid assignment");
   }

}

void error(const char *error_type) {
   printf(
      "ERROR\n"\
      "Line %d\n"\
      "Type: %s\n",
      LINE_COUNT,
      error_type
   );

   fclose(F_SOURCE);
   fclose(F_OUTPUT);
   
   exit(1);
}

void registersInit() {
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

Register *add(char type1, int index1, char type2, int index2) {
   Register *r = getRegister(NULL, CALLER_SAVED);

   fprintf(F_OUTPUT, "movl %ci%d, %%%s\n", type2, index2, r->name32);
   fprintf(F_OUTPUT, "addl %ci%d, %%%s\n", type1, index1, r->name32);

   return r;
}

Register *sub(char type1, int index1, char type2, int index2) {
   Register *r = getRegister(NULL, CALLER_SAVED);

   fprintf(F_OUTPUT, "movl %ci%d, %%%s\n", type2, index2, r->name32);
   fprintf(F_OUTPUT, "subl %ci%d, %%%s\n", type1, index1, r->name32);

   return r;
}


Register *mul(char type1, int index1, char type2, int index2) {
   Register *r = getRegister(NULL, CALLER_SAVED);

   fprintf(F_OUTPUT, "movl %ci%d, %%%s\n", type2, index2, r->name32);
   fprintf(F_OUTPUT, "imull %ci%d, %%%s\n", type1, index1, r->name32);

   return r;
}


Register *getRegister(char *name64, int type) {
   int i;

   for(i = 0; i < MAX_REGISTER; i++) {
      if(REGISTERS[i].free && REGISTERS[i].type == type) {
 
         if(
            name64 != NULL &&
            strcmp(REGISTERS[i].name64, name64) != 0
         ) continue;

         REGISTERS[i].free = false;
         return &REGISTERS[i];
      }
   }

   return NULL;
}

void freeRegister(Register **r) {
   (*r)->free = true;
   *r = NULL;
}

Register *divi(char type1, int index1, char type2, int index2) {
   Register *r1 = getRegister("rax", CALLER_SAVED);
   Register *r2 = getRegister(NULL, CALLER_SAVED);

   fprintf(F_OUTPUT, "movl %ci%d, %%%s\n", type1, index1, r1->name32);
   fprintf(F_OUTPUT, "movl %ci%d, %%%s\n", type2, index2, r2->name32);
   fprintf(F_OUTPUT, "cltd\n");
   fprintf(F_OUTPUT, "idiv %%%s\n", r2->name32);

   freeRegister(&r2);

   return r1;
}