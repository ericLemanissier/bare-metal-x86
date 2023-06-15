export module geom;

import <cstddef>;
import <cstdint>;


export struct Delta
{
    int32_t x{};
    int32_t y{};
};

export struct Size
{
    size_t w{};
    size_t h{};

    Size operator*(int i) const
    {
        return {w*i, h*i};
    }

    Size operator/(int i) const
    {
        return {w/i, h/i};
    }

    Size& operator+=(Size o)
    {
        w+=o.w;
        h+=o.h;
        return *this;
    }

    Size& operator-=(Size o)
    {
        w -= o.w;
        h -= o.h;
        return *this;
    }
};

export struct Point
{
    int32_t x{};
    int32_t y{};

    Point operator+(Size d) const
    {
        return {x+d.w, y+d.h};
    }

    Point operator-(Size d) const
    {
        return {x-d.w, y-d.h};
    }

    Point operator+(Delta d) const
    {
        return {x+d.x, y+d.y};
    }

    Point& operator-=(Size d)
    {
        x-=d.w;
        y-=d.h;
        return *this;
    }

    Point& operator+=(Size d)
    {
        x+=d.w;
        y+=d.h;
        return *this;
    }

    Point& operator+=(Delta d)
    {
        x+=d.x;
        y+=d.y;
        return *this;
    }
};

export Delta operator-(Point p1, Point p2)
{
    return {p1.x-p2.x, p1.y - p2.y};
};

export struct Speed
{
    double x{};
    double y{};

    double mod{};
    double angle{};

    Delta operator*(uint64_t t) const
    {
        return {x*t,y*t};
    }
};

export struct Rect
{
    Point m_top_left{};
    Size size{};

    constexpr Point top_left() const
    {
        return m_top_left;
    }

    constexpr Point top_right() const
    {
        return {m_top_left.x + width() - 1, m_top_left.y};
    }

    constexpr Point bottom_left() const
    {
        return {m_top_left.x, m_top_left.y + height() - 1};
    }

    constexpr Point bottom_right() const
    {
        return {m_top_left.x + width() - 1, m_top_left.y + height() - 1};
    }

    constexpr size_t width() const
    {
        return size.w;
    }

    constexpr size_t height() const
    {
        return size.h;
    }

    constexpr Point center() const
    {
        return {m_top_left.x + width()/2, m_top_left.y + height()/2};
    }

    constexpr int32_t left() const
    {
        return m_top_left.x;
    }

    constexpr int32_t right() const
    {
        return bottom_right().x;
    }

    constexpr int32_t top() const
    {
        return m_top_left.y;
    }

    constexpr int32_t bottom() const
    {
        return bottom_right().y;
    }
};
