.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# pi1 -> %edi | -8(%rbp)
# pa2 -> %rsi | -16(%rbp)
subq $16, %rsp

# salvando registradores caller saved
movl %edi, -8(%rbp)
movq %rsi, -16(%rbp)

# passando parâmetros para função
movl -8(%rbp), %edi
movq -16(%rbp), %rsi

call f2

# recuperando registradores caller saved
# pi1 -> %ecx | -8(%rbp)
movl -8(%rbp), %ecx
# pa2 -> %rdx | -16(%rbp)
movq -16(%rbp), %rdx

# return
movl %eax, -4(%rbp)

movl -4(%rbp), %eax
leave
ret


.globl f2
f2:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# pi1 -> %edi | -8(%rbp)
# pa2 -> %rsi | -16(%rbp)
subq $16, %rsp

leaq (%rsi), %rcx
movq $5, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl %edi, (%rdx)

leaq (%rsi), %rcx
movq $5, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl (%rdx), %edx
movl %edx, -4(%rbp)

movl -4(%rbp), %ecx
addl %edi, %ecx
movl %ecx, -4(%rbp)

movl -4(%rbp), %eax
leave
ret


