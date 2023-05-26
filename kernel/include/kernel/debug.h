#ifndef _KERNEL_DEBUG_H
#define _KERNEL_DEBUG_H

#include <source_location>

void debug(const char* message, const std::source_location location =
               std::source_location::current());

#endif
