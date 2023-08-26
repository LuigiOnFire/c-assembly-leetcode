#include <stdio.h>
#include <stdlib.h>


__attribute__ ((naked)) int maxProfit(int* prices, int pricesSize){
__asm__(R"(
    .intel_syntax noprefix    

    push rbx

    # rdi is the start of our price array
    # esi is its size
    # let rax be our best profit so far
    # let rbx be our best min so far
    # let rcx be our current index
    # rdx will be used to calculate our sum

    mov rax, 0
    xor rbx, rbx
    mov ebx, 0x7FFFFF # this is int max
    mov rcx, 0

    start_loop:
    cmp ecx, esi
    je end_loop

    mov edx, DWORD PTR[rdi + 4*rcx]
    sub edx, ebx

    cmp edx, eax
    jle no_new_profit
    mov eax, edx

    no_new_profit:

    cmp ebx, DWORD PTR[rdi + 4*rcx]
    jle no_new_min
    mov ebx, DWORD PTR[rdi + 4*rcx]

    no_new_min:
    
    inc rcx
    jmp start_loop
    
    end_loop:

    pop rbx
    ret

    .att_syntax prefix
)");
}

int main(){
    int prices[] = {1, 2, 3};
    int pricesSize = 3;
    int profit = maxProfit(prices, pricesSize);
    printf("The max profit is: %d\n", profit);
}