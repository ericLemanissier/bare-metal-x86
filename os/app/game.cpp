export module game;

import <cstdint>;
import <numbers>;
import <array>;
import <cstdlib>;
import geom;
import pixel;
import keyboard;
import collision;

export class Game
{
    static constexpr int N_LINES = 2;
    static constexpr int N_COLS = 10;
    Size m_size{};

    struct Brick
    {
        static constexpr Size SIZE = Size{80,30};
        static constexpr std::array COLORS{
            RED,
            GREEN,
            BLUE,
            YELLOW,
            CYAN,
            MAGENTA,
            WHITE,
        };
        Point pos{};
        Color color{};
        int health{1};
        Rect rect() const
        {
            return {pos, SIZE};
        }
    };
    std::array<Brick, N_LINES*N_COLS> m_bricks;
    DynRect m_ball;

    struct Paddle
    {
        static constexpr Size SIZE = Size{120,10};
        static constexpr Color color = WHITE;
        static constexpr double speed = 1;
        Point pos{};
        Rect rect() const
        {
            return {pos, SIZE};
        }
    } m_paddle;

public:
    explicit Game(const Size &world_size):
    m_size(world_size),
    m_ball(Rect(Point(10,10), Size(10, 10)), {0.1, 0.3}),
    m_paddle(Point((m_size.w - Paddle::SIZE.w) / 2,
                    m_size.h -Paddle::SIZE.h))
    {
        for(std::size_t i = 0; i < std::size(m_bricks); i++)
        {
            const auto row = i / N_COLS;
            const auto col = i % N_COLS;
            const auto color = Brick::COLORS[i % std::size(Brick::COLORS)];
            const auto x = 30+col*Brick::SIZE.w;
            const auto y = 40+row*(Brick::SIZE.h);
            m_bricks[i] = Brick{Point(x, y), color};
        }
    }

    void draw(Pixel &pixel_screen) const
    {
        pixel_screen.clear_screen();
        pixel_screen.draw_rect({Point{0,0}, this->m_size}, BLUE);
        pixel_screen.fill_rect(m_paddle.rect(), Paddle::color);

        for(const auto &b:m_bricks)
            if(b.health) pixel_screen.fill_rect(Rect{b.pos, Brick::SIZE}, b.color);

        pixel_screen.fill_circle(Point(
            m_ball.r.center().x,
            m_ball.r.center().y), m_ball.r.width()/2, BLUE);
    }


    void update(std::uint64_t elapsed)
    {
        /*if(Keyboard::is_key_pressed(0x48))
            pos.y-=speed*elapsed;*/
        if(Keyboard::is_key_pressed(0x4B))
            m_paddle.pos.x-=Paddle::speed*elapsed;
        if(Keyboard::is_key_pressed(0x4D))
            m_paddle.pos.x+=Paddle::speed*elapsed;
        /*if(Keyboard::is_key_pressed(0x50))
            pos.y+=speed*elapsed;*/
        if(m_paddle.pos.x < 0) m_paddle.pos.x = 0;
        if(m_paddle.pos.x + Paddle::SIZE.w >= m_size.w) m_paddle.pos.x = m_size.w - Paddle::SIZE.w;
        /*if(pos.y < 0) pos.y = 0;
        if(pos.y + 100 >= m_size.h) pos.y = m_size.h - 100;*/


        {
            auto remaining = elapsed;
            while(remaining)
            {
                auto collision = test_collision(m_ball, m_paddle.rect(), remaining);

                bool paddleColision = collision.has_value();

                Brick *b= nullptr;
                for(auto &brick: m_bricks)
                {
                    if(brick.health)
                    {
                        const auto collision_candidate = test_collision(m_ball, brick.rect(), remaining);
                        if(collision_candidate.has_value() && (!collision.has_value() || collision_candidate->t < collision->t))
                        {
                            collision = collision_candidate;
                            b = &brick;
                            paddleColision = false;
                        }
                    }
                }
                if(b)
                    b->health--;

                if(collision.has_value())
                {
                    m_ball = collision->r;
                    remaining-=collision->t;
                    if(paddleColision)
                    {
                        const auto mod = __builtin_sqrt(m_ball.speed.x * m_ball.speed.x + m_ball.speed.y * m_ball.speed.y);
                        const auto angle = (m_ball.r.center() - m_paddle.rect().center()).x*std::numbers::pi_v<double> / m_paddle.rect().width();
                        m_ball.speed.x = __builtin_sin(angle) * mod;
                        m_ball.speed.y = -__builtin_cos(angle) * mod;
                    }
                    else
                    {
                        m_ball.speed.x *= 1.05;
                        m_ball.speed.y *= 1.05;
                    }
                }
                else
                {
                    m_ball.r.m_top_left += m_ball.speed * remaining;
                    remaining = 0;
                }
            }
        }


        if(m_ball.r.left() < 0)
        {
            m_ball.r.m_top_left.x = 0;
            m_ball.speed.x = -m_ball.speed.x;
        }
        if(m_ball.r.right() >= m_size.w)
        {
            m_ball.r.m_top_left.x = m_size.w - m_ball.r.size.w;
            m_ball.speed.x = -m_ball.speed.x;
        }
        if(m_ball.r.top() < 0)
        {
            m_ball.r.m_top_left.y = 0;
            m_ball.speed.y = -m_ball.speed.y;
        }
        if(m_ball.r.bottom() >= m_size.h)
        {
            std::abort();
        }
    }
};
