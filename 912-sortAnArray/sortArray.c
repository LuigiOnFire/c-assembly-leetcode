#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* sortArray(int* nums, int numsSize, int* returnSize){
    int* out = malloc(numsSize*sizeof(int));
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

            "push rdi\n"
            "push rdx\n"
            "push rsi\n"
            "call merge_sort\n"
            "pop rsi\n"
            "pop rdx\n"
            "pop rdi\n"

            "inc rdx\n" //m = m + 1

            "push rsi\n"
            "push rdi\n"
            "push rdx\n"
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
            : "rax", "rbx", "rcx", "rdx", "rsi", "rdi");

            return nums;
}

int main(){
    printf("Hello World!");
    int numsSize = 1;
    //int nums[] = {0, 1, 2, 3};
    int nums[] = {0};
    int returnSize;
    
    int* out = sortArray(nums, numsSize, &returnSize);
    for(int i = 0; i < numsSize; i++){
        printf("%d, ", out[i]);
    }
    printf("\n");
}