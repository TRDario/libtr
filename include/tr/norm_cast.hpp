/**
 * @file norm_cast.hpp
 * @brief Provides a cast between normalized integers and floats.
 */

#pragma once
#include "concepts.hpp"

#include <cassert>
#include <limits>

namespace tr {
	/******************************************************************************************************************
	 * Converts a floating point number in the range [0-1] to another floating point number in the range [0-1].
	 *
	 * @param from A floating point number in the range [0-1]. A failed assertion may be triggered if a value outside
	 *             this range is passed.
	 *
	 * @return A floating point number in the range [0-1].
	 ******************************************************************************************************************/
	template <FloatingPoint To, FloatingPoint From> constexpr To norm_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Converts a normalized integer value to a floating point number in the range [0-1].
	 *
	 * @param from A normalized integer value.
	 *
	 * @return A floating point number in the range [0-1].
	 ******************************************************************************************************************/
	template <FloatingPoint To, std::integral From> constexpr To norm_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Converts a floating point number in the range [0-1] to a normalized integer.
	 *
	 * @param from A floating point number in the range [0-1]. A failed assertion may be triggered if a value outside
	 *             this range is passed.
	 *
	 * @return A normalized integer.
	 ******************************************************************************************************************/
	template <std::integral To, FloatingPoint From> constexpr To norm_cast(From from) noexcept;

	/******************************************************************************************************************
	 * Converts a normalized integer value to another normalied integer value.
	 *
	 * @param from A normalized integer value.
	 *
	 * @return A normalized integer value.
	 ******************************************************************************************************************/
	template <std::integral To, std::integral From> constexpr To norm_cast(From from) noexcept;
} // namespace tr

/// @cond IMPLEMENTATION

template <tr::FloatingPoint To, tr::FloatingPoint From> constexpr To tr::norm_cast(From from) noexcept
{
	assert(from >= 0 && from <= 1);
	return To(from);
}

template <tr::FloatingPoint To, std::integral From> constexpr To tr::norm_cast(From from) noexcept
{
	return To(double(from) / std::numeric_limits<From>::max());
}

template <std::integral To, tr::FloatingPoint From> constexpr To tr::norm_cast(From from) noexcept
{
	assert(from >= 0 && from <= 1);
	return To(double(from) * std::numeric_limits<To>::max());
}

template <std::integral To, std::integral From> constexpr To tr::norm_cast(From from) noexcept
{
	if constexpr (sizeof(From) == 8 || sizeof(To) == 8) {
		return To(double(from) / std::numeric_limits<From>::max() * std::numeric_limits<To>::max());
	}
	else if constexpr (std::signed_integral<To> || std::signed_integral<From>) {
		return To(std::int64_t(from) * std::numeric_limits<To>::max() / std::numeric_limits<From>::max());
	}
	else {
		return To(std::uint64_t(from) * std::numeric_limits<To>::max() / std::numeric_limits<From>::max());
	}
}

/// @endcond
