movl ci9, vi1

movl vi2, %eax
addl ci2, %eax
movl %eax, vi1

movl vi2, %eax
subl ci4, %eax
movl %eax, vi1

movl vi2, %eax
imull ci8, %eax
movl %eax, vi1

movl vi2, %eax
movl ci9, %ecx
cltd
idiv %ecx
movl %eax, vi1

.text
call .f1
