#include <stdint.h>

#define UART_STATUS 0x80000200ea001018ULL
#define UART_DATA 0x80000200ea001014ULL
#define POST_ADDR 0x8000020000061008ULL

#define REG64(addr) ((uint64_t*)(addr))
#define BIT(bit) (1<<bit)

void POST(uint8_t code) {
  *REG64(POST_ADDR) = code << 56;
}

void putch(unsigned char c) {
  while (!(*REG64(UART_STATUS)) & BIT(25));
  *REG64(UART_DATA) = (c << 24) & 0xFF000000;
}

int main() {
  POST(0x42);
  putch("A");
  putch("B");
  putch("C");
  return 0;
}
