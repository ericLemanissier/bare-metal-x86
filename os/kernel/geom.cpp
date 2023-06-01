export module geom;

import <cstddef>;
import <cstdint>;


export struct Point
{
	int32_t x{};
	int32_t y{};
};

export struct Size
{
	size_t w{};
	size_t h{};
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
};
