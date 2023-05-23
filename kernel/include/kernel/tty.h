#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include "ll.h"

#ifdef __cplusplus
extern "C" {
#endif

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_setcolor(uint8_t color);
void terminal_putentryat(char c, size_t x, size_t y);

void disable_cursor();

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void update_cursor(int x, int y);

uint16_t get_cursor_position(void);
#ifdef __cplusplus
}
#endif

#endif
