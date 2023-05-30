module keyboard;

import <cstdint>;
import <bitset>;
import <cstdlib>;
import serial;

namespace Keyboard {


    std::bitset<0x80> keys_pressed{};

    void interrupt(uint8_t d)
    {
        if constexpr (false)
        {
            write_serial(d & 0x80 ? "Key released: " : "Key pressed: ");
            write_serial<16>(d & 0x7F);
            write_serial("\n");
        }
        keys_pressed.set(d & 0x7f, !(d & 0x80));
    }

    bool is_key_pressed(uint8_t d)
    {
        if(d >= keys_pressed.size())
        {
            write_serial("wrong key code: 0x");
            write_serial<16>(d);
            write_serial("\n");
            std::abort();
        }
        return keys_pressed[d];
    }
}
