#pragma once
#include "draw_geometry.hpp"

inline std::size_t tr::smoothPolygonVerticesCount(float r) noexcept
{
	return std::max(std::size_t(10 * std::sqrt(r)), std::size_t(3));
}

inline std::size_t tr::smoothArcVerticesCount(float r, AngleF sizeth) noexcept
{
	return std::max(std::size_t((10 * std::sqrt(r)) / (360_degf / (sizeth % 360_degf))), std::size_t(3));
}

template <std::output_iterator<std::uint16_t> It>
constexpr void tr::fillPolygonIndices(It out, std::uint16_t vertices, std::uint16_t base)
{
	for (std::uint16_t i = 0; i < vertices - 2; ++i) {
		*out++ = base;
		*out++ = base + i + 1;
		*out++ = base + i + 2;
	}
}

template <std::output_iterator<std::uint16_t> It>
constexpr void tr::fillPolygonOutlineIndices(It out, std::uint16_t vertices, std::uint16_t base)
{
	for (std::uint16_t i = 0; i < vertices - 1; ++i) {
		*out++ = base + i;
		*out++ = base + i + 1;
		*out++ = base + i + vertices;
		*out++ = base + i + 1;
		*out++ = base + i + vertices;
		*out++ = base + i + vertices + 1;
	}
	*out++ = base + vertices - 1;
	*out++ = base;
	*out++ = base + 2 * vertices - 1;
	*out++ = base;
	*out++ = base + 2 * vertices - 1;
	*out++ = base + vertices;
}

template <std::output_iterator<glm::vec2> It> constexpr void tr::fillRectVertices(It out, glm::vec2 tl, glm::vec2 size)
{
	*out++ = tl;
	*out++ = glm::vec2{tl.x, tl.y + size.y};
	*out++ = tl + size;
	*out++ = glm::vec2{tl.x + size.x, tl.y};
}

template <std::output_iterator<glm::vec2> It>
void tr::fillArcVertices(It out, std::size_t vertices, CircleF circ, AngleF startth, AngleF sizeth)
{
	auto      dth{sizeth / vertices};
	auto      dsin{dth.sin()};
	auto      dcos{dth.cos()};
	glm::vec2 delta{circ.r * startth.cos(), circ.r * startth.sin()};
	for (std::size_t i = 0; i < vertices; ++i) {
		*out++ = delta + circ.c;
		delta  = glm::vec2{dcos * delta.x - dsin * delta.y, dsin * delta.x + dcos * delta.y};
	}
}

template <std::output_iterator<glm::vec2> It>
void tr::fillPolygonVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation)
{
	fillArcVertices(out, vertices, circ, rotation, 360_degf);
}

template <std::output_iterator<glm::vec2> It>
void tr::fillPolygonOutlineVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation, float thick)
{
	fillPolygonVertices(out, vertices, {circ.c, circ.r + thick / 2}, rotation);
	fillPolygonVertices(out, vertices, {circ.c, circ.r - thick / 2}, rotation);
}