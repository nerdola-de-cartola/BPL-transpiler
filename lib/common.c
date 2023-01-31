#include "../include/common.h"

FILE *F_SOURCE;
FILE *F_OUTPUT;
char BUFFER[MAX_LINE_SIZE];
int LINE_COUNT;

void remove_newline(char *ptr)
{
   while (*ptr)
   {
      if (*ptr == '\n' || *ptr == '\r' || *ptr == '#')
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

void beginFile()
{
   fprintf(F_OUTPUT, ".text\n");
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

char *readNewLine()
{
   char *ptr = NULL;

   while (true)
   {
      ptr = fgets(BUFFER, MAX_LINE_SIZE, F_SOURCE);

      LINE_COUNT++;
      remove_newline(BUFFER);

      if (BUFFER[0] != '\0' || ptr == NULL)
         break;
   }

   return ptr;
}