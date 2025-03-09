#include <stdint.h>
#define REG64(addr) ((volatile uint64_t*)(addr))
#define REG32(addr) ((volatile uint32_t*)(addr))
#define BIT(bit) (1<<bit)

static inline uint64_t ld(volatile void *addr) {
  uint64_t l;
  asm volatile("ld %0, 0(%1)" : "=r" (l) : "b" (addr));
  return l;
}
static inline void std(volatile void *addr, uint64_t v) {
  asm volatile("std %1, 0(%0)" : : "b" (addr), "r" (v));
}
#define mtspr(rn, v)  asm volatile("mtspr " #rn ",%0" : : "r" (v))

void flush_code(volatile void *addr, int len);
void dcache_flush(volatile void *addr, int len);
void dcache_inv(volatile void *addr, int len);

void place_jump(void *addr, void *_target) {
  uint64_t target = (uint64_t)_target;
  dcache_flush(addr - 0x80, 0x100);
  *REG32(addr - 0x18 + 0x00) = 0x3C600000 | (uint32_t)((uint64_t)(target >> 48) & 0xFFFF);
  *REG32(addr - 0x18 + 0x04) = 0x786307C6;
  *REG32(addr - 0x18 + 0x08) = 0x64630000 | ((target >> 16) & 0xFFFF);
  *REG32(addr - 0x18 + 0x0C) = 0x60630000 | (target & 0xFFFF);
  *REG32(addr - 0x18 + 0x10) = 0x7C6803A6;
  *REG32(addr - 0x18 + 0x14) = 0x4E800021;
  flush_code(addr - 0x18, 0x18);
  *REG32(addr + 0) = 0x4BFFFFE8;
  flush_code(addr, 0x80);
}

void *memset(void *s, int c, uint64_t count) {
  char *xs = (char*)s;
  while (count--) {
    *xs++ = c;
  }
  return s;
}
void *memcpy(void *dest, const void *src, uint64_t count) {
  char *d = (char*)dest;
  char *s = (char*)src;
  while (count--) {
    *d++ = *s++;
  }
  return dest;
}