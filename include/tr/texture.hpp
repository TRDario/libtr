#pragma once
#include "bitmap.hpp"
#include "handle.hpp"

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
		friend std::uint64_t getImGUITextureID(const Texture& texture) noexcept;
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
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture1D(int size, int mipmaps, TextureFormat format = TextureFormat::RGBA8);

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
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture1D(int size, int layers, int mipmaps, TextureFormat format);

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
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture2D(glm::ivec2 size, int mipmaps, TextureFormat format);

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
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		ArrayTexture2D(glm::ivec2 size, int layers, int mipmaps, TextureFormat format);

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
		 * @param[in] mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param[in] format The internal format of the texture.
		 **************************************************************************************************************/
		Texture3D(glm::ivec3 size, int mipmaps, TextureFormat format);

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
