int f1(int p1, int p2, int *p3);

int main(void) {

   int a[3] = {13, 22, 42};

   //     return 154     return 0
   return f1(2, 5, a) + f1(0, 10, a);
}