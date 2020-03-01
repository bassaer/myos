#ifndef MYOS_IO_H
#define MYOS_IO_H

/**
 * HLT : CPU停止
 */
void io_hlt(void);

/**
 * STI(set interrupt flag) : 割り込み有効化
 */
void io_sti(void);

/**
  * 割り込み有効 + HLT
  * STIとHLTの間の割り込みで取りこぼしを防ぐ
 */
void io_stihlt(void);

/**
 * CLI(clear interrupt flag) : 割り込み命令無視
 */
void io_cli(void);

/**
 * GDTR(global descriptor table register) : GDT設定用レジスタ
 * GDT : 大域セグメント記述子, セグメンテーションを割り当てたテーブル
 */
void load_gdtr(int limit, int addr);

/**
 * IDTR(interrupt descriptor table register) : IDT設定用レジスタ
 * IDT : 割り込みと割り込みハンドラの対応テーブル
 */
void load_idtr(int limit, int addr);

/**
 * ポート指定でのレジスタ書き込み
 */
void outb_p(int port, unsigned char data);

/**
 * ポート指定でレジスタ読み込み
 */
int io_in(int port);

/**
 * 割り込みハンドラ
 */
void asm_handle_intr20(void);
void asm_handle_intr21(void);
void asm_handle_intr27(void);

/**
 * EFLAGS読み込み
 */
int io_load_eflags(void);

/**
 * EFLAGS書き込み
 */
void io_store_eflags(int eflags);

/**
 * CR0読み込み
 */
int load_cr0(void);

/**
 * CR0書き込み
 */
void store_cr0(int cr0);

#endif
