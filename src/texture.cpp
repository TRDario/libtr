#include "../include/tr/texture.hpp"
#include "bitmap_to_gl_format.hpp"
#include "gl_call.hpp"

namespace tr {
	// Determines the size of the array texture from a spam of bitmaps.
	glm::ivec2 determineArrayTextureSize(std::span<SubBitmap> layers);
} // namespace tr

glm::ivec2 tr::determineArrayTextureSize(std::span<SubBitmap> layers)
{
	assert(!layers.empty());
	assert(std::ranges::all_of(layers, [&](auto& bitmap) { return bitmap.size() == layers[0].size(); }));
	return layers[0].size();
}

tr::Texture::Texture(unsigned int target) noexcept
	: _target{target}
{
	GLuint id;
	TR_GL_CALL(glCreateTextures, target, 1, &id);
	_id.reset(id);
}

void tr::Texture::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteTextures(1, &id);
}

tr::TextureFormat tr::Texture::format() const noexcept
{
	int glFormat;
	TR_GL_CALL(glGetTextureLevelParameteriv, _id.get(), 0, GL_TEXTURE_INTERNAL_FORMAT, &glFormat);
	return static_cast<TextureFormat>(glFormat);
}

int tr::Texture::width() const noexcept
{
	int width;
	TR_GL_CALL(glGetTextureLevelParameteriv, _id.get(), 0, GL_TEXTURE_WIDTH, &width);
	return width;
}

int tr::Texture::height() const noexcept
{
	int height;
	TR_GL_CALL(glGetTextureLevelParameteriv, _id.get(), 0, GL_TEXTURE_HEIGHT, &height);
	return height;
}

int tr::Texture::depth() const noexcept
{
	int depth;
	TR_GL_CALL(glGetTextureLevelParameteriv, _id.get(), 0, GL_TEXTURE_DEPTH, &depth);
	return depth;
}

void tr::Texture::setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept
{
	std::array<int, 4> glSwizzle{static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a)};
	TR_GL_CALL(glTextureParameteriv, _id.get(), GL_TEXTURE_SWIZZLE_RGBA, glSwizzle.data());
}

void tr::Texture::setMinFilter(MinFilter filter) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
}

void tr::Texture::setMagFilter(MagFilter filter) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
}

void tr::Texture::setMinLOD(int lod) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MIN_LOD, lod);
}

void tr::Texture::setMaxLOD(int lod) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MAX_LOD, lod);
}

void tr::Texture::disableComparison() noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_COMPARE_MODE, GL_NONE);
}

void tr::Texture::setComparisonMode(Compare op) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(op));
}

void tr::Texture::setWrap(Wrap wrap) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap));
}

