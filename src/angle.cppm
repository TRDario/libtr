/**
 * @file angle.cppm
 * @brief Provides an angle type template alongside related functions and literals.
 */

export module tr:angle;

import std;
import :concepts;

export namespace tr {
	/******************************************************************************************************************
	 * Concept denoting a valid angle base type.
	 *
	 * To fulfill this concept, @em T must be a floating point type.
	 ******************************************************************************************************************/
	template <class T> concept AngleBase = std::floating_point<T>;


	/******************************************************************************************************************
	 * Angular value type.
	 *
	 * @tparam T The base type of the angle, must be a floating point type.
	 ******************************************************************************************************************/
	template <AngleBase T>
	class Angle {
	public:
		/**************************************************************************************************************
         * Default-constructs an angle to 0 radians.
         **************************************************************************************************************/
		constexpr Angle() noexcept = default;

		/**************************************************************************************************************
         * Constructs an angle from a numeric value.
		 *
		 * tr::rads() should be preferred for readibility.
		 *
		 * @param rads The numeric value of the angle in radians.
         **************************************************************************************************************/
		constexpr explicit Angle(T rads) noexcept;

		/**************************************************************************************************************
         * Constructs an angle from another angle (potentially of a different type).
		 *
		 * tr::rads() should be preferred for readibility.
		 *
		 * @param th The angle whose value to copy.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr Angle(Angle<U> th) noexcept;


		/**************************************************************************************************************
         * Three-way comparison operator.
		 *
		 * @param r An angle to compare against.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr auto operator<=>(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
         * Equality comparison operator.
		 *
		 * @param r An angle to compare against.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr bool operator==(const Angle<U>& r) const noexcept;


		/**************************************************************************************************************
         * Performs addition on the angle.
		 *
		 * @param r The angle value to add.
		 *
		 * @return A reference to the angle being added to.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr Angle& operator+=(const Angle<U>& r) noexcept;

		/**************************************************************************************************************
         * Performs subtraction on the angle.
		 *
		 * @param r The angle value to subtract.
		 *
		 * @return A reference to the angle being subtracted from.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr Angle& operator-=(const Angle<U>& r) noexcept;

		/**************************************************************************************************************
         * Performs multiplication on the angle.
		 *
		 * @param r The multiplication factor.
		 *
		 * @return A reference to the angle being multiplied.
         **************************************************************************************************************/
		template <Arithmetic U>
		constexpr Angle& operator*=(const U& r) noexcept;

		/**************************************************************************************************************
         * Performs division on the angle.
		 *
		 * @param r The division factor.
		 *
		 * @return A reference to the angle being divided.
         **************************************************************************************************************/
		template <Arithmetic U>
		constexpr Angle& operator/=(const U& r) noexcept;


		/**************************************************************************************************************
         * Negates the angle.

		 * @return The negated value of the angle.
         **************************************************************************************************************/
		constexpr Angle operator-() const noexcept;

		/**************************************************************************************************************
         * Adds two angles.
		 *
		 * @param r The angle value to add.
		 *
		 * @return The sum of the angles. Standard promotion rules apply to the resulting angle type.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr auto operator+(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
         * Subtracts two angles.
		 *
		 * @param r The angle value to subtract.
		 *
		 * @return The difference of the angles. Standard promotion rules apply to the resulting angle type.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr auto operator-(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
         * Multiplies an angle by a factor.
		 *
		 * @param r The multiplication factor.
		 *
		 * @return The value of the left side angle multiplied by the factor. Standard promotion rules apply to the
		 *		   resulting angle type.
         **************************************************************************************************************/
		template <Arithmetic U>
		constexpr auto operator*(const U& r) const noexcept;

		/**************************************************************************************************************
         * Divides an angle by a factor.
		 *
		 * @param r The division factor.
		 *
		 * @return The value of the left side angle divided by the factor. Standard promotion rules apply to the
		 * 		   resulting angle type.
         **************************************************************************************************************/
		template <Arithmetic U>
		constexpr auto operator/(const U& r) const noexcept;

		/**************************************************************************************************************
         * Divides two angles.
		 *
		 * @param r The angle value to divide by.
		 *
		 * @return The ratio between the angles. Standard promotion rules apply to the resulting angle type.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr auto operator/(const Angle<U>& r) const noexcept;

		/**************************************************************************************************************
         * Modulos an angle by another.
		 *
		 * @param r The angle value to modulo by.
		 *
		 * @return The modulod value of the left side angle. Standard promotion rules apply to the resulting angle type.
         **************************************************************************************************************/
		template <AngleBase U>
		constexpr auto operator%(const Angle<U>& r) const noexcept;


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
	 * @param th The numeric value in radians.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <Arithmetic T>
	constexpr auto rads(T th) noexcept;

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param th The numeric value in degrees.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <Arithmetic T>
	constexpr auto degs(T th) noexcept;

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param th The numeric value in turns.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <Arithmetic T>
	constexpr auto turns(T th) noexcept;

