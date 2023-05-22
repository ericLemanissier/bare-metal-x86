#include <kernel/tty.h>
#include "vga.h"

static constexpr size_t VGA_WIDTH = 80;
static constexpr size_t VGA_HEIGHT = 25;

size_t terminal_row = 0;
size_t terminal_column = 0;
static uint8_t terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);;
uint16_t* terminal_buffer = (uint16_t*) 0xB8000;

void terminal_initialize(void)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

static void terminal_putentryat_(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_newline()
{
	terminal_column = 0;
	if (++terminal_row == VGA_HEIGHT)
	{
		for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
			}
		}
		terminal_row--;
		for (size_t x = 0; x < VGA_WIDTH; x++)
			terminal_buffer[terminal_row * VGA_WIDTH + x] = 0;
	}
}


void terminal_putchar(char c)
{
	if(c == '\n')
	{
		terminal_newline();
	}
	else {
		terminal_putentryat_(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_newline();
		}
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	while(*data)
		terminal_putchar(*data++);
}


void terminal_putentryat(char c, size_t x, size_t y)
{
	terminal_putentryat_(c, terminal_color, x, y);
}

