OUTPUT_FORMAT(binary)

ENTRY("main")

/*
MEMORY {
  KERNEL (rwx) : ORIGIN = 0x0000000000001000, LENGTH = 0xfff
  RAM (rwx) : ORIGIN = 0x0000000000002000, LENGTH = 0x7FE000
}

SECTIONS {
  .header : {
    QUAD(__bss_start)
    QUAD(__bss_end - __bss_start)
  } > KERNEL
  .body : {
    *(.text)
    *(.rodata)
    *(.data)
    __bss_start = .;
    *(.bss)
    __bss_end = .;
    *(.eh_frame)
  } > RAM AT > KERNEL
}

*/

SECTIONS {
  . = 0x002000;     /** カーネル開始アドレス */
  .header : {}
  .text : {}        /** プログラムを配置 */
  .rodata : {}      /** 定数を配置*/
  .data : {}        /** 初期値ありグローバル変数を配置 */
  .bss : {}         /** 初期値なしグローバル変数を配置 */
  _KERN_END = .;    /** カーネル終端アドレス */
}
