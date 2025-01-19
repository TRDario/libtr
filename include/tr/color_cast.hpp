#pragma once
#include "color.hpp"

namespace tr {
	/** @addtogroup color
	 *  @{
	 */

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
	template <class T> struct ColorCaster {};

	/******************************************************************************************************************
	 * Concept that denotes a type able to be color casted from.
	 *
	 * To fulfill this condition, ColorCaster<T> must have a static function @em toBuiltin taking a @em T
	 * and returning a builtin color type.
	 *******************************************************************************************************************/
	template <class T>
	concept ColorCastableFrom = BuiltinColor<std::remove_cvref_t<ReturnTypeT<decltype(ColorCaster<T>::toBuiltin)>>>;

	/******************************************************************************************************************
	 * Concept that denotes a type able to be color casted to.
	 *
	 * To fulfill this condition, ColorCaster<T> must have a static function @em fromBuiltin taking a BuiltinColor
	 * and returning <em>T</em>.
	 ******************************************************************************************************************/
	template <class T>
	concept ColorCastableTo = BuiltinColor<std::remove_cvref_t<ArgumentTypeT<decltype(ColorCaster<T>::fromBuiltin)>>> &&
							  std::same_as<std::remove_cvref_t<ReturnTypeT<decltype(ColorCaster<T>::fromBuiltin)>>, T>;

	/******************************************************************************************************************
	 * Converts a red channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of R.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<R> From> constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts a green channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of G.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<G> From> constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts a blue channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of B.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<B> From> constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts an RG channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RG.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To, SpecializationOf<RG> From> constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts a three-channel (RGB/BGR) channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RGB or BGR.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To, class From>
		requires SpecializationOf<From, RGB> || SpecializationOf<From, BGR>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts a four-channel (RGBA/BGRA) channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RGBA or BGRA.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To, class From>
		requires SpecializationOf<From, RGBA> || SpecializationOf<From, BGRA>
	constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts a RGB_Ui8_2_3_3 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const RGB_Ui8_2_3_3& from) noexcept;

	/******************************************************************************************************************
	 * Converts a RGB_Ui16_5_6_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const RGB_Ui16_5_6_5& from) noexcept;

	/******************************************************************************************************************
	 * Converts a BGR_Ui8_3_3_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const BGR_Ui8_3_3_2& from) noexcept;

	/******************************************************************************************************************
	 * Converts a BGR_Ui16_5_6_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const BGR_Ui16_5_6_5& from) noexcept;

	/******************************************************************************************************************
	 * Converts a RGBA_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const RGBA_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
	 * Converts a RGBA_Ui16_5_5_5_1 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const RGBA_Ui16_5_5_5_1& from) noexcept;

	/******************************************************************************************************************
	 * Converts a RGBA_Ui32_10_10_10_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const RGBA_Ui32_10_10_10_2& from) noexcept;

	/******************************************************************************************************************
	 * Converts a ABGR_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const ABGR_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
	 * Converts a ABGR_Ui16_1_5_5_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const ABGR_Ui16_1_5_5_5& from) noexcept;

	/******************************************************************************************************************
	 * Converts a ABGR_Ui32_2_10_10_10 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const ABGR_Ui32_2_10_10_10& from) noexcept;

	/******************************************************************************************************************
	 * Converts a BGRA_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const BGRA_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
	 * Converts a BGRA_Ui16_5_5_5_1 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const BGRA_Ui16_5_5_5_1& from) noexcept;

	/******************************************************************************************************************
	 * Converts a BGRA_Ui32_10_10_10_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const BGRA_Ui32_10_10_10_2& from) noexcept;

	/******************************************************************************************************************
	 * Converts a ARGB_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const ARGB_Ui16_4_4_4_4& from) noexcept;

	/******************************************************************************************************************
	 * Converts a ARGB_Ui16_1_5_5_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const ARGB_Ui16_1_5_5_5& from) noexcept;

	/******************************************************************************************************************
	 * Converts a ARGB_Ui32_2_10_10_10 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To> constexpr To color_cast(const ARGB_Ui32_2_10_10_10& from) noexcept;

	/******************************************************************************************************************
	 * Converts a custom castable color type to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A type with a specialized ColorCaster.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <BuiltinColor To, ColorCastableFrom From> constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts a built-in color to a custom castable color type.
	 *
	 * @tparam To A type with a specialized ColorCaster.
	 * @tparam From A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <ColorCastableTo To, BuiltinColor From> constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Converts a custom castable color type to another custom castable color type.
	 *
	 * @tparam To A type with a specialized ColorCaster.
	 * @tparam From A type with a specialized ColorCaster.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <ColorCastableTo To, ColorCastableFrom From> constexpr To color_cast(const From& from) noexcept;

	/******************************************************************************************************************
	 * Specialization of ColorCaster for HSV.
	 ******************************************************************************************************************/
	template <> struct ColorCaster<HSV> {
		/******************************************************************************************************************
		 * Casts an HSV value to a floating point RGBA value.
		 *
		 * @param[in] from The HSV value to be converted.
		 *
		 * @return The corresponding RGBAF value.
		 ******************************************************************************************************************/
		static constexpr RGBAF toBuiltin(const HSV& from) noexcept;

		/******************************************************************************************************************
		 * Casts an RGBAF value to an HSV value.
		 *
		 * @param[in] from The RGBAF value to be converted.
		 *
		 * @return The corresponding HSV value.
		 ******************************************************************************************************************/
		static constexpr HSV fromBuiltin(const RGBAF& from) noexcept;
	};

	/// @}
} // namespace tr

#include "color_cast_impl.hpp"