.code32
.global io_cli, io_sti, io_stihlt, io_hlt
.global load_gdtr, load_idtr, outb_p, io_in
.global asm_handle_intr
.extern handle_intr
.text

io_hlt:
    hlt
    jmp     io_hlt

io_sti:
    sti
    ret

io_stihlt:
    sti
    hlt
    ret

io_cli:
    cli
    ret

load_gdtr:
    movw    4(%esp),    %ax
    movw    %ax,        6(%esp)
    lgdt    6(%esp)
    ret


load_idtr:
    movw    4(%esp),    %ax
    movw    %ax,        6(%esp)
    lidt    6(%esp)
    ret

# void outb_p(int port, int data)
outb_p:
    movl    4(%esp),    %edx      # port
    movb    8(%esp),    %al       # data
    outb    %al,        %dx
    ret

io_in:
    movl    4(%esp),    %edx
    movl    $0,         %eax
    inb     %dx,        %al
    ret

asm_handle_intr:
    pushw   %es
    pushw   %ds
    pusha
    movl    %esp,       %eax
    push    %eax
    movw    %ss,        %ax
    movw    %ax,        %ds
    movw    %ax,        %es
    call    handle_intr
    popl    %eax
    popa
    popw    %ds
    popw    %es
    iret
