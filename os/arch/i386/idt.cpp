module;

#include "isr.h"

export module idt;

import <cstdint>;
import <cstdlib>;
import ll;
import debug;
import serial;
import ticks;
import keyboard;
import unroll;

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


idtdesc 	kidt[IDTSIZE]{}; 		/* IDT table */

void init_idt_desc(uint16_t select, uint32_t offset, uint16_t type, struct idtdesc *desc)
{
    desc->offset0_15 = (offset & 0xffffu);
    desc->select = select;
    desc->type = type;
    desc->offset16_31 = (offset & 0xffff0000u) >> 16u;
    return;
}

export void init_idt(void)
{
    /* Init irq */

    for_range<0, 255>([]<auto i>()
    {
        init_idt_desc(0x08, (uint32_t)isr_PF_exc<i>, INTGATE, &kidt[i]);
    });

    lidt(kidt, IDTSIZE * 8);
}


typedef void (*ISRHandler)();

ISRHandler g_ISRHandlers[256]{};

extern "C++" void i686_ISR_Handler(std::uint32_t interrupt)
{
    if(interrupt > 256)
    {
        debug("exception too high");
        write_serial(interrupt);write_serial("\n");
        debug("KERNEL PANIC!");
        std::abort();
    }
    if (g_ISRHandlers[interrupt] != NULL)
        g_ISRHandlers[interrupt]();
    else if(interrupt == 32) inc_ticks();
    else if(interrupt == 33) Keyboard::interrupt(inb(0x60));
    else if (interrupt >= 32){
        debug("Unhandled interrupt !");
        write_serial(interrupt);write_serial("\n");
    }
    else
    {
        debug("Unhandled exception");
        write_serial(interrupt);write_serial("\n");
        debug("KERNEL PANIC!");

        std::abort();
    }
    outb(0x20,0x20);
    outb(0xA0,0x20);

}
