export module keyboard;

import <cstdint>;

export namespace Keyboard {

    void interrupt(uint8_t d);
    bool is_key_pressed(uint8_t d);
}
