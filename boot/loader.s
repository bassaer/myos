.equ KERNEL,   0x00280000
.equ DSKCAC,   0x00100000
.equ DSKCAC0,  0x00008000
.equ CYLS,     0x0ff0
.equ LEDS,     0x0ff1

.text
.code16

    movw    $init_msg,  %si
    call    print

keystatus:
    movb $0x02, %ah
    int  $0x16
    movb %al, (LEDS)

    call    waitkbout
    movb    $0xd1,     %al
    outb    %al,       $0x64
    call    waitkbout
    movb    $0xdf,     %al
    outb    %al,       $0x60
    cli
    call    waitkbout

.arch i486
    lgdtl   (GDTR0)
    movl    %cr0,       %eax
    andl    $0x7fffffff, %eax
    orl     $0x00000001, %eax
    movl    %eax, %cr0
    jmp     pipelineflush

pipelineflush:
    movw $1*8, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    movl $kernel_entry, %esi
    movl $KERNEL, %edi
    movl $512*1024/4, %ecx
    call memcpy

    movl $0x7c00, %esi
    movl $DSKCAC, %edi
    movl $512/4, %ecx
    call memcpy

    movl $DSKCAC0+512, %esi
    movl $DSKCAC+512, %edi
    movl $0x00, %ecx
    movb (CYLS), %cl
    imull $512*18*2/4, %ecx
    subl $512/4, %ecx
    call memcpy

    ljmpl   $2*8, $KERNEL

waitkbout:
    inb     $0x64,      %al
    andb    $0x02,      %al
    jnz     waitkbout
    ret

memcpy:
    movl (%esi), %eax
    addl $4, %esi
    movl %eax, (%edi)
    addl $4, %edi
    subl $1, %ecx
    jnz  memcpy
    ret

print:
    movb    (%si),      %al
    add     $1,         %si
    cmpb    $0,         %al
    je      return
    movb    $0x0e,      %ah
    movw    $15,        %bx
    int     $0x10
    jmp     print

return:
    ret

GDT0:
    .skip   8, 0x00
    .word   0xffff, 0x0000, 0x9200, 0x00cf
    .word   0xffff, 0x0000, 0x9a00, 0x00cf
    .word   0x0000

GDTR0:
    .word   8*3-1
    .int    GDT0

init_msg:
    .string "second loader..\r\n"

kernel_entry:
