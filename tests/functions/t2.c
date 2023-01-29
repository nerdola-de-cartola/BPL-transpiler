#include<stdio.h>

void f1(int p1, int *p2);
void f2(int *p1);

int main() {
    int a[10];
    int b[10];
    
    f1(1, a);
    f2(b);

    return 0;
}