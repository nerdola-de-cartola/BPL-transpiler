.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# vi2: -8
subq $16, %rsp

leave
ret

