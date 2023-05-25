#ifndef _KERNEL_MULTIBOOT_H
#define _KERNEL_MULTIBOOT_H

#include <stdint.h>

struct aout_symbol_table
{
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t* addr;
  uint32_t reserved;
};


/* The section header table for ELF. */
struct elf_section_header_table
{
  uint32_t num;
  uint32_t size;
  uint32_t* addr;
  uint32_t shndx;
};

struct multiboot_info
{
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t boot_device;
  uint32_t cmdline;
  uint32_t mods_count;
  uint32_t* mods_addr;
  union
  {
    aout_symbol_table aout_sym;
    elf_section_header_table elf_sec;
  } u;
  uint32_t mmap_length;
  uint32_t* mmap_addr;
  uint32_t drives_length;
  uint32_t* drives_addr;
  uint32_t config_table;
  const char* boot_loader_name;
  uint32_t apm_table;
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;
  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
  uint8_t framebuffer_type;
  uint32_t color_info;
};

#endif
