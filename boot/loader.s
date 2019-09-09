.equ KERNEL,   0x00280000
.equ DSKCAC,   0x00100000
.equ DSKCAC0,  0x00008000
.equ CYLS,     0x0ff0
.equ LEDS,     0x0ff1

.text
.code16

keystatus:
    movb    $0x02,          %ah
    int     $0x16
    movb    %al,            (LEDS)

# PICが一切割り込みをしないようにする
    movb    $0xff,          %al
    outb    %al,            $0x21     # マスタPICすべてで割り込み禁止
    nop                               # 何もしない命令(1クロックだけ停止)
    outb    %al,            $0xa1     # スレーブPICすべてで割り込み禁止
    cli                               # CPUレベルで割り込み禁止

# CPUから1MB以上のメモリにアクセスできるようにA20GATEを設定
    call    waitkbout
    movb    $0xd1,          %al
    outb    %al,            $0x64
    call    waitkbout
    movb    $0xdf,          %al       # AT20をON
    outb    %al,            $0x60
    cli
    call    waitkbout

# プロテクトモード移行
.arch i486                            # 386以降のキーワードを使用するため
    lgdtl   (GDTR0)                   # 暫定GDTを設定
    movl    %cr0,           %eax      # CR0(control register 0)の先頭を0と末尾を1にする
    andl    $0x7fffffff,    %eax      # bit31を0にする(ページング禁止のため)
    orl     $0x00000001,    %eax      # bit0を1にする(プロテクトモード移行のため)
    movl    %eax,           %cr0      # CR0に値を戻すとプロテクトモードに移行する
    jmp     pipelineflush

# プロテクトモードに入るとセグメントレジスタの意味が変わるので
# 値を0x0000から0x0008に変更
# 0x0008はgdt+1のセグメントに相当
pipelineflush:
    movw    $1*8,           %ax
    movw    %ax,            %ds
    movw    %ax,            %es
    movw    %ax,            %fs
    movw    %ax,            %gs
    movw    %ax,            %ss

# memcpyを使用し、メモリをコピー
# カーネルのアドレスから512KBを0x00280000にコピー
    movl    $kernel_entry,  %esi
    movl    $KERNEL,        %edi
    movl    $512*1024/4,    %ecx
    call    memcpy

# 0x00007c00から512バイトを0x00100000にコピー
# ブートセクタを1MB以降のメモリにコピー
    movl    $0x7c00,        %esi
    movl    $DSKCAC,        %edi
    movl    $512/4,         %ecx
    call    memcpy

# 0x00008200からディスクの内容を0x00100200にコピー
    movl    $DSKCAC0+512,   %esi
    movl    $DSKCAC+512,    %edi
    movl    $0x00,          %ecx
    movb    (CYLS),         %cl
    imull   $512*18*2/4,    %ecx
    subl    $512/4,         %ecx
    call    memcpy

# カーネルの0x10c8から0x11a8までを0x00310000へコピー
    movl    $KERNEL,        %ebx
    movl    $0x11a8,        %ebx
    addl    $3,             %ecx      # ecx += 3;
    shrl    $2,             %ecx      # ecx /= 3;
    jz      skip
    movl    $0x10c8,        %edi
    addl    %ebx,           %esi
    movl    $0x00310000,    %edi
    call    memcpy

# カーネルに移動
skip:
    movl    $0x00310000,    %esp
    ljmpl   $2*8,           $KERNEL   # セグメント2(カーネル)に移動

# キーボード制御回路が制御命令を受付可能になるまで待つ
waitkbout:
    inb     $0x64,          %al
    andb    $0x02,          %al
    jnz     waitkbout
    ret

# 転送元, 転送先のアドレスと転送サイズを指定し, コピーする
memcpy:
    movl    (%esi),         %eax
    addl    $4,             %esi
    movl    %eax,           (%edi)
    addl    $4,             %edi
    subl    $1,             %ecx
    jnz     memcpy
    ret

    .align  8                         # GDT0ラベルが8の倍数になるようにゼロ埋め

# 暫定GDT
GDT0:
    .skip   8, 0x00                         # ヌルセレクタ
    .word   0xffff, 0x0000, 0x9200, 0x00cf  # 読み書き可能な32bit
    .word   0xffff, 0x0000, 0x9a00, 0x00cf  # 実行可能なセグメント32bit, カーネル用
    .word   0x0000

# LGDT命令でGDT0にGDTが存在することを示す
GDTR0:
    .word   8*3-1
    .int    GDT0

    .align  8

# ２次ブートローダのあとにカーネルを配置
kernel_entry:
