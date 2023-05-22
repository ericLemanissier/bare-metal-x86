#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

extern "C" void kernel_main(void);

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static constexpr uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static constexpr uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

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

static constexpr size_t VGA_WIDTH = 80;
static constexpr size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
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

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
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
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
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
	terminal_write(data, strlen(data));
}

void terminal_write_hex_nibble(uint8_t n)
{
	if(n > 9)
		terminal_putchar('A' + n - 10);
	else
		terminal_putchar('0' + n);
}

void terminal_writehex(size_t h)
{
	if(const auto high = h / 16)
		terminal_writehex(high);
	terminal_write_hex_nibble(h % 16);
}

enum class Outline
{
	Single,
	Double
};


void terminal_putentryat(char c, size_t x, size_t y, uint8_t color = terminal_color)
{
	terminal_putentryat(c, color, x, y);
}

void terminal_putentryat(char c, Point p, uint8_t color = terminal_color)
{
	terminal_putentryat(c, color, p.x, p.y);
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

void kernel_main(void)
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
	terminal_setcolor(VGA_COLOR_BLUE | VGA_COLOR_WHITE << 4);
	terminal_writestring("how are you doing ?\n I'm doing fine\n");
	terminal_setcolor(VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4);
	/*const auto n = 9;
	for(size_t i = n * VGA_HEIGHT; i < (n + 1) * VGA_HEIGHT; i++)
	{
		terminal_writestring("0x");
		terminal_writehex(i);
		terminal_putchar(':');
		terminal_putchar(i);
		terminal_putchar('\n');
	}*/
	//terminal_writestring("0x");terminal_writehex(0);
	terminal_writestring("0x");terminal_writehex(0);
	terminal_writestring(" 0x");terminal_writehex(1);
	terminal_writestring(" 0x");terminal_writehex(255);
	terminal_writestring(" 0x");terminal_writehex(256);
	terminal_writestring(" 0x");terminal_writehex(65535);
	terminal_writestring(" 0x");terminal_writehex(65536);
	terminal_rect(Rectangle{Point{9, 9}, Size{12,10}}, Outline::Single);
	terminal_rect(Rectangle{Point{10, 10}, Size{5,8}}, Outline::Double);
	terminal_rect(Rectangle{Point{15, 10}, Size{5,3}}, Outline::Single);
}
