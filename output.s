.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# va2: -44
# pa1 -> %rdi | -56(%rbp)
# pi2 -> %esi | -60(%rbp)
subq $64, %rsp

movl $3, -4(%rbp)

cmpl $0, -4(%rbp)
je .if0
movl $0, -4(%rbp)
.if0:

cmpl $0, $10
je .if1
leaq -44(%rbp), %rax
movq $5, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl $2, (%rcx)
.if1:

cmpl $0, %esi
je .if2
.if2:

leave
ret

