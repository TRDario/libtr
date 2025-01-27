#pragma once
#include "bitmap.hpp"
#include "handle.hpp"

namespace tr {
	class Texture;
	namespace ImGui {
		std::uint64_t getTextureID(const Texture& texture) noexcept;
	}
} // namespace tr

namespace tr {
	/** @ingroup graphics
	 *  @defgroup texture Textures
	 *  Texture classes and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Texture format types.
	 ******************************************************************************************************************/
	enum class TextureFormat {
		R8          = 0x8229,
		R8_SNORM    = 0x8F94,
		R16         = 0x822A,
		R16_SNORM   = 0x8F98,
		RG8         = 0x822B,
		RG8_SNORM   = 0x8F95,
		RG16        = 0x822C,
		RG16_SNORM  = 0x8F99,
		R3G3B2      = 0x2A10,
		RGB4        = 0x804F,
		RGB5        = 0x8050,
		RGB8        = 0x8051,
		RGB8_SNORM  = 0x8F96,
		RGB10       = 0x8052,
		RGB12       = 0x8053,
		RGB16       = 0x8054,
		RGB16_SNORM = 0x8F9A,
		RGBA2       = 0x8055,
		RGBA4       = 0x8056,
		RGB5A1      = 0x8057,
		RGBA8       = 0x8058,
		RGBA8_SNORM = 0x8F97,
		RGB10A2     = 0x8059,
		RGB10A2_UI  = 0x906F,
		RGBA12      = 0x805A,
		RGBA16      = 0x805B,
		SRGB8       = 0x8C41,
		SRGBA8      = 0x8C43,
		R_FP16      = 0x822D,
		RG_FP16     = 0x822F,
		RGB_FP16    = 0x881B,
		RGBA_FP16   = 0x881A,
		R_FP32      = 0x822E,
		RG_FP32     = 0x8230,
		RGB_FP32    = 0x8815,
		RGBA_FP32   = 0x8814,
		RG11B10_FP  = 0x8C3A,
		RGB9E5      = 0x8C3D,
		R_SI8       = 0x8231,
		R_UI8       = 0x8232,
		R_SI16      = 0x8233,
		R_UI16      = 0x8234,
		R_SI32      = 0x8235,
		R_UI32      = 0x8236,
		RGB_SI8     = 0x8D8F,
		RGB_UI8     = 0x8D7D,
		RGB_SI16    = 0x8D89,
		RGB_UI16    = 0x8D77,
		RGB_SI32    = 0x8D83,
		RGB_UI32    = 0x8D71,
		RGBA_SI8    = 0x8D8E,
		RGBA_UI8    = 0x8D7C,
		RGBA_SI16   = 0x8D88,
		RGBA_UI16   = 0x8D76,
		RGBA_SI32   = 0x8D82,
		RGBA_UI32   = 0x8D70,
		DEPTH16     = 0x81A5,
		DEPTH24     = 0x81A6,
		DEPTH_FP32  = 0x8CAC,
		STENCIL8    = 0x8D48
	};

	/******************************************************************************************************************
	 * Texture wrapping types.
	 ******************************************************************************************************************/
	enum class Wrap {
		/**************************************************************************************************************
		 * The texture is repeated.
		 **************************************************************************************************************/
		REPEAT = 0x2901,

		/**************************************************************************************************************
		 * The texture is repeated and mirrored.
		 **************************************************************************************************************/
		MIRROR_REPEAT = 0x8370,

		/**************************************************************************************************************
		 * The value of the edge pixel is used.
		 **************************************************************************************************************/
		EDGE_CLAMP = 0x812F,

		/**************************************************************************************************************
		 * The value of the border color is used.
		 **************************************************************************************************************/
		BORDER_CLAMP = 0x812D
	};

	/******************************************************************************************************************
	 * Minifying filter types.
	 ******************************************************************************************************************/
	enum class MinFilter {
		/**************************************************************************************************************
		 * The value of the texture element that is nearest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		NEAREST = 0x2600,

		/**************************************************************************************************************
		 * The average of the four texture elements that are closest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		LINEAR = 0x2601,

		/**************************************************************************************************************
		 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		 **************************************************************************************************************/
		NMIP_NEAREST = 0x2700,

