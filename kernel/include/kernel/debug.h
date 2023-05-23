#ifndef _KERNEL_DEBUG_H
#define _KERNEL_DEBUG_H

#include <stdint.h>

#define DEBUG_MESSAGE(m) debug(m, __FILE__, __LINE__, __PRETTY_FUNCTION__)

void debug(const char* message, const char* file, uint32_t line, const char* func);
#endif