	/******************************************************************************************************************
	 * Converts a sine value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param sin The value of the sine.
	 *
	 * @return An angle value as if produced by `tr::rads(std::asin(sin))`.
	 ******************************************************************************************************************/
	template <Arithmetic T>
	constexpr auto asin(T sin) noexcept;

	/******************************************************************************************************************
	 * Converts a cosine value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param cos The value of the cosine.
	 *
	 * @return An angle value as if produced by `tr::rads(std::acos(sin))`.
	 ******************************************************************************************************************/
	template <Arithmetic T>
	constexpr auto acos(T cos) noexcept;

	/******************************************************************************************************************
	 * Converts a tangent value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param tan The value of the tangent.
	 *
	 * @return An angle value as if produced by `tr::rads(std::atan(sin))`.
	 ******************************************************************************************************************/
	template <Arithmetic T>
	constexpr auto atan(T tan) noexcept;

	/******************************************************************************************************************
	 * Converts tangent x and y values into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an AngleD value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param y, x Tangent values.
	 *
	 * @return An angle value as if produced by `tr::rads(std::atan2(y, x))`.
	 ******************************************************************************************************************/
	template <Arithmetic T>
	constexpr auto atan2(T y, T x) noexcept;


	/******************************************************************************************************************
	 * Inline namespace containing angle value literals.
	 ******************************************************************************************************************/
	inline namespace angle_literals {
		/**************************************************************************************************************
         * Angle<float> degree literal.
		 *
		 * @param deg The value of the angle in degrees.
		 *
		 * @return An AngleF value.
         **************************************************************************************************************/
		consteval AngleF operator "" _degf(long double deg) noexcept;

		/**************************************************************************************************************
         * Angle<float> degree literal.
		 *
		 * @param deg The value of the angle in degrees.
		 *
		 * @return An AngleF value.
         **************************************************************************************************************/
		consteval AngleF operator "" _degf(unsigned long long deg) noexcept;

		/**************************************************************************************************************
         * Angle<double> degree literal.
		 *
		 * @param deg The value of the angle in degrees.
		 *
		 * @return An AngleD value.
         **************************************************************************************************************/
		consteval AngleD operator "" _deg(long double deg) noexcept;

		/**************************************************************************************************************
         * Angle<double> degree literal.
		 *
		 * @param deg The value of the angle in degrees.
		 *
		 * @return An AngleD value.
         **************************************************************************************************************/
		consteval AngleD operator "" _deg(unsigned long long deg) noexcept;

		/**************************************************************************************************************
         * Angle<long double> degree literal.
		 *
		 * @param deg The value of the angle in degrees.
		 *
		 * @return An AngleL value.
         **************************************************************************************************************/
		consteval AngleL operator "" _degl(long double deg) noexcept;

		/**************************************************************************************************************
         * Angle<long double> degree literal.
		 *
		 * @param deg The value of the angle in degrees.
		 *
		 * @return An AngleL value.
         **************************************************************************************************************/
		consteval AngleL operator "" _degl(unsigned long long deg) noexcept;

		/**************************************************************************************************************
         * Angle<float> radian literal.
		 *
		 * @param rad The value of the angle in radians.
		 *
		 * @return An AngleF value.
         **************************************************************************************************************/
		consteval AngleF operator "" _radf(long double rad) noexcept;

		/**************************************************************************************************************
         * Angle<float> radian literal.
		 *
		 * @param rad The value of the angle in radians.
		 *
		 * @return An AngleF value.
         **************************************************************************************************************/
		consteval AngleF operator "" _radf(unsigned long long rad) noexcept;

		/**************************************************************************************************************
         * Angle<double> radian literal.
		 *
		 * @param rad The value of the angle in radians.
		 *
		 * @return An AngleD value.
         **************************************************************************************************************/
		consteval AngleD operator "" _rad(long double rad) noexcept;

		/**************************************************************************************************************
         * Angle<double> radian literal.
		 *
		 * @param rad The value of the angle in radians.
		 *
		 * @return An AngleD value.
         **************************************************************************************************************/
		consteval AngleD operator "" _rad(unsigned long long rad) noexcept;

		/**************************************************************************************************************
         * Angle<long double> radian literal.
		 *
		 * @param rad The value of the angle in radians.
		 *
		 * @return An AngleL value.
         **************************************************************************************************************/
		consteval AngleL operator "" _radl(long double rad) noexcept;

		/**************************************************************************************************************
         * Angle<long double> radian literal.
		 *
		 * @param rad The value of the angle in radians.
		 *
		 * @return An AngleL value.
         **************************************************************************************************************/
		consteval AngleL operator "" _radl(unsigned long long rad) noexcept;

		/**************************************************************************************************************
         * Angle<float> turns literal.
		 *
		 * @param tr The value of the angle in turns.
		 *
		 * @return An AngleF value.
         **************************************************************************************************************/
		consteval AngleF operator "" _turnsf(long double tr) noexcept;

		/**************************************************************************************************************
         * Angle<float> turns literal.
		 *
		 * @param tr The value of the angle in turns.
		 *
		 * @return An AngleF value.
         **************************************************************************************************************/
		consteval AngleF operator "" _turnsf(unsigned long long tr) noexcept;

		/**************************************************************************************************************
         * Angle<double> turns literal.
		 *
		 * @param tr The value of the angle in turns.
		 *
		 * @return An AngleD value.
         **************************************************************************************************************/
		consteval AngleD operator "" _turns(long double tr) noexcept;

		/**************************************************************************************************************
         * Angle<double> turns literal.
		 *
		 * @param tr The value of the angle in turns.
		 *
		 * @return An AngleD value.
         **************************************************************************************************************/
		consteval AngleD operator "" _turns(unsigned long long tr) noexcept;

		/**************************************************************************************************************
         * Angle<long double> turns literal.
		 *
		 * @param tr The value of the angle in turns.
		 *
		 * @return An AngleL value.
         **************************************************************************************************************/
		consteval AngleL operator "" _turnsl(long double tr)  noexcept;

		/**************************************************************************************************************
         * Angle<long double> turns literal.
		 *
		 * @param tr The value of the angle in turns.
		 *
		 * @return An AngleL value.
         **************************************************************************************************************/
		consteval AngleL operator "" _turnsl(unsigned long long tr) noexcept;
	}
}

// IMPLEMENTATION

template <tr::AngleBase T>
constexpr tr::Angle<T>::Angle(T rads) noexcept
	: _rads { rads }
{}

template <tr::AngleBase T> template <tr::AngleBase U>
constexpr tr::Angle<T>::Angle(Angle<U> th) noexcept
	: _rads { T(th.rads()) }
{}

template <tr::AngleBase T> template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator<=>(const Angle<U>& r) const noexcept
{
	return _rads <=> r.rads();
}

template <tr::AngleBase T> template <tr::AngleBase U>
constexpr bool tr::Angle<T>::operator== (const Angle<U>& r) const noexcept
{
	return _rads == r.rads();
}

template <tr::AngleBase T> template <tr::AngleBase U>
constexpr tr::Angle<T>& tr::Angle<T>::operator+=(const Angle<U>& r) noexcept
{
	_rads += r.rads();
	return *this;
}

template <tr::AngleBase T> template <tr::AngleBase U>
constexpr tr::Angle<T>& tr::Angle<T>::operator-=(const Angle<U>& r) noexcept
{
	_rads -= r.rads();
	return *this;
}

template <tr::AngleBase T> template <tr::Arithmetic U>
constexpr tr::Angle<T>& tr::Angle<T>::operator*=(const U& r) noexcept
{
	_rads *= r;
	return *this;
}

template <tr::AngleBase T> template <tr::Arithmetic U>
constexpr tr::Angle<T>& tr::Angle<T>::operator/=(const U& r) noexcept
{
	_rads /= r;
	return *this;
}

template <tr::AngleBase T>
constexpr tr::Angle<T> tr::Angle<T>::operator-() const noexcept
{
	return Angle<T> { -_rads };
}

template <tr::AngleBase T> template <tr::AngleBase U> 
constexpr auto tr::Angle<T>::operator+(const Angle<U>& r) const noexcept
{
	return tr::rads(_rads + r.rads());
}

template <tr::AngleBase T> template <tr::AngleBase U> 
constexpr auto tr::Angle<T>::operator-(const Angle<U>& r) const noexcept
{
	return tr::rads(_rads - r.rads());
}

template <tr::AngleBase T> template <tr::Arithmetic U>
constexpr auto tr::Angle<T>::operator*(const U& r) const noexcept
{
	return tr::rads(_rads * r);
}

template <tr::AngleBase T> template <tr::Arithmetic U>
constexpr auto tr::Angle<T>::operator/(const U& r) const noexcept
{
	return tr::rads(_rads / r);
}

template <tr::AngleBase T> template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator/(const Angle<U>& r) const noexcept
{
	return _rads / r.rads();
}

template <tr::AngleBase T> template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator%(const Angle<U>& r) const noexcept
{
	return tr::rads(std::fmod(_rads, r.rads()));
}

template <tr::Arithmetic T>
constexpr auto tr::rads(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return Angle<T> { th };
	}
	else {
		return AngleD { double(th) };
	}
}

