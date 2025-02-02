#pragma once
#include "common.hpp"

namespace tr {
	/** @ingroup misc
	 *  @defgroup concepts Concepts
	 *  Miscellaneous concepts.
	 *  @{
	 */

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
	template <class T>
	concept FloatingPoint = std::floating_point<T> || std::same_as<Half, T>;

	/******************************************************************************************************************
	 * Concept wrapper over std::is_arithmetic_v.
	 ******************************************************************************************************************/
	template <class T>
	concept Arithmetic = std::is_arithmetic_v<T>;

	/******************************************************************************************************************
	 * Concept denoting a non-const reference.
	 ******************************************************************************************************************/
	template <class T>
	concept NonConstReference = std::is_lvalue_reference_v<T> && !std::is_const_v<std::remove_reference_t<T>>;

	/******************************************************************************************************************
	 * Concept denoting a reference to const.
	 ******************************************************************************************************************/
	template <class T>
	concept ConstReference = std::is_lvalue_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>;

	/******************************************************************************************************************
	 * Concept wrapper over std::is_enum_v.
	 ******************************************************************************************************************/
	template <class T>
	concept Enumerator = std::is_enum_v<T>;

	/******************************************************************************************************************
	 * Concept wrapper over std::is_standard_layout_v.
	 ******************************************************************************************************************/
	template <class T>
	concept StandardLayout = std::is_standard_layout_v<T>;

	/******************************************************************************************************************
	 * Concept denoting a contiguous range of standard layout objects.
	 ******************************************************************************************************************/
	template <class T>
	concept StandardLayoutRange = std::ranges::contiguous_range<T> && StandardLayout<std::ranges::range_value_t<T>>;

	/// @cond IMPLEMENTATION
	template <class T, template <class...> class Z> struct isSpecializationOf : std::false_type {};

	template <class... Args, template <class...> class Z> struct isSpecializationOf<Z<Args...>, Z> : std::true_type {};
	/// @endcond

	/******************************************************************************************************************
	 * Concept that denotes a type is a specialization of a certain template.
	 ******************************************************************************************************************/
	template <class T, template <class...> class Z>
	concept SpecializationOf = isSpecializationOf<T, Z>::value;

	/// @cond IMPLEMENTATION
	template <typename T> struct RemoveNoexcept {
		using type = T;
	};
	template <typename R, typename... P> struct RemoveNoexcept<R(P...) noexcept> {
		using type = R(P...);
	};
	/// @endcond

	/******************************************************************************************************************
	 * Removes noexcept qualification from a function type.
	 ******************************************************************************************************************/
	template <typename T> using RemoveNoexceptT = RemoveNoexcept<T>::type;

	/// @cond IMPLEMENTATION
	template <class T> struct ReturnType {};
	///
	template <class T, class... Ts> struct ReturnType<T(Ts...)> {
		using type = T;
	};
	template <class T, class... Ts> struct ReturnType<T (*)(Ts...)> {
		using type = T;
	};
	template <class T, class... Ts> struct ReturnType<T(Ts...) noexcept> {
		using type = T;
	};
	template <class T, class... Ts> struct ReturnType<T (*)(Ts...) noexcept> {
		using type = T;
	};
	/// @endcond

	/******************************************************************************************************************
	 * Gets the return type of a function.
	 ******************************************************************************************************************/
	template <typename T> using ReturnTypeT = ReturnType<T>::type;

	/******************************************************************************************************************
	 * Removes noexcept qualification from a function type.
	 ******************************************************************************************************************/
	template <typename T> using RemoveNoexceptT = RemoveNoexcept<T>::type;

	/// @cond IMPLEMENTATION
	template <class T> struct ArgumentType {};
	///
	template <class T, class T1> struct ArgumentType<T(T1)> {
		using type = T1;
	};
	template <class T, class T1> struct ArgumentType<T (*)(T1)> {
		using type = T1;
	};
	template <class T, class T1> struct ArgumentType<T(T1) noexcept> {
		using type = T1;
	};
	template <class T, class T1> struct ArgumentType<T (*)(T1) noexcept> {
		using type = T1;
	};
	/// @endcond

	/******************************************************************************************************************
	 * Gets the type of the argument of a function.
	 ******************************************************************************************************************/
	template <typename T> using ArgumentTypeT = ArgumentType<T>::type;

	/// @}
} // namespace tr
