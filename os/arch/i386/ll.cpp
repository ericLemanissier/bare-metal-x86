export module ll;

import <cstdint>;

export {
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
}

uint32_t farpeekl(uint16_t sel, void* off)
{
    uint32_t ret;
    asm ( "push %%fs\n\t"
          "mov  %1, %%fs\n\t"
          "mov  %%fs:(%2), %0\n\t"
          "pop  %%fs"
          : "=r"(ret) : "g"(sel), "r"(off) );
    return ret;
}
void farpokeb(uint16_t sel, void* off, uint8_t v)
{
    asm ( "push %%fs\n\t"
          "mov  %0, %%fs\n\t"
          "movb %2, %%fs:(%1)\n\t"
          "pop %%fs"
          : : "g"(sel), "r"(off), "r"(v) );
    /* TODO: Should "memory" be in the clobber list here? */
}
void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void io_wait(void)
{
    outb(0x80, 0);
}
bool are_interrupts_enabled()
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

unsigned long save_irqdisable(void)
{
    unsigned long flags;
    asm volatile ("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
    return flags;
}
void enable_interrupts(void)
{
    asm volatile("sti");
}

void irqrestore(unsigned long flags)
{
    asm ("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
}

void lidt(void* base, uint16_t size)
{   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };

    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}

void lgdt(void* base, uint16_t size)
{   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) GDTR = { size, base };

    asm ( "lgdt %0" : : "m"(GDTR) );  // let the compiler choose an addressing mode
}

void cpuid(int code, uint32_t* a, uint32_t* d)
{
    asm volatile ( "cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx" );
}

uint64_t rdtsc()
{
    uint64_t ret;
    asm volatile ( "rdtsc" : "=A"(ret) );
    return ret;
}

unsigned long read_cr0(void)
{
    unsigned long val;
    asm volatile ( "mov %%cr0, %0" : "=r"(val) );
    return val;
}

void invlpg(void* m)
{
    /* Clobber memory to avoid optimizer re-ordering access before invlpg, which may cause nasty bugs. */
    asm volatile ( "invlpg (%0)" : : "b"(m) : "memory" );
}
void wrmsr(uint32_t msr_id, uint64_t msr_value)
{
    asm volatile ( "wrmsr" : : "c" (msr_id), "A" (msr_value) );
}

uint64_t rdmsr(uint32_t msr_id)
{
    uint64_t msr_value;
    asm volatile ( "rdmsr" : "=A" (msr_value) : "c" (msr_id) );
    return msr_value;
}
