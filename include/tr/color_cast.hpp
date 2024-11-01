/**
 * @file color_cast.hpp
 * @brief Provides color casting facilities.
 */

#pragma once
#include <boost/type_traits.hpp>
#include "color.hpp"
#include "norm_cast.hpp"

namespace tr {
	/******************************************************************************************************************
     * Interface for custom casters for use in color_cast.
	 *
	 * To fulfill the requirements for a valid color caster specialization, the following must be present:
	 *
	 * static function toBuiltin that takes a value/const reference of type @em T and returns a builtin color type.
	 *
	 * static function fromBuiltin that takes a value/const reference of a BuiltinColor type and returns a value
	 * of type <em>T</em>.
	 *
	 * If these conditions are properly met, @em T will fulfill ColorCastableFrom and ColorCastableTo.
     ******************************************************************************************************************/
	template <class T>
	struct ColorCaster {};


	/******************************************************************************************************************
     * Concept that denotes a type able to be color casted from.
	 *
	 * To fulfill this condition, ColorCaster<T> must have a static function @em toBuiltin taking a @em T
	 * and returning a builtin color type.
     *******************************************************************************************************************/
	template <class T> concept ColorCastableFrom = requires(T val) { { ColorCaster<T>::toBuiltin(val) } -> BuiltinColor; };

	/******************************************************************************************************************
     * Concept that denotes a type able to be color casted to.
	 *
	 * To fulfill this condition, ColorCaster<T> must have a static function @em fromBuiltin taking a BuiltinColor
	 * and returning <em>T</em>.
     ******************************************************************************************************************/
	template <class T> concept ColorCastableTo = 
		BuiltinColor<std::remove_cvref_t<typename boost::function_traits<remove_noexcept_t<decltype(ColorCaster<T>::fromBuiltin)>>::argument_type>> &&
		requires(typename boost::function_traits<remove_noexcept_t<decltype(ColorCaster<T>::fromBuiltin)>>::argument_type val) { { ColorCaster<T>::fromBuiltin(val) } -> std::same_as<T>; };


	/******************************************************************************************************************
     * Converts a red channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of R.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<R> From>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts a green channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of G.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<G> From>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts a blue channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of B.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<B> From>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts an RG channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RG.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<RG> From>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts a three-channel (RGB/BGR) channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RGB or BGR.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To, class From> requires SpecializationOf<From, RGB> || SpecializationOf<From, BGR>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts a four-channel (RGBA/BGRA) channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RGBA or BGRA.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To, class From> requires SpecializationOf<From, RGBA> || SpecializationOf<From, BGRA>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts a RGB_Ui8_2_3_3 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const RGB_Ui8_2_3_3& from) noexcept;

	/******************************************************************************************************************
     * Converts a RGB_Ui16_5_6_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const RGB_Ui16_5_6_5& from) noexcept;

	/******************************************************************************************************************
     * Converts a BGR_Ui8_3_3_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const BGR_Ui8_3_3_2& from) noexcept;

	/******************************************************************************************************************
     * Converts a BGR_Ui16_5_6_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const BGR_Ui16_5_6_5& from) noexcept;

	/******************************************************************************************************************
     * Converts a RGBA_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const RGBA_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
     * Converts a RGBA_Ui16_5_5_5_1 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const RGBA_Ui16_5_5_5_1& from) noexcept;

	/******************************************************************************************************************
     * Converts a RGBA_Ui32_10_10_10_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const RGBA_Ui32_10_10_10_2& from) noexcept;

	/******************************************************************************************************************
     * Converts a ABGR_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const ABGR_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
     * Converts a ABGR_Ui16_1_5_5_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const ABGR_Ui16_1_5_5_5& from) noexcept;

	/******************************************************************************************************************
     * Converts a ABGR_Ui32_2_10_10_10 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const ABGR_Ui32_2_10_10_10& from) noexcept;

	/******************************************************************************************************************
     * Converts a BGRA_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const BGRA_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
     * Converts a BGRA_Ui16_5_5_5_1 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const BGRA_Ui16_5_5_5_1& from) noexcept;

	/******************************************************************************************************************
     * Converts a BGRA_Ui32_10_10_10_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const BGRA_Ui32_10_10_10_2& from) noexcept;

	/******************************************************************************************************************
     * Converts a ARGB_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const ARGB_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
     * Converts a ARGB_Ui16_1_5_5_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const ARGB_Ui16_1_5_5_5& from) noexcept;

	/******************************************************************************************************************
     * Converts a ARGB_Ui32_2_10_10_10 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To>
	constexpr To color_cast(const ARGB_Ui32_2_10_10_10& from) noexcept;

	/******************************************************************************************************************
     * Converts a custom castable color type to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A type with a specialized ColorCaster.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <BuiltinColor To, ColorCastableFrom From>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts a built-in color to a custom castable color type.
	 *
	 * @tparam To A type with a specialized ColorCaster.
	 * @tparam From A built-in color type.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <ColorCastableTo To, BuiltinColor From>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
     * Converts a custom castable color type to another custom castable color type.
	 *
	 * @tparam To A type with a specialized ColorCaster.
	 * @tparam From A type with a specialized ColorCaster.
	 *
	 * @param from The value to be cast.
	 *
	 * @return The converted value.
     ******************************************************************************************************************/
	template <ColorCastableTo To, ColorCastableFrom From>
	constexpr To color_cast(const From& from) noexcept;


