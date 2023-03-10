.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# pi1 -> %edi | -8(%rbp)
# pi2 -> %esi | -12(%rbp)
# pa3 -> %rdx | -24(%rbp)
subq $32, %rsp

cmpl $0, %edi
je .if0
# salvando registradores caller saved
movl %edi, -8(%rbp)
movl %esi, -12(%rbp)
movq %rdx, -24(%rbp)

# passando parâmetros para função
movl -12(%rbp), %edi
movq -24(%rbp), %rsi

call f2

# recuperando registradores caller saved
# pi1 -> %edi | -8(%rbp)
movl -8(%rbp), %edi
# pi2 -> %esi | -12(%rbp)
movl -12(%rbp), %esi
# pa3 -> %rdx | -24(%rbp)
movq -24(%rbp), %rdx

# return
movl %eax, -4(%rbp)
.if0:

cmpl $0, %edi
je .if1
movl -4(%rbp), %eax
leave
ret
.if1:

movl %esi, %eax
leave
ret


.globl f2
f2:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# vi2: -8
# pi1 -> %edi | -12(%rbp)
# pa2 -> %rsi | -24(%rbp)
subq $32, %rsp

movl %edi, -4(%rbp)

leaq (%rsi), %rcx
movq $0, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl (%rdx), %edx
movl %edx, -8(%rbp)

movl -4(%rbp), %ecx
addl -8(%rbp), %ecx
movl %ecx, -4(%rbp)

leaq (%rsi), %rcx
movq $1, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl (%rdx), %edx
movl %edx, -8(%rbp)

movl -4(%rbp), %ecx
addl -8(%rbp), %ecx
movl %ecx, -4(%rbp)

leaq (%rsi), %rcx
movq $2, %rdx
imulq $4, %rdx
addq %rcx, %rdx
movl (%rdx), %edx
movl %edx, -8(%rbp)

movl -4(%rbp), %ecx
addl -8(%rbp), %ecx
movl %ecx, -4(%rbp)

movl -4(%rbp), %eax
leave
ret


