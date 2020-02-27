OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386)

ENTRY("main")

SECTIONS {
  . = 0x280000;     /** カーネル開始アドレス */
  .text : {}        /** プログラムを配置 */
  .rodata : {}      /** 定数を配置*/
  .data : {}        /** 初期値ありグローバル変数を配置 */
  .bss : {}         /** 初期値なしグローバル変数を配置 */
  _KERN_END = .;    /** カーネル終端アドレス */
}
