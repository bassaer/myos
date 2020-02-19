.code32
.global io_cli, io_sti, io_stihlt, io_hlt
.global load_gdtr, load_idtr, outb_p, io_in
.global asm_handle_intr, asm_handle_intr27
.global io_load_eflags, io_store_eflags, load_cr0, store_cr0
.extern handle_keyboard, handle_intr27
.text

io_hlt:
    hlt
    ret

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

io_load_eflags:
    pushf
    popl    %eax
    ret

io_store_eflags:
    movl    4(%esp),     %eax
    pushl   %eax
    popf
    ret

load_cr0:
    movl    %cr0,       %eax     # CR0を返すためにEAXにいれる
    ret

store_cr0:
    movl    4(%esp),    %eax
    movl    %eax,       %cr0      # CR0を値をセット

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
    push    %es
    push    %ds
    pusha
    mov     %esp,       %eax
    push    %eax
    mov     %ss,        %ax
    mov     %ax,        %ds
    mov     %ax,        %es
    call    handle_keyboard
    pop     %eax
    popa
    pop     %ds
    pop     %es
    iret                        # 割り込み処理からの復帰

asm_handle_intr27:
    push    %es
    push    %ds
    pusha
    mov     %esp,       %eax
    push    %eax
    mov     %ss,        %ax
    mov     %ax,        %ds
    mov     %ax,        %es
    call    handle_intr27
    pop     %eax
    popa
    pop     %ds
    pop     %es
    iret
