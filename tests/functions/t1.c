#include<stdio.h>

void f1(int *p1, int *p2, int p3);

int main() {
    int a[10];
    int b[10];
    
    f1(a, b, 3);

    return 0;
}