export module tr:angle;

import std;
import :concepts;

export namespace tr {
	// The allowed base classes for the angle class.
	template <class T> concept AngleBase = std::floating_point<T>;

	template <AngleBase T> class Angle {
	public:
		constexpr Angle() noexcept = default;
		constexpr explicit Angle(T rads) noexcept;
		template <AngleBase U> constexpr Angle(Angle<U> th) noexcept;

		template <AngleBase U> constexpr auto operator<=>(const Angle<U>& r) const noexcept;
		template <AngleBase U> constexpr bool operator== (const Angle<U>& r) const noexcept;

		template <AngleBase U>  constexpr Angle& operator+=(const Angle<U>& r) noexcept;
		template <AngleBase U>  constexpr Angle& operator-=(const Angle<U>& r) noexcept;
		template <Arithmetic U> constexpr Angle& operator*=(const U& r) 	   noexcept;
		template <Arithmetic U> constexpr Angle& operator/=(const U& r) 	   noexcept;

								constexpr Angle operator-() 				 const noexcept;
		template <AngleBase U>  constexpr auto  operator+(const Angle<U>& r) const noexcept;
		template <AngleBase U>  constexpr auto  operator-(const Angle<U>& r) const noexcept;
		template <Arithmetic U> constexpr auto  operator*(const U& r) 	     const noexcept;
		template <Arithmetic U> constexpr auto  operator/(const U& r) 	     const noexcept;
		template <AngleBase U>  constexpr auto  operator/(const Angle<U>& r) const noexcept;
		template <AngleBase U>  constexpr auto  operator%(const Angle<U>& r) const noexcept;

		constexpr T rads () const noexcept;
		constexpr T degs () const noexcept;
		constexpr T turns() const noexcept;
		constexpr T sin  () const noexcept;
		constexpr T cos  () const noexcept;
		constexpr T tan  () const noexcept;
	private:
		T _rads;
	};
	using AngleF = Angle<float>;
	using AngleD = Angle<double>;
	using AngleL = Angle<long double>;

	template <Arithmetic T> constexpr auto rads (T th)     noexcept;
	template <Arithmetic T> constexpr auto degs (T th)     noexcept;
	template <Arithmetic T> constexpr auto turns(T th)     noexcept;
	template <Arithmetic T> constexpr auto asin (T sin)    noexcept;
	template <Arithmetic T> constexpr auto acos (T cos)    noexcept;
	template <Arithmetic T> constexpr auto atan (T tan)    noexcept;
	template <Arithmetic T> constexpr auto atan2(T y, T x) noexcept;

	inline namespace angle_literals {
		consteval AngleF operator "" _degf  (long double 		deg) noexcept;
		consteval AngleF operator "" _degf  (unsigned long long deg) noexcept;
		consteval AngleD operator "" _deg   (long double 		deg) noexcept;
		consteval AngleD operator "" _deg   (unsigned long long deg) noexcept;
		consteval AngleL operator "" _degl  (long double 		deg) noexcept;
		consteval AngleL operator "" _degl  (unsigned long long deg) noexcept;
		consteval AngleF operator "" _radf  (long double 		rad) noexcept;
		consteval AngleF operator "" _radf  (unsigned long long rad) noexcept;
		consteval AngleD operator "" _rad   (long double 		rad) noexcept;
		consteval AngleD operator "" _rad   (unsigned long long rad) noexcept;
		consteval AngleL operator "" _radl  (long double 		rad) noexcept;
		consteval AngleL operator "" _radl  (unsigned long long rad) noexcept;
		consteval AngleF operator "" _turnsf(long double 		tr)  noexcept;
		consteval AngleF operator "" _turnsf(unsigned long long tr)  noexcept;
		consteval AngleD operator "" _turns (long double 		tr)  noexcept;
		consteval AngleD operator "" _turns (unsigned long long tr)  noexcept;
		consteval AngleL operator "" _turnsl(long double 	 	tr)  noexcept;
		consteval AngleL operator "" _turnsl(unsigned long long tr)  noexcept;
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