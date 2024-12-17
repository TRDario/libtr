#pragma once
#include "color.hpp"
#include "handle.hpp"
#include <string_view>

namespace tr {
	/** @ingroup graphics
	 *  @defgroup sampler Sampler
	 *  Sampler class and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Texture wrapping types.
	 ******************************************************************************************************************/
	enum class Wrap {
		/**************************************************************************************************************
		 * The texture is repeated.
		 **************************************************************************************************************/
		REPEAT = 0x29'01,

		/**************************************************************************************************************
		 * The texture is repeated and mirrored.
		 **************************************************************************************************************/
		MIRROR_REPEAT = 0x83'70,

		/**************************************************************************************************************
		 * The value of the edge pixel is used.
		 **************************************************************************************************************/
		EDGE_CLAMP = 0x81'2F,

		/**************************************************************************************************************
		 * The value of the border color is used.
		 **************************************************************************************************************/
		BORDER_CLAMP = 0x81'2D
	};

	/******************************************************************************************************************
	 * Minifying filter types.
	 ******************************************************************************************************************/
	enum class MinFilter {
		/**************************************************************************************************************
		 * The value of the texture element that is nearest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		NEAREST = 0x26'00,

		/**************************************************************************************************************
		 * The average of the four texture elements that are closest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		LINEAR = 0x26'01,

		/**************************************************************************************************************
		 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		 **************************************************************************************************************/
		NMIP_NEAREST = 0x27'00,

		/**************************************************************************************************************
		 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		 **************************************************************************************************************/
		NMIP_LINEAR = 0x27'02,

		/**************************************************************************************************************
		 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		 **************************************************************************************************************/
		LMIPS_NEAREST = 0x27'01,

		/**************************************************************************************************************
		 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
		 **************************************************************************************************************/
		LMIPS_LINEAR = 0x27'03
	};

	/******************************************************************************************************************
	 * Magnifying filter types.
	 ******************************************************************************************************************/
	enum class MagFilter {
		/**************************************************************************************************************
		 * The value of the texture element that is nearest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		NEAREST = 0x26'00,

		/**************************************************************************************************************
		 * The average of the four texture elements that are closest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		LINEAR = 0x26'01
	};

	/******************************************************************************************************************
	 * Depth comparison functions.
	 ******************************************************************************************************************/
	enum class Compare {
		/**************************************************************************************************************
		 * The function always returns false.
		 **************************************************************************************************************/
		NEVER = 0x02'00,

		/**************************************************************************************************************
		 * The function returns true if the value is less than the value being compared to.
		 **************************************************************************************************************/
		LESS = 0x02'01,

		/**************************************************************************************************************
		 * The function returns true if the value is equal to the value being compared to.
		 **************************************************************************************************************/
		EQUAL = 0x02'02,

		/**************************************************************************************************************
		 * The function returns true if the value is less than or equal to the value being compared to.
		 **************************************************************************************************************/
		LEQUAL = 0x02'03,

		/**************************************************************************************************************
		 * The function returns true if the value is greater than the value being compared to.
		 **************************************************************************************************************/
		GREATER = 0x02'04,

		/**************************************************************************************************************
		 * The function returns true if the value is not equal to the value being compared to.
		 **************************************************************************************************************/
		NEQUAL = 0x02'05,

		/**************************************************************************************************************
		 * The function returns true if the value is greater than or equal to the value being compared to.
		 **************************************************************************************************************/
		GEQUAL = 0x02'06,

		/**************************************************************************************************************
		 * The function always returns true.
		 **************************************************************************************************************/
		ALWAYS = 0x02'07
	};

	/******************************************************************************************************************
	 * GPU Texture sampler.
	 ******************************************************************************************************************/
	class Sampler {
	  public:
		/**************************************************************************************************************
		 * Creates a sampler.
		 **************************************************************************************************************/
		Sampler() noexcept;

		friend bool operator==(const Sampler&, const Sampler&) noexcept = default;

		/**************************************************************************************************************
		 * Sets the minifying filter used by the sampler.
		 *
		 * @param[in] filter The new minifying filter type.
		 **************************************************************************************************************/
		void setMinFilter(MinFilter filter) noexcept;

		/**************************************************************************************************************
		 * Sets the magnifying filter used by the sampler.
		 *
		 * @param[in] filter The new magnifying filter type.
		 **************************************************************************************************************/
		void setMagFilter(MagFilter filter) noexcept;

		/**************************************************************************************************************
		 * Sets the minimum allowed level-of-detail parameter used by the sampler.
		 *
		 * @param[in] lod The new minimum LOD.
		 **************************************************************************************************************/
		void setMinLOD(int lod) noexcept;

		/**************************************************************************************************************
		 * Sets the maximum allowed level-of-detail parameter used by the sampler.
		 *
		 * @param[in] lod The new maximum LOD.
		 **************************************************************************************************************/
		void setMaxLOD(int lod) noexcept;

		/**************************************************************************************************************
		 * Disables the use of depth comparison.
		 **************************************************************************************************************/
		void disableComparison() noexcept;

		/**************************************************************************************************************
		 * Enables the use of depth comparison and sets the depth comparison operator of the sampler.
		 *
		 * @param[in] op The function to use for comparison.
		 **************************************************************************************************************/
		void setComparisonMode(Compare op) noexcept;

		/**************************************************************************************************************
		 * Sets the wrapping used for the s/x coordinate.
		 *
		 * @param[in] wrap The new wrapping type.
		 **************************************************************************************************************/
		void setWrapS(Wrap wrap) noexcept;

		/**************************************************************************************************************
		 * Sets the wrapping used for the t/y coordinate.
		 *
		 * @param[in] wrap The new wrapping type.
		 **************************************************************************************************************/
		void setWrapT(Wrap wrap) noexcept;

		/**************************************************************************************************************
		 * Sets the wrapping used for the r/z coordinate.
		 *
		 * @param[in] wrap The new wrapping type.
		 **************************************************************************************************************/
		void setWrapR(Wrap wrap) noexcept;

		/**************************************************************************************************************
		 * Sets the wrapping used for all coordinates.
		 *
		 * @param[in] wrap The new wrapping type.
		 **************************************************************************************************************/
		void setWrap(Wrap wrap) noexcept;

		/**************************************************************************************************************
		 * Sets the border color of the sampler (used when Wrap::BORDER_CLAMP is in use).
		 *
		 * @param[in] color The border color in floating point RGBA format.
		 **************************************************************************************************************/
		void setBorderColor(RGBAF color) noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the sampler.
		 *
		 * @param[in] label The new label of the sampler.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;

		friend class TextureUnit;
		friend class std::hash<Sampler>;
	};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <> struct std::hash<tr::Sampler> {
	inline std::size_t operator()(const tr::Sampler& sampler) const noexcept;
};

std::size_t std::hash<tr::Sampler>::operator()(const tr::Sampler& sampler) const noexcept
{
	return std::hash<decltype(sampler._id)>{}(sampler._id);
}

/// @endcond
