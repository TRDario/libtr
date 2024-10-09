export module tr:draw_geometry;

import std;
import glm;
import :angle;
import :geometry;
import :integer;

export namespace tr {
	// Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	inline Size smoothPolygonVerticesCount(float r) noexcept;
	// Calculates the number of segments needed to draw a smooth arc.
	inline Size smoothArcVerticesCount(float r, AngleF sizeth) noexcept;

	// Outputs indices for a convex polygon.
	// base - The starting index value.
	template <std::output_iterator<Ui16> It> constexpr void fillPolygonIndices(It out, Ui16 vtx, Ui16 base);
	// Outputs indices for a polygon outline.
	// base - The base index value.
	template <std::output_iterator<Ui16> It> constexpr void fillPolygonOutlineIndices(It out, Ui16 vtx, Ui16 base);

	// Outputs unrotated rect vertices.
	template <std::output_iterator<glm::vec2> It> constexpr void fillRectVertices(It out, glm::vec2 tl, glm::vec2 size);
	// Calculates vertex positions along an arc.
	template <std::output_iterator<glm::vec2> It> void fillArcVertices(It out, Size vtx, CircleF circ, AngleF startth, AngleF sizeth);
	// Calculates vertex positions for a polygon.
	// circ - The tangent circle of the polygon.
	template <std::output_iterator<glm::vec2> It> void fillPolygonVertices(It out, Size vtx, CircleF circ, AngleF rot = 0_degf);
	// Calculates vertex positions for a polygon outline.
	// The outer vertices are inserted first, followed by the inner vertices.
	// circle - The tangent circle of the polygon.
	template <std::output_iterator<glm::vec2> It> void fillPolygonOutlineVertices(It out, Size vtx, CircleF circ, AngleF rot, float thick);
}

// IMPLEMENTATION

inline tr::Size tr::smoothPolygonVerticesCount(float r) noexcept
{
	return std::max(Size(10 * std::sqrt(r)), Size(3));
}

inline tr::Size tr::smoothArcVerticesCount(float r, AngleF sizeth) noexcept
{
	return std::max(Size((10 * std::sqrt(r)) / (360_degf / (sizeth % 360_degf))), Size(3));
}

template <std::output_iterator<tr::Ui16> It>
constexpr void tr::fillPolygonIndices(It out, Ui16 vtx, Ui16 base)
{
	for (Ui16 i = 0; i < vtx - 2; ++i) {
		*out++ = base;
		*out++ = base + i + 1;
		*out++ = base + i + 2;
	}
}

template <std::output_iterator<tr::Ui16> It>
constexpr void tr::fillPolygonOutlineIndices(It out, Ui16 vtx, Ui16 base)
{
	for (Ui16 i = 0; i < vtx - 1; ++i) {
		*out++ = base + i;
		*out++ = base + i + 1;
		*out++ = base + i + vtx;
		*out++ = base + i + 1;
		*out++ = base + i + vtx;
		*out++ = base + i + vtx + 1;
	}
	*out++ = base + vtx - 1;
	*out++ = base;
	*out++ = base + 2 * vtx - 1;
	*out++ = base;
	*out++ = base + 2 * vtx - 1;
	*out++ = base + vtx;
}

template <std::output_iterator<glm::vec2> It>
constexpr void tr::fillRectVertices(It out, glm::vec2 tl, glm::vec2 size)
{
	*out++ = tl;
	*out++ = glm::vec2 { tl.x, tl.y + size.y };
	*out++ = tl + size;
	*out++ = glm::vec2 { tl.x + size.x, tl.y };
}

template <std::output_iterator<glm::vec2> It>
void tr::fillArcVertices(It out, Size vtx, CircleF circ, AngleF startth, AngleF sizeth)
{
	auto dth { sizeth / vtx };
	auto dsin { dth.sin() };
	auto dcos { dth.cos() };
	glm::vec2 delta { circ.r * startth.cos(), circ.r * startth.sin() };
	for (Size i = 0; i < vtx; ++i) {
		*out++ = delta + circ.c;
		delta = glm::vec2 { dcos * delta.x - dsin * delta.y, dsin * delta.x + dcos * delta.y };
	}
}

template <std::output_iterator<glm::vec2> It>
void tr::fillPolygonVertices(It out, Size vtx, CircleF circ, AngleF rot)
{
	fillArcVertices(out, vtx, circ, rot, 360_degf);
}

template <std::output_iterator<glm::vec2> It>
void tr::fillPolygonOutlineVertices(It out, Size vtx, CircleF circ, AngleF rot, float thick)
{
	fillPolygonVertices(out, vtx, { circ.c, circ.r + thick / 2 }, rot);
	fillPolygonVertices(out, vtx, { circ.c, circ.r - thick / 2 }, rot);
}