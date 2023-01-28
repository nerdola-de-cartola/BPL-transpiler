.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# va2: -44
# va3: -52
# pi1 -> %edi | -56(%rbp)
# pi2 -> %esi | -60(%rbp)
# pa3 -> %rdx | -72(%rbp)
subq $80, %rsp

leaq -44(%rbp), %rax
movq $5, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl $2, (%rcx)


leaq -44(%rbp), %rax
movq $10, %rcx
imulq $4, %rcx
addq %rax, %rcx

leaq -52(%rbp), %rax
movq $10, %rcx
imulq $4, %rcx
addq %rax, %rcx

leave
ret

