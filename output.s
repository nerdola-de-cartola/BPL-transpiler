# vi1: -4
# va2: -44
# vi3: -48
subq $48, %rsp


leaq -44(%rbp), %rax
movq $5, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl $2, (%rcx)

leaq -44(%rbp), %rax
movq $5, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl (%rcx), %ecx
movl %ecx, -4(%rbp)

leaq -44(%rbp), %rax
movq $10, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl -4(%rbp), %edx
movl %edx, (%rcx)

leaq -44(%rbp), %rax
movq $10, %rcx
imulq $4, %rcx
addq %rax, %rcx
movl (%rcx), %ecx
movl %ecx, -48(%rbp)

.text
call .f1
