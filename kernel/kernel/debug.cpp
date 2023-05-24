#include "kernel/debug.h"

#include "kernel/idt.h"
#include "kernel/serial.h"

#define DEBUG_MESSAGE(m) debug(m, __FILE__, __LINE__, __PRETTY_FUNCTION__)

void debug(const char* message, const char* file, uint32_t line, const char* func)
{
	write_serial(get_tick());
	write_serial(" ");
	write_serial(file);
	write_serial(":");
	write_serial(line);
	write_serial(":");
	write_serial(func);
	write_serial(": ");
	write_serial(message);
	write_serial("\n");
}
