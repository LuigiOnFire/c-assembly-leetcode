#include <stdio.h>
#include <stdlib.h>

__attribute__ ((naked)) int* twoSum(int* nums, int numsSize, int target, int* returnSize){
__asm__(R"(
    .intel_syntax noprefix    

            # rdi has nums
            # esi has numSize
            # edx has target
            # ecx has returnSize            

            # malloc our tiny new return array
            push rdi
            push rcx # rcx is caller saved
            push rsi # rsi is caller saved
            xor rdi, rdi
            sal edx, 2
            mov edi, edx
            call malloc # just leave our return array in rax
            pop rsi
            pop rcx
            pop rdi

            # sort the input array
            # rdi and rsi get to remain the same
            # rax and rdx are going to get clobbere
            push rax
            push rdx
            mov rdx, rcx # we don't need this because we know our array size, but we need it to be something safe, like a real pointer
            call sort_array
            pop rdx
            pop rax            

            # set up our two iterators
            mov rbx, 0
            mov rcx, rsi

            # start looping
            cmp_loop:

            # make the sum, compare
            mov r8, rdi
            add r8, rbx
            mov r8d, DWORD PTR [r8]

            mov r9, rdi
            add r9, rcx
            mov r9d, DWORD PTR [r9]

            mov r10, r8
            add r10, r9

            cmp r10, rdx

            # if it's even we're done
            je found_match

            # if it's too small, increment the left iterator's register
            jl too_small

            # if it's too big, increment the right iterator's register
            jg too_big

            too_small:
            inc rbx
            jmp cmp_loop

            too_big:
            dec rcx
            jmp cmp_loop

            found_match:
            mov DWORD PTR [rax], ebx
            mov DWORD PTR [rax + 4], ecx

            ret

            sort_array:
            # int* sortArray(int* nums, int numsSize, int* returnSize)
            mov DWORD PTR[rdx], esi # numsize into return size

            push rbx
            push rcx
            push rdx
            push rsi
            push rdi
            push r8
            push r9
            push r10
            push r11

            # use rbx as temporary register to hold nums
            lea rbx, [rdi]

            # make a new malloc call to get out array
            # then put that array in r11
            push rsi
            lea rdi, [rsi]
            sal rdi, 2 # multiplies by 4, which is size of int
            call malloc
            pop rsi
            lea r11, [rax]

            mov rdi, rsi
            mov esi, 0

            # move nums back into rax
            mov rax, rbx

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

            merge_cleanup:

            # restore rbp and rsp
            mov rsp, rbp
            pop rbp                        

            # end merge
            ret            
            
            end:

            # at the end rax is nums, which is what we want to return anyway
            pop r11
            pop r10
            pop r9
            pop r8
            pop rdi
            pop rsi
            pop rdx
            pop rcx            
            pop rbx       

            ret

            .att_syntax prefix
)");
}

int main(){
    int numsSize = 3;
    int nums[] = {1, 1, 0};
    int target = 2;
    int returnSize;
    
    int* out = twoSum(nums, numsSize, target, &returnSize);
    for(int i = 0; i < returnSize; i++){
        printf("%d, ", nums[i]);
    }
    printf("\n");
}
