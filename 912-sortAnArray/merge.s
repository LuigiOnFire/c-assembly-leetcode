    .intel_syntax noprefix
    .text

    # void merge(int*, size_t, size_t)
merge:
    sub rsp, 24
    mov rsp[0], rdi
    mov rsp[8], rsi
    mov rsp[16], rdx
    lea rdi, [4*rsi]
    call malloc
    # rax is the destination
    # rdx is the first index of the source
    # r8 is the second index of the source
    # rdi is the end of the source
    # rsi is the end of the first part of the source
    # rcx is the number of iterations left
    mov rdx, rsp[0]
    mov r8, rsp[16]
    lea r8, [rdx+4*r8]
    mov rcx, rsp[8]
    lea rdi, [rdx+4*rcx]
    mov rsi, r8
    test rcx, rcx
    jz .loop_done
    # in the loop
    # r10d is the thing to be stored
    # compare j and n
    cmp r8, rdi
    jne 1f
    mov r10d, [rdx]
    add rdx, 4
    jmp .store
1:
    # compare i and m
    cmp rdx, rsi
    jne 1f
    mov r10d, [r8]
    add r8, 4
    jmp .store
1:
    mov r10d, [rdx]
    mov r9d, [r8]
    cmp r10d, r9d
    cmovg r10d, r9d
    mov r9, 0
    cmovg r9, 4
    add r8, r9
    xor r9, 4
    add rdx, r9
.store: