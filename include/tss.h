#ifndef MYOS_TSS_H
#define MYOS_TSS_H

/**
 * 32bit TSS(Task Status Segment)
 */
typedef struct {
  /**
   * タスク設定の内容
   */
  int backlink;
  int esp0;
  int ss0;
  int esp1;
  int ss1;
  int esp2;
  int ss2;
  int cr3;

  /**
   * 32bitレジスタ
   */
  int eip;
  int eflags;
  int eax;
  int ecx;
  int edx;
  int ebx;
  int esp;
  int ebp;
  int esi;
  int edi;

  /**
   * 16bitレジスタ
   */
  int es;
  int cs;
  int ss;
  int ds;
  int fs;
  int gs;

  /**
   * タスク設定の内容
   *
   * 不正な値が入るとコンテキストスイッチが動作しないので
   * 以下の初期値を設定する
   * idtr  = 0
   * iomap = 0x400000000
   */
  int ldtr;
  int iomap;
} tss_t;

#endif

