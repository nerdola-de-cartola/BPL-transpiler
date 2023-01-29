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

movl -4(%rbp), %ecx
addl $1, %ecx
movl %ecx, -4(%rbp)

leaq -44(%rbp), %rcx
movq $5, %r9
imulq $4, %r9
addq %rcx, %r9
movl $2, (%r9)


leaq -44(%rbp), %rcx
movq $8, %r9
imulq $4, %r9
addq %rcx, %r9
movl (%r9), %r8d
movl %r8d, -4(%rbp)

movl %edi, %eax

leave
ret

