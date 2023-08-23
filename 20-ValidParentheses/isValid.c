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
        push rcx
        push rdi
        push rdx
        mov rdi, rcx
        call malloc # just leave our return array in rax
        pop rdx
        pop rdi
        pop rcx

        mov rsi, rcx
        mov rcx, 0
        mov rdx, 0

        check_next_char:
        cmp rcx, rsi
        je check_stack_then_done

        mov bl, BYTE PTR[rdi + rcx]

        # if open round paren
        cmp bl, 40
        je is_open_paren

        # if open square paren 
        cmp bl, 91
        je is_open_paren

        # if open curly paren
        cmp bl, 123
        je is_open_paren

        # if close round paren
        cmp bl, 41        
        je is_close_round_paren

        # if close square paren
        cmp bl, 93
        je is_close_square_paren

        # if close curly paren
        cmp bl, 125
        je is_close_curly_paren

        # if invalid character
        jmp return_false

        is_open_paren:
        mov BYTE PTR[rax + rdx], bl
        inc rdx
        inc rcx
        jmp check_next_char

        pop_stack:
        dec rdx
        mov bh, BYTE PTR[rax + rdx]
        mov BYTE PTR[rax + rdx], 0 # not necessary but nice for debugging
        ret

        is_close_round_paren:
        call pop_stack
        cmp bh, 40 # open round paren
        jne return_false
        inc rcx
        jmp check_next_char

        is_close_square_paren:
        call pop_stack
        cmp bh, 91 # open square paren
        jne return_false
        inc rcx
        jmp check_next_char

        is_close_curly_paren:
        call pop_stack
        cmp bh, 123 # open curly paren
        jne return_false
        inc rcx
        jmp check_next_char

        check_stack_then_done:
        cmp rdx, 0
        je return_true
        jmp return_false

        return_true:
        mov rax, 1
        jmp done

        return_false:
        mov rax, 0
        jmp done

        done:
        pop rbx
        ret

        .att_syntax prefix
)");
}

int main(){
    char s[] = "(";
    bool out = isValid(s);
        
    printf("The result is %d.\n", out);

    return 0;
}