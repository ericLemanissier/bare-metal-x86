export module pixel;

import serial;
import debug;
import multiboot;
import <cstdint>;
import <cstdlib>;
import <array>;
import <algorithm>;
import geom;
import ll;

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
public:
    static constexpr auto MAX_WIDTH = 1024;
    static constexpr auto MAX_HEIGHT = 768;
    static constexpr auto BPP = 32;

private:

    alignas(64) static std::array<uint32_t, MAX_WIDTH * MAX_HEIGHT * BPP / 32> buf;

    uint64_t framebuffer_addr{};
    uint32_t framebuffer_pitch{};
    uint32_t framebuffer_width{};
    uint32_t framebuffer_height{};
    uint8_t framebuffer_bpp{};
    uint8_t framebuffer_type{};

    uint8_t state = 0;

    auto get_addr(Point p)
    {
        return buf.data() + p.x + p.y * this->framebuffer_pitch/4;
    }
public:
    explicit Pixel(const multiboot_info& mbi)
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

            if(this->framebuffer_bpp != BPP)
            {
                debug("Huho, wrong framebuffer bpp :/");
                std::abort();
            }
            if(this->framebuffer_height > MAX_HEIGHT)
            {
                debug("Huho, framebuffer height too big :/");
                std::abort();
            }
            if(this->framebuffer_width > MAX_WIDTH)
            {
                debug("Huho, framebuffer width too big :/");
                std::abort();
            }
        }
    }

    auto width() const
    {
        return this->framebuffer_width;
    }
    auto height() const
    {
        return this->framebuffer_height;
    }

    auto size() const
    {
        return Size(width(), height());
    }


    void fill_rect(Rect r, uint32_t color)
    {
        auto line_start = get_addr(r.top_left());
        for(std::size_t j = 0; j < r.height(); j++)
        {
            std::fill_n(line_start, r.width(), color);
            line_start += this->framebuffer_pitch/4;
        }
    }

    void fill_circle(Point center, std::size_t radius, uint32_t color)
    {
        const auto radius2 = radius*radius;

        auto top_line = get_addr(center);
        auto bottom_line = top_line;

        std::fill(top_line - radius, top_line + radius, color);

        for(std::size_t y = 1; y <= radius;y++)
        {
            top_line -= framebuffer_pitch/4;
            bottom_line += framebuffer_pitch/4;
            const auto expected = radius2 - y*y;
            for(std::size_t x = radius - y; x <= radius; x++)
                if(x*x >= expected)
                {
                    std::fill(top_line - x, top_line + x, color);
                    std::fill(bottom_line - x, bottom_line + x, color);
                    break;
                }
        }
    }

    void fill_circle(Rect rect, uint32_t color)
    {
        fill_circle(rect.center(), rect.width() / 2, color);
    }

    void clear_screen()
    {
        std::fill_n(reinterpret_cast<std::byte*>(buf.data()), framebuffer_width*framebuffer_height * 4, std::byte{});
    }

    void fill_screen(uint32_t color)
    {
        std::fill_n(buf.data(), framebuffer_width*framebuffer_height, color);
    }

    void update_screen()
    {
        std::copy_n(buf.data(), framebuffer_width*framebuffer_height, reinterpret_cast<uint32_t*>(this->framebuffer_addr));

    }

    bool should_repaint()
    {
        const auto val = inb(0x03DA);
        switch(state)
        {
        case 0:
            if((val & 0x08) == 0x08)
            {
                state++;
            }
            break;
        case 1:
            if((val & 0x08) == 0)
            {
                state = 0;
                return true;
            }
        }
        return false;

    }
};

std::array<uint32_t, Pixel::MAX_WIDTH * Pixel::MAX_HEIGHT * Pixel::BPP / 32> Pixel::buf = {};
