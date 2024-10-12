module;
#include <half.hpp>

export module tr:concepts;

import std;

export namespace tr {
	using Half = half_float::half;

	template <class... Ts> using Promoted = std::common_type_t<Ts...>;
	template <class T> concept Arithmetic = std::is_arithmetic_v<T>;
	template <class T> concept Enumerator = std::is_enum_v<T>;
	template <class T> concept FloatingPoint = std::floating_point<T> || std::same_as<Half, T>;

	// Circumvents static_assert(false) not being a valid expression (WTF?)
	template <class...> inline constexpr bool ALWAYS_FALSE = false;

	template <class T, template <class...> class Z> struct isSpecializationOf : std::false_type {};
	template <class... Args, template <class...> class Z> struct isSpecializationOf<Z<Args...>, Z> : std::true_type {};
	template <class T, template <class...> class Z> inline constexpr bool isSpecializationOfV = isSpecializationOf<T, Z>::value;
	template<class T, template <class...> class Z> concept SpecializationOf = isSpecializationOfV<T, Z>;

	/******************************************************************************************************************
    * Concept denoting a C++ standard layout object.
    ******************************************************************************************************************/
    template <class T> concept StandardLayout = std::is_standard_layout_v<T>;
	
	/******************************************************************************************************************
    * Concept denoting a contiguous range of standard layout objects.
    ******************************************************************************************************************/
    template <class T> concept StandardLayoutRange = std::ranges::contiguous_range<T> && StandardLayout<std::ranges::range_value_t<T>>;
}