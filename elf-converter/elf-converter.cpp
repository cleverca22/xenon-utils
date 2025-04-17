#include <elf.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <bit>

template <typename T> T bswap_elf(T &input) {
  return std::byteswap<T>(input);
}

void byteswap_ehdr64(Elf64_Ehdr *out, const Elf64_Ehdr *in) {
  memcpy(out->e_ident, in->e_ident, EI_NIDENT);
#define SWAP(field) out->field = bswap_elf(in->field)
  SWAP(e_type);
  SWAP(e_machine);
  SWAP(e_version);
  SWAP(e_entry);
  SWAP(e_phoff);
  SWAP(e_shoff);
  SWAP(e_flags);
  SWAP(e_ehsize);
  SWAP(e_phentsize);
  SWAP(e_phnum);
  SWAP(e_shentsize);
  SWAP(e_shnum);
  SWAP(e_shstrndx);
#undef SWAP
}

void byteswap_ehdr32(Elf32_Ehdr *out, const Elf32_Ehdr *in) {
  memcpy(out->e_ident, in->e_ident, EI_NIDENT);
#define SWAP(field) out->field = bswap_elf(in->field)
  SWAP(e_type);
  SWAP(e_machine);
  SWAP(e_version);
  SWAP(e_entry);
  SWAP(e_phoff);
  SWAP(e_shoff);
  SWAP(e_flags);
  SWAP(e_ehsize);
  SWAP(e_phentsize);
  SWAP(e_phnum);
  SWAP(e_shentsize);
  SWAP(e_shnum);
  SWAP(e_shstrndx);
#undef SWAP
}

void byteswap_phdr64(Elf64_Phdr &out, const Elf64_Phdr &in) {
#define SWAP(field) out.field = bswap_elf(in.field)
    SWAP(p_type);
    SWAP(p_offset);
    SWAP(p_vaddr);
    SWAP(p_paddr);
    SWAP(p_filesz);
    SWAP(p_memsz);
    SWAP(p_flags);
    SWAP(p_align);
#undef SWAP
}

void byteswap_phdr32(Elf32_Phdr &out, const Elf32_Phdr &in) {
#define SWAP(field) out.field = bswap_elf(in.field)
    SWAP(p_type);
    SWAP(p_offset);
    SWAP(p_vaddr);
    SWAP(p_paddr);
    SWAP(p_filesz);
    SWAP(p_memsz);
    SWAP(p_flags);
    SWAP(p_align);
#undef SWAP
}

void byteswap_shdr64(Elf64_Shdr &out, const Elf64_Shdr &in) {
#define SWAP(field) out.field = bswap_elf(in.field)
  SWAP(sh_name);
  SWAP(sh_type);
  SWAP(sh_flags);
  SWAP(sh_addr);
  SWAP(sh_offset);
  SWAP(sh_size);
  SWAP(sh_link);
  SWAP(sh_info);
  SWAP(sh_addralign);
  SWAP(sh_entsize);
#undef SWAP
}

void byteswap_shdr32(Elf32_Shdr &out, const Elf32_Shdr &in) {
#define SWAP(field) out.field = bswap_elf(in.field)
  SWAP(sh_name);
  SWAP(sh_type);
  SWAP(sh_flags);
  SWAP(sh_addr);
  SWAP(sh_offset);
  SWAP(sh_size);
  SWAP(sh_link);
  SWAP(sh_info);
  SWAP(sh_addralign);
  SWAP(sh_entsize);
#undef SWAP
}

std::vector<Elf32_Shdr> byteswap_shdrs32(const std::vector<Elf32_Shdr> in) {
  std::vector<Elf32_Shdr> out;
  for (auto shdr : in) {
    Elf32_Shdr swapped;
    byteswap_shdr32(swapped, shdr);
    out.push_back(swapped);
  }
  return out;
}

void squash_sheader(Elf32_Shdr &out, const Elf64_Shdr &in) {
  out.sh_name = in.sh_name;
  out.sh_type = in.sh_type;
  out.sh_flags = in.sh_flags;
  out.sh_addr = in.sh_addr;
  out.sh_offset = in.sh_offset;
  out.sh_size = in.sh_size;
  out.sh_link = in.sh_link;
  out.sh_info = in.sh_info;
  out.sh_addralign = in.sh_addralign;
  out.sh_entsize = in.sh_entsize;
}

