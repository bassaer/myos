.equ KERNEL,   0x00280000
.equ DSKCAC,   0x00100000
.equ DSKCAC0,  0x00008000
.equ CYLS,     0x0ff0
.equ LEDS,     0x0ff1
.equ VMODE,    0x0ff2
.equ SCRNX,    0x0ff4
.equ SCRNY,    0x0ff6
.equ VRAM,     0x0ff8

.text
.code16


#.org 0xc200

    movb    $0x13,          %al     # VGAグラフィックス 320x200x8bit color
    movb    $0x00,          %ah
    int     $0x10
    movb    $0x08,          (VMODE)
    movw    $320,           (SCRNX)
    movw    $200,           (SCRNY)
    movl    $0x000a0000,    (VRAM)

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
    movl    $kernel_entry,  %esi      # バイナリをcatでつなげるため、kernel_entryラベルはカーネルの先頭を表す
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

# カーネルの0x10c8から0x11a8までを0x00310000へコピー(スタック)
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
    movl    $0x00310000,    %esp   # スタック初期値
    ljmpl   $2*8,           $KERNEL  # セグメント2(カーネル)

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

    .align  8                      # GDT0ラベルが8の倍数になるようにゼロ埋め

# 暫定GDT
# セグメンテーションの情報を64bitで表す
# cf. http://softwaretechnique.jp/OS_Development/kernel_loader2.html
GDT0:
    .skip   8,      0x00 # ヌルセレクタ(全部0), Intelの仕様

# データディスクリプタ
# 読み書き可能な32bit

# セグメントのサイズ(下位16bit), 0xfffffx4KB=1GBがセグメントサイズ(55bit目でGビットを設定)
    .word   0xffff
# コードセグメントの開始アドレス(ベースアドレス下位16bit), 4GB全体を１つのセグメントと扱う
    .word   0x0000
# 32-39bitはコードセグメントの開始アドレス16-23bit目(00000000)
# 40-47bit(10010010)逆に見る
# 40-43bitはセグメントタイプでデータセグメントとアクセスビットにフラグ
# 44bitは1(コードoデータセグメント), 45-46bitは特権レベル00(最高リング0), 47bitは1(セグメントがメモリ内に有)
    .word   0x9200
# 48-51bitはセグメントサイズ(上位4bit)
# 52bitは自由使用可, 53bitは予約領域0
# 54bitはデフォルトオペレーションサイズ(1:32bit用セグメント)
# 55bitはGビット(1:x4Kバイト単位)
# 48-55bitは(11001111)
# 56-63bitはセグメント開始アドレス(上位8bit)
    .word   0x00cf

# コードセグメントディスクリプタ
# 実行可能なセグメント32bit, カーネル用
    .word   0xffff
    .word   0x0000
# 40-47bit(10011010)データディスクリプタとの違いは43bit目(1:コードセグメント)
    .word   0x9a00
    .word   0x00cf


# LGDT命令でGDT0にGDTが存在することを示す
GDTR0:
    .word   8*3
    .int    GDT0

    .align 8

# ２次ブートローダのあとにカーネルを配置
kernel_entry:
