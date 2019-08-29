OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386:x86-64)

SECTIONS {
  . = 0xc200;
  .text : { *(.text) }
}
