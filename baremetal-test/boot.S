.section .text.boot

.globl _start
_start:
  # Stop the linker from trying to shift main by 8 (why?????)
  lis %r3, main@h
  ori %r3, %r3, main@l
  mtctr %r3
  bctr
1:
  li %r10, 0
  # mtspr ctrlwr, r10
  mtspr 152, %r10
  b 1b