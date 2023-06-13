export module isr;

import <cstdlib>;
import keyboard;
import ll;
import debug;
import serial;
import ticks;

struct interrupt_frame;

export __attribute__ ((interrupt))
void keyboard_isr (struct interrupt_frame *frame)
{
    Keyboard::interrupt(inb(0x60));

    outb(0x20,0x20);
    outb(0xA0,0x20);
}


export __attribute__ ((interrupt))
void isr_schedule_int(struct interrupt_frame *frame)
{
    outb(0x20,0x20);
}

export __attribute__ ((interrupt))
void isr_timer_int(struct interrupt_frame *frame)
{
    inc_ticks();
    outb(0x20,0x20);
    outb(0xA0,0x20);
}

export __attribute__ ((interrupt))
void do_syscalls(struct interrupt_frame *frame){
    debug("Syscall !\n");
    std::abort();
    outb(0x20,0x20);
}

#ifdef __x86_64__
using uword_t = unsigned long long int;
#else
using uword_t = unsigned int;
#endif

export __attribute__ ((interrupt))
void isr_GP_exc(struct interrupt_frame *frame, uword_t error_code)
{
    debug("\n General protection fault !\n");
    write_serial(error_code);
    std::abort();
    outb(0x20,0x20);
}

export __attribute__ ((interrupt))
void isr_PF_exc(struct interrupt_frame *frame)
{
    debug("\n Protection Fault !\n");
    std::abort();
    outb(0x20,0x20);
}

export __attribute__ ((interrupt))
void isr_DF_exc(struct interrupt_frame *frame)
{
    debug("\n Double Fault !\n");
    std::abort();
    outb(0x20,0x20);
}

export __attribute__ ((interrupt))
void isr_MC_exc(struct interrupt_frame *frame)
{
    debug("\n Machine check !\n");
    std::abort();
    outb(0x20,0x20);
}

export __attribute__ ((interrupt))
void isr_AC_exc(struct interrupt_frame *frame)
{
    debug("\n Alignment check !\n");
    std::abort();
    outb(0x20,0x20);
}

export __attribute__ ((interrupt))
void isr_MF_exc(struct interrupt_frame *frame)
{
    debug("\n x87 Exception !\n");
    std::abort();
    outb(0x20,0x20);
}

export __attribute__ ((interrupt))
void isr_XMXF_exc(struct interrupt_frame *frame)
{
    debug("\n SIMD Exception !\n");
    std::abort();
    outb(0x20,0x20);
}
