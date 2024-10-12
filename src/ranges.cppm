/**
 * @file ranges.cppm
 * @brief Provides miscellaneous range functionality.
 */

module;
#include <cassert>

export module tr:ranges;

import std;
import :concepts;
import :integer;

export namespace tr {
	/******************************************************************************************************************
    * Gets a view of a contiguous range as a span of immutable bytes.
    *
    * @tparam T A type that fulfills <em>StandardLayoutRange</em>.
    *
    * @param range The range to get a byte view of.
    *
    * @return A span over the range's bytes.
    ******************************************************************************************************************/
	template <StandardLayoutRange T>
    auto asBytes(const T& range) noexcept;

	/******************************************************************************************************************
    * Gets a view of an object as a span of immutable bytes.
    *
    * @tparam T A standard layout type.
    *
    * @param object The object to get a byte view of.
    *
    * @return A span over the object's bytes.
    ******************************************************************************************************************/
    template <StandardLayout T> requires (!StandardLayoutRange<T>)
    std::span<const Byte, sizeof(T)> asBytes(const T& object) noexcept;

    /******************************************************************************************************************
    * Gets a view of a contiguous range as a span of mutable bytes.
    *
    * @tparam T A type that fulfills <em>StandardLayoutRange</em>.
    *
    * @param range The range to get a byte view of.
    *
    * @return A mutable span over the range's bytes.
    ******************************************************************************************************************/
	template <StandardLayoutRange T>
    auto asMutBytes(T& object) noexcept;

	/******************************************************************************************************************
    * Gets a view of an object as a span of mutable bytes.
    *
    * @tparam T A standard layout type.
    *
    * @param object The object to get a byte view of.
    *
    * @return A mutable span over the object's bytes.
    ******************************************************************************************************************/
    template <StandardLayout T> requires (!StandardLayoutRange<T>)
    std::span<Byte, sizeof(T)> asMutBytes(T& object) noexcept;

    /******************************************************************************************************************
    * Reinterprets a span of mutable bytes as a span of objects.
    *
    * The span must be an integer multiple of the size of the object being cast into. If the span has a static length,
    * the function will fail to compile, and if the span has a dynamic length, a failed assertion may be triggered.
    *
    * @param bytes The byte span to reinterpret.
    *
    * @return A mutable span over objects.
    ******************************************************************************************************************/
	template <class T, Size S>
    inline auto asObjects(std::span<Byte, S> bytes) noexcept;

	/******************************************************************************************************************
    * Reinterprets a span of immutable bytes as a span of const objects.
    *
    * The span must be an integer multiple of the size of the object being cast into. If the span has a static length,
    * the function will fail to compile, and if the span has a dynamic length, a failed assertion may be triggered.
    *
    * @param bytes The byte span to reinterpret.
    *
    * @return A span over const objects.
    ******************************************************************************************************************/
	template <class T, Size S>
    inline auto asObjects(std::span<const Byte, S> bytes) noexcept;


    /******************************************************************************************************************
    * Creates an adaptor for a transformed view over a range as a range of one of its members.
    *
    * @param ptr A class member pointer.
    *
    * @return An adaptor for the projection transformer.
    ******************************************************************************************************************/
	template <class R>
    constexpr auto project(auto R::* ptr) noexcept;

	/******************************************************************************************************************
    * Creates a transformed view over a range as a range of one of its members.
    *
    * @tparam R A range type.
    *
    * @param range The range to transform.
    * @param ptr A class member pointer.
    *
    * @return The transformed view.
    ******************************************************************************************************************/
	template <std::ranges::range R>
    constexpr auto project(R&& range, auto std::ranges::range_value_t<R>::* ptr) noexcept;
}

// IMPLEMENTATION

template <tr::StandardLayoutRange T>
auto tr::asBytes(const T& range) noexcept
{
    return std::as_bytes(std::span { range });
}

template <tr::StandardLayoutRange T>
auto tr::asMutBytes(T& range) noexcept
{
    return std::as_writable_bytes(std::span { range });
}

template <tr::StandardLayout T> requires (!tr::StandardLayoutRange<T>)
std::span<const tr::Byte, sizeof(T)> tr::asBytes(const T& object) noexcept
{
    return std::as_bytes(std::span<const T, 1> { std::addressof(object), 1 });
}

template <tr::StandardLayout T>  requires (!tr::StandardLayoutRange<T>)
std::span<tr::Byte, sizeof(T)> tr::asMutBytes(T& object) noexcept 
{
    return std::as_writable_bytes(std::span<T, 1> { std::addressof(object), 1 });
}

template <class T, tr::Size S>
inline auto tr::asObjects(std::span<Byte, S> bytes) noexcept
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
inline auto tr::asObjects(std::span<const Byte, S> bytes) noexcept
{
    if constexpr (S != std::dynamic_extent) {
        static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
        return std::span<const T, S / sizeof(T)> { reinterpret_cast<const T*>(bytes.data()) };
    }
    else {
        assert(bytes.size() % sizeof(T) == 0);
        return std::span { (const T*)(bytes.data()), bytes.size() / sizeof(T) };
    }
}

template <class R>
constexpr auto tr::project(auto R::* ptr) noexcept
{
    return std::views::transform([=] (auto&& val) -> auto&& { return val.*ptr; });
}

template <std::ranges::range R>
constexpr auto tr::project(R&& range, auto std::ranges::range_value_t<R>::* ptr) noexcept
{
    return std::views::transform(std::forward<R&&>(range), [=] (auto&& val) -> auto&& { return val.*ptr; });
}