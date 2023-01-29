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

movl $10, %ecx
cmpl $0, %ecx
je .if1
leaq -44(%rbp), %rdx
movq $5, %r9
imulq $4, %r9
addq %rdx, %r9
movl $2, (%r9)
.if1:

cmpl $0, %esi
je .if2
movl $-10, %eax
.if2:

leave
ret

