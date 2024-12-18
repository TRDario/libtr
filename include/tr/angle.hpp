#pragma once
#include "concepts.hpp"
#include <concepts>

namespace tr {
	/** @defgroup misc Miscellaneous
	 *  Functionality not directly associated with the primary systems.
	 *  @{
	 *  @}
	 */

	/** @ingroup misc
	 *  @defgroup angle Angles
	 *  Angle datatype and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Concept denoting a valid angle base type.
	 *
	 * To fulfill this concept, @em T must be a floating point type.
	 ******************************************************************************************************************/
	template <class T>
	concept AngleBase = std::floating_point<T>;

	/******************************************************************************************************************
	 * Angular value type.
	 *
	 * @tparam T The base type of the angle, must be a floating point type.
	 ******************************************************************************************************************/
	template <AngleBase T> class Angle {
	  public:
		/**************************************************************************************************************
		 * Default-constructs an angle to 0 radians.
		 **************************************************************************************************************/
		constexpr Angle() noexcept = default;

		/**************************************************************************************************************
		 * Constructs an angle from a numeric value.
		 *
		 * @note While this constructor is equivalent to tr::rads(), that function should be preferred for readibility.
		 *
		 * @param[in] rads The numeric value of the angle in radians.
		 **************************************************************************************************************/
		constexpr explicit Angle(T rads) noexcept;

		/**************************************************************************************************************
		 * Constructs an angle from another angle (potentially of a different type).
		 *
		 * @param[in] th The angle whose value to copy.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr Angle(Angle<U> th) noexcept;

		/**************************************************************************************************************
		 * Three-way comparison operator.
		 *
		 * @param[in] r An angle to compare against.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr auto operator<=>(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 *
		 * @param[in] r An angle to compare against.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr bool operator==(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
		 * Performs addition on the angle.
		 *
		 * @param[in] r The angle value to add.
		 *
		 * @return A reference to the angle being added to.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr Angle& operator+=(const Angle<U>& r) noexcept;

		/**************************************************************************************************************
		 * Performs subtraction on the angle.
		 *
		 * @param[in] r The angle value to subtract.
		 *
		 * @return A reference to the angle being subtracted from.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr Angle& operator-=(const Angle<U>& r) noexcept;

		/**************************************************************************************************************
		 * Performs multiplication on the angle.
		 *
		 * @param[in] r The multiplication factor.
		 *
		 * @return A reference to the angle being multiplied.
		 **************************************************************************************************************/
		template <Arithmetic U> constexpr Angle& operator*=(const U& r) noexcept;

		/**************************************************************************************************************
		 * Performs division on the angle.
		 *
		 * @param[in] r The division factor.
		 *
		 * @return A reference to the angle being divided.
		 **************************************************************************************************************/
		template <Arithmetic U> constexpr Angle& operator/=(const U& r) noexcept;

		/**************************************************************************************************************
		 * Negates the angle.

		 * @return The negated value of the angle.
		 **************************************************************************************************************/
		constexpr Angle operator-() const noexcept;

		/**************************************************************************************************************
		 * Adds two angles.
		 *
		 * @param[in] r The angle value to add.
		 *
		 * @return The sum of the angles. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr auto operator+(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
		 * Subtracts two angles.
		 *
		 * @param[in] r The angle value to subtract.
		 *
		 * @return The difference of the angles. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr auto operator-(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
		 * Multiplies an angle by a factor.
		 *
		 * @param[in] r The multiplication factor.
		 *
		 * @return The value of the left side angle multiplied by the factor. Standard promotion rules apply to the
		 *		   resulting angle type.
		 **************************************************************************************************************/
		template <Arithmetic U> constexpr auto operator*(const U& r) const noexcept;

		/**************************************************************************************************************
		 * Divides an angle by a factor.
		 *
		 * @param[in] r The division factor.
		 *
		 * @return The value of the left side angle divided by the factor. Standard promotion rules apply to the
		 * 		   resulting angle type.
		 **************************************************************************************************************/
		template <Arithmetic U> constexpr auto operator/(const U& r) const noexcept;

		/**************************************************************************************************************
		 * Divides two angles.
		 *
		 * @param[in] r The angle value to divide by.
		 *
		 * @return The ratio between the angles. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr auto operator/(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
		 * Modulos an angle by another.
		 *
		 * @param[in] r The angle value to modulo by.
		 *
		 * @return The modulod value of the left side angle. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <AngleBase U> constexpr auto operator%(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
		 * Converts the angle value into a numeric radian value.
		 *
		 * @return The value of the angle in radians.
		 **************************************************************************************************************/
		constexpr T rads() const noexcept;

		/**************************************************************************************************************
		 * Converts the angle value into a numeric degree value.
		 *
		 * @return The value of the angle in degrees.
		 **************************************************************************************************************/
		constexpr T degs() const noexcept;

		/**************************************************************************************************************
		 * Converts the angle value into a numeric turn value.
		 *
		 * @return The value of the angle in turns.
		 **************************************************************************************************************/
		constexpr T turns() const noexcept;

		/**************************************************************************************************************
		 * Computes the sine of the angle
		 *
		 * @return The sine of the angle.
		 **************************************************************************************************************/
		constexpr T sin() const noexcept;

		/**************************************************************************************************************
		 * Computes the cosine of the angle
		 *
		 * @return The cosine of the angle.
		 **************************************************************************************************************/
		constexpr T cos() const noexcept;

		/**************************************************************************************************************
		 * Computes the tangent of the angle
		 *
		 * @return The tangent of the angle.
		 **************************************************************************************************************/
		constexpr T tan() const noexcept;

	  private:
		T _rads;
	};

