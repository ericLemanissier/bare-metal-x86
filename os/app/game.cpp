export module game;

import <cstdint>;
import <numbers>;
import <array>;
import <cstdlib>;
import <algorithm>;
import geom;
import pixel;
import keyboard;
import collision;

export class Game
{
    static constexpr int N_LINES = 3;
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
        int health{3};
        Rect rect() const
        {
            return {pos, SIZE};
        }
    };
    std::array<Brick, N_LINES*N_COLS> m_bricks;

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
        bool sticky{false};
    } m_paddle;
    DynRect m_ball;
    bool m_ball_stuck{true};

public:
    explicit Game(const Size &world_size):
    m_size(world_size),
    m_paddle(Point((m_size.w - Paddle::SIZE.w) / 2,
                    m_size.h -Paddle::SIZE.h)),
    m_ball(Rect(Point(m_paddle.rect().center().x,m_paddle.rect().top() - 11), Size(10, 10)), {-0.1, -0.3})
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
            if(b.health)
            {
                Rect r = {b.pos, Brick::SIZE};
                for(int i = 0; i < b.health; i++)
                {
                    pixel_screen.draw_rect(r, WHITE);
                    r.m_top_left += Delta(1,1);
                    r.size -= Size(2,2);
                }
                pixel_screen.fill_rect(r, b.color);
            }

        pixel_screen.fill_circle(Point(
            m_ball.r.center().x,
            m_ball.r.center().y), m_ball.r.width()/2, BLUE);
    }


    void update(std::uint64_t elapsed)
    {
        /*if(Keyboard::is_key_pressed(0x48))
            pos.y-=speed*elapsed;*/
        int deltaX = 0;
        if(Keyboard::is_key_pressed(0x4B))
            deltaX = -std::min<double>(Paddle::speed*elapsed, m_paddle.pos.x);

        if(Keyboard::is_key_pressed(0x4D))
            deltaX = +std::min<double>(Paddle::speed*elapsed, m_size.w - Paddle::SIZE.w - m_paddle.pos.x);


        /*if(Keyboard::is_key_pressed(0x50))
            pos.y+=speed*elapsed;*/

        if(Keyboard::is_key_pressed(0x39))
            m_ball_stuck = false;

        m_paddle.pos.x+=deltaX;
        if(m_ball_stuck)
            m_ball.r.m_top_left.x += deltaX;
        else
        {
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
                        else if(b->health == 0)
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
    }

    bool is_finished() const
    {
        return std::ranges::all_of(m_bricks, [](const auto h){return h == 0;}, &Brick::health);
    }
};
