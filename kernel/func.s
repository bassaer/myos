.code32
.global hlt, load_gdtr, load_idtr, outb_p
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

# void outb_p(int port, int data)
outb_p:
    movl    4(%esp),    %edx      # port
    movb    8(%esp),    %al       # data
    outb    %al,        %dx
    ret
