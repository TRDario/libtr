#pragma once
#include "bitmap.hpp"
#include "handle.hpp"

namespace tr {
	/** @addtogroup graphics
	 *  @{
	 */

	/******************************************************************************************************************
	 * Texture format types.
	 ******************************************************************************************************************/
	enum class TextureFormat {
		R8          = 0x82'29,
		R8_SNORM    = 0x8F'94,
		R16         = 0x82'2A,
		R16_SNORM   = 0x8F'98,
		RG8         = 0x82'2B,
		RG8_SNORM   = 0x8F'95,
		RG16        = 0x82'2C,
		RG16_SNORM  = 0x8F'99,
		R3G3B2      = 0x2A'10,
		RGB4        = 0x80'4F,
		RGB5        = 0x80'50,
		RGB8        = 0x80'51,
		RGB8_SNORM  = 0x8F'96,
		RGB10       = 0x80'52,
		RGB12       = 0x80'53,
		RGB16       = 0x80'54,
		RGB16_SNORM = 0x8F'9A,
		RGBA2       = 0x80'55,
		RGBA4       = 0x80'56,
		RGB5A1      = 0x80'57,
		RGBA8       = 0x80'58,
		RGBA8_SNORM = 0x8F'97,
		RGB10A2     = 0x80'59,
		RGB10A2_UI  = 0x90'6F,
		RGBA12      = 0x80'5A,
		RGBA16      = 0x80'5B,
		SRGB8       = 0x8C'41,
		SRGBA8      = 0x8C'43,
		R_FP16      = 0x82'2D,
		RG_FP16     = 0x82'2F,
		RGB_FP16    = 0x88'1B,
		RGBA_FP16   = 0x88'1A,
		R_FP32      = 0x82'2E,
		RG_FP32     = 0x82'30,
		RGB_FP32    = 0x88'15,
		RGBA_FP32   = 0x88'14,
		RG11B10_FP  = 0x8C'3A,
		RGB9E5      = 0x8C'3D,
		R_SI8       = 0x82'31,
		R_UI8       = 0x82'32,
		R_SI16      = 0x82'33,
		R_UI16      = 0x82'34,
		R_SI32      = 0x82'35,
		R_UI32      = 0x82'36,
		RGB_SI8     = 0x8D'8F,
		RGB_UI8     = 0x8D'7D,
		RGB_SI16    = 0x8D'89,
		RGB_UI16    = 0x8D'77,
		RGB_SI32    = 0x8D'83,
		RGB_UI32    = 0x8D'71,
		RGBA_SI8    = 0x8D'8E,
		RGBA_UI8    = 0x8D'7C,
		RGBA_SI16   = 0x8D'88,
		RGBA_UI16   = 0x8D'76,
		RGBA_SI32   = 0x8D'82,
		RGBA_UI32   = 0x8D'70,
		DEPTH16     = 0x81'A5,
		DEPTH24     = 0x81'A6,
		DEPTH_FP32  = 0x8C'AC,
		STENCIL8    = 0x8D'48
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
		R = 0x19'03,

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
	 * Sentinel value representing the number of layers for a texture with no mipmaps.
	 ******************************************************************************************************************/
	inline constexpr int NO_MIPMAPS{1};

	/******************************************************************************************************************
	 * Sentinel value representing the number of layers for a texture with all mipmaps.
	 ******************************************************************************************************************/
	inline constexpr int ALL_MIPMAPS{INT_MAX};

	/******************************************************************************************************************
	 * Error thrown when a texture allocation failed.
	 ******************************************************************************************************************/
	struct TextureBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
	};

	/******************************************************************************************************************
	 * Base texture class.
	 *
	 * This class cannot be constructed directly.
	 ******************************************************************************************************************/
	class Texture {
	  public:
		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		friend bool operator==(const Texture&, const Texture&) noexcept;

		/**************************************************************************************************************
		 * Gets the format of the texture.
		 *
		 * @return The texture format.
		 **************************************************************************************************************/
		TextureFormat format() const noexcept;

		/**************************************************************************************************************
		 * Gets the swizzle parameter for the red channel.
		 *
		 * @return The red channel swizzle.
		 **************************************************************************************************************/
		Swizzle swizzleR() const noexcept;

