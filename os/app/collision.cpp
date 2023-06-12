export module collision;

import <cstdint>;
import <optional>;
import geom;

export struct DynRect
{
    Rect r;
    Speed speed;
};

export struct CollisionRes
{
    DynRect r;
    uint64_t t{};
};


export std::optional<CollisionRes> test_collision(DynRect dynR, Rect sR, uint64_t t)
{
    sR.m_top_left-=dynR.r.size/2;
    sR.size+=dynR.r.size;

    const auto begin_pos = dynR.r.center();
    const auto delta = dynR.speed * t;
    const auto end_pos = dynR.r.center() + delta;

    if(begin_pos.y <= sR.top())
    {
        if(end_pos.y > sR.top())
        {
            const auto r = static_cast<double>(sR.top() - begin_pos.y) / delta.y;
            const auto candidate_t = t * r;
            if(candidate_t <= t)
            {
                const auto inter = begin_pos.x + r * delta.x;
                if(inter >= sR.left() && inter <= sR.right())
                {
                    return CollisionRes{DynRect{Rect{Point{inter, sR.top()} - dynR.r.size/2,dynR.r.size}, {dynR.speed.x, -dynR.speed.y}}, candidate_t};
                }
            }
        }

    } else if(begin_pos.y >= sR.bottom())
    {
        if(end_pos.y < sR.bottom())
        {
            const auto r = static_cast<double>(begin_pos.y - sR.bottom()) / -delta.y;
            const auto candidate_t = t * r;
            if(candidate_t <= t)
            {
                const auto inter = begin_pos.x + r * delta.x;
                if(inter >= sR.left() && inter <= sR.right())
                {
                    return CollisionRes{DynRect{Rect{Point{inter, sR.bottom()} - dynR.r.size/2,dynR.r.size}, {dynR.speed.x, -dynR.speed.y}}, candidate_t};
                }
            }
        }
    }

    if(begin_pos.x <= sR.left())
    {
        if(end_pos.x > sR.left())
        {
            const auto r = static_cast<double>(sR.left() - begin_pos.x) / delta.x;
            const auto candidate_t = t * r;
            if(candidate_t <= t)
            {
                const auto inter = begin_pos.y + r * delta.y;
                if(inter >= sR.top() && inter <= sR.bottom())
                {
                    return CollisionRes{DynRect{Rect{Point{sR.left(), inter} - dynR.r.size/2,dynR.r.size}, {-dynR.speed.x, dynR.speed.y}}, candidate_t};
                }
            }
        }
    } else if(begin_pos.x >= sR.right())
    {
        if(end_pos.x < sR.right())
        {
            const auto r = static_cast<double>(begin_pos.x - sR.right()) / -delta.x;
            const auto candidate_t = t * r;
            if(candidate_t <= t)
            {
                const auto inter = begin_pos.y + r * delta.y;
                if(inter >= sR.top() && inter <= sR.bottom())
                {
                    return CollisionRes{DynRect{Rect{Point{sR.right(), inter} - dynR.r.size/2,dynR.r.size}, {-dynR.speed.x, dynR.speed.y}}, candidate_t};
                }
            }
        }
    }
    return std::nullopt;
}
