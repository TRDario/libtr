export module tr:geometry;

import std;
import glm;
import :angle;
import :concepts;

export namespace tr {
    template <int S, class T> struct Rect {
        glm::vec<S, T> tl;   // The offset of the top-left corner of the rect.
        glm::vec<S, T> size; // The size of the rectangle.

        constexpr Rect() = default;
        constexpr Rect(glm::vec<S, T> tl, glm::vec<S, T> size) noexcept;
        template <class T1> constexpr Rect(const Rect<S, T1>& rect) noexcept;

        template <class T1> constexpr bool operator==(const Rect<S, T1>& r) const noexcept;
    };
    template <class T> using Rect2 = Rect<2, T>;
    using RectI2 = Rect2<int>;
    using RectF2 = Rect2<float>;
    template <class T> using Rect3 = Rect<3, T>;
    using RectI3 = Rect3<int>;
    using RectF3 = Rect3<float>;

    template <class T> struct Circle {
        glm::tvec2<T> c; // The center of the circle.
        T r;             // The radius of the circle.

        constexpr Circle() = default;
        constexpr Circle(glm::tvec2<T> c, T r) noexcept;
        template <class T1> constexpr Circle(const Circle<T1>& r) noexcept;

        template <class T1> constexpr bool operator==(const Circle<T1>& r) const noexcept;
    };
    using CircleF = Circle<float>;

    // Determines if a point is located within a rect.
	template <class T1, class T2> bool within(glm::tvec2<T2> p, const Rect2<T1>& r) noexcept;
	// Determines if a point is located within a rect.
	template <class T1, class T2> bool within(glm::tvec3<T2> p, const Rect3<T1>& r) noexcept;
	// Determines if a point is located within a circle.
	template <class T1, class T2> bool within(glm::tvec2<T2> p, const Circle<T1>& c) noexcept;

	// Gets whether 3 points are collinear.
	template <class T1, class T2, class T3> constexpr bool collinear(glm::tvec2<T1> a, glm::tvec2<T2> b, glm::tvec2<T3> c) noexcept;

	// Determines if two rects intersect.
	template <class T1, class T2> bool intersecting(const Rect2<T1>& r1, const Rect2<T2>& r2) noexcept;
	// Determines if two circles intersect.
	template <class T> bool intersecting(const Circle<T>& c1, const Circle<T>& c2) noexcept;

	// Calculates the closest point to p on the line ab.
	template <std::floating_point T> constexpr glm::tvec2<T> closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept;
	// Calculates the closest point to p on the line ab.
	template <std::integral T> constexpr glm::dvec2 closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept;

