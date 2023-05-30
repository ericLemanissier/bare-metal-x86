export module idt;

import <cstdint>;
import <cstdlib>;
import ll;
import debug;
import serial;
import ticks;
import keyboard;



/* Descripteur de segment */
struct idtdesc {
	uint16_t offset0_15{};
	uint16_t select{};
	uint16_t type{};
	uint16_t offset16_31{};
} __attribute__ ((packed));

constexpr auto IDTSIZE = 0xFF;	/* nombre max. de descripteurs dans la table */

constexpr auto INTGATE = 0x8E00;		/* utilise pour gerer les interruptions */
constexpr auto TRAPGATE = 0xEF00;		/* utilise pour faire des appels systemes */


idtdesc 	kidt[IDTSIZE]; 		/* IDT table */

void init_idt_desc(uint16_t select, uint32_t offset, uint16_t type, struct idtdesc *desc)
{
    desc->offset0_15 = (offset & 0xffffu);
    desc->select = select;
    desc->type = type;
    desc->offset16_31 = (offset & 0xffff0000u) >> 16u;
    return;
}

extern "C" void _asm_schedule();
extern "C" void _asm_timer();
extern "C" void _asm_int_kbd();
extern "C" void _asm_syscalls();
extern "C" void _asm_exc_GP(void);
extern "C" void _asm_exc_PF(void);

export void init_idt(void)
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

extern "C" void isr_timer_int()
{
    inc_ticks();
	outb(0x20,0x20);
	outb(0xA0,0x20);
}

extern "C" void isr_kbd_int()
{
	Keyboard::interrupt(inb(0x60));

	outb(0x20,0x20);
	outb(0xA0,0x20);
}

extern "C" void do_syscalls(int num){
	debug("Syscall !\n");
	std::abort();
}


extern "C" void isr_GP_exc(void)
{
	debug("\n General protection fault !\n");
	std::abort();
}

extern "C" void isr_PF_exc(void)
{
	debug("\n Protection Fault !\n");
	std::abort();
}
