export module tty;

import <cstddef>;
import <cstdint>;

import vga;

import ll;

export {

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
}

constexpr size_t VGA_WIDTH = 80;
constexpr size_t VGA_HEIGHT = 25;

size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);;
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

void terminal_putentryat_(char c, uint8_t color, size_t x, size_t y)
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


void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}