	// Calculates the intersection point of two line segments.
	template <std::floating_point T> constexpr std::optional<glm::tvec2<T>> segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;
	// Calculates the intersection point of two line segments.
	template <std::integral T> constexpr std::optional<glm::dvec2> segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;
	// Calculates the intersection point of a line and line segment.
	template <std::floating_point T> std::optional<glm::tvec2<T>> segmentIntersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;
	// Calculates the intersection point of a line and line segment.
	template <std::integral T> std::optional<glm::dvec2> segmentIntersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	// Calculates the intersection point of two lines.
	template <std::floating_point T> std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, Angle<T> th2) noexcept;
	// Calculates the intersection point of two lines.
	template <std::integral T> std::optional<glm::dvec2> intersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2, AngleD th2) noexcept;
	// Calculates the intersection point of two lines.
	template <std::floating_point T> constexpr std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;
	// Calculates the intersection point of two lines, one defined by two points and one by a point and angle.
	template <std::floating_point T> std::optional<glm::tvec2<T>> intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept;

	// Calculates the 2D cross product of two vectors.
	template <class T1, class T2> constexpr std::common_type_t<T1, T2> cross2(glm::tvec2<T1> a, glm::tvec2<T2> b) noexcept;

	// Generic modulo operation.
	template <Arithmetic T1, Arithmetic T2> std::common_type_t<T1, T2> mod(T1 v, T2 mod) noexcept;

	// Performs a mirror repeat mapping of val to the range min-max.
	template <Arithmetic T> T mirrorRepeat(T v, T min, T max) noexcept;
	// Performs a mirror repeat mapping of val to the range min-max.
	template <int S, class T> glm::vec<S, T> mirrorRepeat(glm::vec<S, T> v, glm::vec<S, T> min, glm::vec<S, T> max) noexcept;
	// Performs a mirror repeat mapping on a vector to a rect.
	template <class T> glm::tvec2<T> mirrorRepeat(glm::tvec2<T> v, const Rect2<T>& r) noexcept;
	// Performs a mirror repeat mapping on a vector to a rect.
	template <class T> glm::tvec3<T> mirrorRepeat(glm::tvec3<T> v, const Rect3<T>& r) noexcept;

    template <std::floating_point T> constexpr glm::tvec2<T> inverse(glm::tvec2<T> p) noexcept;
	template <std::integral T> 		 constexpr glm::dvec2    inverse(glm::tvec2<T> p) noexcept;
    template <std::floating_point T> constexpr glm::tvec3<T> inverse(glm::tvec3<T> p) noexcept;
	template <std::integral T> 		 constexpr glm::dvec3    inverse(glm::tvec3<T> p) noexcept;

	inline namespace matrix_operators {
        template <class T> constexpr glm::tvec2<T> operator*(const glm::tvec2<T>& v, const glm::tmat4x4<T>& m) noexcept;
        template <class T> constexpr glm::tvec3<T> operator*(const glm::tvec3<T>& v, const glm::tmat4x4<T>& m) noexcept;
        template <class T> constexpr glm::tvec2<T> operator*(const glm::tmat4x4<T>& m, const glm::tvec2<T>& v) noexcept;
        template <class T> constexpr glm::tvec3<T> operator*(const glm::tmat4x4<T>& m, const glm::tvec3<T>& v) noexcept;
    }

	inline constexpr glm::vec3 X_AXIS { 1.0f, 0.0f, 0.0f };
	inline constexpr glm::vec3 Y_AXIS { 0.0f, 1.0f, 0.0f };
	inline constexpr glm::vec3 Z_AXIS { 0.0f, 0.0f, 1.0f };
	
	// Applies a 2D scaling around a point to a matrix.
	template <class T> glm::tmat4x4<T> scaleAroundPoint2(const glm::tmat4x4<T>& m, const glm::tvec2<T>& c, const glm::tvec2<T>& s) noexcept;
	// Applies a 3D scaling around a point to a matrix.
	template <class T> glm::tmat4x4<T> scaleAroundPoint3(const glm::tmat4x4<T>& m, const glm::tvec3<T>& c, const glm::tvec3<T>& s) noexcept;

	// Applies a 2D rotation around a point to a matrix.
	template <class T> glm::tmat4x4<T> rotateAroundPoint2(const glm::tmat4x4<T>& m, const glm::tvec2<T>& c, const Angle<T>& th) noexcept;
	// Applies a 3D rotation around a point to a matrix.
	template <class T> glm::tmat4x4<T> rotateAroundPoint3(const glm::tmat4x4<T>& m, const glm::tvec3<T>& c, const Angle<T>& th, const glm::tvec3<T>& axis) noexcept;
}

// IMPLEMENTATION

template <int S, class T>
constexpr tr::Rect<S, T>::Rect(glm::vec<S, T> tl, glm::vec<S, T> size) noexcept
    : tl { tl }
    , size { size }
{}

template <int S, class T> template <class T1>
constexpr tr::Rect<S, T>::Rect(const Rect<S, T1>& rect) noexcept
    : tl { glm::vec<S, T>(rect.tl) }
    , size { glm::vec<S, T>(rect.size) }
{}

template <int S, class T> template <class T1>
constexpr bool tr::Rect<S, T>::operator==(const Rect<S, T1>& r) const noexcept
{
    return tl == r.tl && size == r.size;
}

template <class T> 
constexpr tr::Circle<T>::Circle(glm::tvec2<T> c, T r) noexcept
    : c { c }
    , r { r }
{}

template <class T> template <class T1>
constexpr tr::Circle<T>::Circle(const Circle<T1>& r) noexcept
    : c { glm::tvec2<T>(r.c) }
    , r { T(r.r) }
{}

template <class T> template <class T1>
constexpr bool tr::Circle<T>::operator==(const Circle<T1>& r) const noexcept
{
    return c == r.c && r == r.r;
}

template <class T1, class T2>
bool tr::within(glm::tvec2<T2> p, const Rect2<T1>& r) noexcept
{
    return p.x >= r.tl.x && p.x <= r.tl.x + r.size.x && p.y >= r.tl.y && p.y <= r.tl.y + r.size.y;
}

template <class T1, class T2>
bool tr::within(glm::tvec3<T2> p, const Rect3<T1>& r) noexcept
{
    return p.x >= r.tl.x && p.x <= r.tl.x + r.size.x && p.y >= r.tl.y && p.y <= r.tl.y + r.size.y && p.z >= r.tl.z && p.z <= r.tl.z + r.size.z;
}

