export module game;

import geom;
import pixel;
import <array>;
import <cstdint>;
import keyboard;

export class Game
{
    static constexpr int N_LINES = 2;
    static constexpr int N_COLS = 10;
    Size m_size{};

    struct Brick
    {
        static constexpr Size SIZE = Size{50,20};
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
    };
    std::array<Brick, N_LINES*N_COLS> m_bricks;
    struct
    {
        Point pos{};
        struct
        {
            float x{};
            float y{};
        } speed{};
        Size size{};
    } m_ball;


	Point pos{};
	double speed{};

public:
    explicit Game(const Size &world_size):
    m_size(world_size),
	m_ball(Point(10,10), {0.3, 0.1}, Size(10, 10)),
	pos{90, 90},
    speed{0.2}
    {
        for(int i = 0; i < std::size(m_bricks); i++)
        {
            const auto row = i / N_COLS;
            const auto col = i % N_COLS;
            const auto color = Brick::COLORS[i % std::size(Brick::COLORS)];
            const auto x = 10+col*Brick::SIZE.w;
            const auto y = 10+row*(Brick::SIZE.h + 2);
            m_bricks[i] = Brick{Point(x, y), color};
        }
    }

    void draw(Pixel &pixel_screen) const
    {
		pixel_screen.clear_screen();
		pixel_screen.fill_rect(Rect{pos, Size{120,100}}, YELLOW);
		pixel_screen.fill_rect(Rect{Point{pos.x + 10, pos.y + 10}, Size{50,80}}, CYAN);
		pixel_screen.fill_rect(Rect{Point{pos.x + 60, pos.y + 10}, Size{50,30}}, MAGENTA);

        for(const auto &b:m_bricks)
            if(b.health) pixel_screen.fill_rect(Rect{b.pos, Brick::SIZE}, b.color);

		pixel_screen.fill_circle(Point(
			m_ball.pos.x + m_ball.size.w/2,
			m_ball.pos.y + m_ball.size.h/2), m_ball.size.w/2, BLUE);
    }


    void update(uint64_t elapsed)
    {
		if(Keyboard::is_key_pressed(0x48))
			pos.y-=speed*elapsed;
		if(Keyboard::is_key_pressed(0x4B))
			pos.x-=speed*elapsed;
		if(Keyboard::is_key_pressed(0x4D))
			pos.x+=speed*elapsed;
		if(Keyboard::is_key_pressed(0x50))
			pos.y+=speed*elapsed;
		if(pos.x < 0) pos.x = 0;
		if(pos.x + 120 >= m_size.w) pos.x = m_size.w - 120;
		if(pos.y < 0) pos.y = 0;
		if(pos.y + 100 >= m_size.h) pos.y = m_size.h - 100;


        m_ball.pos.x += m_ball.speed.x * elapsed;
        m_ball.pos.y += m_ball.speed.y * elapsed;

		if(m_ball.pos.x < 0)
        {
            m_ball.pos.x = 0;
            m_ball.speed.x = -m_ball.speed.x;
        }
		if(m_ball.pos.x + m_ball.size.w >= m_size.w)
        {
            m_ball.pos.x = m_size.w - m_ball.size.w;
            m_ball.speed.x = -m_ball.speed.x;
        }
		if(m_ball.pos.y < 0)
        {
            m_ball.pos.y = 0;
            m_ball.speed.y = -m_ball.speed.y;
        }
		if(m_ball.pos.y + m_ball.size.h >= m_size.h)
        {
            m_ball.pos.y = m_size.h - m_ball.size.h;
            m_ball.speed.y = -m_ball.speed.y;
        }
    }
};
