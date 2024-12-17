#pragma once
#include "angle.hpp"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <optional>

namespace tr {
	/** @ingroup misc
	 *  @defgroup geometry Geometry
	 *  Geometry-related datatypes and functions.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Rectangle object.
	 ******************************************************************************************************************/
	template <int S, class T> struct Rect {
		/**************************************************************************************************************
		 * The offset of the top-left corner of the rect.
		 **************************************************************************************************************/
		glm::vec<S, T> tl;

		/**************************************************************************************************************
		 * The size of the rectangle.
		 **************************************************************************************************************/
		glm::vec<S, T> size;

		/**************************************************************************************************************
		 * Default-constructs a rect.
		 **************************************************************************************************************/
		constexpr Rect() = default;

		/**************************************************************************************************************
		 * Constructs a rect.
		 *
		 * @param[in] tl The offset of the top-left corner of the rect.
		 * @param[in] size The size of the rectangle.
		 **************************************************************************************************************/
		constexpr Rect(glm::vec<S, T> tl, glm::vec<S, T> size) noexcept;

		/**************************************************************************************************************
		 * Constructs a rect with the top-left corner at the origin.
		 *
		 * @param[in] size The size of the rectangle.
		 **************************************************************************************************************/
		constexpr Rect(glm::vec<S, T> size) noexcept;

		/**************************************************************************************************************
		 * Copy-constructs a rect.
		 *
		 * @param[in] rect The rect to copy (may be of a different base type).
		 **************************************************************************************************************/
		template <class T1> constexpr Rect(const Rect<S, T1>& rect) noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		template <class T1> constexpr bool operator==(const Rect<S, T1>&) const noexcept;

		/**************************************************************************************************************
		 * Determines whether a point is contained inside the rect.
		 *
		 * @param[in] point The point to check.
		 *
		 * @return True if the point within the bounds of the rect and false otherwise.
		 **************************************************************************************************************/
		template <class T1> constexpr bool contains(glm::vec<S, T1> point) const noexcept;
	};

	/******************************************************************************************************************
	 * 2D rectangle.
	 ******************************************************************************************************************/
	template <class T> using Rect2 = Rect<2, T>;

	/******************************************************************************************************************
	 * Shorthand for an int 2D rect.
	 ******************************************************************************************************************/
	using RectI2 = Rect2<int>;

	/******************************************************************************************************************
	 * Shorthand for a float 2D rect.
	 ******************************************************************************************************************/
	using RectF2 = Rect2<float>;

	/******************************************************************************************************************
	 * 3D rectangle.
	 ******************************************************************************************************************/
	template <class T> using Rect3 = Rect<3, T>;

	/******************************************************************************************************************
	 * Shorthand for an int 3D rect.
	 ******************************************************************************************************************/
	using RectI3 = Rect3<int>;

	/******************************************************************************************************************
	 * Shorthand for a float 3D rect.
	 ******************************************************************************************************************/
	using RectF3 = Rect3<float>;

	/******************************************************************************************************************
	 * 2D circle.
	 ******************************************************************************************************************/
	template <class T> struct Circle {
		/**************************************************************************************************************
		 * The offset of the center of the circle.
		 **************************************************************************************************************/
		glm::tvec2<T> c;

		/**************************************************************************************************************
		 * The radius of the circle.
		 **************************************************************************************************************/
		T r;

		/**************************************************************************************************************
		 * Default-constructs a circle.
		 **************************************************************************************************************/
		constexpr Circle() = default;

		/**************************************************************************************************************
		 * Constructs a circle.
		 *
		 * @param[in] c The offset of the center of the circle.
		 * @param[in] r The radius of the circle.
		 **************************************************************************************************************/
		constexpr Circle(glm::tvec2<T> c, T r) noexcept;

		/**************************************************************************************************************
		 * Copy-constructs a circle.
		 *
		 * @param[in] circle The circle to copy (may be of a different base type).
		 **************************************************************************************************************/
		template <class T1> constexpr Circle(const Circle<T1>& circle) noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		template <class T1> constexpr bool operator==(const Circle<T1>& r) const noexcept;

		/**************************************************************************************************************
		 * Determines whether a point is contained inside the circle.
		 *
		 * @param[in] point The point to check.
		 *
		 * @return True if the point within the bounds of the circle and false otherwise.
		 **************************************************************************************************************/
		template <class T1> constexpr bool contains(glm::tvec2<T1> point) const noexcept;
	};

	/******************************************************************************************************************
	 * Shorthand for a float circle.
	 ******************************************************************************************************************/
	using CircleF = Circle<float>;

	/******************************************************************************************************************
	 * Determines whether 3 points are collinear.
	 *
	 * @param[in] a, b, c The points to check for collinearity.
	 *
	 * @return True if the points are collinear, and false otherwise.
	 ******************************************************************************************************************/
	template <class T1, class T2, class T3>
	constexpr bool collinear(glm::tvec2<T1> a, glm::tvec2<T2> b, glm::tvec2<T3> c) noexcept;

