/**
 * @file draw_geometry.hpp
 * @brief Provides common geometry output functions used in drawing.
 */

#pragma once
#include "geometry.hpp"

namespace tr {
	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	 *
	 * @param r The radius of the circle. Must be in the range (0, +Inf.).
	 *
	 * @return The number of segments needed to draw a smooth circle with pixel radius r.
	 ******************************************************************************************************************/
	inline std::size_t smoothPolygonVerticesCount(float r) noexcept;

	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth arc.
	 *
	 * @param r The radius of the arc circle. Must be in the range (0, +Inf.).
	 * @param sizeth The angular size of the arc. Must be in the range (0, 360deg).
	 *
	 * @return The number of segments needed to draw a smooth arc.
	 ******************************************************************************************************************/
	inline std::size_t smoothArcVerticesCount(float r, AngleF sizeth) noexcept;

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param out The output iterator. There has to be space for `(vertices - 2) * 3` indices.
	 * @param vertices The number of vertices in the polygon. Must be greater than 2.
	 * @param base The "base" index offset added to every index value.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It>
	constexpr void fillPolygonIndices(It out, std::uint16_t vertices, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon outline.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param out The output iterator. There has to be space for `(vertices - 2) * 6` indices.
	 * @param vertices The number of vertices in the polygon. Must be greater than 2.
	 * @param base The "base" index offset added to every index value.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It>
	constexpr void fillPolygonOutlineIndices(It out, std::uint16_t vertices, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param out The output iterator. There has to be space for 4 vertices.
	 * @param tl The position of the top-left corner of the rectangle.
	 * @param size The size of the rectangle
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> constexpr void fillRectVertices(It out, glm::vec2 tl, glm::vec2 size);

	/******************************************************************************************************************
	 * Outputs vertex positions along an arc.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param out The output iterator. There has to be space for all of the vertices.
	 * @param vertices The number of vertices on the arc.
	 * @param circ The arc circle.
	 * @param startth The starting angle on the arc circle of the arc.
	 * @param sizeth The angular size of the arc.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	void fillArcVertices(It out, std::size_t vertices, CircleF circ, AngleF startth, AngleF sizeth);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param out The output iterator. There has to be space for all of the vertices.
	 * @param vertices The number of polygon vertices.
	 * @param circ The tangent circle of the polygon.
	 * @param rotation The rotation of the polygon.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	void fillPolygonVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation = 0_degf);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon's outline.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param out The output iterator. There has to be space for `vertices * 2`.
	 * @param vertices The number of polygon vertices.
	 * @param circ The tangent circle of the polygon.
	 * @param rotation The rotation of the polygon.
	 * @param thickness The thickness of the polygon.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	void fillPolygonOutlineVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation, float thickness);
} // namespace tr

/// @cond IMPLEMENTATION

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

/// @endcond
