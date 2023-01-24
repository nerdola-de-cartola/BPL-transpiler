# vi1: -4
# va2: -44
subq $48, %rsp

movl $3, -4(%rbp)

cmpl $0, -4(%rbp)
je .if0
movl $0, -4(%rbp)
.if0:

cmpl $0, -4(%rbp)
je .if1
movl $11, -4(%rbp)
.if1:

cmpl $0, -4(%rbp)
je .if2
movl $22, -4(%rbp)
.if2:

.text
call .f1
