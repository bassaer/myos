OUTPUT_FORMAT(binary)

ENTRY("main")

MEMORY {
  KERNEL (rwx) : ORIGIN = 0x0000000000001000, LENGTH = 0x7FF000
}

SECTIONS {
  .text : {}        /** プログラムを配置 */
  .rodata : {}      /** 定数を配置*/
  .data : {}        /** 初期値ありグローバル変数を配置 */
  .bss : {}         /** 初期値なしグローバル変数を配置 */
  _KERN_END = .;    /** カーネル終端アドレス */
}
