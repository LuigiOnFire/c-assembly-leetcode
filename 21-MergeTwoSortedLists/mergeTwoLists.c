#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

__attribute__ ((naked)) ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2){
__asm__(R"(
    .intel_syntax noprefix    
            push rbx            

            # find which one is smaller so we can put that (and leave it) in rax            
            mov ebx, DWORD PTR[rdi]
            mov ecx, DWORD PTR[rsi]
            
            cmp ebx, ecx
            jl second_is_bigger:
            mov rax, rdi
            mov rdi, QWORD PTR[rdi + 4]

            jmp found_larger

            second_is_bigger:
            mov rax, rsi
            mov rsi, QWORD PTR[rsi + 4]

            found_larger:
            mov rbx, rax #rbx will hold the previous head


            start_loop:
            cmp 0, QWORD PTR[rdi + 4]
            je stop_looping

            cmp 0, QWORD PTR[rsi + 4]
            je stop_looping                    

            mov ecx, DWORD PTR [rdi]
            mov edx, DWORD PTR [rsi]

            cmp ecx, edx
            jg append_second:
            mov DWORD PTR [rbx + 4], rdi
            mov rbx, DWORD PTR [rbx + 4]
            mov rdi, DWORD PTR [rbx + 4] # stopped here, but not sure this makes sense
            jmp start_loop

            append_second:
            mov DWORD PTR [rbx + 4], rsi
            mov rsi, DWORD PTR []

            mov r8d, DWORD PTR [rbp - 0x8] # temporarily holding m - l
            cmp ebx, r8d            
            jge fill_l

            mov r8d, DWORD PTR [rbp - 0x10]
            cmp ecx, r8d
            jge fill_l

            # Do L if L[i] >= R[j] jump to doing R instead

            mov r8, rax
            mov r10d, esi
            add r10d, ebx
            sal r10d, 2
            add r8, r10
            mov r8d, DWORD PTR[r8] # L[l + i]

            mov r9, rax
            mov r10d, edx
            add r10d, ecx
            sal r10d, 2
            add r9, r10
            mov r9d, DWORD PTR[r9] # R[m + j]
        
            cmp r8d, r9d
            jg merge_r

            # nums[l + i + j] = L[l + i]
            mov r9, r11
            mov r10d, esi
            add r10d, ebx
            add r10d, ecx
            sal r10d, 2
            add r9, r10
            mov DWORD PTR[r9], r8d # OUT[l + i + j] = L[l + i]            

            # i++
            inc ebx

            # jump back to merge both
            jmp merge_both

            merge_r:

            # nums[l + i + j] = R[m + j]            
            mov r8, r11
            mov r10d, esi
            add r10d, ebx
            add r10d, ecx
            sal r10d, 2
            add r8, r10
            mov DWORD PTR[r8], r9d # OUT[l + i + j] = R[m + j]

            # j++
            inc ecx


            # jump back to merge both
            jmp merge_both
                        
            fill_l:

            # if i >= m - l jump to fill r
            mov r8d, DWORD PTR [rbp - 0x8]
            cmp ebx, r8d
            jge fill_r

            # nums[l + i + j] = L[l + i]            
            mov r8, rax
            mov r10d, esi
            add r10d, ebx
            sal r10d, 2
            add r8, r10
            mov r8d, DWORD PTR[r8] # L[l + i]

            mov r9, r11
            mov r10d, esi
            add r10d, ebx
            add r10d, ecx
            sal r10d, 2
            add r9, r10
            mov DWORD PTR[r9], r8d # OUT[l + i + j] = L[l + i]

            inc ebx

            # jump back to fill_l
            jmp fill_l

            fill_r:

            # if j >= m - l jump to merge_cleanup
            mov r8d, DWORD PTR [rbp - 0x10]
            cmp ecx, r8d
            jge copy_out_to_in

            # nums[l + i + j] = R[m + j]
            mov r9, rax
            mov r10d, edx
            add r10d, ecx
            sal r10d, 2
            add r9, r10
            mov r9d, DWORD PTR[r9] # R[m + j]

            mov r8, r11
            mov r10d, esi
            add r10d, ebx
            add r10d, ecx
            sal r10d, 2
            add r8, r10
            mov DWORD PTR[r8], r9d # OUT[l + i + j] = R[m + j]

            inc ecx

            # jump back to fill_r
            jmp fill_r

            copy_out_to_in:

            xor r8, r8
            mov r8d, edi
            sub r8d, esi # r8 = r - l            
            xor rcx, rcx    

            copy_array_back_start:

            cmp ecx, r8d
            jge copy_array_back_end
            mov r10d, esi
            add r10d, ecx
            mov ebx, DWORD PTR [r11 + r10*4]
            mov DWORD PTR [rax + r10*4], ebx
            inc ecx
            jmp copy_array_back_start
            copy_array_back_end:
                        
            
            # at the end rax is nums, which is what we want to return anyway                        
            pop rbx       

            ret

            .att_syntax prefix
)");
}

int main(){
    
    int numsSize = 2;
    int nums[] = {1, -2};
    int returnSize = 0;
    
    int* out = sortArray(nums, numsSize, &returnSize);
    for(int i = 0; i < returnSize; i++){
        printf("%d, ", nums[i]);
    }
    printf("\n");
}
