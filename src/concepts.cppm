/**
 * @file concepts.cppm
 * @brief Provides miscellaneous concepts.
 */

module;
#include <half.hpp>

export module tr:concepts;

import std;

export namespace tr {
	/// @private
	// Circumvents static_assert(false) not being a valid expression.
	template <class...> inline constexpr bool ALWAYS_FALSE = false;


	/******************************************************************************************************************
     * Typedef for a 16-bit floating-point type.
     ******************************************************************************************************************/
	using Half = half_float::half;

	/******************************************************************************************************************
     * Extended floating point concept that includes Half.
     ******************************************************************************************************************/
	template <class T> concept FloatingPoint = std::floating_point<T> || std::same_as<Half, T>;


	/******************************************************************************************************************
     * Concept wrapper over std::is_arithmetic_v.
     ******************************************************************************************************************/
	template <class T> concept Arithmetic = std::is_arithmetic_v<T>;

	/******************************************************************************************************************
     * Concept wrapper over std::is_enum_v.
     ******************************************************************************************************************/
	template <class T> concept Enumerator = std::is_enum_v<T>;

	/******************************************************************************************************************
     * Concept wrapper over std::is_standard_layout_v.
     ******************************************************************************************************************/
    template <class T> concept StandardLayout = std::is_standard_layout_v<T>;
	

	/// @private
	template <class T, template <class...> class Z>
	struct isSpecializationOf : std::false_type {};

	/// @private
	template <class... Args, template <class...> class Z>
	struct isSpecializationOf<Z<Args...>, Z> : std::true_type {};

	/******************************************************************************************************************
     * Concept that denotes a type is a specialization of a certain template.
     ******************************************************************************************************************/
	template <class T, template <class...> class Z>
	concept SpecializationOf = isSpecializationOf<T, Z>::value;


	/// @private
	template <typename T>
	struct remove_noexcept {
		/// @private
		using type = T;
	};

	/// @private
	template <typename R, typename ...P>
	struct remove_noexcept<R(P...) noexcept> {
		/// @private
		using type = R(P...);
	};

	/******************************************************************************************************************
     * Removes noexcept qualification from a function type.
     ******************************************************************************************************************/
	template <typename T>
	using remove_noexcept_t = remove_noexcept<T>::type;

	
	/******************************************************************************************************************
     * Concept denoting a contiguous range of standard layout objects.
     ******************************************************************************************************************/
    template <class T> concept StandardLayoutRange = std::ranges::contiguous_range<T> && StandardLayout<std::ranges::range_value_t<T>>;
}