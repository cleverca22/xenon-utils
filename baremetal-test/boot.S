.section .text.boot
_start:
  bl main
1:
  #li r10, 0
  #mtspr ctrlwr, r10
  b 1b
