#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* sortArray(int* nums, int numsSize, int* returnSize){
    int* out = malloc(numsSize*sizeof(int));
    __asm__ volatile(".intel_syntax noprefix\n"
            "mov eax, %2\n"
            "mov [%3], eax\n"
            "xor rcx, rcx\n"
            "xor rsi, rsi\n"
            "xor rdi, rdi\n"
            // rax will remain the start of our array
            "mov rax, %1\n"

            "mov esi, 0\n"
            "mov edi, %2\n"
            "push rsi\n"
            "push rdi\n"
            "call merge_sort\n" 

            "merge_sort:\n"\
            "push rbp\n"
            "mov rbp, rsp\n"
            "mov rsi, QWORD PTR [rbp + 0x18]\n" // this is l
            "mov rdi, QWORD PTR [rbp + 0x10]\n" // this is r
            "cmp rsi, rdi\n" // if l < r
            "jl merge_sort_end\n"
            "mov rdx, rsi\n"
            "add rdx, rdi\n"
            "shr rdx, 1\n" // rdx contains m = (l + r) / 2

            "push rsi\n"
            "push rdx\n"
            "call merge_sort\n"

            "inc rdx\n" //m = m + 1
            "push rdx\n"
            "push rdi\n"
            "call merge_sort\n"
            "pop rdi\n"
            "pop rdx\n"

            "push rsi\n"
            "push rdi\n"
            "call merge\n"
            "pop rdi\n"
            "pop rsi\n"
            "pop rbp\n"
            "merge_sort_end:" 
            "ret\n"

            "merge: \n"
           
            "ret\n"
            ".att_syntax\n"
            : "+r" (out)
            : "r" (nums), "r" (numsSize), "r" (returnSize)
            : "rax", "rbx", "rcx", "rdx", "rsi", "rdi");

            return nums;
}

int main(){
    printf("Hello World!");
    int numsSize = 4;
    int nums[] = {0, 1, 2, 3};
    int returnSize;
    
    int* out = sortArray(nums, numsSize, &returnSize);
    for(int i = 0; i < numsSize; i++){
        printf("%d, ", out[i]);
    }
    printf("\n");
}