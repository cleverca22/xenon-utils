#define CACHELINE_SIZE 128
#define CACHELINE_SIZE_LOG2 7

.globl dcache_flush
dcache_flush:
  // Round down address
  li %r5, CACHELINE_SIZE - 1
  andc %r5, %r3, %r5
  // Increase size by alignment change
  rldicl %r3, %r3, 0, 64 - CACHELINE_SIZE_LOG2
  add %r4, %r4, %r3
1:
  dcbst 0, 5
  addic %r5, %r5, CACHELINE_SIZE /* also clears carry */
  subic. %r4, %r4, CACHELINE_SIZE
  bge 1b
  sync
  blr

.globl dcache_inv
dcache_inv:
  // Round down address
  li %r5, CACHELINE_SIZE - 1
  andc %r5, %r3, %r5
  // Increase size by alignment change
  rldicl %r3, %r3, 0, 64 - CACHELINE_SIZE_LOG2
  add %r4, %r4, %r3
1:
  dcbf 0, 5
  addic %r5, %r5, CACHELINE_SIZE /* also clears carry */
  subic. %r4, %r4, CACHELINE_SIZE
  bge 1b
  sync
  blr

.globl flush_code
flush_code:
  // Round down address
  li %r5, CACHELINE_SIZE - 1
  andc %r5, %r3, %r5
  // Increase size by alignment change
  rldicl %r3, %r3, 0, 64 - CACHELINE_SIZE_LOG2
  add %r4, %r4, %r3
1:
  dcbst 0, 5
  icbi 0, 5
  addic %r5, %r5, CACHELINE_SIZE // Also clears carry
  subic. %r4, %r4, CACHELINE_SIZE
  bge 1b
  sync
  isync
  blr