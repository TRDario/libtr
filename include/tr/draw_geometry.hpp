#pragma once
#include "geometry.hpp"

namespace tr {
	/** @ingroup misc
	 *  @defgroup drawing Drawing
	 *  Helper functionality for drawing.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	 *
	 * @param[in] r The radius of the circle.
	 * @param[in] scale The field scale parameter to apply to the calculation.
	 *
	 * @return The number of segments needed to draw a smooth circle with pixel radius r.
	 ******************************************************************************************************************/
	inline std::size_t smoothPolygonVerticesCount(float r, float scale = 1.0f) noexcept;

	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth arc.
	 *
	 * @param[in] r The radius of the arc circle.
	 * @param[in] sizeth The angular size of the arc.
	 * @param[in] scale The field scale parameter to apply to the calculation.
	 *
	 * @return The number of segments needed to draw a smooth arc.
	 ******************************************************************************************************************/
	inline std::size_t smoothArcVerticesCount(float r, AngleF sizeth, float scale = 1.0f) noexcept;

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for `(vertices - 2) * 3` indices.
	 * @endparblock
	 * @param[in] vertices
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vertices must be greater than 2.
	 * @endparblock
	 * @param[in] base The "base" index offset added to every index value.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It>
	constexpr It fillPolygonIndices(It out, std::uint16_t vertices, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon outline.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for `(vertices - 2) * 6` indices.
	 * @endparblock
	 * @param[in] vertices
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vertices must be greater than 2.
	 * @endparblock
	 * @param[in] base The "base" index offset added to every index value.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It>
	constexpr It fillPolygonOutlineIndices(It out, std::uint16_t vertices, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] tl The position of the top-left corner of the rectangle.
	 * @param[in] size The size of the rectangle.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> constexpr It fillRectVertices(It out, glm::vec2 tl, glm::vec2 size);

	/******************************************************************************************************************
	 * Outputs rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] tl The position of the top-left corner of the rectangle.
	 * @param[in] size The size of the rectangle.
	 * @param[in] transform A transformation matrix to apply to the vertices.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	constexpr It fillRectVertices(It out, glm::vec2 tl, glm::vec2 size, const glm::mat4& transform);

	/******************************************************************************************************************
	 * Outputs rotated rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] pos The position of the anchor point of the rectangle.
	 * @param[in] posAnchor The position of the anchor point within the rectangle ((0, 0) = top-left).
	 * @param[in] size The size of the rectangle.
	 * @param[in] rotation The rotation of the rectangle.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	inline It fillRotatedRectangleVertices(It out, glm::vec2 pos, glm::vec2 posAnchor, glm::vec2 size, AngleF rotation);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle outline vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] tl The position of the top-left corner of the rectangle.
	 * @param[in] size The size of the rectangle.
	 * @param[in] thickness The thickness of the outline.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	constexpr It fillRectOutlineVertices(It out, glm::vec2 tl, glm::vec2 size, float thickness);

	/******************************************************************************************************************
	 * Outputs rectangle outline vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] tl The position of the top-left corner of the rectangle.
	 * @param[in] size The size of the rectangle.
	 * @param[in] thickness The thickness of the outline.
	 * @param[in] transform A transformation matrix to apply to the vertices.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	constexpr It fillRectOutlineVertices(It out, glm::vec2 tl, glm::vec2 size, float thickness,
										 const glm::mat4& transform);

	/******************************************************************************************************************
	 * Outputs rotated rectangle outline vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] pos The position of the anchor point of the rectangle.
	 * @param[in] posAnchor The position of the anchor point within the rectangle ((0, 0) = top-left).
	 * @param[in] size The size of the rectangle.
	 * @param[in] rotation The rotation of the rectangle.
	 * @param[in] thickness The thickness of the outline.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	inline It fillRotatedRectangleOutlineVertices(It out, glm::vec2 pos, glm::vec2 posAnchor, glm::vec2 size,
												  AngleF rotation, float thickness);

	/******************************************************************************************************************
	 * Outputs vertex positions along an arc.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for all of the vertices.
	 * @endparblock
	 * @param[in] vertices
	 * @parblock
	 * The number of vertices on the arc.
	 *
	 * @pre @em vertices must be greater than 2.
	 * @endparblock
	 * @param[in] circ The arc circle.
	 * @param[in] startth The starting angle on the arc circle of the arc.
	 * @param[in] sizeth The angular size of the arc.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	It fillArcVertices(It out, std::size_t vertices, CircleF circ, AngleF startth, AngleF sizeth);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for all of the vertices.
	 * @endparblock
	 * @param[in] vertices
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vertices must be greater than 2.
	 * @endparblock
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rotation The rotation of the polygon.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	It fillPolygonVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation = 0_degf);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon's outline.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for `vertices * 2`.
	 * @endparblock
	 * @param[in] vertices
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vertices must be greater than 2.
	 * @endparblock
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rotation The rotation of the polygon.
	 * @param[in] thickness The thickness of the polygon.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	It fillPolygonOutlineVertices(It out, std::size_t vertices, CircleF circ, AngleF rotation, float thickness);

	/// @}
} // namespace tr

#include "draw_geometry_impl.hpp"