	/******************************************************************************************************************
	 * Determines if two rects intersect.
	 *
	 * @param[in] r1, r2 The rects to check for intersection.
	 *
	 * @return True if the rects intersect, and false otherwise.
	 ******************************************************************************************************************/
	template <class T1, class T2> bool intersecting(const Rect2<T1>& r1, const Rect2<T2>& r2) noexcept;

	/******************************************************************************************************************
	 * Determines if two circles intersect.
	 *
	 * @param[in] c1, c2 The circles to check for intersection.
	 *
	 * @return True if the circles intersect, and false otherwise.
	 ******************************************************************************************************************/
	template <class T> bool intersecting(const Circle<T>& c1, const Circle<T>& c2) noexcept;

	/******************************************************************************************************************
	 * Calculates the closest point to p on the line ab.
	 *
	 * @param[in] p A point.
	 * @param[in] a, b The start and end point of the line segment.
	 *
	 * @return The closest point to p located on the line ab.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	constexpr glm::tvec2<T> closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept;

	/******************************************************************************************************************
	 * Calculates the closest point to p on the line ab.
	 *
	 * @param[in] p A point.
	 * @param[in] a, b The start and end point of the line segment.
	 *
	 * @return The closest point to p located on the line ab.
	 ******************************************************************************************************************/
	template <std::integral T>
	constexpr glm::dvec2 closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two line segments.
	 *
	 * @param[in] a1, b1 Delimiting points of the first line segment.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	constexpr std::optional<glm::tvec2<T>> segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
															glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two line segments.
	 *
	 * @param[in] a1, b1 Delimiting points of the first line segment.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::integral T>
	constexpr std::optional<glm::dvec2> segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
														 glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of a line with a line segment.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	std::optional<glm::tvec2<T>> segmentIntersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2,
												  glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of a line with a line segment.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, b2 Delimiting points of the second line segment.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::integral T>
	std::optional<glm::dvec2> segmentIntersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2,
											   glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, th2 Point and angle of the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, Angle<T> th2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, th2 Point and angle of the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::integral T>
	std::optional<glm::dvec2> intersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2, AngleD th2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, b1 Two points on the first line.
	 * @param[in] a2, b2 Two points on the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	constexpr std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
													 glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the intersection point of two lines.
	 *
	 * @param[in] a1, th1 Point and angle of the first line.
	 * @param[in] a2, b2 Two points on the second line.
	 *
	 * @return The intersection point, if one exists.
	 ******************************************************************************************************************/
	template <std::floating_point T>
	std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	/******************************************************************************************************************
	 * Calculates the 2D cross product of two vectors.
	 *
	 * @param[in] a, b The two vectors.
	 *
	 * @return The 2D cross product of the vectors, standard promotion rules apply.
	 ******************************************************************************************************************/
	template <class T1, class T2>
	constexpr std::common_type_t<T1, T2> cross2(glm::tvec2<T1> a, glm::tvec2<T2> b) noexcept;

	/******************************************************************************************************************
	 * Generic modulo operation.
	 *
	 * @param[in] v The value to modulo.
	 * @param[in] mod The modulus.
	 *
	 * @return The modulo of v by mod, standard promotion rules apply.
	 ******************************************************************************************************************/
	template <Arithmetic T1, Arithmetic T2> std::common_type_t<T1, T2> mod(T1 v, T2 mod) noexcept;

	/******************************************************************************************************************
	 * Performs a mirror repeat mapping.
	 *
	 * @param[in] v The value to map.
	 * @param[in] min, max The bounds of the range.
	 *
	 * @return v mirror mapped to [min, max).
	 ******************************************************************************************************************/
	template <Arithmetic T> T mirrorRepeat(T v, T min, T max) noexcept;

	/******************************************************************************************************************
	 * Performs a mirror repeat mapping.
	 *
	 * @param[in] v The value to map.
	 * @param[in] min, max The bounds of the range.
	 *
	 * @return v mirror mapped to [min, max).
	 ******************************************************************************************************************/
	template <int S, class T>
	glm::vec<S, T> mirrorRepeat(const glm::vec<S, T>& v, const glm::vec<S, T>& min, const glm::vec<S, T>& max) noexcept;

	/******************************************************************************************************************
	 * Performs a mirror repeat mapping of a vector on a rect.
	 *
	 * @param[in] v The value to map.
	 * @param[in] rect The bounds of the range.
	 *
	 * @return v mirror mapped to rect.
	 ******************************************************************************************************************/
	template <std::size_t S, class T>
	glm::vec<S, T> mirrorRepeat(const glm::vec<S, T>& v, const Rect<S, T>& rect) noexcept;

	/******************************************************************************************************************
	 * Gets the inverse of a vector.
	 *
	 * @param[in] vec The vector to invert.
	 *
	 * @return The inverse vector.
	 ******************************************************************************************************************/
	template <std::size_t S, std::floating_point T>
	constexpr glm::vec<S, T> inverse(const glm::vec<S, T>& vec) noexcept;

