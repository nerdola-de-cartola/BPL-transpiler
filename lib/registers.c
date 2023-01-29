
#include "../include/registers.h"

Register REGISTERS[MAX_REGISTER];

void registersInit()
{
   REGISTERS[15].free = true;
   REGISTERS[15].type = CALLER_SAVED;
   strcpy(REGISTERS[15].name32, "eax");
   strcpy(REGISTERS[15].name64, "rax");

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

   REGISTERS[0].free = true;
   REGISTERS[0].type = CALLEE_SAVED;
   strcpy(REGISTERS[0].name32, "r15d");
   strcpy(REGISTERS[0].name64, "r15");
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

   error("No register available");

   return NULL;
}

void freeRegister(Register **r)
{
   (*r)->free = true;
   *r = NULL;
}

void registerName(int index, char name32[4], char name64[4]) {
   switch (index)
   {
   case 1:
      strcpy(name32, "edi");
      strcpy(name64, "rdi");
      break;

   case 2:
      strcpy(name32, "esi");
      strcpy(name64, "rsi");
      break;

   case 3:
      strcpy(name32, "edx");
      strcpy(name64, "rdx");
      break;
   }
}