	/******************************************************************************************************************
     * Specialization of ColorCaster for HSV.
     ******************************************************************************************************************/
	template <> struct ColorCaster<HSV> {
		/******************************************************************************************************************
    	 * Casts an HSV value to a floating point RGBA value.
		 *
		 * @param from The HSV value to be converted.
		 *
		 * @return The corresponding RGBAF value.
    	 ******************************************************************************************************************/
		static constexpr RGBAF toBuiltin(const HSV& from) noexcept;

		/******************************************************************************************************************
    	 * Casts an RGBAF value to an HSV value.
		 *
		 * @param from The RGBAF value to be converted.
		 *
		 * @return The corresponding HSV value.
    	 ******************************************************************************************************************/
		static constexpr HSV fromBuiltin(const RGBAF& from) noexcept;
	};

}

/// @cond IMPLEMENTATION

namespace tr {
	consteval std::size_t umax(std::uint8_t bits) noexcept;
}

consteval std::size_t tr::umax(std::uint8_t bits) noexcept
{
	return (std::size_t { 1 } << bits) - 1;
}

template <tr::BuiltinColor To, tr::SpecializationOf<tr::R> From>
constexpr To tr::color_cast(const From& from) noexcept
{
	if constexpr (SpecializationOf<To, G> || SpecializationOf<To, B>) {
		return { 0 };
	}
	else {
		using T = decltype(To::r);
		constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

		if constexpr (SpecializationOf<To, R>) {
			return { norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RG>) {
			return { norm_cast<T>(from.r), 0 };
		}
		else if constexpr (SpecializationOf<To, RGB>) {
			return { norm_cast<T>(from.r), 0, 0 };
		}
		else if constexpr (SpecializationOf<To, BGR>) {
			return { 0, 0, norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RGBA>) {
			return { norm_cast<T>(from.r), 0, 0, norm_cast<T>(1.0) };
		}
		else if constexpr (SpecializationOf<To, BGRA>) {
			return { 0, 0, norm_cast<T>(from.r), norm_cast<T>(1.0) };
		}
		else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
			return { std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
			return { std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
			return { 0, 0, std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
			return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
			return { std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), 0, 0, std::uint16_t(umax(4)) };
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
			return { std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0, std::uint16_t(umax(1)) };
		}
		else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
			return { std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), 0, 0, std::uint32_t(umax(2)) };
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
			return { std::uint16_t(umax(4)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
			return { std::uint16_t(umax(1)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
			return { std::uint32_t(umax(2)), 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
			return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), std::uint16_t(umax(4)) };
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
			return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), std::uint16_t(umax(1)) };
		}
		else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
			return { 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), std::uint32_t(umax(2)) };
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
			return { std::uint16_t(umax(4)), std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
			return { std::uint16_t(umax(1)), std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
			return { std::uint32_t(umax(2)), std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX), 0, 0 };
		}
	}
}

template <tr::BuiltinColor To, tr::SpecializationOf<tr::G> From>
constexpr To tr::color_cast(const From& from) noexcept
{
	if constexpr (SpecializationOf<To, R> || SpecializationOf<To, B>) {
		return { 0 };
	}
	else {
		using T = decltype(To::g);
		constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

		if constexpr (SpecializationOf<To, G>) {
			return { norm_cast<T>(from.g) };
		}
		else if constexpr (SpecializationOf<To, RG>) {
			return { 0, norm_cast<T>(from.g) };
		}
		else if constexpr (SpecializationOf<To, RGB> || SpecializationOf<To, RGB>) {
			return { 0, norm_cast<T>(from.g), 0 };
		}
		else if constexpr (SpecializationOf<To, RGBA>) {
			return { 0, norm_cast<T>(from.g), 0, norm_cast<T>(1.0) };
		}
		else if constexpr (SpecializationOf<To, BGRA>) {
			return { 0, norm_cast<T>(from.g), 0, norm_cast<T>(1.0) };
		}
		else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
			return { 0, std::uint8_t(color_cast<G<std::uint8_t>>(from).g * umax(3) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
			return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(6) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
			return { 0, std::uint8_t(color_cast<G<std::uint8_t>>(from).g * umax(3) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
			return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(6) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
			return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0, std::uint16_t(umax(4)) };
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
			return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0, std::uint16_t(umax(1)) };
		}
		else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
			return { 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0, std::uint32_t(umax(2)) };
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
			return { std::uint16_t(umax(4)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
			return { std::uint16_t(umax(1)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
			return { std::uint32_t(umax(2)), 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
			return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0, std::uint16_t(umax(4)) };
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
			return { 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0, std::uint16_t(umax(1)) };
		}
		else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
			return { 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0, std::uint32_t(umax(2)) };
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
			return { std::uint16_t(umax(4)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(4) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
			return { std::uint16_t(umax(1)), 0, std::uint16_t(color_cast<G<std::uint8_t>>(from).g * umax(5) / UI32_MAX), 0 };
		}
		else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
			return { std::uint32_t(umax(2)), 0, std::uint32_t(color_cast<G<std::uint16_t>>(from).g * umax(10) / UI32_MAX), 0 };
		}
	}
}

template <tr::BuiltinColor To, tr::SpecializationOf<tr::B> From>
constexpr To tr::color_cast(const From& from) noexcept
{
	if constexpr (SpecializationOf<To, R> || SpecializationOf<To, G> || SpecializationOf<To, RG>) {
		return { 0 };
	}
	else {
		using T = decltype(To::b);
		constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

		if constexpr (SpecializationOf<To, B>) {
			return { norm_cast<T>(from.b) };
		}
		else if constexpr (SpecializationOf<To, RGB>) {
			return { 0, 0, norm_cast<T>(from.b) };
		}
		else if constexpr (SpecializationOf<To, BGR>) {
			return { norm_cast<T>(from.b), 0, 0 };
		}
		else if constexpr (SpecializationOf<To, RGBA>) {
			return { 0, 0, norm_cast<T>(from.b), norm_cast<T>(1.0) };
		}
		else if constexpr (SpecializationOf<To, BGRA>) {
			return { norm_cast<T>(from.b), 0, 0, norm_cast<T>(1.0) };
		}
		else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
			return { 0, 0, std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
			return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
			return { std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
			return { std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
			return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), std::uint16_t(umax(4)) };
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
			return { 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), std::uint16_t(umax(1)) };
		}
		else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
			return { 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), std::uint32_t(umax(2)) };
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
			return { std::uint16_t(umax(4)), std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
			return { std::uint16_t(umax(1)), std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
			return { std::uint32_t(umax(2)), std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), 0, 0 };
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
			return { std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX), 0, 0, std::uint16_t(umax(4)) };
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
			return { std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX), 0, 0, std::uint16_t(umax(1)) };
		}
		else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
			return { std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX), 0, 0, std::uint32_t(umax(2)) };
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
			return { std::uint16_t(umax(4)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
			return { std::uint16_t(umax(1)), 0, 0, std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX) };
		}
		else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
			return { std::uint32_t(umax(2)), 0, 0, std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX) };
		}
	}
}

