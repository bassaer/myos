.code32
.global io_hlt
.text

io_hlt:
  hlt
  jmp     io_hlt
