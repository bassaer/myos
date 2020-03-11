.code32
.global write_mem8
.text

write_mem8:
  movl    4(%esp),  %ecx
  movb    8(%esp),  %al
  movb    %al,      (%ecx)
  ret
