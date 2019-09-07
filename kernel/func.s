.code32
.global hlt, load_gdtr, load_idtr
.text

hlt:
    hlt
    jmp     hlt


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
