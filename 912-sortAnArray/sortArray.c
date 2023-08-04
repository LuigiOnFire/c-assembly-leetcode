#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* sortArray(int* nums, int numsSize, int* returnSize){
    int* out = malloc(numsSize*sizeof(int));

    // __asm__ volatile(R"(.intel_syntax noprefix
    //     )"

    __asm__ volatile(".intel_syntax noprefix\n"

            "jmp init\n"

            "merge_sort:\n"\
            "push rbp\n"
            "mov rbp, rsp\n"
            "mov rsi, QWORD PTR [rbp + 0x10]\n" // this is l
            "mov rdi, QWORD PTR [rbp + 0x18]\n" // this is r
            "cmp rsi, rdi\n" // if l >= r
            "jge merge_sort_end\n"
            "mov rdx, rsi\n"
            "add rdx, rdi\n"
            "shr rdx, 1\n" // rdx contains m = (l + r) / 2

            "push rdi\n" // this one is not used 
            "push rdx\n" // this will be the new r
            "push rsi\n" // this will be the new l
            "call merge_sort\n"
            "pop rsi\n"
            "pop rdx\n"
            "pop rdi\n"

            "inc rdx\n" //m = m + 1

            "push rsi\n" // this one is not used 
            "push rdi\n" // this will be the new r
            "push rdx\n" // this will be the new l
            "call merge_sort\n"
            "pop rdx\n"
            "pop rdi\n"
            "pop rsi\n"

            "dec rdx\n" //m = m - 1 (restoring it to previous value)

            "push rdi\n"
            "push rsi\n"
            "call merge\n"
            "pop rsi\n"
            "pop rdi\n"
            
            "merge_sort_end:" 

            "mov rsp, rbp\n"
            "pop rbp\n"
            "ret\n"

            "merge:\n"

            "push rbp\n"
            "mov rbp, rsp\n"
            
            "mov rsi, QWORD PTR [rbp + 0x10]\n" // new l
            "mov rdi, QWORD PTR [rbp + 0x18]\n" // new r
            
            
            // get new m
            "mov rdx, rsi\n"
            "add rdx, rdi\n"
            "shr rdx, 1\n" // new m

            // save size of L = m - l
            "xor rbx, rbx\n"
            "mov rbx, rdx\n"
            "sub rbx, rsi\n"
            "push rbx\n"

            // save size of R = r - m
            "xor rbx, rbx\n"
            "mov rbx, rdi\n"
            "sub rbx, rdx\n"
            "push rbx\n"

            // move each element from A[l..r)
            "xor r8, r8\n"
            "mov r8, rdi\n"
            "sub r8, rsi\n" // r8 = r - l
            
            "xor rcx, rcx\n"
            "copy_array_start:\n"            
            "cmp rcx, r8\n"
            "jg copy_array_end\n"            
            "mov rbx, QWORD PTR [%1 + rcx*4]\n"
            "mov QWORD PTR [%0 + rcx*4], rbx\n"
            "inc rcx\n"
            "jmp copy_array_start\n"
            "copy_array_end:\n"

            // set up i = 0
            "xor rbx, rbx\n"

            // set up j = 0
            "xor rcx, rcx\n"

            "merge_both:\n"

            // if i >= m - l || j >= r - m then jump to fill_l            

            "mov r8, QWORD PTR [rbp]\n" // temporarily holding m - l
            "cmp rbx, r8\n"
            "jg fill_l\n"

            "mov r8, QWORD PTR [rbp - 8]\n"
            "cmp rcx, r8\n"
            "jg fill_l\n"

            // Do L if L[i] >= R[j] jump to doing R instead

            "mov r8, %1\n"
            "mov r10, rsi\n"
            "add r10, rbx\n"
            "shl r10, 2\n"
            "add r8, r10\n"
            "mov r8, QWORD PTR[r8]\n" // L[l + i]

            "mov r9, %1\n"
            "mov r10, rdx\n"
            "add r10, rcx\n"
            "shl r10, 2\n"
            "add r8, r10\n"
            "mov r9, QWORD PTR[r9]\n" // R[m + j]
        
            "cmp r8, r9\n"
            "jg merge_r\n"

            // nums[l + i + j] = L[l + i]
            "mov r9, %0\n"
            "mov r10, rsi\n"
            "add r10, rbx\n"
            "add r10, rcx\n"
            "shl r10, 2\n"
            "add r9, r10\n"
            "mov QWORD PTR[r9], r8\n" // OUT[l + i + j] = L[l + i]            

            // i++            
            "inc rbx\n"

            // jump back to merge both
            "jmp merge_both\n"

            "merge_r:\n"

            // nums[l + i + j] = R[m + j]            
            "mov r8, %0\n"
            "mov r10, rsi\n"
            "add r10, rbx\n"
            "add r10, rcx\n"
            "shl r10, 2\n"
            "add r8, r10\n"
            "mov QWORD PTR[r8], r9\n" // OUT[l + i + j] = R[m + j]

            // j++
            "inc rbx\n"


            // jump back to merge both
            "jmp merge_both\n"
                        
            "fill_l:\n"

            // if i >= m - l jump to fill r
            "mov r8, QWORD PTR [rbp]\n"
            "cmp rbx, r8\n"
            "jge fill_r\n"

            // nums[l + i + j] = L[l + i]            
            "mov r8, %1\n"
            "mov r10, rsi\n"
            "add r10, rbx\n"
            "shl r10, 2\n"
            "add r8, r10\n"
            "mov r8, QWORD PTR[r8]\n" // L[l + i]

            "mov r9, %0\n"
            "mov r10, rsi\n"
            "add r10, rbx\n"
            "add r10, rcx\n"
            "shl r10, 2\n"
            "add r9, r10\n"
            "mov QWORD PTR[r9], r8\n" // OUT[l + i + j] = L[l + i]

            "inc rbx\n"

            // jump back to fill_l
            "jmp fill_l\n"

            "fill_r:\n"

            // if j >= n - l jump to merge_cleanup
            "mov r8, QWORD PTR [rbp]\n"
            "cmp rbx, r8\n"
            "jge copy_out_to_in\n"

            // nums[l + i + j] = R[j]
            "mov r9, %1\n"
            "mov r10, rdx\n"
            "add r10, rcx\n"
            "shl r10, 2\n"
            "add r9, r10\n"
            "mov r9, QWORD PTR[r9]\n" // R[m + j]

            "mov r8, %0\n"
            "mov r10, rsi\n"
            "add r10, rbx\n"
            "add r10, rcx\n"
            "shl r10, 2\n"
            "add r8, r10\n"
            "mov QWORD PTR[r8], r9\n" // OUT[l + i + j] = R[m + j]

            "inc rcx\n"

            // jump back to fill_r
            "jmp fill_r\n"

            "copy_out_to_in:\n"

            "xor r8, r8\n"
            "mov r8, rdi\n"
            "sub r8, rsi\n" // r8 = r - l
            
            "xor rcx, rcx\n"

            "copy_array_back_start:\n"
            "cmp rcx, r8\n"
            "jg copy_array_back_end\n"            
            "mov rbx, QWORD PTR [%0 + rcx*4]\n"
            "mov QWORD PTR [%1 + rcx*4], rbx\n"
            "inc ecx\n"
            "jmp copy_array_start\n"
            "copy_array_back_end:\n"

            "merge_cleanup:\n"

            // restore rbp and rsp
            "mov rsp, rbp\n"
            "pop rbp\n"

            // end merge
            "ret\n"
            "init:\n"

            "mov eax, %2\n"
            "mov [%3], eax\n"
            "xor rcx, rcx\n"
            "xor rsi, rsi\n"
            "xor rdi, rdi\n"
            // rax will remain the start of our array

            "xor rax, rax\n"
            "mov rax, %1\n"

            "mov rsi, 0\n"
            "mov edi, %2\n"
            "push rdi\n"
            "push rsi\n"
            "call merge_sort\n" 
            "pop rsi\n"
            "pop rdi\n"
            
            ".att_syntax\n"
            : "+r" (out)
            : "r" (nums), "r" (numsSize), "r" (returnSize)
            : "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10");

            return nums;
}

int main(){
    int numsSize = 2;
    //int nums[] = {0, 1, 2, 3};
    int nums[] = {1, 0};
    int returnSize;
    
    int* out = sortArray(nums, numsSize, &returnSize);
    for(int i = 0; i < numsSize; i++){
        printf("%d, ", out[i]);
    }
    printf("\n");
}