		/**************************************************************************************************************
		 * Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		 **************************************************************************************************************/
		NMIP_LINEAR = 0x2702,

		/**************************************************************************************************************
		 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		 **************************************************************************************************************/
		LMIPS_NEAREST = 0x2701,

		/**************************************************************************************************************
		 * Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
		 **************************************************************************************************************/
		LMIPS_LINEAR = 0x2703
	};

	/******************************************************************************************************************
	 * Magnifying filter types.
	 ******************************************************************************************************************/
	enum class MagFilter {
		/**************************************************************************************************************
		 * The value of the texture element that is nearest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		NEAREST = 0x2600,

		/**************************************************************************************************************
		 * The average of the four texture elements that are closest to the specified texture coordinates is used.
		 **************************************************************************************************************/
		LINEAR = 0x2601
	};

	/******************************************************************************************************************
	 * Depth comparison functions.
	 ******************************************************************************************************************/
	enum class Compare {
		/**************************************************************************************************************
		 * The function always returns false.
		 **************************************************************************************************************/
		NEVER = 0x0200,

		/**************************************************************************************************************
		 * The function returns true if the value is less than the value being compared to.
		 **************************************************************************************************************/
		LESS = 0x0201,

		/**************************************************************************************************************
		 * The function returns true if the value is equal to the value being compared to.
		 **************************************************************************************************************/
		EQUAL = 0x0202,

		/**************************************************************************************************************
		 * The function returns true if the value is less than or equal to the value being compared to.
		 **************************************************************************************************************/
		LEQUAL = 0x0203,

		/**************************************************************************************************************
		 * The function returns true if the value is greater than the value being compared to.
		 **************************************************************************************************************/
		GREATER = 0x0204,

		/**************************************************************************************************************
		 * The function returns true if the value is not equal to the value being compared to.
		 **************************************************************************************************************/
		NEQUAL = 0x0205,

		/**************************************************************************************************************
		 * The function returns true if the value is greater than or equal to the value being compared to.
		 **************************************************************************************************************/
		GEQUAL = 0x0206,

		/**************************************************************************************************************
		 * The function always returns true.
		 **************************************************************************************************************/
		ALWAYS = 0x0207
	};

	/******************************************************************************************************************
	 * Texture swizzles.
	 ******************************************************************************************************************/
	enum class Swizzle {
		/**************************************************************************************************************
		 * The channel is set to 0.0.
		 **************************************************************************************************************/
		ZERO,

		/**************************************************************************************************************
		 * The channel is set to 1.0.
		 **************************************************************************************************************/
		ONE,

		/**************************************************************************************************************
		 * The channel will use the red channel value.
		 **************************************************************************************************************/
		R = 0x1903,

		/**************************************************************************************************************
		 * The channel will use the green channel value.
		 **************************************************************************************************************/
		G,

		/**************************************************************************************************************
		 * The channel will use the blue channel value.
		 **************************************************************************************************************/
		B,

		/**************************************************************************************************************
		 * The channel will use the alpha channel value.
		 **************************************************************************************************************/
		A
	};