		/**************************************************************************************************************
		 * Gets the swizzle parameter for the green channel.
		 *
		 * @return The green channel swizzle.
		 **************************************************************************************************************/
		Swizzle swizzleG() const noexcept;

		/**************************************************************************************************************
		 * Gets the swizzle parameter for the blue channel.
		 *
		 * @return The blue channel swizzle.
		 **************************************************************************************************************/
		Swizzle swizzleB() const noexcept;

		/**************************************************************************************************************
		 * Gets the swizzle parameter for the alpha channel.
		 *
		 * @return The alpha channel swizzle.
		 **************************************************************************************************************/
		Swizzle swizzleA() const noexcept;

		/**************************************************************************************************************
		 * Sets the swizzle parameter for the red channel.
		 *
		 * @param[in] swizzle The new red channel swizzle.
		 **************************************************************************************************************/
		void setSwizzleR(Swizzle swizzle) noexcept;

		/**************************************************************************************************************
		 * Sets the swizzle parameter for the green channel.
		 *
		 * @param[in] swizzle The new green channel swizzle.
		 **************************************************************************************************************/
		void setSwizzleG(Swizzle swizzle) noexcept;

		/**************************************************************************************************************
		 * Sets the swizzle parameter for the blue channel.
		 *
		 * @param[in] swizzle The new blue channel swizzle.
		 **************************************************************************************************************/
		void setSwizzleB(Swizzle swizzle) noexcept;

		/**************************************************************************************************************
		 * Sets the swizzle parameter for the alpha channel.
		 *
		 * @param[in] swizzle The new alpha channel swizzle.
		 **************************************************************************************************************/
		void setSwizzleA(Swizzle swizzle) noexcept;

		/**************************************************************************************************************
		 * Sets the swizzle parameters of the texture.
		 *
		 * @param[in] r, g, b, a The new swizzles.
		 **************************************************************************************************************/
		void setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept;

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
	};

	/******************************************************************************************************************
	 * One-dimensional texture.
	 ******************************************************************************************************************/
	struct Texture1D : Texture {
		/**************************************************************************************************************
		 * Allocates an uninitialized 1D texture.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture1D(int size, int mipmaps, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Constructs a 1D texture with data uploaded from a bitmap.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] bitmap The bitmap data. The bitmap must be 1 pixel tall.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture1D(SubBitmap bitmap, int mipmaps, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		int size() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of the texture.
		 *
		 * The region must fully be inside the bounds of the texture.
		 *
		 * @param[in] offset The starting offset within the texture
		 * @param[in] bitmap The bitmap data to set the region to. The bitmap must be 1 pixel tall.
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
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] layers The number of layers in the texture.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture1D(int size, int layers, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
		 * Constructs a 1D array texture with data uploaded from a bitmap.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] bitmap The bitmap data.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture1D(SubBitmap bitmap, int mipmaps, TextureFormat format);

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
		 * The region must fully be inside the bounds of the texture.
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
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture2D(glm::ivec2 size, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
		 * Constructs a 2D texture with data uploaded from a bitmap.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] bitmap The bitmap data.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture2D(SubBitmap bitmap, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of the texture.
		 *
		 * The region must fully be inside the bounds of the texture.
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
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] layers The number of layers in the texture.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture2D(glm::ivec2 size, int layers, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
		 * Constructs a 2D array texture with data uploaded from a list of bitmaps.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] layers the layer bitmaps. The span cannot be empty, and the bitmaps must all be of the same size.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture2D(std::span<SubBitmap> layers, int mipmaps, TextureFormat format);

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
		 * The region must fully be inside the bounds of the texture.
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
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] size The size of the texture in texels.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture3D(glm::ivec3 size, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
		 * Constructs a 3D texture with data uploaded from a list of bitmaps.
		 *
		 * @exception TextureBadAlloc If allocating the texture fails.
		 *
		 * @param[in] layers the Z layer bitmaps. The span cannot be empty, and the bitmaps must all be of the same
		 *                   size.
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture3D(std::span<SubBitmap> layers, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
		 * Gets the size of the texture.
		 *
		 * @return The size of the texture in texels.
		 **************************************************************************************************************/
		glm::ivec3 size() const noexcept;

		/**************************************************************************************************************
		 * Sets a region of a Z layer.
		 *
		 * The region must fully be inside the bounds of the texture.
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
