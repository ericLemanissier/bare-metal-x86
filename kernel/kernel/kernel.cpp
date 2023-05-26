#include <cstddef>
#include <cstdint>
#include "kernel/tty.h"
#include "kernel/multiboot.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern "C" void kernel_main(const multiboot_info *multiboot_info_pointer, uint32_t multiboot_magic_value);

struct Point
{
	size_t x{};
	size_t y{};
};

struct Size
{
	size_t w{};
	size_t h{};
};

struct Rectangle
{
	Point m_top_left{};
	Size size{};

	constexpr Point top_left() const
	{
		return m_top_left;
	}

	constexpr Point top_right() const
	{
		return {m_top_left.x + width() - 1, m_top_left.y};
	}

	constexpr Point bottom_left() const
	{
		return {m_top_left.x, m_top_left.y + height() - 1};
	}

	constexpr Point bottom_right() const
	{
		return {m_top_left.x + width() - 1, m_top_left.y + height() - 1};
	}

	constexpr size_t width() const
	{
		return size.w;
	}

	constexpr size_t height() const
	{
		return size.h;
	}
};

enum class Outline
{
	Single,
	Double
};

void terminal_putentryat(char c, Point p)
{
	terminal_putentryat(c, p.x, p.y);
}

void terminal_rect(const Rectangle &rect, Outline o = Outline::Single)
{
	terminal_putentryat(o == Outline::Single ? 0xDA : 0xC9, rect.top_left());
	terminal_putentryat(o == Outline::Single ? 0xC0 : 0xC8, rect.bottom_left());
	terminal_putentryat(o == Outline::Single ? 0xBF : 0xBB, rect.top_right());
	terminal_putentryat(o == Outline::Single ? 0xD9 : 0xBC, rect.bottom_right());

	for(size_t y = 1; y + 1 < rect.height(); y++)
	{
		terminal_putentryat(o == Outline::Single ? 0xB3 : 0xBA, rect.top_left().x, rect.top_left().y + y);
		terminal_putentryat(o == Outline::Single ? 0xB3 : 0xBA, rect.top_right().x, rect.top_right().y + y);
	}

	for(size_t x = 1; x + 1 < rect.width(); x++)
	{
		terminal_putentryat(o == Outline::Single ? 0xC4 : 0xCD, rect.top_left().x + x, rect.top_left().y);
		terminal_putentryat(o == Outline::Single ? 0xC4 : 0xCD, rect.bottom_left().x + x, rect.bottom_left().y);
	}

}

#include "kernel/ll.h"
#include "kernel/serial.h"
#include "kernel/debug.h"
#include "kernel/idt.h"
#include "kernel/gdt.h"
#include "kernel/pic.h"
#include "kernel/pit.h"


void kernel_main(const multiboot_info *multiboot_info_pointer, uint32_t multiboot_magic_value)
{
	/* Initialize terminal interface */
	terminal_initialize();

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
	if(multiboot_info_pointer->flags & 0x800)
	{
		debug("VBE info: ");
		write_serial<16>(multiboot_info_pointer->vbe_mode);write_serial("\n");
	}
	if(multiboot_info_pointer->flags & 0x1000)
	{
		debug("Frame Buffer info: ");
		write_serial(multiboot_info_pointer->framebuffer_width); write_serial("x");
		write_serial(multiboot_info_pointer->framebuffer_height); write_serial(" ");
		write_serial(multiboot_info_pointer->framebuffer_pitch); write_serial(" ");
		write_serial(multiboot_info_pointer->framebuffer_bpp); write_serial("\n");
		switch(multiboot_info_pointer->framebuffer_type)
		{
		case 0:
			debug("Indexed colors");
			break;
		case 1:
			debug("RGB colors");
			break;
		case 2:
			debug("EGA text mode");
			break;
		default:
			debug("unknown framebuffer type: ");
    		write_serial(multiboot_info_pointer->framebuffer_type);write_serial("\n");
		}


		auto* pixel = reinterpret_cast<uint8_t*>(multiboot_info_pointer->framebuffer_addr);
		for(int i = 0; i < 10; i++)
		{
			pixel+=multiboot_info_pointer->framebuffer_pitch;
			*reinterpret_cast<uint32_t*>(pixel) = 0xFFFFFF;
		}
		for(int i = 0; i < 20; i++)
		{
			pixel+=multiboot_info_pointer->framebuffer_bpp/8;
			*reinterpret_cast<uint32_t*>(pixel) = 0xFF0000;
		}
		for(int i = 0; i < 20; i++)
		{
			pixel+=multiboot_info_pointer->framebuffer_bpp/8;
			*reinterpret_cast<uint32_t*>(pixel) = 0x00FF00;
		}
		for(int i = 0; i < 20; i++)
		{
			pixel+=multiboot_info_pointer->framebuffer_bpp/8;
			*reinterpret_cast<uint32_t*>(pixel) = 0x0000FF;
		}
	}

	enable_interrupts();
	disable_cursor();

	debug("booting kernel");

	//puts("Hello from puts\n   and it works!\n");

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
	terminal_rect(Rectangle{Point{9, 9}, Size{12,10}}, Outline::Single);
	terminal_rect(Rectangle{Point{10, 10}, Size{5,8}}, Outline::Double);
	terminal_rect(Rectangle{Point{15, 10}, Size{5,3}}, Outline::Single);

	debug("Hello debug world!");

	while (1)
    {
    	asm volatile ( "hlt" );
	}
}
