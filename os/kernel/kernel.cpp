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
import keyboard;
import ticks;
import ball;

void kernel_main(const multiboot_info *multiboot_info_pointer, uint32_t multiboot_magic_value)
{
	terminal_clear();

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

	auto pos = Point{90, 90};
	Ball b(Point(10,10), Speed(0.3, 0.1), Size(10, 10), pixel_screen.size());

	const auto repaint = [&]()
	{
		pixel_screen.clear_screen();
		pixel_screen.fill_rect(Rect{pos, Size{120,100}}, YELLOW);
		pixel_screen.fill_rect(Rect{Point{pos.x + 10, pos.y + 10}, Size{50,80}}, CYAN);
		pixel_screen.fill_rect(Rect{Point{pos.x + 60, pos.y + 10}, Size{50,30}}, MAGENTA);

		pixel_screen.fill_circle(Point(
			b.pos().x + b.size().w/2,
			b.pos().y + b.size().h/2), b.size().w/2, BLUE);
	};

	const auto speed = 0.2;


	const auto update_world = [&](const auto duration)
	{
		if(Keyboard::is_key_pressed(0x48))
			pos.y-=speed*duration;
		if(Keyboard::is_key_pressed(0x4B))
			pos.x-=speed*duration;
		if(Keyboard::is_key_pressed(0x4D))
			pos.x+=speed*duration;
		if(Keyboard::is_key_pressed(0x50))
			pos.y+=speed*duration;
		if(pos.x < 0) pos.x = 0;
		if(pos.x + 120 >= pixel_screen.width()) pos.x = pixel_screen.width() - 120;
		if(pos.y < 0) pos.y = 0;
		if(pos.y + 100 >= pixel_screen.height()) pos.y = pixel_screen.height() - 100;

		b.update(duration);

	};


	auto last_update = get_tick();
	while (1)
    {
    	asm volatile ( "hlt" );
		const auto cur_tick = get_tick();

		if(cur_tick >= last_update + 10)
		{
			update_world(cur_tick - last_update);
			last_update = cur_tick;
			repaint();
		}

		if(pixel_screen.should_repaint())
		{
			pixel_screen.update_screen();
		}
	}
}