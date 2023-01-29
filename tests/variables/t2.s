.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# va2: -124
# vi3: -128
# vi2: -132
subq $144, %rsp

movl -4(%rbp), %eax

leave
ret

