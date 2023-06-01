export module ball;

import geom;
import <cstdint>;

export struct Speed
{
    float x{};
    float y{};
};

export class Ball
{
    Point m_pos;
    Speed m_speed;
    Size m_size;
    Size m_world;


public:
    explicit Ball(const Point &point, const Speed &speed, const Size &size, const Size &world)
    :m_pos(point), m_speed(speed), m_size(size), m_world(world)
    {}

    auto pos() const
    {
        return m_pos;
    }

    auto size() const
    {
        return m_size;
    }

    void update(uint64_t elapsed)
    {
        m_pos.x += m_speed.x * elapsed;
        m_pos.y += m_speed.y * elapsed;

		if(m_pos.x < 0)
        {
            m_pos.x = 0;
            m_speed.x = -m_speed.x;
        }
		if(m_pos.x + m_size.w >= m_world.w)
        {
            m_pos.x = m_world.w - m_size.w;
            m_speed.x = -m_speed.x;
        }
		if(m_pos.y < 0)
        {
            m_pos.y = 0;
            m_speed.y = -m_speed.y;
        }
		if(m_pos.y + m_size.h >= m_world.h)
        {
            m_pos.y = m_world.h - m_size.h;
            m_speed.y = -m_speed.y;
        }
    }
};
