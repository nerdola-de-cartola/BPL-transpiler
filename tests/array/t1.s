.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# va2: -44
# pi1 -> %edi | -48(%rbp)
# pa2 -> %rsi | -56(%rbp)
subq $64, %rsp

leaq -44(%rbp), %rcx
movq $5, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl $2, (%rdx)


leaq -44(%rbp), %rcx
movq $8, %rdx
imulq $4, %rdx
addq %rcx, %rdx

leave
ret

