#pragma once
#include "angle.hpp"

template <tr::AngleBase T>
constexpr tr::Angle<T>::Angle(T rads) noexcept
	: _rads{rads}
{
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr tr::Angle<T>::Angle(Angle<U> th) noexcept
	: _rads{static_cast<T>(th.rads())}
{
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator<=>(const Angle<U>& r) const noexcept
{
	return _rads <=> r.rads();
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr bool tr::Angle<T>::operator==(const Angle<U>& r) const noexcept
{
	return _rads == r.rads();
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr tr::Angle<T>& tr::Angle<T>::operator+=(const Angle<U>& r) noexcept
{
	_rads += r.rads();
	return *this;
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr tr::Angle<T>& tr::Angle<T>::operator-=(const Angle<U>& r) noexcept
{
	_rads -= r.rads();
	return *this;
}

template <tr::AngleBase T>
template <tr::Arithmetic U>
constexpr tr::Angle<T>& tr::Angle<T>::operator*=(const U& r) noexcept
{
	_rads *= r;
	return *this;
}

template <tr::AngleBase T>
template <tr::Arithmetic U>
constexpr tr::Angle<T>& tr::Angle<T>::operator/=(const U& r) noexcept
{
	_rads /= r;
	return *this;
}

template <tr::AngleBase T> constexpr tr::Angle<T> tr::Angle<T>::operator-() const noexcept
{
	return Angle<T>{-_rads};
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator+(const Angle<U>& r) const noexcept
{
	return tr::rads(_rads + r.rads());
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator-(const Angle<U>& r) const noexcept
{
	return tr::rads(_rads - r.rads());
}

template <tr::AngleBase T> template <tr::Arithmetic U> constexpr auto tr::Angle<T>::operator*(const U& r) const noexcept
{
	return tr::rads(_rads * r);
}

template <tr::AngleBase T> template <tr::Arithmetic U> constexpr auto tr::Angle<T>::operator/(const U& r) const noexcept
{
	return tr::rads(_rads / r);
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator/(const Angle<U>& r) const noexcept
{
	return _rads / r.rads();
}

template <tr::AngleBase T>
template <tr::AngleBase U>
constexpr auto tr::Angle<T>::operator%(const Angle<U>& r) const noexcept
{
	return tr::rads(std::fmod(_rads, r.rads()));
}

template <tr::Arithmetic T> constexpr auto tr::rads(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return Angle<T>{th};
	}
	else {
		return AngleD{static_cast<double>(th)};
	}
}

template <tr::Arithmetic T> constexpr auto tr::degs(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return Angle<T>{std::numbers::pi_v<T> / 180 * th};
	}
	else {
		return AngleD{std::numbers::pi / 180 * th};
	}
}

template <tr::Arithmetic T> constexpr auto tr::turns(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return Angle<T>{std::numbers::pi_v<T> * 2 * th};
	}
	else {
		return AngleD{std::numbers::pi * 2 * th};
	}
}

template <tr::Arithmetic T> constexpr auto tr::asin(T sin) noexcept
{
	return Angle{std::asin(sin)};
}

template <tr::Arithmetic T> constexpr auto tr::acos(T cos) noexcept
{
	return Angle{std::acos(cos)};
}

template <tr::Arithmetic T> constexpr auto tr::atan(T tan) noexcept
{
	return Angle{std::atan(tan)};
}

template <tr::Arithmetic T> constexpr auto tr::atan2(T y, T x) noexcept
{
	return Angle{std::atan2(y, x)};
}

template <tr::AngleBase T> constexpr T tr::Angle<T>::rads() const noexcept
{
	return _rads;
}

template <tr::AngleBase T> constexpr T tr::Angle<T>::degs() const noexcept
{
	return _rads * (180 / std::numbers::pi_v<T>);
}

template <tr::AngleBase T> constexpr T tr::Angle<T>::turns() const noexcept
{
	return _rads / (2 * std::numbers::pi_v<T>);
}

template <tr::AngleBase T> constexpr T tr::Angle<T>::sin() const noexcept
{
	return std::sin(_rads);
}

template <tr::AngleBase T> constexpr T tr::Angle<T>::cos() const noexcept
{
	return std::cos(_rads);
}

template <tr::AngleBase T> constexpr T tr::Angle<T>::tan() const noexcept
{
	return std::tan(_rads);
}

consteval tr::AngleF tr::angle_literals::operator"" _degf(long double deg) noexcept
{
	return degs(static_cast<float>(deg));
}

consteval tr::AngleF tr::angle_literals::operator"" _degf(unsigned long long deg) noexcept
{
	return degs(static_cast<float>(deg));
}

consteval tr::AngleD tr::angle_literals::operator"" _deg(long double deg) noexcept
{
	return degs(static_cast<double>(deg));
}

consteval tr::AngleD tr::angle_literals::operator"" _deg(unsigned long long deg) noexcept
{
	return degs(static_cast<double>(deg));
}

consteval tr::AngleL tr::angle_literals::operator"" _degl(long double deg) noexcept
{
	return degs(deg);
}

consteval tr::AngleL tr::angle_literals::operator"" _degl(unsigned long long deg) noexcept
{
	return degs(static_cast<long double>(deg));
}

consteval tr::AngleF tr::angle_literals::operator"" _radf(long double rad) noexcept
{
	return rads(static_cast<float>(rad));
}

consteval tr::AngleF tr::angle_literals::operator"" _radf(unsigned long long rad) noexcept
{
	return rads(static_cast<float>(rad));
}

consteval tr::AngleD tr::angle_literals::operator"" _rad(long double rad) noexcept
{
	return rads(static_cast<double>(rad));
}

consteval tr::AngleD tr::angle_literals::operator"" _rad(unsigned long long rad) noexcept
{
	return rads(rad);
}

consteval tr::AngleL tr::angle_literals::operator"" _radl(long double rad) noexcept
{
	return rads(rad);
}

consteval tr::AngleL tr::angle_literals::operator"" _radl(unsigned long long rad) noexcept
{
	return rads(static_cast<long double>(rad));
}

consteval tr::AngleF tr::angle_literals::operator"" _turnsf(long double tr) noexcept
{
	return turns(static_cast<float>(tr));
}

consteval tr::AngleF tr::angle_literals::operator"" _turnsf(unsigned long long tr) noexcept
{
	return turns(static_cast<float>(tr));
}

consteval tr::AngleD tr::angle_literals::operator"" _turns(long double tr) noexcept
{
	return turns(static_cast<double>(tr));
}

consteval tr::AngleD tr::angle_literals::operator"" _turns(unsigned long long tr) noexcept
{
	return turns(static_cast<double>(tr));
}

consteval tr::AngleL tr::angle_literals::operator"" _turnsl(long double tr) noexcept
{
	return turns(tr);
}

consteval tr::AngleL tr::angle_literals::operator"" _turnsl(unsigned long long tr) noexcept
{
	return turns(static_cast<long double>(tr));
}