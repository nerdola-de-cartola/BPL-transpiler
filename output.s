.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# va2: -44
# pa1 -> %rdi | -56(%rbp)
# pa2 -> %rsi | -64(%rbp)
# pi3 -> %edx | -68(%rbp)
subq $80, %rsp

movl -4(%rbp), %eax
addl $1, %eax
movl %eax, -4(%rbp)

leaq -44(%rbp), %rax
movq $5, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl $2, (%rcx)


leaq -44(%rbp), %rax
movq $8, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl (%rcx), %ecx
movl %ecx, -4(%rbp)

movl %edi, %eax
leave
ret

