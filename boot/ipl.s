# シリンダ数
.equ CYLS,  20
.text
.code16

    jmp     entry

    .byte   0x90
    .ascii  "MYOSIPL "        # ブートセクタの名前(8バイト) 
    .word   512               # 1セクタのサイズ（512固定）
    .byte   1                 # クラスタの大きさ(1セクタ固定)
    .word   1                 # FATの開始位置(1セクタから)
    .byte   2                 # FATの個数(2固定)
    .word   224               # ルートディレクトリの大きさ(224エントリ)
    .word   2880              # ドライブの大きさ (2880固定)
    .byte   0xf0              # メディアタイプ(0xf0固定)
    .word   9                 # FAT領域の長さ(9セクタ固定)
    .word   18                # 1トラックのセクタ数(18固定)
    .word   2                 # ヘッド数(2固定)
    .long   0                 # パーティション未使用(0固定)
    .long   2880              # ドライブの大きさ再定義 (2880固定)
    .byte   0,0,0x29          # 謎
    .long   0xffffffff        # ボリュームシリアル番号
    .ascii  "MYOS       "     # ディスク名(11バイト)
    .ascii  "FAT12   "        # フォーマット名(8バイト)
    .skip   18,0x00           # 18バイト開ける（謎）


#============================================
# 各レジスタ                                ;
# ax : accumulator                          ;
# cx : counter                              ;
# dx : data                                 ;
# bx : base                                 ;
# sp : stack pointer                        ;
# bp : base pointer                         ;
# si : source index (読み込みindex)         ;
# di : destination index (書き込みindex)    ;
#============================================

# メモリマップ
# 0x7c00 - 0x7dff : ブートセクタ
# 0x7e00 - 0x7fff : ファイル読み込み
# 0x8000 - 0x81ff : (ブートセクタ)
# 0x8200 - 0x9bff : 空き

entry:
    movw    $0,         %ax
    movw    %ax,        %ss
    movw    $0x07c0,    %sp
    movw    %ax,        %ds
    movw    %ax,        %es

#  画面クリア
    movw    $0x0003,    %ax
    int     $0x10

#   起動メッセージ表示
#    movw    $boot_msg,  %si
#    call    print

# ディスク読み込み
# 1枚のディスクは512バイトのセクタが1シリンダに18, 1ヘッドに80シリンダ、ヘッドが２ある
# 512 x 18 x 80 x 2 = 1440 KB

    movw    $0x0820,    %ax
    movw    %ax,        %es
    movb    $0x00,      %ch   # シリンダ : 0
    movb    $0x00,      %dh   # ヘッド : 0
    movb    $0x02,      %cl   # セクタ : 2 (セクタ1はブートセクタを格納するため、2から)

#    movw    $load_msg,  %si
#    call    print

readloop:
    movw    $0x00,      %si   # 失敗回数をカウント

retry:
    movb    $0x02,      %ah   # AH=0x02 ディスク読み込み
    movb    $0x01,      %al   # 1セクタ
    movw    $0,         %bx
    movb    $0x00,      %dl   # ドライブ0 : どのドライブのディスクか FDドライブは１つなので0固定
    int     $0x13             # ディスクBIOS呼び出し
    jnc     next              # エラーがなければnextへ(jump if not carry)
    addw    $0x01,      %si   # エラー回数をインクリメント
    cmpw    $0x05,      %si   # エラー回数と5を比較
    jae     error             # エラーが5回以上であればerrorへ (jump if above or equal)
    movb    $0x00,      %ah   # リセット
    movb    $0x00,      %dl   # リセット
    int     $013              # リセット実行
    jmp     retry             # retryへ

next:
    movw    %es,        %ax   # アドレスを0x20進める
    addw    $0x20,      %ax   # 512/16 -> 0x20
    movw    %ax,        %es   # addw $0x020, %es は無理？
    addb    $0x01,      %cl   # セクタ番号をインクリメント
    cmpb    $18,        %cl   # セクタ番号と18を比較
    jbe     readloop          # 18セクタまで完了していなければ、readloop (jump if below of equal)

    movb    $0x01,      %cl   # セクタ : 1
    addb    $0x01,      %dh   # ヘッド : 1
    cmpb    $0x02,      %dh   # ヘッドと2を比較
    jb      readloop          # ２ヘッドまで完了していなければ、readloop (jump if below)

    movb    $0x00,      %dh
    addb    $0x01,      %ch
    cmpb    $CYLS,      %ch   # 事前定義シリンダ数と比較
    jb      readloop          # 想定シリンダまで完了していなければ、readloop

#    movw    $done_msg,  %si
#    call    print

    movb    $CYLS, (0x0ff0)
    jmp     0xc200

fin:
    hlt
    jmp fin

print:
    movb    (%si),      %al
    add     $1,         %si
    cmpb    $0,         %al
    je      return
    movb    $0x0e,      %ah
    movw    $15,        %bx
    int     $0x10
    jmp     print

return:
    ret

error:
    movw    $err_msg,       %si

boot_msg:
    .string "Booting MyOS...\r\n"

load_msg:
    .string "Loading from disk...\r\n"

done_msg:
    .ascii "Finished Loading data!\r\n"
    .byte  0

err_msg:
    .string "load error\r\n"

    .org    0x1fe
# 511, 512バイト目がそれぞれ0x55, 0xAAの場合0x7c00に最初のセクタがコピーされ、それ以外の場合は次のセクタを読見込む
# 0x7c00にブートローダがコピーされると0x7c00から処理を開始する
    .byte   0x55
    .byte   0xaa
