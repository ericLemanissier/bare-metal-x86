module;

#include <source_location>

export module debug;


export void debug(const char* message, const std::source_location location =
               std::source_location::current());
