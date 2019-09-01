    .code16
    .text
    .global main

    movw    $init_msg,  %si
    call    print
    pushl   $main
    movw    $done_msg,  %si
    call    print

fin:
    hlt
    jmp     fin

print:
    movb    (%si),      %al
    add     $1,         %si
    cmpb    $0,         %al
    je      print_ret
    movb    $0x0e,      %ah
    movw    $15,        %bx
    int     $0x10             # call Video BIOS
    jmp     print

print_ret:
    ret

init_msg:
    .string "Start kernel\r\n"

done_msg:
    .string "done.\r\n"