template <tr::BuiltinColor To, tr::SpecializationOf<tr::RG> From>
constexpr To tr::color_cast(const From& from) noexcept
{
	if constexpr (SpecializationOf<To, B>) {
		return { 0 };
	}
	else {
		using T = decltype(To::r);
		constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

		if constexpr (SpecializationOf<To, R>) {
			return { norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RG>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g) };
		}
		else if constexpr (SpecializationOf<To, RGB>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g), 0 };
		}
		else if constexpr (SpecializationOf<To, BGR>) {
			return { 0, norm_cast<T>(from.g), norm_cast<T>(from.r) };
		}
		else if constexpr (SpecializationOf<To, RGBA>) {
			return { norm_cast<T>(from.r), norm_cast<T>(from.g), 0, norm_cast<T>(1.0) };
		}
		else if constexpr (SpecializationOf<To, BGRA>) {
			return { 0, norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(1.0) };
		}
		else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
			return {
				std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				0
			};
		}
		else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				0
			};
		}
		else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
			return {
				0,
				std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
				std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
			return {
				0,
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				0,
				std::uint16_t(umax(4))
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
			return {
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				0,
				std::uint16_t(umax(1))
			};
		}
		else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
			return {
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				0,
				std::uint32_t(umax(2))
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(umax(4)),
				0,
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
			return {
				std::uint16_t(umax(1)),
				0,
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
			return {
				std::uint32_t(umax(2)),
				0,
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
			return {
				0,
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(umax(4))
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
			return {
				0,
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(umax(1))
			};
		}
		else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
			return {
				0,
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(umax(2))
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
			return {
				std::uint16_t(umax(4)),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
				0
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
			return {
				std::uint16_t(umax(1)),
				std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
				std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
				0
			};
		}
		else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
			return {
				std::uint32_t(umax(2)),
				std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
				std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
				0
			};
		}
	}
}

template <tr::BuiltinColor To, class From> requires tr::SpecializationOf<From, tr::RGB> || tr::SpecializationOf<From, tr::BGR>
constexpr To tr::color_cast(const From& from) noexcept
{
	using T = decltype(To::r);
	constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

	if constexpr (SpecializationOf<To, R>) {
		return { norm_cast<T>(from.r) };
	}
	else if constexpr (SpecializationOf<To, RG>) {
		return { norm_cast<T>(from.r), norm_cast<T>(from.g) };
	}
	else if constexpr (SpecializationOf<To, RGB>) {
		return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b) };
	}
	else if constexpr (SpecializationOf<To, BGR>) {
		return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r) };
	}
	else if constexpr (SpecializationOf<To, RGBA>) {
		return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b), norm_cast<T>(1.0) };
	}
	else if constexpr (SpecializationOf<To, BGRA>) {
		return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(1.0) };
	}
	else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
		return {
			std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
		return {
			std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
			std::uint16_t(umax(4))
		};
	}
	else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(umax(1))
		};
	}
	else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
		return {
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
			std::uint32_t(umax(2))
		};
	}
	else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(umax(4)),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
		return {
			std::uint16_t(umax(1)),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
		return {
			std::uint32_t(umax(2)),
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
			std::uint16_t(umax(4))
		};
	}
	else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(umax(1))
		};
	}
	else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
		return {
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
			std::uint32_t(umax(2))
		};
	}
	else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(umax(4)),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
		return {
			std::uint16_t(umax(1)),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
		return {
			std::uint32_t(umax(2)),
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX)
		};
	}
}

