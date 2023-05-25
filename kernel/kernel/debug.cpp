#include "kernel/debug.h"

#include "kernel/idt.h"
#include "kernel/serial.h"

void debug(const char* message, const std::source_location location)
{
	write_serial(get_tick());
	write_serial(" ");
	write_serial(location.file_name());
	write_serial(":");
	write_serial(location.line());
	write_serial(":");
	write_serial(location.function_name());
	write_serial(": ");
	write_serial(message);
	write_serial("\n");
}
