export module multiboot;

import <cstdint>;

export struct aout_symbol_table
{
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t* addr;
  uint32_t reserved;
};


/* The section header table for ELF. */
export struct elf_section_header_table
{
  uint32_t num;
  uint32_t size;
  uint32_t* addr;
  uint32_t shndx;
};

export struct vbe_info_block{
   char VbeSignature[4];             // == "VESA"
   uint16_t VbeVersion;                 // == 0x0300 for VBE 3.0
   uint16_t OemStringPtr[2];            // isa vbeFarPtr
   uint8_t Capabilities[4];
   uint16_t VideoModePtr[2];         // isa vbeFarPtr
   uint16_t TotalMemory;             // as # of 64KB blocks
} __attribute__((packed));

export struct vbe_mode_info_structure {
  uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
  uint8_t window_a;			// deprecated
  uint8_t window_b;			// deprecated
  uint16_t granularity;		// deprecated; used while calculating bank numbers
  uint16_t window_size;
  uint16_t segment_a;
  uint16_t segment_b;
  uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
  uint16_t pitch;			// number of bytes per horizontal line
  uint16_t width;			// width in pixels
  uint16_t height;			// height in pixels
  uint8_t w_char;			// unused...
  uint8_t y_char;			// ...
  uint8_t planes;
  uint8_t bpp;			// bits per pixel in this mode
  uint8_t banks;			// deprecated; total number of banks in this mode
  uint8_t memory_model;
  uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
  uint8_t image_pages;
  uint8_t reserved0;

  uint8_t red_mask;
  uint8_t red_position;
  uint8_t green_mask;
  uint8_t green_position;
  uint8_t blue_mask;
  uint8_t blue_position;
  uint8_t reserved_mask;
  uint8_t reserved_position;
  uint8_t direct_color_attributes;

  uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
  uint32_t off_screen_mem_off;
  uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
  uint8_t reserved1[206];
} __attribute__ ((packed));

export struct multiboot_info
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
  vbe_info_block* vbe_control_info;
  vbe_mode_info_structure *vbe_mode_info;
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
