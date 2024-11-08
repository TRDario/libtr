/**
 * @file concepts.hpp
 * @brief Provides miscellaneous concepts.
 */

#pragma once
#include "../include/tr/dependencies/half.hpp"

#include <concepts>

namespace tr {
	/// @private
	// Circumvents static_assert(false) not being a valid expression.
	template <class...>
	inline constexpr bool ALWAYS_FALSE        = false;

	/******************************************************************************************************************
     * Typedef for a 16-bit floating-point type.
     ******************************************************************************************************************/
	using Half                                = half_float::half;

	/******************************************************************************************************************
     * Extended floating point concept that includes Half.
     ******************************************************************************************************************/
	template <class T> concept FloatingPoint  = std::floating_point<T> || std::same_as<Half, T>;

	/******************************************************************************************************************
     * Concept wrapper over std::is_arithmetic_v.
     ******************************************************************************************************************/
	template <class T> concept Arithmetic     = std::is_arithmetic_v<T>;

	/******************************************************************************************************************
     * Concept wrapper over std::is_enum_v.
     ******************************************************************************************************************/
	template <class T> concept Enumerator     = std::is_enum_v<T>;

	/******************************************************************************************************************
     * Concept wrapper over std::is_standard_layout_v.
     ******************************************************************************************************************/
	template <class T> concept StandardLayout = std::is_standard_layout_v<T>;

	/// @cond IMPLEMENTATION
	template <class T, template <class...> class Z>
	struct isSpecializationOf : std::false_type {};

	template <class... Args, template <class...> class Z>
	struct isSpecializationOf<Z<Args...>, Z> : std::true_type {};
	/// @endcond

	/******************************************************************************************************************
     * Concept that denotes a type is a specialization of a certain template.
     ******************************************************************************************************************/
	template <class T, template <class...> class Z> concept SpecializationOf = isSpecializationOf<T, Z>::value;

	/// @cond IMPLEMENTATION
	template <typename T>
	struct remove_noexcept {
		using type = T;
	};

	template <typename R, typename... P>
	struct remove_noexcept<R(P...) noexcept> {
		using type = R(P...);
	};

	/// @endcond

	/******************************************************************************************************************
     * Removes noexcept qualification from a function type.
     ******************************************************************************************************************/
	template <typename T>
	using remove_noexcept_t = remove_noexcept<T>::type;

	/******************************************************************************************************************
     * Concept denoting a contiguous range of standard layout objects.
     ******************************************************************************************************************/
	template <class T>
	concept StandardLayoutRange = std::ranges::contiguous_range<T> && StandardLayout<std::ranges::range_value_t<T>>;
} // namespace tr
