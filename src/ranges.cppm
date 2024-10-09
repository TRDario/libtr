module;
#include <cassert>

export module tr:ranges;

import std;
import :integer;

export namespace tr {
	// Converts a continuous container object to a span of immutable bytes.
	template <std::ranges::contiguous_range T> auto asBytes(const T& object) noexcept;
	// Gets a span of immutable bytes representing an object.
	template <class T> 				           auto asBytes(const T& object) noexcept;
	// Converts a continuous container object to a span of bytes.
	template <std::ranges::contiguous_range T> auto asMutBytes(T& object) noexcept;
	// Gets a span of bytes representing an object.
	template <class T> 				           auto asMutBytes(T& object) noexcept;

	// Reinterprets a byte span to a span of objects.
	template <class T, Size S> inline auto spanFromBytes(std::span<Byte, S> bytes) noexcept;
	// Reinterprets a byte span to a span of objects.
	template <class T, Size S> inline auto spanFromBytes(std::span<const Byte, S> bytes) noexcept;

	// Transforms a range into a view of one of the range's elements' members.
	template <std::ranges::range R, class M> constexpr auto member(R&& range, M std::ranges::range_value_t<R>::* ptr) noexcept;
}

// IMPLEMENTATION

template <std::ranges::contiguous_range T>
auto tr::asBytes(const T& object) noexcept
{
    return std::as_bytes(std::span { object });
}

template <std::ranges::contiguous_range T>
auto tr::asMutBytes(T& object) noexcept
{
    return std::as_writable_bytes(std::span { object });
}

template <class T>
auto tr::asBytes(const T& object) noexcept
{
    return std::as_bytes(std::span<const T, 1> { std::addressof(object), 1 });
}

template <class T>
auto tr::asMutBytes(T& object) noexcept 
{
    return std::as_writable_bytes(std::span<T, 1> { std::addressof(object), 1 });
}

template <class T, tr::Size S>
inline auto tr::spanFromBytes(std::span<Byte, S> bytes) noexcept
{
    if constexpr (S != std::dynamic_extent) {
        static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
        return std::span<T, S / sizeof(T)> { (T*)(bytes.data()) };
    }
    else {
        assert(bytes.size() % sizeof(T) == 0);
        return std::span { (T*)(bytes.data()), bytes.size() / sizeof(T) };
    }
}

template <class T, tr::Size S>
inline auto tr::spanFromBytes(std::span<const Byte, S> bytes) noexcept
{
    if constexpr (S != std::dynamic_extent) {
        static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
        return std::span<const T, S / sizeof(T)> { reinterpret_cast<const T*>(bytes.data()) };
    }
    else {
        assert(bytes.size() % sizeof(T) == 0);
        return std::span { reinterpret_cast<const T*>(bytes.data()), bytes.size() / sizeof(T) };
    }
}

template <std::ranges::range R, class M>
constexpr auto tr::member(R&& range, M std::ranges::range_value_t<R>::* ptr) noexcept
{
    return std::views::transform(std::forward<R&&>(range), [=] (auto& val) -> auto& { return val.*ptr; });
}