void tr::Texture::setBorderColor(RGBAF color) noexcept
{
	TR_GL_CALL(glTextureParameterfv, _id.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::Texture::setLabel(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_TEXTURE, _id.get(), label.size(), label.data());
}

tr::Texture1D::Texture1D(int size, bool mipmapped, TextureFormat format)
	: Texture{GL_TEXTURE_1D}
{
	assert(size > 0);

	TR_GL_CALL(glTextureStorage1D, _id.get(), mipmapped ? std::floor(std::log2(size)) + 1 : 1,
			   static_cast<GLenum>(format), size);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::Texture1D::Texture1D(SubBitmap bitmap, bool mipmapped, TextureFormat format)
	: Texture1D{bitmap.size().x, mipmapped, format}
{
	setRegion(0, bitmap);
}

int tr::Texture1D::size() const noexcept
{
	return Texture::width();
}

void tr::Texture1D::setRegion(int offset, SubBitmap bitmap) noexcept
{
	assert(bitmap.size().y == 1);
	assert(offset + bitmap.size().x <= size());

	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, 0);
	TR_GL_CALL(glTextureSubImage1D, _id.get(), 0, offset, bitmap.size().x, format, type, bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::ArrayTexture1D::ArrayTexture1D(int size, int layers, bool mipmapped, TextureFormat format)
	: Texture{GL_TEXTURE_1D_ARRAY}
{
	assert(size > 0 && layers > 0);

	TR_GL_CALL(glTextureStorage2D, _id.get(), mipmapped ? std::floor(std::log2(size)) + 1 : 1,
			   static_cast<GLenum>(format), size, layers);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::ArrayTexture1D::ArrayTexture1D(SubBitmap bitmap, bool mipmapped, TextureFormat format)
	: ArrayTexture1D{bitmap.size().x, bitmap.size().y, mipmapped, format}
{
	setRegion({0, 0}, bitmap);
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
#ifndef NDEBUG
	RectI2 bounds{{size(), layers()}};
	assert(bounds.contains(tl + bitmap.size()));
#endif

	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage2D, _id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::Texture2D::Texture2D(glm::ivec2 size, bool mipmapped, TextureFormat format)
	: Texture{GL_TEXTURE_2D}
{
	assert(size.x > 0 && size.y > 0);

	glTextureStorage2D(_id.get(), mipmapped ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : 1,
					   static_cast<GLenum>(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::Texture2D::Texture2D(SubBitmap bitmap, bool mipmapped, TextureFormat format)
	: Texture2D{bitmap.size(), mipmapped, format}
{
	setRegion({0, 0}, bitmap);
}

glm::ivec2 tr::Texture2D::size() const noexcept
{
	return {Texture::width(), Texture::height()};
}

void tr::Texture2D::setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert(RectI2{size()}.contains(tl + bitmap.size()));
	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage2D, _id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::ArrayTexture2D::ArrayTexture2D(glm::ivec2 size, int layers, bool mipmapped, TextureFormat format)
	: Texture{GL_TEXTURE_2D_ARRAY}
{
	assert(size.x > 0 && size.y > 0 && layers > 0);

	TR_GL_CALL(glTextureStorage3D, _id.get(), mipmapped ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : 1,
			   static_cast<GLenum>(format), size.x, size.y, layers);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::ArrayTexture2D::ArrayTexture2D(std::span<SubBitmap> layers, bool mipmapped, TextureFormat format)
	: ArrayTexture2D{determineArrayTextureSize(layers), static_cast<int>(layers.size()), mipmapped, format}
{
	for (std::size_t i = 0; i < layers.size(); ++i) {
		setLayerRegion(i, {}, layers[i]);
	}
}

glm::ivec2 tr::ArrayTexture2D::size() const noexcept
{
	return {Texture::width(), Texture::height()};
}

int tr::ArrayTexture2D::layers() const noexcept
{
	return Texture::depth();
}

void tr::ArrayTexture2D::setLayerRegion(int layer, glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert(layer <= layers());
	assert(RectI2{size()}.contains(tl + bitmap.size()));
	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage3D, _id.get(), 0, tl.x, tl.y, layer, bitmap.size().x, bitmap.size().y, 1, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::Texture3D::Texture3D(glm::ivec3 size, bool mipmapped, TextureFormat format)
	: Texture{GL_TEXTURE_3D}
{
	assert(size.x > 0 && size.y > 0 && size.z > 0);

	TR_GL_CALL(glTextureStorage3D, _id.get(), mipmapped ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : 1,
			   static_cast<GLenum>(format), size.x, size.y, size.z);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

glm::ivec3 tr::Texture3D::size() const noexcept
{
	return {Texture::width(), Texture::height(), Texture::depth()};
}

void tr::Texture3D::setLayerRegion(glm::ivec3 tl, SubBitmap bitmap) noexcept
{
	assert(tl.z <= size().z);
	assert(RectI2{static_cast<glm::ivec2>(size())}.contains(static_cast<glm::ivec2>(tl) + bitmap.size()));
	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage3D, _id.get(), 0, tl.x, tl.y, tl.z, bitmap.size().x, bitmap.size().y, 1, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}
