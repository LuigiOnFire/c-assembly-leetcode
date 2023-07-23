;godbolt code to make an array
mov     eax, DWORD PTR [rbp-36] ;move num into eax 
movsx   rdx, eax                ;copy AND sign extend eax into rdx
sub     rdx, 1                  ;subtract one from rdx
mov     QWORD PTR [rbp-8], rdx  ;saves nums - 1 near the base pointer
cdqe                            ;sign extend eax into rax
                                ;now rax contains num and rdx contains num-1
lea     rdx, [0+rax*4]          ;now rdx continas nums*4
mov     eax, 16                 ;eax contains 16  
sub     rax, 1                  ;rax contains 15
add     rax, rdx                ;rax contains 15 + nums*4
mov     esi, 16                 ;esi contains 16
mov     edx, 0                  ;edx contains 9
div     rsi                     ;divides rax by rsi and stores the remainder in rdx
                                ;basically rax = rax/rsi 
                                ;rax = (15 + nums * 4) / 16
imul    rax, rax, 16            ;multiplies rax by 16, seemingly undoing the previous instruction?
sub     rsp, rax                ;subtracts rax from rsp i.e. rsp = rsp - 15 * nums * 4 (this moves the stack pointer)
mov     rax, rsp                ;rax now contains what rsp has (saving the current stack pointer)
add     rax, 3                  ;rax = rax + 3
shr     rax, 2                  ;rax = rax / 4
sal     rax, 2                  ;rax = rax * 4
mov     QWORD PTR [rbp-16], rax ;saves the value of rax

;summarized my way
xor     rax, rax
mov     eax, DWORD PTR [rbp-36] ;move num into eax 
sub     rsp, rax                ;subtracts rax from rsp i.e. rsp = rsp - 15 * nums * 4 (this moves the stack pointer)