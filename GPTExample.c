#include <stdio.h>

int main() {
    int a;
    __asm__(
            // "mov eax, 42\n"
            "mov %0, eax\n"
            : "=r" (a)
            :
            : "eax");

    printf("a = %d\n", a);
    return 0;
}