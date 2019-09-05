.code32
.global io_hlt, load_gdtr, load_idtr
.text


io_hlt:
    hlt
    jmp     io_hlt


load_gdtr:
    movw    4(%esp),    %ax
    movw    %ax,        6(%esp)
    lgdt    6(%esp)
    ret


load_idtr:
    movw    4(%esp),    %ax
    movw    %ax,        6(%esp)
    lgdt    6(%esp)
    ret
