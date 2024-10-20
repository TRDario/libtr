/**
 * @file texture.cppm
 * @brief Provides texture functionality.
 */

module;
#include <cassert>
#include <magic_enum/magic_enum.hpp>
#include <GL/glew.h>

export module tr:texture;

import std;
import glm;
import :bitmap_format;
import :bitmap;
import :bitmap_to_gl_format;
import :geometry;
import :handle;

export namespace tr {
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
		R_FP16 	    = 0x822D,
		RG_FP16     = 0x822F,
		RGB_FP16    = 0x881B,
		RGBA_FP16   = 0x881A,
		R_FP32 	    = 0x822E,
		RG_FP32     = 0x8230,
		RGB_FP32    = 0x8815,
		RGBA_FP32   = 0x8814,
		RG11B10_FP  = 0x8C3A,
		RGB9E5 	    = 0x8C3D,
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
	inline constexpr int NO_MIPMAPS { 1 };

	/******************************************************************************************************************
	 * Sentinel value representing the number of layers for a texture with all mipmaps.
	 ******************************************************************************************************************/
	inline constexpr int ALL_MIPMAPS { INT_MAX };


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
		 * @param swizzle The new red channel swizzle.
	     **************************************************************************************************************/
		void setSwizzleR(Swizzle swizzle) noexcept;
		
		/**************************************************************************************************************
         * Sets the swizzle parameter for the green channel.
		 *
		 * @param swizzle The new green channel swizzle.
	     **************************************************************************************************************/
		void setSwizzleG(Swizzle swizzle) noexcept;
		
		/**************************************************************************************************************
         * Sets the swizzle parameter for the blue channel.
		 *
		 * @param swizzle The new blue channel swizzle.
	     **************************************************************************************************************/
		void setSwizzleB(Swizzle swizzle) noexcept;
		
		/**************************************************************************************************************
         * Sets the swizzle parameter for the alpha channel.
		 *
		 * @param swizzle The new alpha channel swizzle.
	     **************************************************************************************************************/
		void setSwizzleA(Swizzle swizzle) noexcept;
		
		/**************************************************************************************************************
         * Sets the swizzle parameters of the texture.
		 *
		 * @param r, g, b, a The new swizzles.
	     **************************************************************************************************************/
		void setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept;


