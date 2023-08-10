#include <stdio.h>
#include <stdlib.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

void start_merge(int* nums, int numsSize, int* returnSize, int* out);

int* sortArray(int* nums, int numsSize, int* returnSize){
    int* out = malloc(numsSize*sizeof(int));
    start_merge(nums, numsSize, returnSize, out);
    return out;
}

__asm__(R"(
    .intel_syntax noprefix
    start_merge:
            mov DWORD PTR[r8], edx # numsize into return size

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

            # rax is already remain the start of our array


            # out will be in r11
            mov r11, r9

            mov rax, rcx
            mov edi, edx # this moves numsSize into edi
            mov esi, 0
            
            push rdi
            push rsi
            call merge_sort 
            pop rsi
            pop rdi

            jmp end

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
            mov ebx, DWORD PTR [rax + rcx*4]
            mov QWORD PTR [r11 + rcx*4], rbx
            inc ecx
            jmp copy_array_start
            copy_array_end:

            # set up i = 0
            xor ebx, ebx

            # set up j = 0
            xor ecx, ecx

            merge_both:

            # if i >= m - l || j >= r - m then jump to fill_l            

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
            add r8d, r10d
            mov r8d, DWORD PTR[r8] # L[l + i]

            mov r9, rax
            mov r10d, edx
            add r10d, ecx
            sal r10d, 2
            add r9d, r10d
            mov r9d, DWORD PTR[r9] # R[m + j]
        
            cmp r8d, r9d
            jg merge_r

            # nums[l + i + j] = L[l + i]
            mov r9, r11
            mov r10d, esi
            add r10d, ebx
            add r10d, ecx
            sal r10d, 2
            add r9d, r10d
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
            add r8d, r10d
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
            add r8d, r10d
            mov r8d, DWORD PTR[r8] # L[l + i]

            mov r9, r11
            mov r10d, esi
            add r10d, ebx
            add r10d, ecx
            sal r10d, 2
            add r9d, r10d
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
            add r9d, r10d
            mov r9d, DWORD PTR[r9] # R[m + j]

            mov r8, r11
            mov r10d, esi
            add r10d, ebx
            add r10d, ecx
            sal r10d, 2
            add r8d, r10d
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

            merge_cleanup:

            # restore rbp and rsp
            mov rsp, rbp
            pop rbp                        

            # end merge
            ret            
            
            end:

            mov rdx, rdi# rsi should be numsize
            mov r8, r11 #the out put array should be in rcx

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
