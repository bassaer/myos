OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386)

ENTRY("main")

SECTIONS {
  . = 0x280000;
  /*
  .text : { *(.text) }
  .data : { *(.data) }
  . = 0xc200 + 36350;
  .sign : {SHORT(0xbeef)}
  */
}
