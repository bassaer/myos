#include <intr.h>
#include <io.h>


/**
 * PIC(programmable interrupt controller) : 設定可能な割り込みコントロら
 * マスタPIC : CPUに直接接続しているPIC
 * スレーブPIC : マスタPICに接続するPIC(IRQ2でマスタと接続)
 * IMR(interrupt mask register) : マスクしたPICは割り込みを無視する
 * ICW(initial control word) : 初期化制御データ
 */
void init_pic() {
  outb_p(PIC0_IMR, 0xff);    // すべての割り込みを拒否
  outb_p(PIC1_IMR, 0xff);    // すべての割り込みを拒否

  outb_p(PIC0_ICW1, 0x11);   // エッジトリガモード
  outb_p(PIC0_ICW2, 0x20);   // IRQ0-7 は INT20-27で受ける
  outb_p(PIC0_ICW3, 1 << 2); // PIC1はIR2で接続
  outb_p(PIC0_ICW4, 0x01);   // ノンバッファモード

  outb_p(PIC1_ICW1, 0x11);   // エッジトリガモード
  outb_p(PIC1_ICW2, 0x28);   // IRQ8-15はINT28-2fで受ける
  outb_p(PIC1_ICW3, 2);      // PIC1はIRQ2に接続
  outb_p(PIC1_ICW4, 0x01);   // ノンバッファモード

  outb_p(PIC0_IMR, 0xfb);    // PIC1以外すべて禁止
  outb_p(PIC1_IMR, 0xff);    // すべての割り込みを拒否
}
