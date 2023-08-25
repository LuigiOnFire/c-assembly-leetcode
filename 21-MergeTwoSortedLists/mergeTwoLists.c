#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

__attribute__ ((naked)) struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2){
__asm__(R"(
    .intel_syntax noprefix    
            push rbx     
            push rdi
            push rsi       

            # find which one is smaller so we can put that (and leave it) in rax            
            mov ebx, DWORD PTR[rdi]
            mov ecx, DWORD PTR[rsi]
            
            cmp ebx, ecx
            jl second_is_bigger
            mov rax, rdi
            mov rdi, QWORD PTR[rdi + 8]

            jmp found_larger

            second_is_bigger:
            mov rax, rsi
            mov rsi, QWORD PTR[rsi + 8]

            found_larger:
            mov rbx, rax #rbx will hold the previous head


            start_loop:
            cmp rdi, 0
            je stop_looping

            cmp rsi, 0  
            je stop_looping                    

            mov ecx, DWORD PTR [rdi]
            mov edx, DWORD PTR [rsi]

            cmp ecx, edx
            jg append_second

            mov QWORD PTR [rbx + 8], rdi
            mov rbx, QWORD PTR [rbx + 8]
            mov rdi, QWORD PTR [rbx + 8] # stopped here, but not sure this makes sense
            jmp start_loop

            append_second:
            mov QWORD PTR [rbx + 8], rsi
            mov rbx, QWORD PTR [rbx + 8]
            mov rsi, QWORD PTR [rbx + 8]
            jmp start_loop

            stop_looping: # now fill from left
            cmp rdi, 0
            je fill_from_second

            mov QWORD PTR [rbx + 8], rdi
            mov rbx, QWORD PTR [rbx + 8]
            mov rdi, QWORD PTR [rbx + 8] # stopped here, but not sure this makes sense
            
            jmp stop_looping

            fill_from_second:   
            cmp rsi, 0
            je done

            mov QWORD PTR [rbx + 8], rsi
            mov rbx, QWORD PTR [rbx + 8]
            mov rsi, QWORD PTR [rbx + 8]

            jmp fill_from_second

            done:

            pop rsi
            pop rdi
            pop rbx       

            ret

            .att_syntax prefix
)");
}

int main(){

   ListNode* list1 = (ListNode*)malloc(sizeof(struct ListNode));
   ListNode* list2 = (ListNode*)malloc(sizeof(struct ListNode));
   
   ListNode* a = (ListNode*)malloc(sizeof(struct ListNode));
   ListNode* b = (ListNode*)malloc(sizeof(struct ListNode));
   ListNode* c = (ListNode*)malloc(sizeof(struct ListNode));
   ListNode* d = (ListNode*)malloc(sizeof(struct ListNode));

   a->val = 2;
   b->val = 3;
   c->val = 4;
   d->val = 5;

   list1->val = 1;
   list2->val = 8;

   list1->next = a;
   list2->next = b;
   a->next = c;
   b->next = d;
   c->next = NULL;
   d->next = NULL;

   ListNode* output = mergeTwoLists(list1, list2);

   return 0;
}
