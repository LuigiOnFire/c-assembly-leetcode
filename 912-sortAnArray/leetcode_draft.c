#include <stdio.h>
#include <stdlib.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

// cheat sheet
// nums rdi
// numSize rsi
// returnSize rdx
// out rcx

__attribute__ ((naked)) void start_merge(int* nums, int numsSize, int* returnSize, int* out){
__asm__(R"(
    .intel_syntax noprefix
            mov DWORD PTR[rdx], esi # numsize into return size

            push rax
            push rbx
            push rcx
            push rdx
            push rsi
            push rdi        
            push r8
            push r9
            push r10
            push r11            

            # make rax be nums
            mov rax, rdi
        

            # out will be in r11
            mov r11, rcx

            mov rdi, rsi # this moves numsSize into edi
            mov esi, 0

            push rdi
            push rsi
            call merge_sort 
            pop rsi
            pop rdi           

            jmp end
            
            ret

            merge_sort:
            push rbp
            mov rbp, rsp
            mov esi, DWORD PTR [rbp + 0x10] # this is l
            mov edi, DWORD PTR [rbp + 0x18] # this is r
            mov ebx, edi
            sub ebx, esi
            cmp ebx, 1 # if r - l <= 1
            jle merge_sort_end
            mov edx, esi
            add edx, edi
            sar edx, 1 # edx contains m = (l + r) / 2

            push rdi # this one is not used 
            push rdx # this will be the new r
            push rsi # this will be the new l
            call merge_sort
            pop rsi
            pop rdx
            pop rdi

            push rsi # this one is not used 
            push rdi # this will be the new r
            push rdx # this will be the new l
            call merge_sort
            pop rdx
            pop rdi
            pop rsi

            push rdi
            push rsi
            
            call merge
            pop rsi
            pop rdi
            
            merge_sort_end: 

            mov rsp, rbp
            pop rbp
            ret

            merge:

            push rbp
            mov rbp, rsp

            call check_vitals

            mov esi, DWORD PTR [rbp + 0x10] # new l
            mov edi, DWORD PTR [rbp + 0x18] # new r
            
            
            # get new m
            mov edx, esi
            add edx, edi
            sar edx, 1 # new m

            # save size of L = m - l
            xor ebx, ebx
            mov ebx, edx
            sub ebx, esi
            push rbx

            # save size of R = r - m
            xor ebx, ebx
            mov ebx, edi
            sub ebx, edx
            push rbx

            # # move each element from A[l..r]
            xor r8d, r8d
            mov r8d, edi
            sub r8d, esi # r8 = r - l

            xor ecx, ecx
            copy_array_start:            
            cmp ecx, r8d
            jg copy_array_end            

            # mov ebx, DWORD PTR [rax + rcx*4]
            # mov QWORD PTR [r11 + rcx*4], rbx
            inc ecx
            jmp copy_array_start
            copy_array_end:

            # restore rbp and rsp
            mov rsp, rbp
            pop rbp                        

            # end merge
            ret            

            end:        

            pop r11
            pop r10
            pop r9
            pop r8
            pop rdi
            pop rsi
            pop rdx
            pop rcx            
            pop rbx       
            pop rax     

            ret

            .att_syntax prefix
)");
}

int* sortArray(int* nums, int numsSize, int* returnSize){
    int* out = malloc(numsSize*sizeof(int));
    start_merge(nums, numsSize, returnSize, out);
    return out;
}


void print_register_value(int val1) {
    puts("At least we made it here");

    printf("Value in rdi?: %x,", val1);    
}

void check_vitals(){
    puts("It's still alive.");
}


int main(){
    int numsSize = 6;
    int nums[] = {-6, 5, 4, 3, 2, 1};
    int returnSize = 0;
    
    int* out = sortArray(nums, numsSize, &returnSize);
    for(int i = 0; i < returnSize; i++){
        printf("%d, ", nums[i]);
    }
    printf("\n");
}