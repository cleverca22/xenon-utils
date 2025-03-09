ENTRY(_start)

SECTIONS {
  . = 0x1000;
  .text : {
    *(.text.boot)
    *(.text)
    *(.text.*)
  }
  .rodata : {
    *(.rodata)
    *(.rodata.*)
  }
  .data : {
    *(.data)
    *(.data.*)
  }
  .bss : {
    *(.bss)
    *(.bss.*)
  }
  .stack : {
    . += 8k;
    __stack_bottom = .;
  }
}
