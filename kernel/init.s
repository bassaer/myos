#.equ        CYLS,        0xff0  # ブートセクタ
#.equ        LEDS,        0xff1  # 謎
#.equ        VMODE,       0xff2  # 色に関する情報
#.equ        SCRNX,       0xff4  # 解像度 X
#.equ        SCRNY,       0xff6  # 解像度 Y
#.equ        VRAM,        0xff8  # グラフィックバッファの開始番地
#
    .text
    .code32
#    movb    $0x13,       %al
#    movb    $0x00,       %ah
#    int     $0x10
#    movb    $0x08,       (VMODE)
#    movw    $320,        (SCRNX)
#    movw    $200,        (SCRNY)
#    movw    $0x000a0000, (VRAM)
#

#
# キービードの状態をBIOSから取得
#keystatus:
#    movb    $0x02,       %ah
#    int     $0x16
#    movb    $al          (LEDS)
#    movw    $init_msg,  %si

print:
    movb    (%si),      %al
    add     $1,         %si
    cmpb    $0,         %al
    je      fin
    movb    $0x0e,      %ah
    movw    $15,        %bx
    int     $0x10             # call Video BIOS
    jmp     print

fin:
    hlt
    jmp     fin

init_msg:
    .ascii "start kernel\r\n"
    .byte    0
