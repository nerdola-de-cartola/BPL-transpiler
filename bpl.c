#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 256

FILE *f_source;
FILE *f_output;
int line_count;
char buffer[MAX_LINE_SIZE];

void writeMain();
bool charInStr(const char c, const char *str);
void assignment();
void error(const char *error_type);


int main(int argc, char **argv) {
   int r;

   if(argc != 3) {
      printf("Quantidade de argumentos inválida\n");
      printf("Modo de uso: bpl.exe <seu_codio.bpl> <codigo_traduzido.s>\n");
   }

   f_source = fopen(argv[1], "rt");
   f_output = fopen(argv[2], "wt");

   line_count = 1;
   while(fgets(buffer, MAX_LINE_SIZE, f_source) != NULL) {

      if(charInStr('=', buffer)) 
         assignment();

      line_count++;
   }
   

   writeMain();

   fclose(f_source);
   fclose(f_output);

   return 0;
}

void writeMain() {
   fprintf(f_output, 
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
      buffer,
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
      if(type_destiny != 'v') error("invalid type in assignment");

      if(type_source1 == 'c') {
         fprintf(f_output, "movl $%d, ", index_source1);
      } else if(type_source1 == 'v') {
         fprintf(f_output, "movl vi%d, ", index_source1);
      } else {
         error("invalid type in assignment");
      }

      fprintf(f_output, "vi%d\n", index_destiny);


   } else if(r == 7) {

   } else {
      error("invalid assignment");
   }

}

void error(const char *error_type) {
   printf(
      "ERROR\n"\
      "Line %d\n"\
      "Type: %s\n",
      line_count,
      error_type
   );

   fclose(f_source);
   fclose(f_output);
   
   exit(1);
}