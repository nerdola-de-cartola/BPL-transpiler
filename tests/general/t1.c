#include <stdio.h>

int f1(int p1, int p2, int *p3);

int main(void) {

   int a[3] = {13, 22, 42};

   printf("%d\n", f1(2, 5, a));
   printf("%d\n", f1(0, 10, a));

   //     return 82     return 10
   return 0;
}