	/******************************************************************************************************************
	 * Gets the inverse of a vector.
	 *
	 * @param[in] vec The vector to invert.
	 *
	 * @return The inverse vector.
	 ******************************************************************************************************************/
	template <std::size_t S, std::integral T> constexpr glm::vec<S, double> inverse(const glm::vec<S, T>& vec) noexcept;

	/******************************************************************************************************************
	 * Matrix multiplication operators.
	 ******************************************************************************************************************/
	inline namespace matrix_operators {
		/**************************************************************************************************************
		 * Performs matrix multiplication on a 2D vector.
		 *
		 * @param[in] v A 2D vector.
		 * @param[in] m A matrix.
		 *
		 * @return `v * m`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec2<T> operator*(const glm::tvec2<T>& v, const glm::tmat4x4<T>& m) noexcept;

		/**************************************************************************************************************
		 * Performs matrix multiplication on a 3D vector.
		 *
		 * @param[in] v A 3D vector.
		 * @param[in] m A matrix.
		 *
		 * @return `v * m`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec3<T> operator*(const glm::tvec3<T>& v, const glm::tmat4x4<T>& m) noexcept;

		/**************************************************************************************************************
		 * Performs matrix multiplication on a 2D vector.
		 *
		 * @param[in] m A matrix.
		 * @param[in] v A 2D vector.
		 *
		 * @return `m * v`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec2<T> operator*(const glm::tmat4x4<T>& m, const glm::tvec2<T>& v) noexcept;

		/**************************************************************************************************************
		 * Performs matrix multiplication on a 3D vector.
		 *
		 * @param[in] m A matrix.
		 * @param[in] v A 3D vector.
		 *
		 * @return `m * v`.
		 **************************************************************************************************************/
		template <class T> constexpr glm::tvec3<T> operator*(const glm::tmat4x4<T>& m, const glm::tvec3<T>& v) noexcept;
	} // namespace matrix_operators

	/******************************************************************************************************************
	 * Normalized X-axis vector.
	 ******************************************************************************************************************/
	inline constexpr glm::vec3 X_AXIS{1.0f, 0.0f, 0.0f};

	/******************************************************************************************************************
	 * Normalized Y-axis vector.
	 ******************************************************************************************************************/
	inline constexpr glm::vec3 Y_AXIS{0.0f, 1.0f, 0.0f};

	/******************************************************************************************************************
	 * Normalized Z-axis vector.
	 ******************************************************************************************************************/
	inline constexpr glm::vec3 Z_AXIS{0.0f, 0.0f, 1.0f};

	/******************************************************************************************************************
	 * Applies a 2D scaling around a point to a matrix.
	 *
	 * @param[in] matrix The matrix to apply the scaling to.
	 * @param[in] center The point to scale around.
	 * @param[in] scale The scale factor.
	 *
	 * @return A matrix with the applied scaling.
	 ******************************************************************************************************************/
	template <class T>
	glm::tmat4x4<T> scaleAroundPoint2(const glm::tmat4x4<T>& matrix, const glm::tvec2<T>& center,
									  const glm::tvec2<T>& scale) noexcept;

	/******************************************************************************************************************
	 * Applies a 3D scaling around a point to a matrix.
	 *
	 * @param[in] matrix The matrix to apply the scaling to.
	 * @param[in] center The point to scale around.
	 * @param[in] scale The scale factor.
	 *
	 * @return A matrix with the applied scaling.
	 ******************************************************************************************************************/
	template <class T>
	glm::tmat4x4<T> scaleAroundPoint3(const glm::tmat4x4<T>& matrix, const glm::tvec3<T>& center,
									  const glm::tvec3<T>& scale) noexcept;

	/******************************************************************************************************************
	 * Applies a 2D rotation around a point to a matrix.
	 *
	 * @param[in] matrix The matrix to apply the rotation to.
	 * @param[in] center The point to rotation around.
	 * @param[in] angle The rotation angle.
	 *
	 * @return A matrix with the applied rotation.
	 ******************************************************************************************************************/
	template <class T>
	glm::tmat4x4<T> rotateAroundPoint2(const glm::tmat4x4<T>& matrix, const glm::tvec2<T>& center,
									   const Angle<T>& angle) noexcept;

	/******************************************************************************************************************
	 * Applies a 3D rotation around a point to a matrix.
	 *
	 * @param[in] matrix The matrix to apply the rotation to.
	 * @param[in] center The point to rotation around.
	 * @param[in] angle The rotation angle.
	 * @param[in] axis The axis to rotate around.
	 *
	 * @return A matrix with the applied rotation.
	 ******************************************************************************************************************/
	template <class T>
	glm::tmat4x4<T> rotateAroundPoint3(const glm::tmat4x4<T>& matrix, const glm::tvec3<T>& center,
									   const Angle<T>& angle, const glm::tvec3<T>& axis) noexcept;

	/// @}
} // namespace tr

#include "geometry_impl.hpp"