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

            "push rdi\n"
            "push rsi\n"
            "call merge\n"
            "pop rsi\n"
            "pop rdi\n"
            
            "merge_sort_end:" 
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
            "mov r8d, edi\n"
            "sub r8d, esi\n" // r8 = r - l
            
            "xor ecx, ecx\n"
            "copy_array_start:\n"            
            "cmp ecx, r8d\n"
            "jg copy_array_end\n"            
            "mov ebx, DWORD PTR [%1 + rcx*4]\n"
            "mov DWORD PTR [%0 + rcx*4], ebx\n"
            "inc ecx\n"
            "jmp copy_array_start\n"
            "copy_array_end:\n"

            // set up i = 0
            "xor rbx, rbx\n"

            // set up j = 0
            "xor rcx, rcx\n"

            "merge_both:\n"

            // if i >= m - l || j >= r - m then jump to fill_l

            

            //"cmp\n"

            // Do L if L[i] >= R[j] jump to doing R instead

            // nums[l + i + j] = L[l + i]

            // i++

            // jump back to merge both

            "merge_r:\n"

            // nums[l + i + j] = R[l + j]

            // j++

            // jump back to merge both
            
            "fill_l:\n"

            // if i >= m - l jump to fill r

            // nums[l + i + j] = L[i]

            // jump back to fill_l

            "fill_r:\n"

            // if j >= n - l jump to merge_cleanup

            // nums[l + i + j] = R[j]

            // jump back to fill_r

            "merge_cleanup:\n"

            // restore rbp and rsp

            "move rsp, rbp\n"
            "pop rbp\n"

            "ret\n"
            "init:\n"

            "mov eax, %2\n"
            "mov [%3], eax\n"
            "xor rcx, rcx\n"
            "xor rsi, rsi\n"
            "xor rdi, rdi\n"
            // rax will remain the start of our array
            "mov rax, %1\n"

            "mov esi, 0\n"
            "mov edi, %2\n"
            "sub edi, 1\n"
            "push rdi\n"
            "push rsi\n"
            "call merge_sort\n" 

            "pop rsi\n"
            "pop rdi\n"
            
            ".att_syntax\n"
            : "+r" (out)
            : "r" (nums), "r" (numsSize), "r" (returnSize)
            : "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8");

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