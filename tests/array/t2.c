#include<stdio.h>

void f1(int p1, int p2, int *p3);

int main() {
    int a[10];
    
    f1(1, 2, a);

    return 0;
}