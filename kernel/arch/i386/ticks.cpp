export module ticks;

import <cstdint>;

uint64_t ticks{};

export uint64_t get_tick()
{
    return ticks;
}

export void inc_ticks()
{
    ticks++;
}
