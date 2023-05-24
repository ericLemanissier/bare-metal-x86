#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

void init_idt(void);

uint64_t get_tick();

#endif
