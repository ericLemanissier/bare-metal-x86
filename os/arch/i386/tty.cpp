export module tty;

import <cstddef>;
import <cstdint>;

import vga;
import geom;
import ll;



constexpr size_t VGA_WIDTH = 80;
constexpr size_t VGA_HEIGHT = 25;

size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);;
uint16_t* terminal_buffer = (uint16_t*) 0xB8000;

export void terminal_clear(void)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

export void terminal_setcolor(uint8_t color)
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


export void terminal_putchar(char c)
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


export template<int base = 10>
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

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

export void terminal_writestring(const char* data)
{
	while(*data)
		terminal_putchar(*data++);
}


void terminal_putentryat(char c, size_t x, size_t y)
{
	terminal_putentryat_(c, terminal_color, x, y);
}


export void disable_cursor()
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


export enum class Outline
{
	Single,
	Double
};

void terminal_putentryat(char c, Point p)
{
	terminal_putentryat(c, p.x, p.y);
}

export void terminal_rect(const Rect &rect, Outline o = Outline::Single)
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
