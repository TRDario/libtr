#pragma once
#include "concepts.hpp"

namespace tr {
	/** @ingroup misc
	 *  @defgroup ranges Ranges
	 *  Custom range functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Gets a view of a contiguous range as a span of immutable bytes.
	 *
	 * @tparam T A type that fulfills <em>StandardLayoutRange</em>.
	 *
	 * @param[in] range The range to get a byte view of.
	 *
	 * @return A span over the range's bytes.
	 ******************************************************************************************************************/
	template <StandardLayoutRange T> auto rangeBytes(const T& range) noexcept;

	/******************************************************************************************************************
	 * Gets a view of an object as a span of immutable bytes.
	 *
	 * @tparam T A standard layout type.
	 *
	 * @param[in] object The object to get a byte view of.
	 *
	 * @return A span over the object's bytes.
	 ******************************************************************************************************************/
	template <StandardLayout T> std::span<const std::byte, sizeof(T)> asBytes(const T& object) noexcept;

	/******************************************************************************************************************
	 * Gets a view of a contiguous range as a span of mutable bytes.
	 *
	 * @tparam T A type that fulfills <em>StandardLayoutRange</em>.
	 *
	 * @param[in] range The range to get a byte view of.
	 *
	 * @return A mutable span over the range's bytes.
	 ******************************************************************************************************************/
	template <StandardLayoutRange T> auto rangeMutBytes(T& range) noexcept;

	/******************************************************************************************************************
	 * Gets a view of an object as a span of mutable bytes.
	 *
	 * @tparam T A standard layout type.
	 *
	 * @param[in] object The object to get a byte view of.
	 *
	 * @return A mutable span over the object's bytes.
	 ******************************************************************************************************************/
	template <StandardLayout T> std::span<std::byte, sizeof(T)> asMutBytes(T& object) noexcept;

	/******************************************************************************************************************
	 * Reinterprets a span of mutable bytes as a span of objects.
	 *
	 * @pre The span must be an integer multiple of the size of the object being cast into. If the span has a static
	 *      length, the function will fail to compile, and if the span has a dynamic length, a failed assertion may be
	 *      triggered.
	 *
	 * @param[in] bytes The byte span to reinterpret.
	 *
	 * @return A mutable span over objects.
	 ******************************************************************************************************************/
	template <StandardLayout T, std::size_t S> auto asMutObjects(std::span<std::byte, S> bytes) noexcept;

	/******************************************************************************************************************
	 * Reinterprets a span of immutable bytes as a span of const objects.
	 *
	 * @pre The span must be an integer multiple of the size of the object being cast into. If the span has a static
	 *      length, the function will fail to compile, and if the span has a dynamic length, a failed assertion may be
	 *      triggered.
	 *
	 * @param[in] bytes The byte span to reinterpret.
	 *
	 * @return A span over const objects.
	 ******************************************************************************************************************/
	template <StandardLayout T, std::size_t S> auto asObjects(std::span<const std::byte, S> bytes) noexcept;

	/******************************************************************************************************************
	 * Creates an adaptor for a transformed view over a range as a range of one of its members.
	 *
	 * @param[in] ptr A class member pointer.
	 *
	 * @return An adaptor for the projection transformer.
	 ******************************************************************************************************************/
	template <class R> constexpr auto project(auto R::* ptr) noexcept;

	/******************************************************************************************************************
	 * Creates a transformed view over a range as a range of one of its members.
	 *
	 * @tparam R A range type.
	 *
	 * @param[in] range The range to transform.
	 * @param[in] ptr A class member pointer.
	 *
	 * @return The transformed view.
	 ******************************************************************************************************************/
	template <std::ranges::range R>
	constexpr auto project(R&& range, auto std::ranges::range_value_t<R>::* ptr) noexcept;

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <tr::StandardLayoutRange T> auto tr::rangeBytes(const T& range) noexcept
{
	return std::as_bytes(std::span{range});
}

template <tr::StandardLayoutRange T> auto tr::rangeMutBytes(T& range) noexcept
{
	return std::as_writable_bytes(std::span{range});
}

template <tr::StandardLayout T> std::span<const std::byte, sizeof(T)> tr::asBytes(const T& object) noexcept
{
	return std::as_bytes(std::span<const T, 1>{std::addressof(object), 1});
}

template <tr::StandardLayout T> std::span<std::byte, sizeof(T)> tr::asMutBytes(T& object) noexcept
{
	return std::as_writable_bytes(std::span<T, 1>{std::addressof(object), 1});
}

template <tr::StandardLayout T, std::size_t S> auto tr::asMutObjects(std::span<std::byte, S> bytes) noexcept
{
	if constexpr (S != std::dynamic_extent) {
		static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<T, S / sizeof(T)>{(T*)(bytes.data())};
	}
	else {
		assert(bytes.size() % sizeof(T) == 0);
		return std::span{(T*)(bytes.data()), bytes.size() / sizeof(T)};
	}
}

template <tr::StandardLayout T, std::size_t S> auto tr::asObjects(std::span<const std::byte, S> bytes) noexcept
{
	if constexpr (S != std::dynamic_extent) {
		static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<const T, S / sizeof(T)>{reinterpret_cast<const T*>(bytes.data())};
	}
	else {
		assert(bytes.size() % sizeof(T) == 0);
		return std::span{(const T*)(bytes.data()), bytes.size() / sizeof(T)};
	}
}

template <class R> constexpr auto tr::project(auto R::* ptr) noexcept
{
	return std::views::transform([=](auto&& val) -> auto&& { return val.*ptr; });
}

template <std::ranges::range R>
constexpr auto tr::project(R&& range, auto std::ranges::range_value_t<R>::* ptr) noexcept
{
	return std::views::transform(std::forward<R&&>(range), [=](auto&& val) -> auto&& { return val.*ptr; });
}

/// @endcond