	/******************************************************************************************************************
	 * Shorthand for a float angle.
	 ******************************************************************************************************************/
	using AngleF = Angle<float>;

	/******************************************************************************************************************
	 * Shorthand for a double angle.
	 ******************************************************************************************************************/
	using AngleD = Angle<double>;

	/******************************************************************************************************************
	 * Shorthand for a long double angle.
	 ******************************************************************************************************************/
	using AngleL = Angle<long double>;

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] th The numeric value in radians.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <Arithmetic T> constexpr auto rads(T th) noexcept;

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] th The numeric value in degrees.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <Arithmetic T> constexpr auto degs(T th) noexcept;

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] th The numeric value in turns.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <Arithmetic T> constexpr auto turns(T th) noexcept;

	/******************************************************************************************************************
	 * Converts a sine value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] sin The value of the sine.
	 *
	 * @return An angle value as if produced by `tr::rads(std::asin(sin))`.
	 ******************************************************************************************************************/
	template <Arithmetic T> constexpr auto asin(T sin) noexcept;

	/******************************************************************************************************************
	 * Converts a cosine value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] cos The value of the cosine.
	 *
	 * @return An angle value as if produced by `tr::rads(std::acos(sin))`.
	 ******************************************************************************************************************/
	template <Arithmetic T> constexpr auto acos(T cos) noexcept;

	/******************************************************************************************************************
	 * Converts a tangent value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] tan The value of the tangent.
	 *
	 * @return An angle value as if produced by `tr::rads(std::atan(sin))`.
	 ******************************************************************************************************************/
	template <Arithmetic T> constexpr auto atan(T tan) noexcept;

	/******************************************************************************************************************
	 * Converts tangent x and y values into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] y, x Tangent values.
	 *
	 * @return An angle value as if produced by `tr::rads(std::atan2(y, x))`.
	 ******************************************************************************************************************/
	template <Arithmetic T> constexpr auto atan2(T y, T x) noexcept;

	/******************************************************************************************************************
	 * Inline namespace containing angle value literals.
	 ******************************************************************************************************************/
	inline namespace angle_literals {
		/**************************************************************************************************************
		 * AngleF degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An AngleF value.
		 **************************************************************************************************************/
		consteval AngleF operator"" _degf(long double deg) noexcept;

		/**************************************************************************************************************
		 * AngleF degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An AngleF value.
		 **************************************************************************************************************/
		consteval AngleF operator"" _degf(unsigned long long deg) noexcept;

		/**************************************************************************************************************
		 * AngleD degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An AngleD value.
		 **************************************************************************************************************/
		consteval AngleD operator"" _deg(long double deg) noexcept;

		/**************************************************************************************************************
		 * AngleD degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An AngleD value.
		 **************************************************************************************************************/
		consteval AngleD operator"" _deg(unsigned long long deg) noexcept;

		/**************************************************************************************************************
		 * AngleL degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An AngleL value.
		 **************************************************************************************************************/
		consteval AngleL operator"" _degl(long double deg) noexcept;

		/**************************************************************************************************************
		 * AngleL degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An AngleL value.
		 **************************************************************************************************************/
		consteval AngleL operator"" _degl(unsigned long long deg) noexcept;

		/**************************************************************************************************************
		 * AngleL radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An AngleF value.
		 **************************************************************************************************************/
		consteval AngleF operator"" _radf(long double rad) noexcept;

		/**************************************************************************************************************
		 * AngleF radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An AngleF value.
		 **************************************************************************************************************/
		consteval AngleF operator"" _radf(unsigned long long rad) noexcept;

		/**************************************************************************************************************
		 * AngleD radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An AngleD value.
		 **************************************************************************************************************/
		consteval AngleD operator"" _rad(long double rad) noexcept;

		/**************************************************************************************************************
		 * AngleD radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An AngleD value.
		 **************************************************************************************************************/
		consteval AngleD operator"" _rad(unsigned long long rad) noexcept;

		/**************************************************************************************************************
		 * AngleL radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An AngleL value.
		 **************************************************************************************************************/
		consteval AngleL operator"" _radl(long double rad) noexcept;

		/**************************************************************************************************************
		 * AngleL radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An AngleL value.
		 **************************************************************************************************************/
		consteval AngleL operator"" _radl(unsigned long long rad) noexcept;

		/**************************************************************************************************************
		 * AngleF turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An AngleF value.
		 **************************************************************************************************************/
		consteval AngleF operator"" _turnsf(long double tr) noexcept;

		/**************************************************************************************************************
		 * AngleF turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An AngleF value.
		 **************************************************************************************************************/
		consteval AngleF operator"" _turnsf(unsigned long long tr) noexcept;

		/**************************************************************************************************************
		 * AngleD turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An AngleD value.
		 **************************************************************************************************************/
		consteval AngleD operator"" _turns(long double tr) noexcept;

		/**************************************************************************************************************
		 * AngleD turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An AngleD value.
		 **************************************************************************************************************/
		consteval AngleD operator"" _turns(unsigned long long tr) noexcept;

		/**************************************************************************************************************
		 * AngleL turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An AngleL value.
		 **************************************************************************************************************/
		consteval AngleL operator"" _turnsl(long double tr) noexcept;

		/**************************************************************************************************************
		 * AngleL turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An AngleL value.
		 **************************************************************************************************************/
		consteval AngleL operator"" _turnsl(unsigned long long tr) noexcept;
	} // namespace angle_literals

	/// @}
} // namespace tr

#include "angle_impl.hpp"