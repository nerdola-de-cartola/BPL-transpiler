.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# va2: -44
subq $48, %rsp

leaq -44(%rbp), %rcx
movq $5, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl $-7, (%rdx)

leaq -44(%rbp), %rcx
movq $5, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl (%rdx), %edx
movl %edx, -4(%rbp)

movl -4(%rbp), %eax
leave
ret


