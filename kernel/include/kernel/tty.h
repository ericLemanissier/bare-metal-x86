#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <cstddef>
#include <cstdint>
#include "ll.h"

#include "kernel/vga.h"

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, size_t x, size_t y);



template<int base = 10>
void terminal_write_number(size_t h)
{
	if(const auto high = h / base)
		terminal_write_number<base>(high);
	const auto n = h % base;
	if(n > 9)
		terminal_putchar('A' + n - 10);
	else
		terminal_putchar('0' + n);
}

void disable_cursor();

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void update_cursor(int x, int y);

uint16_t get_cursor_position(void);

#endif
