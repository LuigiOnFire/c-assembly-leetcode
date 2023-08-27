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

    mov bl, BYTE PTR [rdi + rcx] # bx will hold left
    mov sil, BYTE PTR [rdi + rdx] # sil will hold right
    
    # remove_non_alphanum will use rax to tell us 

    xor rax, rax

    push rdi
    mov rdi, rbx
    call convert_uppercase        
    mov rbx, rdi
    pop rdi
    
    push rdi
    mov rdi, rbx
    call remove_non_alphanum
    mov rbx, rdi
    pop rdi

    test al, 2 # test the left bit
    jnz test_right

    inc rcx

    test_right:

    test al, 1 # test the right bit
    jnz test_jump

    dec rdx

    test_jump:
    cmp al, 3 # test both bits
    je continue_loop

    jmp main_loop

    continue_loop:

    cmp sil, bl
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

    convert_uppercase:
    
    # start with the first char

    cmp dil, 65 # 'A' = 65
    jl uppercase_right

    cmp dil, 90 # 'Z' = 90
    jle convert_left

    jmp uppercase_right

    convert_left:
    add dil, 0x20

    uppercase_right:

    cmp sil, 65
    jl convert_done

    cmp sil, 90
    jle convert_right

    jmp convert_done

    convert_right:
    add sil, 0x20
    
    convert_done:
    ret

    remove_non_alphanum:
    # in rax:
    # least significant bit is for right char
    # second least significant bit is for left char
    mov rax, 3

    # start with the first char
    cmp dil, 48 # '0' = 48
    jl left_failed

    cmp dil, 57 # '9' = 57
    jle check_right

    cmp dil, 97 # 'a' = 97
    jl left_failed

    cmp dil, 122 # 'z' = 122
    jle check_right

    left_failed: # if we fail at the end we can just fall through
    and al, 1 # sets left bit of al to zero

    check_right:
    cmp sil, 48 # '0' = 48
    jl right_failed

    cmp sil, 57 # '9' = 57
    jle alphanum_done

    cmp sil, 97 # 'a' = 97
    jl right_failed

    cmp sil, 122 # 'z' = 122
    jle alphanum_done

    right_failed:
    and al, 2 # sets right bit of al to zero

    alphanum_done:
    ret 

    .att_syntax prefix
)");
}

int main(){
    char s[] = "A man, a plan, a dcanal: Panama";
    bool out = isPalindrome(s);
    printf("The result is: %d\n", out);
    return 0;
}