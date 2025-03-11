#include "main.h"
#define UART_BASE 0xEA001000ULL

void putch(uint8_t c) {
  if (c == '\n')
  putch('\r');

  while (!((*REG32(UART_BASE+0x18)) & BIT(25)));
  *REG32(UART_BASE+0x14) = (c << 24) & 0xFF000000;
}

void init_uart() {
  // Set UART to 115400, 8, N, 1
  *REG32(UART_BASE+0x1C) = 0xE6010000;
}

void putstring(const char *c) {
  while (*c)
    putch(*c++);
}

int puts(const char *c) {
  putstring(c);
  putch('\n');
  return 0;
}

void init_soc() {
  void *soc_29 = (void*)0x30000;
  void *soc_31 = (void*)0x40000;
  void *soc_30 = (void*)0x60000;
  void *soc_03 = (void*)0x48000;
  uint64_t v;

  v = ld(soc_31 + 0x3000);
  v &= (((uint64_t)-0x1A) << 42) | (((uint64_t)-0x1A) >> 22);
  v |= 3ULL << 43;
  std(soc_31 + 0x3000, v);

  std(soc_31 + 0x3110, (((uint64_t)-2) << 46) | (((uint64_t)-2) >> 18));

  std(soc_29 + 0x3110, (((uint64_t)-2) << 41) | (((uint64_t)-2) >> 23));

  v = ld(soc_30 + 0x700);
  v |= (uint64_t)7 << 53;
  std(soc_30 + 0x700, v);

  v = ld(soc_30 + 0x840);
  v &= (((uint64_t)-2) << 46)|(((uint64_t)-2) >> 18);
  v |= (uint64_t)0x0F << 42;
  std(soc_30 + 0x840, v);

  v = ld(soc_31);
  v &= ((uint64_t)0xFFFFFFFFFFFC7FC0 << 46) | ((uint64_t)0xFFFFFFFFFFFC7FC0 >> 18);
  v |= (uint64_t)0x4009 << 48;
  std(soc_31, v);

  v = ld(soc_03);
  v &= (((uint64_t)-0x30) << 57) | (((uint64_t)-0x30) >> 7);
  v |= (uint64_t)0x401 << 46;
  std(soc_03, v);

  v = ld(soc_29);
  v |= (uint64_t)1 << 62;
  std(soc_29, v);
}

int main() {
  init_uart();
  puts("booted");
  //init_soc();
  putch('.');
  putstring("Hello, world!");
  return 0;
}
