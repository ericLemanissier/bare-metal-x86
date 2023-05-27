export module debug;

import <source_location>;


export void debug(const char* message, const std::source_location location =
               std::source_location::current());