	/******************************************************************************************************************
	 * Error thrown when a texture allocation failed.
	 ******************************************************************************************************************/
	struct TextureBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr const char* what() const noexcept override;
	};

	/******************************************************************************************************************
	 * Base texture class.
	 *
	 * @note This class cannot be constructed directly.
	 ******************************************************************************************************************/
	class Texture {
	  public:
		/**************************************************************************************************************
		 * Gets the format of the texture.
		 *
		 * @return The texture format.
		 **************************************************************************************************************/
		TextureFormat format() const noexcept;

		/**************************************************************************************************************
		 * Sets the swizzle parameters of the texture.
		 *
		 * @param[in] r, g, b, a The new swizzles.
		 **************************************************************************************************************/
		void setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept;

		/**************************************************************************************************************
		 * Sets the minifying filter used by the texture sampler.
		 *
		 * @param[in] filter The new minifying filter type.
		 **************************************************************************************************************/
		void setMinFilter(MinFilter filter) noexcept;

		/**************************************************************************************************************
		 * Sets the magnifying filter used by the texture sampler.
		 *
		 * @param[in] filter The new magnifying filter type.
		 **************************************************************************************************************/
		void setMagFilter(MagFilter filter) noexcept;

		/**************************************************************************************************************
		 * Disables the use of depth comparison.
		 **************************************************************************************************************/
		void disableComparison() noexcept;

		/**************************************************************************************************************
		 * Enables the use of depth comparison and sets the depth comparison operator of the texture sampler.
		 *
		 * @param[in] op The function to use for comparison.
		 **************************************************************************************************************/
		void setComparisonMode(Compare op) noexcept;

		/**************************************************************************************************************
		 * Sets the wrapping used for by the texture sampler.
		 *
		 * @param[in] wrap The new wrapping type.
		 **************************************************************************************************************/
		void setWrap(Wrap wrap) noexcept;

		/**************************************************************************************************************
		 * Sets the border color of the texture sampler (used when Wrap::BORDER_CLAMP is in use).
		 *
		 * @param[in] color The border color in floating point RGBA format.
		 **************************************************************************************************************/
		void setBorderColor(RGBAF color) noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the texture.
		 *
		 * @param[in] label The new label of the texture.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  protected:
		/// @cond IMPLEMENTATION
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;
		unsigned int                     _target;

		Texture(unsigned int target) noexcept;

		int width() const noexcept;
		int height() const noexcept;
		int depth() const noexcept;
		/// @endcond

		friend class BasicFramebuffer;
		friend class Framebuffer;
		friend class TextureUnit;
		friend class std::hash<Texture>;
		friend std::uint64_t ImGui::getTextureID(const Texture& texture) noexcept;
	};

	/******************************************************************************************************************
	 * One-dimensional texture.
	 ******************************************************************************************************************/
	struct Texture1D : Texture {
		/**************************************************************************************************************
		 * Allocates an uninitialized 1D texture.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture1D(int size, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Constructs a 1D texture with data uploaded from a bitmap.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] bitmap
		 * @parblock
		 * The bitmap data.
		 *
		 * @pre The bitmap must be 1 pixel tall.
		 * @endparblock
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture1D(SubBitmap bitmap, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		int size() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of the texture.
		 *
		 * @pre The region must fully be inside the bounds of the texture.
		 *
		 * @param[in] offset The starting offset within the texture.
		 * @param[in] bitmap
		 * @parblock
		 * The bitmap data to set the region so.
		 *
		 * @pre The bitmap must be 1 pixel tall.
		 * @endparblock
		 **************************************************************************************************************/
		void setRegion(int offset, SubBitmap bitmap) noexcept;
	};

	/******************************************************************************************************************
	 * One-dimensional array texture.
	 ******************************************************************************************************************/
	struct ArrayTexture1D : Texture {
		/**************************************************************************************************************
		 * Allocates an uninitialized 1D array texture.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] layers The number of layers in the texture.
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture1D(int size, int layers, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Constructs a 1D array texture with data uploaded from a bitmap.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] bitmap The bitmap data.
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture1D(SubBitmap bitmap, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		int size() const noexcept;

		/**************************************************************************************************************
		 * Gets the number of layers in the texture.
		 *
		 * @return The number of layers in the texture.
		 **************************************************************************************************************/
		int layers() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of the texture as if it were a 2D texture.
		 *
		 * @pre The region must fully be inside the bounds of the texture.
		 *
		 * @param[in] tl The top-left corner of the region within the texture.
		 * @param[in] bitmap The bitmap data to set the region to.
		 **************************************************************************************************************/
		void setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept;
	};

	/******************************************************************************************************************
	 * Two-dimensional texture.
	 ******************************************************************************************************************/
	struct Texture2D : Texture {
		/**************************************************************************************************************
		 * Allocates an uninitialized 2D texture.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture2D(glm::ivec2 size, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Constructs a 2D texture with data uploaded from a bitmap.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] bitmap The bitmap data.
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture2D(SubBitmap bitmap, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of the texture.
		 *
		 * @pre The region must fully be inside the bounds of the texture.
		 *
		 * @param[in] tl The top-left corner of the region within the texture.
		 * @param[in] bitmap The bitmap data to set the region to.
		 **************************************************************************************************************/
		void setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept;
	};

	/******************************************************************************************************************
	 * Two-dimensional array texture.
	 ******************************************************************************************************************/
	struct ArrayTexture2D : Texture {
		/**************************************************************************************************************
		 * Allocates an uninitialized 1D array texture.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] layers The number of layers in the texture.
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture2D(glm::ivec2 size, int layers, bool mipmapped = false,
					   TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Constructs a 2D array texture with data uploaded from a list of bitmaps.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] layers
		 * @parblock
		 * The layer bitmaps.
		 *
		 * @pre @em layers cannot be empty, and the bitmaps must all be of the same size.
		 * @endparblock
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture2D(std::span<SubBitmap> layers, bool mipmapped = false,
					   TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Gets the number of layers in the texture.
		 *
		 * @return The number of layers in the texture.
		 **************************************************************************************************************/
		int layers() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of a layer.
		 *
		 * @pre The region must fully be inside the bounds of the texture.
		 *
		 * @param[in] layer The layer whose region to set.
		 * @param[in] tl The top-left corner of the region within the layer.
		 * @param[in] bitmap The bitmap data to set the region to.
		 **************************************************************************************************************/
		void setLayerRegion(int layer, glm::ivec2 tl, SubBitmap bitmap) noexcept;
	};

	/******************************************************************************************************************
	 * Three-dimensional texture.
	 ******************************************************************************************************************/
	struct Texture3D : Texture {
		/**************************************************************************************************************
		 * Allocates an uninitialized 2D texture.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture3D(glm::ivec3 size, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Constructs a 3D texture with data uploaded from a list of bitmaps.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] layers
		 * @parblock
		 * The Z layer bitmaps.
		 *
		 * @pre @em layers cannot be empty, and the bitmaps must all be of the same size.
		 * @endparblock
		 * @param[in] mipmapped Whether the texture will be mipmapped.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture3D(std::span<SubBitmap> layers, bool mipmapped = false, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		glm::ivec3 size() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of a Z layer.
		 *
		 * @pre The region must fully be inside the bounds of the texture.
		 *
		 * @param[in] tl The top-left corner of the region within the texture.
		 * @param[in] bitmap The bitmap data to set the region to.
		 **************************************************************************************************************/
		void setLayerRegion(glm::ivec3 tl, SubBitmap bitmap) noexcept;
	};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <> struct std::hash<tr::Texture> {
	inline std::size_t operator()(const tr::Texture& texture) const noexcept;
};

