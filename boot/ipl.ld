OUTPUT_FORMAT(binary)

/*OUTPUT_ARCH(i386:x86-64)*/
OUTPUT_ARCH(i386)

SECTIONS {
  . = 0x7c00;
  .text : { *(.text) }
}
