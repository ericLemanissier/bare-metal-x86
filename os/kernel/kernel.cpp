import <cstddef>;
import <cstdint>;

import tty;
import vga;
import multiboot;

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern "C" void kernel_main(const multiboot_info *multiboot_info_pointer, uint32_t multiboot_magic_value);

import geom;

import ll;
import serial;
import debug;
import idt;
import gdt;
import pic;
import pit;
import pixel;
import ticks;
import game;

extern "C" void enable_sse();

void kernel_main(const multiboot_info *multiboot_info_pointer, uint32_t multiboot_magic_value)
{
    enable_sse();
    init_gdt();
    init_idt();
    init_pic();
    init_pit();
    init_serial();

    if(multiboot_magic_value != 0x2BADB002)
    {
        debug("!!!Bad multiboot magic value!!!");
        return;
    }
    if(multiboot_info_pointer->flags & 0x200)
    {
        debug("bootloader name: ");
        write_serial(multiboot_info_pointer->boot_loader_name);write_serial("\n");
    }

    Pixel pixel_screen(*multiboot_info_pointer);

    enable_interrupts();
    disable_cursor();

    debug("booting kernel");

    //puts("Hello from puts\n   and it works!\n");

    terminal_clear();
    terminal_writestring("Hello, kernel World!\n");
    terminal_setcolor(VGA_COLOR_BLUE | VGA_COLOR_WHITE << 4);
    terminal_writestring("how are you doing ?\n I'm doing fine\n");
    terminal_setcolor(VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4);
    /*const auto n = 9;
    for(size_t i = n * VGA_HEIGHT; i < (n + 1) * VGA_HEIGHT; i++)
    {
        terminal_writestring("0x");
        terminal_write_number(i);
        terminal_putchar(':');
        terminal_putchar(i);
        terminal_putchar('\n');
    }*/
    //terminal_writestring("0x");terminal_write_number(0);
    terminal_writestring("0x");terminal_write_number<16>(0);
    terminal_writestring(" 0x");terminal_write_number<16>(1);
    terminal_writestring(" 0x");terminal_write_number<16>(255);
    terminal_writestring(" 0x");terminal_write_number<16>(256);
    terminal_writestring(" 0x");terminal_write_number<16>(65535);
    terminal_writestring(" 0x");terminal_write_number<16>(65536);
    terminal_writestring("\n");
    terminal_rect(Rect{Point{9, 9}, Size{12,10}}, Outline::Single);
    terminal_rect(Rect{Point{10, 10}, Size{5,8}}, Outline::Double);
    terminal_rect(Rect{Point{15, 10}, Size{5,3}}, Outline::Single);

    debug("Hello debug world!");

    Game g(pixel_screen.size());


    auto last_update = get_tick();
    while (1)
    {
        asm volatile ( "hlt" );
        const auto cur_tick = get_tick();

        if(cur_tick >= last_update + 10)
        {
            g.update(cur_tick - last_update);
            last_update = cur_tick;
            g.draw(pixel_screen);
        }

        if(pixel_screen.should_repaint())
        {
            pixel_screen.update_screen();
        }
    }
}
