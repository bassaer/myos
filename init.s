.file   "init.s"
.data
msg:    .ascii  "hello world\n"
msgend: .equ    len, msgend - msg

.bss
.text
.globl main
main:
        movl $4,        %eax # write system call(sys_write)
        movl $1,        %ebx # arg1:stdout
        movl $msg,      %ecx # arg2:str
        movl $len,      %edx # arg3:strlen
        int  $0x80           # system call
        ret
