#include "include/bpl.h"

//==============================================================================
// Função Main
//==============================================================================

int main(int argc, char **argv)
{

   // Verifica os argumentos passados
   if (argc != 3)
   {
      printf("Quantidade de argumentos inválida\n");
      printf("Modo de uso: bpl.exe <seu_códio.bpl> <codigo_traduzido.s>\n");
      return 0;
   }

   // Abre o arquivo ´de entrada (código fonte em bpl)
   F_SOURCE = fopen(argv[1], "rt");
   if (F_SOURCE == NULL)
   {
      printf("impossible to read file\n");
      return 1;
   }

   // Abre o arquivo ´de saída (código fonte em assembly)
   F_OUTPUT = fopen(argv[2], "wt");
   if (F_SOURCE == NULL)
   {
      printf("impossible to write file\n");
      return 1;
   }

   // Inicializa as variáveis globais
   LINE_COUNT = 0;
   IF_INDEX = 0;
   registersInit();
   functionsInit();

   beginFile();

   // Lê as definições de funções
   while (readNewLine() != NULL)
   {

      if (strInStr(BUFFER, "function"))
         functionDefinition();

      fprintf(F_OUTPUT, "\n");
   }

   // Fecha os arquivos de entrada e saída
   fclose(F_SOURCE);
   fclose(F_OUTPUT);

   return 0;
}
