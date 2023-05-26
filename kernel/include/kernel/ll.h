#ifndef _KERNEL_LL_H
#define _KERNEL_LL_H

#include <cstdint>

uint32_t farpeekl(uint16_t sel, void* off);
void farpokeb(uint16_t sel, void* off, uint8_t v);
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait(void);
bool are_interrupts_enabled();
unsigned long save_irqdisable(void);
void enable_interrupts(void);
void irqrestore(unsigned long flags);
void lidt(void* base, uint16_t size);
void lgdt(void* base, uint16_t size);
void cpuid(int code, uint32_t* a, uint32_t* d);
uint64_t rdtsc();
unsigned long read_cr0(void);
void invlpg(void* m);
void wrmsr(uint32_t msr_id, uint64_t msr_value);
uint64_t rdmsr(uint32_t msr_id);

#endif