template <class T1, class T2>
bool tr::within(glm::tvec2<T2> p, const Circle<T1>& c) noexcept
{
    return (std::pow(p.x - c.c.x, 2) + std::pow(p.y - c.c.y, 2)) <= std::pow(c.r, 2);
}

template <class T1, class T2, class T3>
constexpr bool tr::collinear(glm::tvec2<T1> a, glm::tvec2<T2> b, glm::tvec2<T3> c) noexcept
{
    auto tol { std::abs(std::max<std::common_type_t<T1, T2, T3>>({ a.x, a.y, b.x, b.y, c.x, c.y })) * 0.000001f };
    return std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) < tol;
}

template <class T1, class T2>
bool tr::intersecting(const Rect2<T1>& r1, const Rect2<T2>& r2) noexcept
{
    return within(r1, r2.tl) || within(r1, r2.tl + r2.size) || within(r2, r1.tl + glm::tvec2<T1> { r1.size.x, 0 }) || within(r2, r1.tl + glm::tvec2<T1> { 0, r1.size.y });
}

template <class T>
bool tr::intersecting(const Circle<T>& c1, const Circle<T>& c2) noexcept
{
    return glm::distance(c1.c, c2.c) <= c1.r + c2.r;
}

template <std::floating_point T>
constexpr glm::tvec2<T> tr::closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept
{
    auto pp { p - a };
    auto bp { b - a };
    return a + bp * std::clamp(dot(pp, bp) / dot(bp, bp), T { 0 }, T { 1 });
}

template <std::integral T>
constexpr glm::dvec2 tr::closestPoint(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept
{
    return closestPoint(glm::dvec2(p), glm::dvec2(a), glm::dvec2(b));
}

template <std::floating_point T>
constexpr std::optional<glm::tvec2<T>> tr::segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
    auto r { b2 - a2 };
    glm::tvec2<T> n { b1.y - a1.y, -b1.x + a1.x };
    auto d { dot(r, n) };
    if (!d) return std::nullopt;
    auto t { dot(a1 - a2, n) / d };
    if (t < 0 || t > 1) return std::nullopt;
    auto result { a2 + r * t };
    if ((result.x < std::min(a1.x, b1.x) || result.x > std::max(a1.x, b1.x)) || (result.x - a1.x == 0 && (result.y < std::min(a1.y, b1.y) || result.y > std::max(a1.y, b1.y)))) return std::nullopt;
    else return result;
}

template <std::integral T>
constexpr std::optional<glm::dvec2> tr::segmentIntersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
    return segmentIntersect(glm::dvec2(a1), glm::dvec2(b1), glm::dvec2(a2), glm::dvec2(b2));
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::segmentIntersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
    auto r { b2 - a2 };
    glm::tvec2<T> n { sin(th1), -cos(th1) };
    auto d { dot(r, n) };
    if (!d) return std::nullopt;
    auto t { dot(a1 - a2, n) / d };
    if (t >= 0 && t <= 1) return a2 + r * t;
    else return std::nullopt;
}

template <std::integral T>
std::optional<glm::dvec2> tr::segmentIntersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
    return segmentIntersect(glm::dvec2(a1), th1, glm::dvec2(a2), glm::dvec2(b2));
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, Angle<T> th2) noexcept
{
    glm::tvec2<T> r { cos(th2), sin(th2) };
    glm::tvec2<T> n { sin(th1), -cos(th1) };
    auto d { dot(r, n) };
    if (!d) return std::nullopt;
    auto t { dot(a1 - a2, n) / d };
    return a2 + r * t;
}

template <std::integral T>
std::optional<glm::dvec2> tr::intersect(glm::tvec2<T> a1, AngleD th1, glm::tvec2<T> a2, AngleD th2) noexcept
{
    return intersect(glm::dvec2(a1), th1, glm::dvec2(a2), th2);
}

template <std::floating_point T>
constexpr std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
    auto r { b2 - a2 };
    glm::tvec2<T> n { b1.y - a1.y, -b1.x + a1.x };
    auto d { dot(r, n) };
    if (!d) return std::nullopt;
    auto t { dot(a1 - a2, n) / d };
    return a2 + r * t;
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, Angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
    auto r { b2 - a2 };
    glm::tvec2<T> n { sin(th1), -cos(th1) };
    auto d { dot(r, n) };
    if (!d) return std::nullopt;
    auto t { dot(a1 - a2, n) / d };
    return a2 + r * t;
}