template <tr::BuiltinColor To, class From> requires tr::SpecializationOf<From, tr::RGBA> || tr::SpecializationOf<From, tr::BGRA>
constexpr To tr::color_cast(const From& from) noexcept
{
	using T = decltype(To::r);
	constexpr auto UI32_MAX { std::numeric_limits<std::uint32_t>::max() };

	if constexpr (SpecializationOf<To, R>) {
		return { norm_cast<T>(from.r) };
	}
	else if constexpr (SpecializationOf<To, RG>) {
		return { norm_cast<T>(from.r), norm_cast<T>(from.g) };
	}
	else if constexpr (SpecializationOf<To, RGB>) {
		return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b) };
	}
	else if constexpr (SpecializationOf<To, BGR>) {
		return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r) };
	}
	else if constexpr (SpecializationOf<To, RGBA>) {
		return { norm_cast<T>(from.r), norm_cast<T>(from.g), norm_cast<T>(from.b), norm_cast<T>(from.a) };
	}
	else if constexpr (SpecializationOf<To, BGRA>) {
		return { norm_cast<T>(from.b), norm_cast<T>(from.g), norm_cast<T>(from.r), norm_cast<T>(from.a) };
	}
	else if constexpr (std::same_as<To, RGB_Ui8_2_3_3>) {
		return {
			std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, RGB_Ui16_5_6_5>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGR_Ui8_3_3_2>) {
		return {
			std::uint8_t(norm_cast<std::uint32_t>(from.b) * umax(3) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.g) * umax(3) / UI32_MAX),
			std::uint8_t(norm_cast<std::uint32_t>(from.r) * umax(2) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGR_Ui16_5_6_5>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(6) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, RGBA_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, RGBA_Ui16_5_5_5_1>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint16_t>(from.a) * umax(1) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, RGBA_Ui32_10_10_10_2>) {
		return {
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ABGR_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ABGR_Ui16_1_5_5_5>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(1) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ABGR_Ui32_2_10_10_10>) {
		return {
			std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGRA_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint16_t>(from.a) * umax(4) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGRA_Ui16_5_5_5_1>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint16_t>(from.a) * umax(1) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, BGRA_Ui32_10_10_10_2>) {
		return {
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ARGB_Ui16_4_4_4_4>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(4) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(4) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ARGB_Ui16_1_5_5_5>) {
		return {
			std::uint16_t(norm_cast<std::uint32_t>(from.a) * umax(1) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.r) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.g) * umax(5) / UI32_MAX),
			std::uint16_t(norm_cast<std::uint32_t>(from.b) * umax(5) / UI32_MAX)
		};
	}
	else if constexpr (std::same_as<To, ARGB_Ui32_2_10_10_10>) {
		return {
			std::uint32_t(norm_cast<std::uint32_t>(from.a) * umax(2) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.r) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.g) * umax(10) / UI32_MAX),
			std::uint32_t(norm_cast<std::uint32_t>(from.b) * umax(10) / UI32_MAX)
		};
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const RGB_Ui8_2_3_3& from) noexcept
{
	if constexpr (std::same_as<To, RGB8>) {
		return { std::uint8_t(from.r * 255 / umax(2)), std::uint8_t(from.g * 255 / umax(3)), std::uint8_t(from.b * 255 / umax(3)), };
	}
	else {
		return color_cast<To>(color_cast<RGB8>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const RGB_Ui16_5_6_5& from) noexcept
{
	if constexpr (std::same_as<To, RGB8>) {
		return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(6)), std::uint8_t(from.b * 255 / umax(5)), };
	}
	else {
		return color_cast<To>(color_cast<RGB8>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const BGR_Ui8_3_3_2& from) noexcept
{
	if constexpr (std::same_as<To, RGB8>) {
		return { std::uint8_t(from.r * 255 / umax(2)), std::uint8_t(from.g * 255 / umax(3)), std::uint8_t(from.b * 255 / umax(3)), };
	}
	else {
		return color_cast<To>(color_cast<RGB8>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const BGR_Ui16_5_6_5& from) noexcept
{
	if constexpr (std::same_as<To, RGB8>) {
		return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(6)), std::uint8_t(from.b * 255 / umax(5)), };
	}
	else {
		return color_cast<To>(color_cast<RGB8>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const RGBA_Ui16_4_4_4_4& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const RGBA_Ui16_5_5_5_1& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const RGBA_Ui32_10_10_10_2& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
		return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const ABGR_Ui16_4_4_4_4& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const ABGR_Ui16_1_5_5_5& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const ABGR_Ui32_2_10_10_10& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
		return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const BGRA_Ui16_4_4_4_4& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const BGRA_Ui16_5_5_5_1& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const BGRA_Ui32_10_10_10_2& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
		return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const ARGB_Ui16_4_4_4_4& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(4)), std::uint8_t(from.g * 255 / umax(4)), std::uint8_t(from.b * 255 / umax(4)), std::uint8_t(from.a * 255 / umax(4)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const ARGB_Ui16_1_5_5_5& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint8_t>>) {
		return { std::uint8_t(from.r * 255 / umax(5)), std::uint8_t(from.g * 255 / umax(5)), std::uint8_t(from.b * 255 / umax(5)), std::uint8_t(from.a * 255 / umax(1)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint8_t>>(from));
	}
}

template <tr::BuiltinColor To>
constexpr To tr::color_cast(const ARGB_Ui32_2_10_10_10& from) noexcept
{
	if constexpr (std::same_as<To, RGBA<std::uint16_t>>) {
		return { std::uint16_t(from.r * 65535 / umax(10)), std::uint16_t(from.g * 65535 / umax(10)), std::uint16_t(from.b * 65535 / umax(10)), std::uint16_t(from.a * 65535 / umax(2)) };
	}
	else {
		return color_cast<To>(color_cast<RGBA<std::uint16_t>>(from));
	}
}

template <tr::BuiltinColor To, tr::ColorCastableFrom From>
constexpr To tr::color_cast(const From& from) noexcept
{
	if constexpr (std::same_as<typename boost::function_traits<remove_noexcept_t<decltype(ColorCaster<From>::toBuiltin)>>::result_type, To>) {
		return ColorCaster<From>::toBuiltin(from);
	}
	else {
		return color_cast<To>(ColorCaster<From>::toBuiltin(from));
	}
}

template <tr::ColorCastableTo To, tr::BuiltinColor From>
constexpr To tr::color_cast(const From& from) noexcept
{
	using NaturalBuiltin = std::remove_cvref_t<typename boost::function_traits<remove_noexcept_t<decltype(ColorCaster<From>::fromBuiltin)>>::argument_type>;

	if constexpr (std::same_as<NaturalBuiltin, From>) {
		return ColorCaster<To>::fromBuiltin(from);
	}
	else {
		return ColorCaster<To>::fromBuiltin(color_cast<NaturalBuiltin>(from));
	}
}

template <tr::ColorCastableTo To, tr::ColorCastableFrom From>
constexpr To tr::color_cast(const From& from) noexcept
{
	return color_cast<To>(color_cast<std::remove_cvref_t<typename boost::function_traits<remove_noexcept_t<decltype(ColorCaster<From>::fromBuiltin)>>::argument_type>>(from));
}

constexpr tr::RGBAF tr::ColorCaster<tr::HSV>::toBuiltin(const HSV& from) noexcept
{
	constexpr auto constexprAbs = [] (float arg) {
		return arg >= 0 ? arg : -arg;
	};
	constexpr auto eucMod = [] (float arg, float modulo) {
		const float mod { arg - modulo * std::int64_t(arg / modulo) };
		return mod >= 0 ? mod : mod + modulo;
	};

	const float h { eucMod(from.h, 360.0f) };
	const float c { from.v * from.s };
	const float m { from.v - c };
	const float x { c * (1 - constexprAbs(eucMod((h / 60.0f), 2.0f) - 1)) + m };

	if (h < 60) {
		return { from.v, x, m, 1.0f };
	}
	else if (h < 120) {
		return { x, from.v, m, 1.0f };
	}
	else if (h < 180) {
		return { m, from.v, x, 1.0f };
	}
	else if (h < 240) {
		return { m, x, from.v, 1.0f };
	}
	else if (h < 300) {
		return { x, m, from.v, 1.0f };
	}
	else {
		return { from.v, m, x, 1.0f };
	}
}

constexpr tr::HSV tr::ColorCaster<tr::HSV>::fromBuiltin(const RGBAF& from) noexcept
{
	constexpr auto eucMod = [] (float arg, float modulo) {
		const float mod { arg - modulo * std::int64_t(arg / modulo) };
		return mod >= 0 ? mod : mod + modulo;
	};

	HSV hsv {};
	hsv.v = std::max({ from.r, from.g, from.b });
	const float delta { hsv.v - std::min({ from.r, from.g, from.b }) };

	if (delta == 0) {
		hsv.h = 0;
	}
	else if (hsv.v == from.r) {
		hsv.h = 60.0f * eucMod((from.g - from.b) / delta, 6.0f);
	}
	else if (hsv.v == from.g) {
		hsv.h = 60.0f * ((from.b - from.r) / delta + 2.0f);
	}
	else {
		hsv.h = 60.0f * ((from.r - from.g) / delta + 4.0f);
	}

	hsv.s = hsv.v != 0 ? delta / hsv.v : 0;

	return hsv;
}

/// @endcond