		/**************************************************************************************************************
	     * Sets the debug label of the texture.
         *
         * @param label The new label of the texture.
	     **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;
	protected:
		/// @cond IMPLEMENTATION
		struct Deleter { void operator()(GLuint id) noexcept; /**< @private */ };
		Handle<GLuint, 0, Deleter> _id;
		GLenum 					   _target;

		Texture(GLenum target) noexcept;

		int width() const noexcept;
		int height() const noexcept;
		int depth() const noexcept;
		/// @endcond


		friend class BasicFramebuffer;
		friend class Framebuffer;
		friend class TextureUnit;
	};

	/******************************************************************************************************************
	 * One-dimensional texture.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	struct Texture1D : Texture {
		/**************************************************************************************************************
         * Allocates an uninitialized 1D texture.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param size The size of the texture in texels.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
	     **************************************************************************************************************/
		Texture1D(int size, int mipmaps, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
         * Constructs a 1D texture with data uploaded from a bitmap.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param bitmap The bitmap data. The bitmap must be 1 pixel tall, otherwise a failed assertion may be triggered.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
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
		 * Trying to set a region outside the bounds of the texture may trigger a failed assertion.
		 *
		 * @param offset The starting offset within the texture
		 * @param bitmap The bitmap data to set the region to. The bitmap must be 1 pixel tall, otherwise a failed
		 *               assertion may be triggered.
	     **************************************************************************************************************/
		void setRegion(int offset, SubBitmap bitmap) noexcept;
	};
	
	/******************************************************************************************************************
	 * One-dimensional array texture.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	struct ArrayTexture1D : Texture {
		/**************************************************************************************************************
         * Allocates an uninitialized 1D array texture.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param size The size of the texture in texels.
		 * @param layers The number of layers in the texture.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
	     **************************************************************************************************************/
		ArrayTexture1D(int size, int layers, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
         * Constructs a 1d array texture with data uploaded from a bitmap.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param bitmap The bitmap data.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
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
		 * Trying to set a region outside the bounds of the texture may trigger a failed assertion.
		 *
		 * @param tl The top-left corner of the region within the texture.
		 * @param bitmap The bitmap data to set the region to.
	     **************************************************************************************************************/
		void setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept;
	};

	/******************************************************************************************************************
	 * Two-dimensional texture.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	struct Texture2D : Texture {
		/**************************************************************************************************************
         * Allocates an uninitialized 2D texture.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param size The size of the texture in texels.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
	     **************************************************************************************************************/
		Texture2D(glm::ivec2 size, int mipmaps, TextureFormat format);
		
		/**************************************************************************************************************
         * Constructs a 2D texture with data uploaded from a bitmap.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param bitmap The bitmap data.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
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
		 * Trying to set a region outside the bounds of the texture may trigger a failed assertion.
		 *
		 * @param tl The top-left corner of the region within the texture.
		 * @param bitmap The bitmap data to set the region to.
	     **************************************************************************************************************/
		void setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept;
	};

	/******************************************************************************************************************
	 * Two-dimensional array texture.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	struct ArrayTexture2D : Texture {
		/**************************************************************************************************************
         * Allocates an uninitialized 1D array texture.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param size The size of the texture in texels.
		 * @param layers The number of layers in the texture.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
	     **************************************************************************************************************/
		ArrayTexture2D(glm::ivec2 size, int layers, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
         * Constructs a 2D array texture with data uploaded from a list of bitmaps.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param layers the layer bitmaps. The span cannot be empty, and the bitmaps must all be of the same size,
		 *               otherwise a failed assertion may be triggered.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
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
		 * Trying to set a region outside the bounds of the texture may trigger a failed assertion.
		 *
		 * @param layer The layer whose region to set.
		 * @param tl The top-left corner of the region within the layer.
		 * @param bitmap The bitmap data to set the region to.
	     **************************************************************************************************************/
		void setLayerRegion(int layer, glm::ivec2 tl, SubBitmap bitmap) noexcept;
	};
	
	/******************************************************************************************************************
	 * Three-dimensional texture.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	struct Texture3D : Texture {
		/**************************************************************************************************************
         * Allocates an uninitialized 2D texture.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param size The size of the texture in texels.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
	     **************************************************************************************************************/
		Texture3D(glm::ivec3 size, int mipmaps, TextureFormat format);

		/**************************************************************************************************************
         * Constructs a 3D texture with data uploaded from a list of bitmaps.
		 *
		 * @exception TextureBadAlloc If allocating the texture failed.
		 *
		 * @param layers the Z layer bitmaps. The span cannot be empty, and the bitmaps must all be of the same size,
		 *               otherwise a failed assertion may be triggered.
		 * @param mipmaps The number of mipmaps to generate. Special values: NO_MIPMAPS, ALL_MIPMAPS.
		 * @param format The internal format of the texture.
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
		 * Trying to set a region outside the bounds of the texture may trigger a failed assertion.
		 *
		 * @param tl The top-left corner of the region within the texture.
		 * @param bitmap The bitmap data to set the region to.
	     **************************************************************************************************************/
		void setLayerRegion(glm::ivec3 tl, SubBitmap bitmap) noexcept;
	};
}

/// @cond IMPLEMENTATION

namespace tr {
	// Determines the size of the array texture from a spam of bitmaps.
	glm::ivec2 determineArrayTextureSize(std::span<SubBitmap> layers);
}

glm::ivec2 tr::determineArrayTextureSize(std::span<SubBitmap> layers)
{
	assert(!layers.empty());
	assert(std::ranges::all_of(layers, [&] (auto bitmap) { return bitmap.size() == layers[0].size(); }));
	return layers[0].size();
}

constexpr const char* tr::TextureBadAlloc::what() const noexcept {
	return "failed texture allocation";
}

tr::Texture::Texture(GLenum target) noexcept
	: _target { target }
{
	GLuint id;
	glCreateTextures(target, 1, &id);
	_id.reset(id);
}

void tr::Texture::Deleter::operator()(GLuint id) noexcept
{
	glDeleteTextures(1, &id);
}

bool tr::operator==(const Texture& lhs, const Texture& rhs) noexcept
{
	return lhs._id == rhs._id;
}

