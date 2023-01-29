.text
.globl f1
f1:

pushq %rbp
movq %rsp, %rbp

# pi1 -> %edi | -4(%rbp)
# pa2 -> %rsi | -16(%rbp)
subq $16, %rsp

movl %edi, %eax

leave
ret

.globl f2
f2:

pushq %rbp
movq %rsp, %rbp

# vi1: -4
# vi2: -8
# va3: -128
# pa1 -> %rdi | -136(%rbp)
subq $144, %rsp

movl $1, -4(%rbp)

# salvando registradores caller saved
movq %rdi, -136(%rbp)

# passando parâmetros para função
movl -4(%rbp), %edi
leaq -128(%rbp), %rsi

call f1

# recuperando registradores caller saved
# pa1 -> %rcx | -136(%rbp)
movq -136(%rbp), %rcx

# return
movl %eax, -8(%rbp)

# salvando registradores caller saved
movq %rcx, -136(%rbp)

# passando parâmetros para função
movl $5, %edi
leaq -136(%rbp), %rsi

call f1

# recuperando registradores caller saved
# pa1 -> %rcx | -136(%rbp)
movq -136(%rbp), %rcx

# return
movl %eax, -8(%rbp)

movl -8(%rbp), %eax

leave
ret

