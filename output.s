.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# vi2: -8
# vi3: -12
# pi1 -> %edi | -16(%rbp)
# pi2 -> %esi | -20(%rbp)
# pi3 -> %edx | -24(%rbp)
subq $32, %rsp

movl $1, -4(%rbp)

movl -4(%rbp), %eax
movl %eax, -8(%rbp)

movl %edi, %eax
addl -8(%rbp), %eax
movl %eax, -4(%rbp)

movl -4(%rbp), %eax
imull $-5, %eax
movl %eax, -8(%rbp)

movl -4(%rbp), %eax
subl %edi, %eax
movl %eax, -4(%rbp)

movl %edi, %eax
movl -8(%rbp), %ecx
cltd
idiv %ecx
movl %eax, -4(%rbp)

leave
ret

