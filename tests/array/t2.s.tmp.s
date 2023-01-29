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

leaq -44(%rbp), %rcx
movq $5, %r9
imulq $4, %r9
addq %rcx, %r9
movl $2, (%r9)


leaq -44(%rbp), %rcx
movq $10, %r9
imulq $4, %r9
addq %rcx, %r9

leaq -52(%rbp), %rcx
movq $10, %r9
imulq $4, %r9
addq %rcx, %r9

leave
ret

