#include <stdio.h>
#include <stdlib.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* sortArray(int* nums, int numsSize, int* returnSize){
    int* out = malloc(numsSize*sizeof(int));
    
    __asm__ volatile(".intel_syntax noprefix\n"
            "mov eax, %3\n"
            "mov [%0], eax\n"
            "xor rcx, rcx\n"
            "xor rsi, rsi\n"
            "xor rdi, rdi\n"
            // rax will remain the start of our array

            "xor rax, rax\n"
            "mov rax, %1\n"

            "mov esi, 0\n"
            "mov edi, %3\n"
            "push rdi\n"
            "push rsi\n"
            "call merge_sort\n" 
            "pop rsi\n"
            "pop rdi\n"

            "jmp end\n"

            "merge_sort:\n"
            "push rbp\n"
            "mov rbp, rsp\n"
            "mov esi, DWORD PTR [rbp + 0x10]\n" // this is l
            "mov edi, DWORD PTR [rbp + 0x18]\n" // this is r
            "mov ebx, edi\n"
            "sub ebx, esi\n"
            "cmp ebx, 1\n" // if r - l <= 1
            "jle merge_sort_end\n"
            "mov edx, esi\n"
            "add edx, edi\n"
            "shr edx, 1\n" // edx contains m = (l + r) / 2

            "push rdi\n" // this one is not used 
            "push rdx\n" // this will be the new r
            "push rsi\n" // this will be the new l
            "call merge_sort\n"
            "pop rsi\n"
            "pop rdx\n"
            "pop rdi\n"

            "push rsi\n" // this one is not used 
            "push rdi\n" // this will be the new r
            "push rdx\n" // this will be the new l
            "call merge_sort\n"
            "pop rdx\n"
            "pop rdi\n"
            "pop rsi\n"

            "push rdi\n"
            "push rsi\n"
            "call merge\n"
            "pop rsi\n"
            "pop rdi\n"
            
            "merge_sort_end:\n" 

            "mov rsp, rbp\n"
            "pop rbp\n"
            "ret\n"

            "merge:\n"

            "push rbp\n"
            "mov rbp, rsp\n"
            
            "mov esi, DWORD PTR [rbp + 0x10]\n" // new l
            "mov edi, DWORD PTR [rbp + 0x18]\n" // new r
            
            
            // get new m
            "mov edx, esi\n"
            "add edx, edi\n"
            "shr edx, 1\n" // new m

            // save size of L = m - l
            "xor ebx, ebx\n"
            "mov ebx, edx\n"
            "sub ebx, esi\n"
            "push rbx\n"

            // save size of R = r - m
            "xor ebx, ebx\n"
            "mov ebx, edi\n"
            "sub ebx, edx\n"
            "push rbx\n"

            // move each element from A[l..r)
            "xor r8d, r8d\n"
            "mov r8d, edi\n"
            "sub r8d, esi\n" // r8 = r - l
            
            "xor ecx, ecx\n"
            "copy_array_start:\n"            
            "cmp ecx, r8d\n"
            "jg copy_array_end\n"            
            "mov ebx, DWORD PTR [%2 + rcx*4]\n"
            "mov QWORD PTR [%1 + rcx*4], rbx\n"
            "inc ecx\n"
            "jmp copy_array_start\n"
            "copy_array_end:\n"

            // set up i = 0
            "xor ebx, ebx\n"

            // set up j = 0
            "xor ecx, ecx\n"

            "merge_both:\n"

            // if i >= m - l || j >= r - m then jump to fill_l            

            "mov r8d, DWORD PTR [rbp - 0x8]\n" // temporarily holding m - l
            "cmp ebx, r8d\n"            
            "jge fill_l\n"

            "mov r8d, DWORD PTR [rbp - 0x10]\n"
            "cmp ecx, r8d\n"
            "jge fill_l\n"

            // Do L if L[i] >= R[j] jump to doing R instead

            "mov r8, %2\n"
            "mov r10d, esi\n"
            "add r10d, ebx\n"
            "shl r10d, 2\n"
            "add r8d, r10d\n"
            "mov r8d, DWORD PTR[r8]\n" // L[l + i]

            "mov r9, %2\n"
            "mov r10d, edx\n"
            "add r10d, ecx\n"
            "shl r10d, 2\n"
            "add r9d, r10d\n"
            "mov r9d, DWORD PTR[r9]\n" // R[m + j]
        
            "cmp r8d, r9d\n"
            "jg merge_r\n"

            // nums[l + i + j] = L[l + i]
            "mov r9, %1\n"
            "mov r10d, esi\n"
            "add r10d, ebx\n"
            "add r10d, ecx\n"
            "shl r10d, 2\n"
            "add r9d, r10d\n"
            "mov DWORD PTR[r9], r8d\n" // OUT[l + i + j] = L[l + i]            

            // i++
            "inc ebx\n"

            // jump back to merge both
            "jmp merge_both\n"

            "merge_r:\n"

            // nums[l + i + j] = R[m + j]            
            "mov r8, %1\n"
            "mov r10d, esi\n"
            "add r10d, ebx\n"
            "add r10d, ecx\n"
            "shl r10d, 2\n"
            "add r8d, r10d\n"
            "mov DWORD PTR[r8], r9d\n" // OUT[l + i + j] = R[m + j]

            // j++
            "inc ecx\n"


            // jump back to merge both
            "jmp merge_both\n"
                        
            "fill_l:\n"

            // if i >= m - l jump to fill r
            "mov r8d, DWORD PTR [rbp - 0x8]\n"
            "cmp ebx, r8d\n"
            "jge fill_r\n"

            // nums[l + i + j] = L[l + i]            
            "mov r8, %2\n"
            "mov r10d, esi\n"
            "add r10d, ebx\n"
            "shl r10d, 2\n"
            "add r8d, r10d\n"
            "mov r8d, DWORD PTR[r8]\n" // L[l + i]

            "mov r9, %1\n"
            "mov r10d, esi\n"
            "add r10d, ebx\n"
            "add r10d, ecx\n"
            "shl r10d, 2\n"
            "add r9d, r10d\n"
            "mov DWORD PTR[r9], r8d\n" // OUT[l + i + j] = L[l + i]

            "inc ebx\n"

            // jump back to fill_l
            "jmp fill_l\n"

            "fill_r:\n"

            // if j >= m - l jump to merge_cleanup
            "mov r8d, DWORD PTR [rbp - 0x10]\n"
            "cmp ecx, r8d\n"
            "jge copy_out_to_in\n"

            // nums[l + i + j] = R[m + j]
            "mov r9, %2\n"
            "mov r10d, edx\n"
            "add r10d, ecx\n"
            "shl r10d, 2\n"
            "add r9d, r10d\n"
            "mov r9d, DWORD PTR[r9]\n" // R[m + j]

            "mov r8, %1\n"
            "mov r10d, esi\n"
            "add r10d, ebx\n"
            "add r10d, ecx\n"
            "shl r10d, 2\n"
            "add r8d, r10d\n"
            "mov DWORD PTR[r8], r9d\n" // OUT[l + i + j] = R[m + j]

            "inc ecx\n"

            // jump back to fill_r
            "jmp fill_r\n"

            "copy_out_to_in:\n"

            "xor r8, r8\n"
            "mov r8d, edi\n"
            "sub r8d, esi\n" // r8 = r - l            
            "xor rcx, rcx\n"    

            "copy_array_back_start:\n"

            "cmp ecx, r8d\n"
            "jge copy_array_back_end\n"
            "mov r10d, esi\n"
            "add r10d, ecx\n"
            "mov ebx, DWORD PTR [%1 + r10*4]\n"
            "mov DWORD PTR [%2 + r10*4], ebx\n"
            "inc ecx\n"
            "jmp copy_array_back_start\n"
            "copy_array_back_end:\n"

            "merge_cleanup:\n"

            // restore rbp and rsp
            "mov rsp, rbp\n"
            "pop rbp\n"

            // end merge
            "ret\n"            
            
            "end:\n"

            ".att_syntax\n"
            : "+r" (returnSize)
            : "r" (out), "r" (nums), "r" (numsSize)
            : "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10");

            return nums;
}

int main(){
    int numsSize = 6;
    int nums[] = {6, 5, 4, 3, 2, 1};
    int returnSize;
    
    int* out = sortArray(nums, numsSize, &returnSize);
    for(int i = 0; i < numsSize; i++){
        printf("%d, ", nums[i]);
    }
    printf("\n");
}


void print_register_value(int value) {
    puts("At least we made it here");

    printf("Value in rax: %x\n", value);
}