void print_elf64(const Elf64_Ehdr &hdr) {
#define PRT(field) printf( #field ":\t 0x%x\n", hdr. field)
  PRT(e_type);
  PRT(e_machine);
  PRT(e_version);
  PRT(e_entry);
  PRT(e_phoff);
  PRT(e_shoff);
  PRT(e_flags);
  PRT(e_ehsize);
  PRT(e_phentsize);
  PRT(e_phnum);
  PRT(e_shentsize);
  PRT(e_shnum);
  PRT(e_shstrndx);
#undef PRT
}

void print_phdr64(const Elf64_Phdr &hdr) {
#define PRT(field) printf( #field ":\t 0x%x\n", hdr.field)
  PRT(p_type);
  PRT(p_offset);
  PRT(p_vaddr);
  PRT(p_paddr);
  PRT(p_filesz);
  PRT(p_memsz);
  PRT(p_flags);
  PRT(p_align);
#undef PRT
}

void print_phdrs64(const std::vector<Elf64_Phdr> phdrs) {
  int i = 0;
  for (auto phdr : phdrs) {
    printf("phdr #%d\n", i);
    print_phdr64(phdr);
    i++;
  }
}

void print_elf32(const Elf32_Ehdr &hdr) {
#define PRT(field) printf( #field ":\t 0x%x\n", hdr. field)
  PRT(e_type);
  PRT(e_machine);
  PRT(e_version);
  PRT(e_entry);
  PRT(e_phoff);
  PRT(e_shoff);
  PRT(e_flags);
  PRT(e_ehsize);
  PRT(e_phentsize);
  PRT(e_phnum);
  PRT(e_shentsize);
  PRT(e_shnum);
  PRT(e_shstrndx);
}

void squash_header(Elf32_Ehdr &out, const Elf64_Ehdr &in) {
  memcpy(out.e_ident, in.e_ident, EI_NIDENT);
  out.e_ident[EI_CLASS] = ELFCLASS32;
  out.e_type = in.e_type;
  out.e_machine = in.e_machine;
  out.e_version = in.e_version;
  out.e_entry = (uint32_t)in.e_entry;
  out.e_phoff = (uint32_t)in.e_phoff;
  out.e_shoff = (uint32_t)in.e_shoff;
  out.e_flags = in.e_flags;
  out.e_ehsize = in.e_ehsize;
  out.e_phentsize = in.e_phentsize;
  out.e_phnum = in.e_phnum;
  out.e_shentsize = in.e_shentsize;
  out.e_shnum = in.e_shnum;
  out.e_shstrndx = in.e_shstrndx;
}

void squash_pheader(Elf32_Phdr &out, const Elf64_Phdr &in) {
  out.p_type = in.p_type;
  out.p_offset = (uint32_t)in.p_offset;
  out.p_vaddr = (uint32_t)in.p_vaddr;
  out.p_paddr = (uint32_t)in.p_paddr;
  out.p_filesz = in.p_filesz;
  out.p_memsz = in.p_memsz;
  out.p_flags = in.p_flags;
  out.p_align = in.p_align;
}

std::vector<Elf32_Phdr> squash_pheaders(const std::vector<Elf64_Phdr> in) {
  std::vector<Elf32_Phdr> out;
  for (auto phdr : in) {
    Elf32_Phdr x;
    squash_pheader(x, phdr);
    out.push_back(x);
  }
  return out;
}

std::vector<Elf32_Shdr> squash_shdrs(const std::vector<Elf64_Shdr> in) {
  std::vector<Elf32_Shdr> out;
  for (auto shdr : in) {
    Elf32_Shdr x;
    squash_sheader(x, shdr);
    out.push_back(x);
  }
  return out;
}