tr::TextureFormat tr::Texture::format() const noexcept
{
	int glFormat;
	glGetTextureLevelParameteriv(_id.get(), 0, GL_TEXTURE_INTERNAL_FORMAT, &glFormat);
	return TextureFormat(glFormat);
}

int tr::Texture::width() const noexcept
{
	int width;
	glGetTextureLevelParameteriv(_id.get(), 0, GL_TEXTURE_WIDTH, &width);
	return width;
}

int tr::Texture::height() const noexcept
{
	int height;
	glGetTextureLevelParameteriv(_id.get(), 0, GL_TEXTURE_HEIGHT, &height);
	return height;
}

int tr::Texture::depth() const noexcept
{
	int depth;
	glGetTextureLevelParameteriv(_id.get(), 0, GL_TEXTURE_DEPTH, &depth);
	return depth;
}

tr::Swizzle tr::Texture::swizzleR() const noexcept
{
	int glSwizzle;
	glGetTextureParameteriv(_id.get(), GL_TEXTURE_SWIZZLE_R, &glSwizzle);
	return Swizzle(glSwizzle);
}

tr::Swizzle tr::Texture::swizzleG() const noexcept
{
	int glSwizzle;
	glGetTextureParameteriv(_id.get(), GL_TEXTURE_SWIZZLE_G, &glSwizzle);
	return Swizzle(glSwizzle);
}

tr::Swizzle tr::Texture::swizzleB() const noexcept
{
	int glSwizzle;
	glGetTextureParameteriv(_id.get(), GL_TEXTURE_SWIZZLE_B, &glSwizzle);
	return Swizzle(glSwizzle);
}

tr::Swizzle tr::Texture::swizzleA() const noexcept
{
	int glSwizzle;
	glGetTextureParameteriv(_id.get(), GL_TEXTURE_SWIZZLE_A, &glSwizzle);
	return Swizzle(glSwizzle);
}

void tr::Texture::setSwizzleR(Swizzle swizzle) noexcept
{
	glTextureParameteri(_id.get(), GL_TEXTURE_SWIZZLE_R, int(swizzle));
}

void tr::Texture::setSwizzleG(Swizzle swizzle) noexcept
{
	glTextureParameteri(_id.get(), GL_TEXTURE_SWIZZLE_G, int(swizzle));
}

void tr::Texture::setSwizzleB(Swizzle swizzle) noexcept
{
	glTextureParameteri(_id.get(), GL_TEXTURE_SWIZZLE_B, int(swizzle));
}

void tr::Texture::setSwizzleA(Swizzle swizzle) noexcept
{
	glTextureParameteri(_id.get(), GL_TEXTURE_SWIZZLE_A, int(swizzle));
}

void tr::Texture::setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept
{
	std::array<int, 4> glSwizzle { int(r), int(g), int(b), int(a) };
	glTextureParameteriv(_id.get(), GL_TEXTURE_SWIZZLE_RGBA, glSwizzle.data());
}

void tr::Texture::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_TEXTURE, _id.get(), label.size(), label.data());
}

tr::Texture1D::Texture1D(int size, int mipmaps, TextureFormat format)
	: Texture { GL_TEXTURE_1D }
{
	assert(size > 0);
	assert(mipmaps > 0);
	mipmaps = mipmaps == ALL_MIPMAPS ? std::floor(std::log2(size)) + 1 : mipmaps;

	glTextureStorage1D(_id.get(), mipmaps, GLenum(format), size);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc {};
	}
}

int tr::Texture1D::size() const noexcept
{
	return Texture::width();
}

void tr::Texture1D::setRegion(int offset, SubBitmap bitmap) noexcept
{
	assert(bitmap.size().y == 1);
	assert(offset + bitmap.size().x <= size());
	auto [format, type] { bitmapToGLFormat(bitmap.format()) };
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTextureSubImage1D(_id.get(), 0, offset, bitmap.size().x, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}

tr::ArrayTexture1D::ArrayTexture1D(int size, int layers, int mipmaps, TextureFormat format)
	: Texture { GL_TEXTURE_1D_ARRAY }
{
	assert(size > 0 && layers > 0);
	assert(mipmaps > 0);
	mipmaps = mipmaps == ALL_MIPMAPS ? std::floor(std::log2(size)) + 1 : mipmaps;

	glTextureStorage2D(_id.get(), mipmaps, GLenum(format), size, layers);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc {};
	}
}

