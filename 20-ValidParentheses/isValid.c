#include <stdio.h>
#include <stdlib.h>

__attribute__ ((naked)) bool isValid(char * s){
__asm__(R"(
        .intel_syntax noprefix    

        push rbx # rbx is callee saved
        
        # get size of array
        xor rcx, rcx
        xor rbx, rbx

        check_size:
        mov bl, BYTE PTR[rdi + rcx]
        cmp bl, 0

        je found_end

        inc rcx
        jmp check_size

        found_end:

        cmp rcx, 0
        je return_true # takes care of the trivial case

        # malloc it to form the stack

        return_true:
        mov rax, 1
        jmp done

        done:
        pop rbx
        ret

        .att_syntax prefix
)");
}

int main(){
    char s[] = "";
    bool out = isValid(s);
        
    printf("The result is %d.\n", out);

    return 0;
}