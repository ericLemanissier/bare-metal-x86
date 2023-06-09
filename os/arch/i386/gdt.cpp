export module gdt;

import <cstdint>;
import ll;


constexpr auto GDTSIZE = 0xFF;	/* nombre max. de descripteurs dans la table */

/* Descripteur de segment */
struct gdtdesc {
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t acces;
    uint8_t lim16_19:4;
    uint8_t other:4;
    uint8_t base24_31;
} __attribute__ ((packed));

gdtdesc 	kgdt[GDTSIZE];		/* GDT */

/*
 * 'init_desc' initialize a segment descriptor in gdt or ldt.
 * 'desc' is a pointer to the address
 */
void init_gdt_desc(uint32_t base, uint32_t limite, uint8_t acces, uint8_t other,struct gdtdesc *desc)
{
    desc->lim0_15 = (limite & 0xffffu);
    desc->base0_15 = (base & 0xffffu);
    desc->base16_23 = (base & 0xff0000u) >> 16u;
    desc->acces = acces;
    desc->lim16_19 = (limite & 0xf0000u) >> 16u;
    desc->other = (other & 0xf);
    desc->base24_31 = (base & 0xff000000u) >> 24u;
    return;
}

/*
 * This function initialize the GDT after the kernel is loaded.
 */
export void init_gdt(void)
{

    /* initialize gdt segments */
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
    init_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);	/* code */
    init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);	/* data */
    init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);		/* stack */

    init_gdt_desc(0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);	/* ucode */
    init_gdt_desc(0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);	/* udata */
    init_gdt_desc(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);		/* ustack */

    lgdt(kgdt, GDTSIZE * 8);


    /* initiliaz the segments */
    asm("   movw $0x10, %ax	\n \
            movw %ax, %ds	\n \
            movw %ax, %es	\n \
            movw %ax, %fs	\n \
            movw %ax, %gs	\n \
            ljmp $0x08, $next	\n \
            next:		\n");

}
