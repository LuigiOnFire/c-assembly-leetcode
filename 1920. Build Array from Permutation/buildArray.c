#include <stdio.h>
#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* buildArray(int* nums, int numsSize, int* returnSize){
    int* out = malloc(numsSize*sizeof(int));
    __asm__ volatile(".intel_syntax noprefix\n"
            "mov eax, %2\n"
            "mov [%3], eax\n"
            "xor ecx, ecx\n"
            "loop:\n"
            "mov ebx, DWORD PTR [%1 + rcx*4]\n" // Now we have nums[i] in ebx
            "mov eax, DWORD PTR [%1 + rbx*4]\n" // now we have nums[nums[i]] in eax
            "mov DWORD PTR[%0 + rcx*4], eax\n"
            "inc ecx\n"
            "cmp ecx, %2\n" 
            "jl loop\n"
            "stop:"
            ".att_syntax\n"
            : "+r" (out)
            : "r" (nums), "r" (numsSize), "r" (returnSize)
            : "rax", "rbx", "rcx", "rdx");

            return out;
}

int main(){
    printf("Hello World!");
    int numsSize = 0;
    int nums[] = {};
    int returnSize;
    
    int* out = buildArray(nums, numsSize, &returnSize);
    for(int i = 0; i < numsSize; i++){
        printf("%d, ", out[i]);
    }
    printf("\n");
}