#pragma once
#include "geometry.hpp"

template <int S, class T>
constexpr tr::Rect<S, T>::Rect(glm::vec<S, T> tl, glm::vec<S, T> size) noexcept
	: tl{tl}, size{size}
{
}

template <int S, class T>
constexpr tr::Rect<S, T>::Rect(glm::vec<S, T> size) noexcept
	: tl{}, size{size}
{
}

template <int S, class T>
template <class T1>
constexpr tr::Rect<S, T>::Rect(const Rect<S, T1>& rect) noexcept
	: tl{glm::vec<S, T>(rect.tl)}, size{glm::vec<S, T>(rect.size)}
{
}

template <int S, class T>
template <class T1>
constexpr bool tr::Rect<S, T>::operator==(const Rect<S, T1>& r) const noexcept
{
	return tl == r.tl && size == r.size;
}

template <int S, class T>
template <class T1>
constexpr bool tr::Rect<S, T>::contains(glm::vec<S, T1> point) const noexcept
{
	for (std::size_t i = 0; i < S; ++i) {
		if (point[i] < tl[i] || point[i] > tl[i] + size[i]) {
			return false;
		}
	}
	return true;
}

template <class T>
constexpr tr::Circle<T>::Circle(glm::tvec2<T> c, T r) noexcept
	: c{c}, r{r}
{
}

template <class T>
template <class T1>
constexpr tr::Circle<T>::Circle(const Circle<T1>& circle) noexcept
	: c{glm::tvec2<T>(circle.c)}, r{T(circle.r)}
{
}

template <class T> template <class T1> constexpr bool tr::Circle<T>::operator==(const Circle<T1>& r) const noexcept
{
	return c == r.c && r == r.r;
}

template <class T> template <class T1> constexpr bool tr::Circle<T>::contains(glm::tvec2<T1> point) const noexcept
{
	const auto dx{point.x - c.x};
	const auto dy{point.y - c.y};
	return dx * dx + dy * dy <= r * r;
}

template <class T1, class T2, class T3>
constexpr bool tr::collinear(glm::tvec2<T1> a, glm::tvec2<T2> b, glm::tvec2<T3> c) noexcept
{
	auto tol{std::abs(std::max<std::common_type_t<T1, T2, T3>>({a.x, a.y, b.x, b.y, c.x, c.y})) * 0.000001f};
	return std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) < tol;
}

template <class T1, class T2> bool tr::intersecting(const Rect2<T1>& r1, const Rect2<T2>& r2) noexcept
{
	return r1.contains(r2.tl) || r1.contains(r2.tl + r2.size) || r2.contains(r1.tl + glm::tvec2<T1>{r1.size.x, 0}) ||
		   r2.contains(r1.tl + glm::tvec2<T1>{0, r1.size.y});
}

template <class T> bool tr::intersecting(const Circle<T>& c1, const Circle<T>& c2) noexcept
{
	return glm::distance(c1.c, c2.c) <= c1.r + c2.r;
}

template <std::floating_point T>
constexpr glm::tvec2<T> tr::closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept
{
	auto pp{p - a};
	auto bp{b - a};
	return a + bp * std::clamp(dot(pp, bp) / dot(bp, bp), T{0}, T{1});
}

template <std::integral T>
constexpr glm::dvec2 tr::closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept
{
	return closestPoint(glm::dvec2(p), glm::dvec2(a), glm::dvec2(b));
}

template <std::floating_point T>
constexpr std::optional<glm::tvec2<T>> tr::segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
															glm::tvec2<T> b2) noexcept
{
	auto          r{b2 - a2};
	glm::tvec2<T> n{b1.y - a1.y, -b1.x + a1.x};
	auto          d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	if (t < 0 || t > 1) {
		return std::nullopt;
	}
	auto result{a2 + r * t};
	if ((result.x < std::min(a1.x, b1.x) || result.x > std::max(a1.x, b1.x)) ||
		(result.x - a1.x == 0 && (result.y < std::min(a1.y, b1.y) || result.y > std::max(a1.y, b1.y)))) {
		return std::nullopt;
	}
	else {
		return result;
	}
}

template <std::integral T>
constexpr std::optional<glm::dvec2> tr::segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
														 glm::tvec2<T> b2) noexcept
{
	return segmentIntersect(glm::dvec2(a1), glm::dvec2(b1), glm::dvec2(a2), glm::dvec2(b2));
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::segmentIntersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2,
												  glm::tvec2<T> b2) noexcept
{
	auto          r{b2 - a2};
	glm::tvec2<T> n{sin(th1), -cos(th1)};
	auto          d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	if (t >= 0 && t <= 1) {
		return a2 + r * t;
	}
	else {
		return std::nullopt;
	}
}

template <std::integral T>
std::optional<glm::dvec2> tr::segmentIntersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2,
											   glm::tvec2<T> b2) noexcept
{
	return segmentIntersect(glm::dvec2(a1), th1, glm::dvec2(a2), glm::dvec2(b2));
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, Angle<T> th2) noexcept
{
	glm::tvec2<T> r{cos(th2), sin(th2)};
	glm::tvec2<T> n{sin(th1), -cos(th1)};
	auto          d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	return a2 + r * t;
}

