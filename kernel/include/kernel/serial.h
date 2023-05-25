#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stdint.h>

int init_serial();

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial_char(char a);

inline void write_serial(const char*s)
{
    while(*s)
        write_serial_char(*s++);
};

template<int b = 10>
inline void write_serial(uint32_t i)
{
	if(const auto high = i / b)
		write_serial<b>(high);
	const auto n = i % b;
	if(n > 9)
		write_serial_char('A' + n - 10);
	else
		write_serial_char('0' + n);
};
#endif