template <class T1, class T2>
constexpr std::common_type_t<T1, T2> tr::cross2(glm::tvec2<T1> a, glm::tvec2<T2> b) noexcept
{
    return a.x * b.y - a.y * b.x;
}

template <tr::Arithmetic T1, tr::Arithmetic T2>
std::common_type_t<T1, T2> tr::mod(T1 v, T2 mod) noexcept
{
    if constexpr (std::floating_point<T1> || std::floating_point<T2>) {
        return std::fmod(std::common_type_t<T1, T2>(v), std::common_type_t<T1, T2>(mod));
    }
    else return std::common_type_t<T1, T2>(v % mod);
}

template <tr::Arithmetic T>
T tr::mirrorRepeat(T v, T min, T max) noexcept
{
    auto shift { v - min };
    auto size { max - min };
    shift = mod(abs(shift + (shift < 0)), (2 * size));
    return min + (shift >= size ? 2 * size - shift - 1 : shift);
}

template <int S, class T>
glm::vec<S, T> tr::mirrorRepeat(glm::vec<S, T> v, glm::vec<S, T> min, glm::vec<S, T> max) noexcept
{
    for (auto i = 0; i < S; ++i) {
        v[i] = mirrorRepeat(v[i], min[i], max[i]);
    }
    return v;
}

template <class T>
glm::tvec2<T> tr::mirrorRepeat(glm::tvec2<T> v, const Rect2<T>& r) noexcept
{
    return mirrorRepeat(v, r.tl, r.tl + r.size);
}

template <class T>
glm::tvec3<T> tr::mirrorRepeat(glm::tvec3<T> v, const Rect3<T>& r) noexcept
{
    return mirrorRepeat(v, r.tl, r.tl + r.size);
}

template <std::floating_point T>
constexpr glm::tvec2<T> tr::inverse(glm::tvec2<T> p) noexcept
{
    return { T(1) / p.x, T(1) / p.y };
}

template <std::integral T>
constexpr glm::dvec2 tr::inverse(glm::tvec2<T> p) noexcept
{
    return { 1.0 / p.x, 1.0 / p.y };
}

template <std::floating_point T>
constexpr glm::tvec3<T> tr::inverse(glm::tvec3<T> p) noexcept
{
    return { T(1) / p.x, T(1) / p.y, T(1) / p.z };
}

template <std::integral T>
constexpr glm::dvec3 tr::inverse(glm::tvec3<T> p) noexcept
{
    return { 1.0 / p.x, 1.0 / p.y, 1.0 / p.z };
}

template <class T>
constexpr glm::tvec2<T> tr::matrix_operators::operator*(const glm::tvec2<T>& v, const glm::tmat4x4<T>& m) noexcept
{
    return glm::tvec4<T> { v, 0, 1 } * m;
}

template <class T>
constexpr glm::tvec3<T> tr::matrix_operators::operator*(const glm::tvec3<T>& v, const glm::tmat4x4<T>& m) noexcept
{
    return glm::tvec4<T> { v, 1 } * m;
}

template <class T>
constexpr glm::tvec2<T> tr::matrix_operators::operator*(const glm::tmat4x4<T>& m, const glm::tvec2<T>& v) noexcept
{
    return m * glm::tvec4<T> { v, 0, 1 };
}

template <class T>
constexpr glm::tvec3<T> tr::matrix_operators::operator*(const glm::tmat4x4<T>& m, const glm::tvec3<T>& v) noexcept
{
    return m * glm::tvec4<T> { v, 1 };
}

template <class T>
glm::tmat4x4<T> tr::scaleAroundPoint2(const glm::tmat4x4<T>& m, const glm::tvec2<T>& c, const glm::tvec2<T>& s) noexcept
{
    return scaleAroundPoint3(m, { c, 0 }, { s, 0 });
}

template <class T>
glm::tmat4x4<T> tr::scaleAroundPoint3(const glm::tmat4x4<T>& m, const glm::tvec3<T>& c, const glm::tvec3<T>& s) noexcept
{
    return translate(scale(translate(m, c), s), -c);
}

template <class T>
glm::tmat4x4<T> tr::rotateAroundPoint2(const glm::tmat4x4<T>& m, const glm::tvec2<T>& c, const Angle<T>& th) noexcept
{
    return rotateAroundPoint3(m, { c, 0 }, th, Z_AXIS);
}

template <class T>
glm::tmat4x4<T> tr::rotateAroundPoint3(const glm::tmat4x4<T>& m, const glm::tvec3<T>& c, const Angle<T>& th, const glm::tvec3<T>& axis) noexcept
{
    return translate(rotate(translate(m, c), th.rads(), axis), -c);
}