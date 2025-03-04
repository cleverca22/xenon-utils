#include <assert.h>
#include <bit>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
  uint16_t nand_magic;
  uint16_t build;
  uint16_t qfe;
  uint16_t flags;
  uint32_t entry;
  uint32_t size;
  char copyright[64];
  uint8_t reserved[16];
  uint32_t keyvault_size;
  uint32_t sys_update_addr;
  uint16_t sys_update_count;
  uint16_t keyvault_version;
  uint32_t keyvault_addr;       // a byte offset into the NAND data (skipping spare)
  uint32_t sys_update_size;
  uint32_t smc_config_addr;
  uint32_t smc_boot_size;
  uint32_t smc_boot_addr;       // a byte offset into the NAND data (skipping spare)
} nand_header_t;

typedef struct {
  uint8_t block_id1;
  uint8_t block_id0 : 4;
  uint8_t unused0 : 4;
  uint8_t todo[10];
  uint8_t fs_block_type : 6;
  uint8_t ecc3 : 2;
  uint8_t ecc2;
  uint8_t ecc1;
  uint8_t ecc0;
} small_block_t;

typedef struct {
  uint8_t fs_sequence0;
  uint8_t block_id1;
  uint8_t block_id0 : 4;
  uint8_t unused0 : 4;
  uint8_t todo[9];
  uint8_t fs_block_type : 6;
  uint8_t ecc3 : 2;
  uint8_t ecc2;
  uint8_t ecc1;
  uint8_t ecc0;
} bigblock_smallnand_t;

typedef struct {
  uint8_t data[512];
  union {
    uint8_t spare[16];
    small_block_t smallblock_footer;
    bigblock_smallnand_t bigblock_smallnand_footer;
  };
} __attribute__((packed)) sector_t;

static_assert(sizeof(sector_t) == (512+16), "sector size error");

void read_sector(FILE *fd, int page_nr, sector_t *out) {
  fseek(fd, page_nr * (512+16), SEEK_SET);
  fread(out, 512+16, 1, fd);
}

void CalculateECD(const uint8_t data[512], uint8_t out[4]) {
  uint32_t i, val=0, v=0;
  uint32_t count = 0;
  for (i=0; i < 0x1066; i++) {
    if ((i & 31) == 0) {
      uint32_t value = uint32_t( (uint8_t)(data[count + 3]) << 24 |
                                 (uint8_t)(data[count + 2]) << 16 |
                                 (uint8_t)(data[count + 1]) << 8 |
                                 (uint8_t)(data[count + 0]));
      v = ~value;
      count += 4;
    }
    val ^= v & 1;
    v >>= 1;
    if (val & 1) val ^= 0x6954559;
    val >>= 1;
  }

  val = ~val;
  out[0] = (val << 6);
  out[1] = (val >> 2) & 0xff;
  out[2] = (val >> 10) & 0xff;
  out[3] = (val >> 18) & 0xff;
}

bool CheckPageECD(const sector_t &in) {
  uint8_t actual[4];
  actual[0] = in.smallblock_footer.ecc3 << 6;
  actual[1] = in.smallblock_footer.ecc2;
  actual[2] = in.smallblock_footer.ecc1;
  actual[3] = in.smallblock_footer.ecc0;
  uint8_t calculated[4];

  CalculateECD(in.data, calculated);

  bool fail = false;
  for (int i=0; i<4; i++) {
    if (actual[0] != calculated[0]) fail = true;
  }
  if (fail) {
    printf("%02x %02x %02x %02x  ", actual[0], actual[1], actual[2], actual[3]);
    printf("%02x %02x %02x %02x\n", calculated[0], calculated[1], calculated[2], calculated[3]);
  }
  return !fail;
}

void print_header(const nand_header_t &h) {
  printf("magic: 0x%4x\n", std::byteswap<uint16_t>(h.nand_magic));
  printf("build: 0x%x\n", std::byteswap<uint16_t>(h.build));
  printf("entry: 0x%x\n", std::byteswap<uint32_t>(h.entry));
  printf("size: 0x%x\n", std::byteswap<uint32_t>(h.size));
  printf("key vault version: 0x%x, addr: 0x%x, size: 0x%x\n", std::byteswap<uint16_t>(h.keyvault_version), std::byteswap<uint32_t>(h.keyvault_addr), std::byteswap<uint32_t>(h.keyvault_size));
  printf("smc config: 0x%x, addr: 0x%x, size: 0x%x\n", std::byteswap<uint32_t>(h.smc_config_addr), std::byteswap<uint32_t>(h.smc_boot_addr), std::byteswap<uint32_t>(h.smc_boot_size));
}

int main(int argc, char **argv) {
  const char *input = argv[1];

  printf("nand header size: %d\n", sizeof(nand_header_t));
  printf("parsing NAND from %s\n", input);

  FILE *nand_fd = fopen(input, "r");
  assert(nand_fd != NULL);

  fseek(nand_fd, 0, SEEK_END);
  int size = ftell(nand_fd);
  fseek(nand_fd, 0, SEEK_SET);

  int pages = size/(2048+64);
  int sectors = pages * 4;

  printf("nand size is %d bytes, %d KB raw, or %d KB data\n", size, size/1024, pages * 2);

  sector_t sector_buffer;

  FILE *output = fopen("out.bin", "w");

  for (int sector=0; sector < sectors; sector++) {
    read_sector(nand_fd, sector, &sector_buffer);

    bool pass = CheckPageECD(sector_buffer);
    if (!pass) {
      printf("ECC fail on sector %d\n", sector);
    }
    if (sector == 0) {
      nand_header_t *header = (nand_header_t*)sector_buffer.data;
      print_header(*header);
    }
    if (sector_buffer.smallblock_footer.fs_block_type != 63) {
      printf("block type %d\n", sector_buffer.smallblock_footer.fs_block_type);
      if (false) printf("%d -> %d %d\n", sector, sector_buffer.smallblock_footer.block_id1, sector_buffer.smallblock_footer.block_id0);
      if (false) {
        uint8_t id1 = sector >> 5;
        uint8_t id0 = sector >> 13;
        if ( (id1 != sector_buffer.bigblock_smallnand_footer.block_id1) || (id0 != sector_buffer.bigblock_smallnand_footer.block_id0) ) {
          printf("%d -> %d %d -> %d %d\n", sector, id1, id0, sector_buffer.bigblock_smallnand_footer.block_id1, sector_buffer.bigblock_smallnand_footer.block_id0);
        }
      }
    }

    fseek(output, sector * 512, SEEK_SET);
    fwrite(&sector_buffer, 512, 1, output);
  }
  fclose(output);
  fclose(nand_fd);
  return 0;
}