tr::ArrayTexture1D::ArrayTexture1D(SubBitmap bitmap, int mipmaps, TextureFormat format)
	: ArrayTexture1D { bitmap.size().x, bitmap.size().y, mipmaps, format }
{
	setRegion({ 0, 0 }, bitmap);
}

int tr::ArrayTexture1D::size() const noexcept
{
	return Texture::width();
}

int tr::ArrayTexture1D::layers() const noexcept
{
	return Texture::height();
}

void tr::ArrayTexture1D::setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert((RectI2 { { size(), layers() } }.contains(tl + bitmap.size())));
	auto [format, type] { bitmapToGLFormat(bitmap.format()) };
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage2D(_id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}


tr::Texture2D::Texture2D(glm::ivec2 size, int mipmaps, TextureFormat format)
	: Texture { GL_TEXTURE_2D }
{
	assert(size.x > 0 && size.y > 0);
	assert(mipmaps > 0);
	mipmaps = mipmaps == ALL_MIPMAPS ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : mipmaps;

	glTextureStorage2D(_id.get(), mipmaps, std::uint32_t(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc {};
	}
}

tr::Texture2D::Texture2D(SubBitmap bitmap, int mipmaps, TextureFormat format)
	: Texture2D { bitmap.size(), mipmaps, format }
{
	setRegion({ 0, 0 }, bitmap);
}

glm::ivec2 tr::Texture2D::size() const noexcept
{
	return { Texture::width(), Texture::height() };
}

void tr::Texture2D::setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert(RectI2 { size() }.contains(tl + bitmap.size()));
	auto [format, type] { bitmapToGLFormat(bitmap.format()) };
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage2D(_id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}


tr::ArrayTexture2D::ArrayTexture2D(glm::ivec2 size, int layers, int mipmaps, TextureFormat format)
	: Texture { GL_TEXTURE_2D_ARRAY }
{
	assert(size.x > 0 && size.y > 0 && layers > 0);
	assert(mipmaps > 0);
	mipmaps = mipmaps == ALL_MIPMAPS ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : mipmaps;

	glTextureStorage3D(_id.get(), mipmaps, std::uint32_t(format), size.x, size.y, layers);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc {};
	}
}

tr::ArrayTexture2D::ArrayTexture2D(std::span<SubBitmap> layers, int mipmaps, TextureFormat format)
	: ArrayTexture2D { determineArrayTextureSize(layers), int(layers.size()), mipmaps, format }
{
	for (int i = 0; i < layers.size(); ++i) {
		setLayerRegion(i, {}, layers[i]);
	}
}

glm::ivec2 tr::ArrayTexture2D::size() const noexcept
{
	return { Texture::width(), Texture::height() };
}

int tr::ArrayTexture2D::layers() const noexcept
{
	return Texture::depth();
}

void tr::ArrayTexture2D::setLayerRegion(int layer, glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert(layer <= layers());
	assert(RectI2 { size() }.contains(tl + bitmap.size()));
	auto [format, type] { bitmapToGLFormat(bitmap.format()) };
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage3D(_id.get(), 0, tl.x, tl.y, layer, bitmap.size().x, bitmap.size().y, 1, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}

tr::Texture3D::Texture3D(glm::ivec3 size, int mipmaps, TextureFormat format)
	: Texture { GL_TEXTURE_3D }
{
	assert(size.x > 0 && size.y > 0 && size.z > 0);
	assert(mipmaps > 0);
	mipmaps = mipmaps == ALL_MIPMAPS ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : mipmaps;

	glTextureStorage3D(_id.get(), mipmaps, std::uint32_t(format), size.x, size.y, size.z);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc {};
	}
}

glm::ivec3 tr::Texture3D::size() const noexcept
{
	return { Texture::width(), Texture::height(), Texture::depth() };
}

void tr::Texture3D::setLayerRegion(glm::ivec3 tl, SubBitmap bitmap) noexcept
{
	assert(tl.z <= size().z);
	assert(RectI2 { glm::ivec2(size()) }.contains(glm::ivec2(tl) + bitmap.size()));
	auto [format, type] { bitmapToGLFormat(bitmap.format()) };
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage3D(_id.get(), 0, tl.x, tl.y, tl.z, bitmap.size().x, bitmap.size().y, 1, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}

/// @endcond