template <> struct std::hash<tr::Texture1D> {
	inline std::size_t operator()(const tr::Texture1D& texture) const noexcept;
};

template <> struct std::hash<tr::ArrayTexture1D> {
	inline std::size_t operator()(const tr::ArrayTexture1D& texture) const noexcept;
};

template <> struct std::hash<tr::Texture2D> {
	inline std::size_t operator()(const tr::Texture2D& texture) const noexcept;
};

template <> struct std::hash<tr::ArrayTexture2D> {
	inline std::size_t operator()(const tr::ArrayTexture2D& texture) const noexcept;
};

template <> struct std::hash<tr::Texture3D> {
	inline std::size_t operator()(const tr::Texture3D& texture) const noexcept;
};

std::size_t std::hash<tr::Texture>::operator()(const tr::Texture& texture) const noexcept
{
	return std::hash<decltype(texture._id)>{}(texture._id);
}

std::size_t std::hash<tr::Texture1D>::operator()(const tr::Texture1D& texture) const noexcept
{
	return std::hash<tr::Texture>{}(texture);
}

std::size_t std::hash<tr::ArrayTexture1D>::operator()(const tr::ArrayTexture1D& texture) const noexcept
{
	return std::hash<tr::Texture>{}(texture);
}

std::size_t std::hash<tr::Texture2D>::operator()(const tr::Texture2D& texture) const noexcept
{
	return std::hash<tr::Texture>{}(texture);
}

std::size_t std::hash<tr::ArrayTexture2D>::operator()(const tr::ArrayTexture2D& texture) const noexcept
{
	return std::hash<tr::Texture>{}(texture);
}

std::size_t std::hash<tr::Texture3D>::operator()(const tr::Texture3D& texture) const noexcept
{
	return std::hash<tr::Texture>{}(texture);
}

constexpr const char* tr::TextureBadAlloc::what() const noexcept
{
	return "failed texture allocation";
}

/// @endcond