template <std::integral T>
std::optional<glm::dvec2> tr::intersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2, AngleD th2) noexcept
{
	return intersect(glm::dvec2(a1), th1, glm::dvec2(a2), th2);
}

template <std::floating_point T>
constexpr std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
													 glm::tvec2<T> b2) noexcept
{
	auto          r{b2 - a2};
	glm::tvec2<T> n{b1.y - a1.y, -b1.x + a1.x};
	auto          d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	return a2 + r * t;
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
	auto          r{b2 - a2};
	glm::tvec2<T> n{sin(th1), -cos(th1)};
	auto          d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	return a2 + r * t;
}

template <class T1, class T2>
constexpr std::common_type_t<T1, T2> tr::cross2(glm::tvec2<T1> a, glm::tvec2<T2> b) noexcept
{
	return a.x * b.y - a.y * b.x;
}

template <tr::Arithmetic T1, tr::Arithmetic T2> std::common_type_t<T1, T2> tr::mod(T1 v, T2 mod) noexcept
{
	if constexpr (std::floating_point<T1> || std::floating_point<T2>) {
		return std::fmod(std::common_type_t<T1, T2>(v), std::common_type_t<T1, T2>(mod));
	}
	else {
		return std::common_type_t<T1, T2>(v % mod);
	}
}

template <tr::Arithmetic T> T tr::mirrorRepeat(T v, T min, T max) noexcept
{
	auto shift{v - min};
	auto size{max - min};
	shift = mod(abs(shift + (shift < 0)), (2 * size));
	return min + (shift >= size ? 2 * size - shift - 1 : shift);
}

template <int S, class T>
glm::vec<S, T> tr::mirrorRepeat(const glm::vec<S, T>& v, const glm::vec<S, T>& min, const glm::vec<S, T>& max) noexcept
{
	glm::vec<S, T> result;
	for (auto i = 0; i < S; ++i) {
		result[i] = mirrorRepeat(v[i], min[i], max[i]);
	}
	return result;
}

template <std::size_t S, class T>
glm::vec<S, T> tr::mirrorRepeat(const glm::vec<S, T>& v, const Rect<S, T>& rect) noexcept
{
	return mirrorRepeat(v, rect.tl, rect.tl + rect.size);
}

template <std::size_t S, std::floating_point T> constexpr glm::vec<S, T> tr::inverse(const glm::vec<S, T>& vec) noexcept
{
	glm::vec<S, T> result;
	for (std::size_t i = 0; i < S; ++i) {
		result[i] = T(1) / vec[i];
	}
	return result;
}

template <std::size_t S, std::integral T> constexpr glm::vec<S, double> tr::inverse(const glm::vec<S, T>& vec) noexcept
{
	glm::vec<S, double> result;
	for (std::size_t i = 0; i < S; ++i) {
		result[i] = 1.0 / vec[i];
	}
	return result;
}

template <class T>
constexpr glm::tvec2<T> tr::matrix_operators::operator*(const glm::tvec2<T>& v, const glm::tmat4x4<T>& m) noexcept
{
	return glm::tvec4<T>{v, 0, 1} * m;
}

template <class T>
constexpr glm::tvec3<T> tr::matrix_operators::operator*(const glm::tvec3<T>& v, const glm::tmat4x4<T>& m) noexcept
{
	return glm::tvec4<T>{v, 1} * m;
}

template <class T>
constexpr glm::tvec2<T> tr::matrix_operators::operator*(const glm::tmat4x4<T>& m, const glm::tvec2<T>& v) noexcept
{
	return m * glm::tvec4<T>{v, 0, 1};
}

template <class T>
constexpr glm::tvec3<T> tr::matrix_operators::operator*(const glm::tmat4x4<T>& m, const glm::tvec3<T>& v) noexcept
{
	return m * glm::tvec4<T>{v, 1};
}

template <class T>
glm::tmat4x4<T> tr::scaleAroundPoint2(const glm::tmat4x4<T>& matrix, const glm::tvec2<T>& center,
									  const glm::tvec2<T>& scale) noexcept
{
	return scaleAroundPoint3(matrix, {center, 0}, {scale, 1});
}

template <class T>
glm::tmat4x4<T> tr::scaleAroundPoint3(const glm::tmat4x4<T>& matrix, const glm::tvec3<T>& center,
									  const glm::tvec3<T>& scale) noexcept
{
	return translate(glm::scale(translate(matrix, center), scale), -center);
}

template <class T>
glm::tmat4x4<T> tr::rotateAroundPoint2(const glm::tmat4x4<T>& matrix, const glm::tvec2<T>& center,
									   const Angle<T>& angle) noexcept
{
	return rotateAroundPoint3(matrix, {center, 0}, angle, Z_AXIS);
}

template <class T>
glm::tmat4x4<T> tr::rotateAroundPoint3(const glm::tmat4x4<T>& matrix, const glm::tvec3<T>& center,
									   const Angle<T>& angle, const glm::tvec3<T>& axis) noexcept
{
	return translate(rotate(translate(matrix, center), angle.rads(), axis), -center);
}