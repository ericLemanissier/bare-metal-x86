#ifndef _ARCH_I386_ISR_H
#define _ARCH_I386_ISR_H

#include <cstdint>

struct interrupt_frame;

consteval bool is_exception(int i) {
    switch(i)
    {
    case 8:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 17:
    case 21:
    case 29:
    case 30:
        return true;
    default:
        return false;
    }
}

extern "C++" void i686_ISR_Handler(std::uint32_t interrupt);

template<int i>
__attribute__ ((interrupt))
void isr_PF_exc(struct interrupt_frame *) requires (!is_exception(i));

template<int i>
__attribute__ ((interrupt))
void isr_PF_exc(struct interrupt_frame *, std::uint32_t ) requires (is_exception(i));

#endif
