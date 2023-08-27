#include <stdio.h>
#include <stdlib.h>


__attribute__ ((naked)) bool isPalindrome(char * s){

    __asm__(R"(
    .intel_syntax noprefix
    push rbx

    xor rcx, rcx
    xor rdx, rdx    

    find_size:
    mov bl, BYTE PTR [rdi + rdx]
    cmp bl, 0
    je found_size
    inc rdx
    jmp find_size

    found_size:
    dec rdx

    main_loop:    

    # rcx will hold our left iterator
    # rdx will hold our right iterator
    # bh will hold the value at the left iterator
    # bl will hold the value at the right iterator    

    cmp rcx, rdx
    jge return_true

    mov bh, BYTE PTR [rdi + rcx]
    mov bl, BYTE PTR [rdi + rdx]

    cmp bh, 48
    jl inc_left

    cmp bh, 57
    jl inc_left

    cmp bh, bl
    jne return_false

    inc rcx
    dec rdx
    jmp main_loop

    return_false:
    xor rax, rax
    pop rbx
    ret

    return_true:
    mov rax, 1
    pop rbx
    ret

    .att_syntax prefix
)");
}

int main(){
    char s[] = "aba";
    bool out = isPalindrome(s);
    printf("The result is: %d\n", out);
    return 0;
}