std::vector<Elf32_Phdr> byteswap_phdrs32(const std::vector<Elf32_Phdr> in) {
  std::vector<Elf32_Phdr> out;
  for (auto phdr : in) {
    Elf32_Phdr x;
    byteswap_phdr32(x, phdr);
    out.push_back(x);
  }
  return out;
}

int main(int argc, char **argv) {
  const char *input = argv[1];
  const char *output = argv[2];

  int input_fd = open(input, O_RDONLY);
  assert(input_fd >= 0);

  off_t input_size = lseek(input_fd, 0, SEEK_END);

  printf("input fd %d size %d\n", input_fd, (uint32_t)input_size);

  void *input_data = malloc(input_size);
  const uint8_t *input_data8 = (const uint8_t*)input_data;

  pread(input_fd, input_data, input_size, 0);

  Elf64_Ehdr eheader_internal;
  std::vector<Elf64_Phdr> program_headers_internal;
  std::vector<Elf64_Shdr> section_headers_internal;

  uint8_t eiclass = input_data8[EI_CLASS];
  uint8_t eidata = input_data8[EI_DATA];
  switch (eiclass) {
  case ELFCLASS32:
    assert(0);
    break;
  case ELFCLASS64:
    if (eidata == ELFDATA2LSB) {
      memcpy(&eheader_internal, input_data, sizeof(eheader_internal));
      assert(0);
    } else {
      byteswap_ehdr64(&eheader_internal, (const Elf64_Ehdr*)input_data);
      for (int i=0; i<eheader_internal.e_phnum; i++) {
        const Elf64_Phdr *phdr = (const Elf64_Phdr *)(input_data + eheader_internal.e_phoff + (eheader_internal.e_phentsize * i));
        Elf64_Phdr swapped;
        byteswap_phdr64(swapped, *phdr);
        program_headers_internal.push_back(swapped);
      }
      for (int i=0; i<eheader_internal.e_shnum; i++) {
        const Elf64_Shdr *shdr = (const Elf64_Shdr *)(input_data + eheader_internal.e_shoff + (eheader_internal.e_shentsize * i));
        Elf64_Shdr swapped;
        byteswap_shdr64(swapped, *shdr);
        section_headers_internal.push_back(swapped);
      }
    }
    break;
  }

  print_elf64(eheader_internal);
  print_phdrs64(program_headers_internal);


  Elf32_Ehdr eheader_internal32;

  squash_header(eheader_internal32, eheader_internal);
  std::vector<Elf32_Phdr> phdrs32 = squash_pheaders(program_headers_internal);
  std::vector<Elf32_Phdr> phdrs32_be = byteswap_phdrs32(phdrs32);
  std::vector<Elf32_Shdr> shdrs32 = squash_shdrs(section_headers_internal);
  std::vector<Elf32_Shdr> shdrs32_be = byteswap_shdrs32(shdrs32);

  eheader_internal32.e_phentsize = sizeof(Elf32_Phdr);
  eheader_internal32.e_ehsize = sizeof(Elf32_Ehdr);
  eheader_internal32.e_phoff = 128;
  eheader_internal32.e_shentsize = sizeof(Elf32_Shdr);
  //eheader_internal32.e_shstrndx = 0;
  eheader_internal32.e_machine = EM_PPC;

  print_elf32(eheader_internal32);

  Elf32_Ehdr eheader_final_32;

  byteswap_ehdr32(&eheader_final_32, &eheader_internal32);
  int output_fd = open(output, O_WRONLY | O_CREAT, 0777);
  assert(output_fd >= 0);
  pwrite(output_fd, &eheader_final_32, sizeof(eheader_final_32), 0);

  pwrite(output_fd, phdrs32_be.data(), phdrs32_be.size() * eheader_internal32.e_phentsize, 128);
  pwrite(output_fd, shdrs32_be.data(), shdrs32_be.size() * eheader_internal32.e_shentsize, eheader_internal32.e_shoff);

  for (auto phdr : phdrs32) {
    if (phdr.p_type == PT_LOAD) {
      pwrite(output_fd, input_data + phdr.p_offset, phdr.p_filesz, phdr.p_offset);
    }
  }

  free(input_data);

  close(input_fd);
  close(output_fd);

  return 0;
}