template <tr::Arithmetic T>
constexpr auto tr::degs(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return Angle<T> { std::numbers::pi_v<T> / 180 * th };
	}
	else {
		return AngleD { std::numbers::pi / 180 * th };
	}
}

template <tr::Arithmetic T>
constexpr auto tr::turns(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return Angle<T> { std::numbers::pi_v<T> * 2 * th };
	}
	else {
		return AngleD { std::numbers::pi * 2 * th };
	}
}

template <tr::Arithmetic T>
constexpr auto tr::asin(T sin) noexcept
{
	return Angle { std::asin(sin) };
}

template <tr::Arithmetic T>
constexpr auto tr::acos(T cos) noexcept
{
	return Angle { std::acos(cos) };
}

template <tr::Arithmetic T>
constexpr auto tr::atan(T tan) noexcept
{
	return Angle { std::atan(tan) };
}

template <tr::Arithmetic T>
constexpr auto tr::atan2(T y, T x) noexcept
{
	return Angle { std::atan2(y, x) };
}

template <tr::AngleBase T>
constexpr T tr::Angle<T>::rads() const noexcept
{
	return _rads;
}

template <tr::AngleBase T>
constexpr T tr::Angle<T>::degs() const noexcept
{
	return _rads / (std::numbers::pi_v<T> * 180);
}

