; myos

    .org    0x7c00              ; 読み込み開始地点

; FAT12 format for floppy disk

    jmp     entry
    .byte   0x90
    .ascii  "MYOSIPL "
    .word   512
    .byte   1
    .word   1
    .byte   2
    .word   224
    .word   2880
    .byte   0xf0
    .word   9
    .word   18
    .word   2
    .long   0
    .long   2880
    .byte   0,0,0x29
    .long   0xffffffff
    .byte   "MYOS       "
    .byte   "FAT12   "
    .org    .+18


;============================================
; 各レジスタ                                ;     
; ax : accumulator                          ;
; cx : counter                              ;
; dx : data                                 ;
; bx : base                                 ;
; sp : stack pointer                        ;
; bp : base pointer                         ;
; si : source index (読み込みindex)         ;
; di : destination index (書き込みindex)    ;
;============================================

entry:
    movl    $0,         %ax
    movl    %ax,        %ss
    movl    $0x7c00,    %sp
    movl    %ax,        %ds
    movl    %ax,        %ew

    movl    msg, %si

putloop:
    movl    (%si),      %al
    add     $1,         %si
    compl   $0,         %al
    je      fin                 ; if %al == 0
    movl    $0x0e,      %ah
    movl    %15,        %bx
    int     $0x10               ; call Video BIOS
    jmp     putloop

fin:
    hlt
    jmp     fin

msg:
    .byte   0x0a, 0x0a          ; two newlines
    .ascii  "hello, world"
    .byte   0x0a                ; newline
    .byte   0

    .org    0x1fe ; jump to 0x001fe

    .byte   0x55, 0xaa

; bootsector 以外
        
    .byte   0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    .org    .+4600
    .byte   0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
    .org    .+1469432
