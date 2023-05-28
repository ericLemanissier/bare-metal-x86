export module pixel;

import serial;
import debug;
import multiboot;
import <cstdint>;
import geom;

export enum Color
{
    BLACK = 0x000000,
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
    YELLOW = RED | GREEN,
    CYAN = BLUE | GREEN,
    MAGENTA = RED | BLUE,
    WHITE = RED | GREEN | BLUE,
};

export class Pixel
{

    uint64_t framebuffer_addr{};
    uint32_t framebuffer_pitch{};
    uint32_t framebuffer_width{};
    uint32_t framebuffer_height{};
    uint8_t framebuffer_bpp{};
    uint8_t framebuffer_type{};

public:
    Pixel(const multiboot_info& mbi)
    {

        if(mbi.flags & 0x800)
        {
            debug("VBE info: ");
            write_serial<16>(mbi.vbe_mode);write_serial("\n");
        }
        if(mbi.flags & 0x1000)
        {
            debug("Frame Buffer info: ");
            write_serial(mbi.framebuffer_width); write_serial("x");
            write_serial(mbi.framebuffer_height); write_serial(" ");
            write_serial(mbi.framebuffer_pitch); write_serial(" ");
            write_serial(mbi.framebuffer_bpp); write_serial("\n");
            switch(mbi.framebuffer_type)
            {
            case 0:
                debug("Indexed colors");
                break;
            case 1:
                debug("RGB colors");
                break;
            case 2:
                debug("EGA text mode");
                break;
            default:
                debug("unknown framebuffer type: ");
                write_serial(mbi.framebuffer_type);write_serial("\n");
            }

            this->framebuffer_addr = mbi.framebuffer_addr;
            this->framebuffer_bpp = mbi.framebuffer_bpp;
            this->framebuffer_height = mbi.framebuffer_height;
            this->framebuffer_pitch = mbi.framebuffer_pitch;
            this->framebuffer_type = mbi.framebuffer_type;
            this->framebuffer_width = mbi.framebuffer_width;

            if(this->framebuffer_bpp != 32)
            {
	            debug("Huho, wrong framebuffer bpp :/");
                while(true){};
            }
        }
    }

    void fill_rect(Rect r, uint32_t color)
    {
        auto line_start = reinterpret_cast<uint8_t*>(this->framebuffer_addr);
        line_start += r.top_left().x * 4;
        line_start += r.top_left().y * this->framebuffer_pitch;
        for(int j = 0; j < r.height(); j++)
        {
            auto p = line_start;
            for(int i = 0; i < r.width(); i++)
            {
                *reinterpret_cast<uint32_t*>(p) = color;
                p += 4;
            }
            line_start += this->framebuffer_pitch;
        }

    }
};
