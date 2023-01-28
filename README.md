# BPL-transpiler
Transpiler de BPL (Bruno Programming Language) para assembly de intel x86

# Compilar
```clang ./lib/array/array.c -o ./lib/array/array.o -c -Wall -Wextra```\
```clang ./lib/assignment/assignment.c -o ./lib/assignment/assignment.o -c -Wall -Wextra```\
```clang ./lib/common/common.c -o ./lib/common/common.o -c -Wall -Wextra```\
```clang ./lib/conditions/conditions.c -o ./lib/conditions/conditions.o -c -Wall -Wextra```\
```clang ./lib/functions/functions.c -o ./lib/functions/functions.o -c -Wall -Wextra```\
```clang ./lib/parameters/parameters.c -o ./lib/parameters/parameters.o -c -Wall -Wextra```\
```clang ./lib/registers/registers.c -o ./lib/registers/registers.o -c -Wall -Wextra```\
```clang ./lib/variables/variables.c -o ./lib/variables/variables.o -c -Wall -Wextra```\
```clang ./bpl.c -o ./bpl.o -c -Wall -Wextra```

# Linkar
```clang ./bpl.o ./lib/array/array.o ./lib/assignment/assignment.o ./lib/common/common.o ./lib/conditions/conditions.o ./lib/functions/functions.o ./lib/parameters/parameters.o ./lib/registers/registers.o ./lib/variables/variables.o -o ./bpl.exe -Wall -Wextra```

# Executar
```./bpl.exe <nome do arquivo a ser testado>  <nome do arquivo de saída em assembly>```
