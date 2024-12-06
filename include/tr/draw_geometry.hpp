#pragma once
#include "geometry.hpp"

namespace tr {
	/** @defgroup drawing Drawing Helpers
	 *  Helper functionality for drawing.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	 *
	 * @param[in] r The radius of the circle. Must be in the range (0, +Inf.).
	 *
	 * @return The number of segments needed to draw a smooth circle with pixel radius r.
	 ******************************************************************************************************************/
	inline std::size_t smoothPolygonVerticesCount(float r) noexcept;

	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth arc.
	 *
	 * @param[in] r The radius of the arc circle. Must be in the range (0, +Inf.).
	 * @param[in] sizeth The angular size of the arc. Must be in the range (0, 360deg).
	 *
	 * @return The number of segments needed to draw a smooth arc.
	 ******************************************************************************************************************/
	inline std::size_t smoothArcVerticesCount(float r, AngleF sizeth) noexcept;

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param[out] out The output iterator. There has to be space for `(vertices - 2) * 3` indices.
	 * @param[in] vertices The number of vertices in the polygon. Must be greater than 2.
	 * @param[in] base The "base" index offset added to every index value.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It>
	constexpr void fillPolygonIndices(It out, std::uint16_t vertices, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon outline.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param[out] out The output iterator. There has to be space for `(vertices - 2) * 6` indices.
	 * @param[in] vertices The number of vertices in the polygon. Must be greater than 2.
	 * @param[in] base The "base" index offset added to every index value.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It>
	constexpr void fillPolygonOutlineIndices(It out, std::uint16_t vertices, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out The output iterator. There has to be space for 4 vertices.
	 * @param[in] tl The position of the top-left corner of the rectangle.
	 * @param[in] size The size of the rectangle
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> constexpr void fillRectVertices(It out, glm::vec2 tl, glm::vec2 size);

	/******************************************************************************************************************
	 * Outputs vertex positions along an arc.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out The output iterator. There has to be space for all of the vertices.
	 * @param[in] vertices The number of vertices on the arc.
	 * @param[in] circ The arc circle.
	 * @param[in] startth The starting angle on the arc circle of the arc.
	 * @param[in] sizeth The angular size of the arc.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	void fillArcVertices(It out, std::size_t vertices, CircleF circ, AngleF startth, AngleF sizeth);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out The output iterator. There has to be space for all of the vertices.
	 * @param[in] vertices The number of polygon vertices.
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rotation The rotation of the polygon.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	void fillPolygonVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation = 0_degf);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon's outline.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out The output iterator. There has to be space for `vertices * 2`.
	 * @param[in] vertices The number of polygon vertices.
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rotation The rotation of the polygon.
	 * @param[in] thickness The thickness of the polygon.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	void fillPolygonOutlineVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation, float thickness);

	/// @}
} // namespace tr

#include "draw_geometry_impl.hpp"