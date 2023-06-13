export module idt;

import <cstdint>;
import ll;
import isr;



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

export void init_idt(void)
{
    /* Init irq */
    for (auto &e : kidt)
    {
        init_idt_desc(0x08, (uint32_t)isr_schedule_int, 0, &e);
    }

    /* Vectors  0 -> 31 are for exceptions */
    init_idt_desc(0x08, (uint32_t) isr_DF_exc, INTGATE, &kidt[8]);		/* #DF */

    init_idt_desc(0x08, (uint32_t) isr_GP_exc, INTGATE, &kidt[13]);		/* #GP */
    init_idt_desc(0x08, (uint32_t) isr_PF_exc, INTGATE, &kidt[14]);     /* #PF */

    init_idt_desc(0x08, (uint32_t) isr_MF_exc, INTGATE, &kidt[16]);		/* #MF */
    init_idt_desc(0x08, (uint32_t) isr_AC_exc, INTGATE, &kidt[17]);		/* #AC */
    init_idt_desc(0x08, (uint32_t) isr_DF_exc, INTGATE, &kidt[18]);		/* #DF */
    init_idt_desc(0x08, (uint32_t) isr_XMXF_exc, INTGATE, &kidt[19]);		/* #XMXF */

    init_idt_desc(0x08, (uint32_t) isr_timer_int, INTGATE, &kidt[32]);
    init_idt_desc(0x08, (uint32_t) keyboard_isr, INTGATE, &kidt[33]);

    //init_idt_desc(0x08, (uint32_t) do_syscalls, TRAPGATE, &kidt[48]);
    //init_idt_desc(0x08, (uint32_t) do_syscalls, TRAPGATE, &kidt[128]); //48


    lidt(kidt, IDTSIZE * 8);
}