template <tr::AngleBase T>
constexpr T tr::Angle<T>::turns() const noexcept
{
	return _rads / (2 * std::numbers::pi_v<T>);
}

template <tr::AngleBase T>
constexpr T tr::Angle<T>::sin() const noexcept
{
	return std::sin(_rads);
}

template <tr::AngleBase T>
constexpr T tr::Angle<T>::cos() const noexcept
{
	return std::cos(_rads);
}

template <tr::AngleBase T>
constexpr T tr::Angle<T>::tan() const noexcept
{
	return std::tan(_rads);
}

consteval tr::AngleF tr::angle_literals::operator "" _degf(long double deg) noexcept
{
	return degs(float(deg));
}

consteval tr::AngleF tr::angle_literals::operator "" _degf(unsigned long long deg) noexcept
{
	return degs(float(deg));
}

consteval tr::AngleD tr::angle_literals::operator "" _deg(long double deg) noexcept
{
	return degs(double(deg));
}

consteval tr::AngleD tr::angle_literals::operator "" _deg(unsigned long long deg) noexcept
{
	return degs(double(deg));
}

consteval tr::AngleL tr::angle_literals::operator "" _degl(long double deg) noexcept
{
	return degs(deg);
}

consteval tr::AngleL tr::angle_literals::operator "" _degl(unsigned long long deg) noexcept
{
	return degs((long double)(deg));
}

consteval tr::AngleF tr::angle_literals::operator "" _radf(long double rad) noexcept
{
	return rads(float(rad));
}

consteval tr::AngleF tr::angle_literals::operator "" _radf(unsigned long long rad) noexcept
{
	return rads(float(rad));
}

consteval tr::AngleD tr::angle_literals::operator "" _rad(long double rad) noexcept
{
	return rads(double(rad));
}

consteval tr::AngleD tr::angle_literals::operator "" _rad(unsigned long long rad) noexcept
{
	return rads(rad);
}

consteval tr::AngleL tr::angle_literals::operator "" _radl(long double rad) noexcept
{
	return rads(rad);
}

consteval tr::AngleL tr::angle_literals::operator "" _radl(unsigned long long rad) noexcept
{
	return rads((long double)(rad));
}

consteval tr::AngleF tr::angle_literals::operator "" _turnsf(long double tr) noexcept
{
	return turns(float(tr));
}

consteval tr::AngleF tr::angle_literals::operator "" _turnsf(unsigned long long tr) noexcept
{
	return turns(float(tr));
}

consteval tr::AngleD tr::angle_literals::operator "" _turns(long double tr) noexcept
{
	return turns(double(tr));
}

consteval tr::AngleD tr::angle_literals::operator "" _turns(unsigned long long tr) noexcept
{
	return turns(double(tr));
}

consteval tr::AngleL tr::angle_literals::operator "" _turnsl(long double tr) noexcept
{
	return turns(tr);
}

consteval tr::AngleL tr::angle_literals::operator "" _turnsl(unsigned long long tr) noexcept
{
	return turns((long double)(tr));
}