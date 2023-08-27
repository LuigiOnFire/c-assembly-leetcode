#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

__attribute__ ((naked)) struct TreeNode* invertTree(struct TreeNode* root){

    __asm__(R"(
    .intel_syntax noprefix    

    # rdi contains our root

    invert_tree:

    cmp rdi, 0
    je skip

    mov rcx, QWORD PTR [rdi + 0x8]
    mov rdx, QWORD PTR [rdi + 0x10]

    mov QWORD PTR [rdi + 0x8], rdx
    mov QWORD PTR [rdi + 0x10], rcx

    push rdi
    mov rdi, QWORD PTR [rdi + 0x8]    
    call invert_tree
    pop rdi
    push rdi
    mov rdi, QWORD PTR [rdi + 0x10]
    call invert_tree
    pop rdi    

    mov rax, rdi

    skip:
    ret 

    .att_syntax prefix
)");
}

int main(){
    TreeNode* root = (TreeNode*)malloc(sizeof(struct TreeNode));

//    TreeNode* a = (TreeNode*)malloc(sizeof(struct TreeNode));
//    TreeNode* b = (TreeNode*)malloc(sizeof(struct TreeNode));
//    TreeNode* c = (TreeNode*)malloc(sizeof(struct TreeNode));
//    TreeNode* d = (TreeNode*)malloc(sizeof(struct TreeNode));

//    root->left = a;
//    root->right = b;

//    a->left = c;
//    a->right = d;
//    b->left = NULL;
//    b->right = NULL;
//    c->left = NULL;
//    c->right = NULL;
//    d->left = NULL;
//    d->right = NULL;

//    root->val = 0;
//    a->val = 1;
//    b->val = 2;
//    c->val = 3;
//    d->val = 4;
   root = NULL;
   root = invertTree(root);
   return 0;    
}