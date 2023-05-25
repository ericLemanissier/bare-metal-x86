#include "kernel/idt.h"
#include <stdint.h>

namespace {

/* Descripteur de segment */
struct idtdesc {
	uint16_t offset0_15{};
	uint16_t select{};
	uint16_t type{};
	uint16_t offset16_31{};
} __attribute__ ((packed));

#define IDTSIZE		0xFF	/* nombre max. de descripteurs dans la table */

#define INTGATE  0x8E00		/* utilise pour gerer les interruptions */
#define TRAPGATE 0xEF00		/* utilise pour faire des appels systemes */


idtdesc 	kidt[IDTSIZE]; 		/* IDT table */

void init_idt_desc(uint16_t select, uint32_t offset, uint16_t type, struct idtdesc *desc)
{
    desc->offset0_15 = (offset & 0xffff);
    desc->select = select;
    desc->type = type;
    desc->offset16_31 = (offset & 0xffff0000) >> 16;
    return;
}

}

#include "kernel/ll.h"
#include "kernel/debug.h"

extern "C" void _asm_schedule();
extern "C" void _asm_timer();
extern "C" void _asm_int_kbd();
extern "C" void _asm_syscalls();
extern "C" void _asm_exc_GP(void);
extern "C" void _asm_exc_PF(void);

void init_idt(void)
{
    /* Init irq */
    for (auto &e : kidt)
    {
        init_idt_desc(0x08, (uint32_t)_asm_schedule, INTGATE, &e); //
    }

	/* Vectors  0 -> 31 are for exceptions */
	init_idt_desc(0x08, (uint32_t) _asm_exc_GP, INTGATE, &kidt[13]);		/* #GP */
	init_idt_desc(0x08, (uint32_t) _asm_exc_PF, INTGATE, &kidt[14]);     /* #PF */

	init_idt_desc(0x08, (uint32_t) _asm_timer, INTGATE, &kidt[32]);
	init_idt_desc(0x08, (uint32_t) _asm_int_kbd, INTGATE, &kidt[33]);

	init_idt_desc(0x08, (uint32_t) _asm_syscalls, TRAPGATE, &kidt[48]);
	init_idt_desc(0x08, (uint32_t) _asm_syscalls, TRAPGATE, &kidt[128]); //48


    lidt(kidt, IDTSIZE * 8);
}


extern "C" void isr_schedule_int()
{
	outb(0x20,0x20);
	outb(0xA0,0x20);
}

#include "kernel/serial.h"

static uint64_t ticks{};
uint64_t get_tick()
{
    return ticks;
}
extern "C" void isr_timer_int()
{
    ticks++;
	outb(0x20,0x20);
	outb(0xA0,0x20);
}

extern "C" void isr_kbd_int()
{
    debug("Keyboard int!");


    uint8_t d = inb(0x60);
    write_serial(d);write_serial("\n");

	outb(0x20,0x20);
	outb(0xA0,0x20);
}

extern "C" void do_syscalls(int num){
	debug("Syscall !\n");
    asm("hlt");
}


extern "C" void isr_GP_exc(void)
{
	debug("\n General protection fault !\n");
    asm("hlt");
}

extern "C" void isr_PF_exc(void)
{
	debug("\n Protection Fault !\n");
    asm("hlt");
}
