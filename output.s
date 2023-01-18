# vi1: -4
# vi2: -8
# vi3: -12
subq $16, %rsp


movl $9, -4(%rbp)

movl $2, %eax
addl -8(%rbp), %eax
movl %eax, -4(%rbp)

movl $4, %eax
subl -8(%rbp), %eax
movl %eax, -4(%rbp)

movl $8, %eax
imull -8(%rbp), %eax
movl %eax, -12(%rbp)

movl -12(%rbp), %eax
movl $9, %ecx
cltd
idiv %ecx
movl %eax, -4(%rbp)

.text
call .f1
   