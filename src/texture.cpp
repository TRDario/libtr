#include "../include/tr/texture.hpp"

#include "bitmap_to_gl_format.hpp"

#include <GL/glew.h>

namespace tr {
	// Determines the size of the array texture from a spam of bitmaps.
	glm::ivec2 determineArrayTextureSize(std::span<SubBitmap> layers);
}

glm::ivec2 tr::determineArrayTextureSize(std::span<SubBitmap> layers)
{
	assert(!layers.empty());
	assert(std::ranges::all_of(layers, [&](auto bitmap) { return bitmap.size() == layers[0].size(); }));
	return layers[0].size();
}

tr::Texture::Texture(unsigned int target) noexcept
	: _target {target}
{
	GLuint id;
	glCreateTextures(target, 1, &id);
	_id.reset(id);
}

void tr::Texture::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteTextures(1, &id);
}

bool tr::operator==(const Texture& l, const Texture& r) noexcept
{
	return l._id == r._id;
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
	std::array<int, 4> glSwizzle {int(r), int(g), int(b), int(a)};
	glTextureParameteriv(_id.get(), GL_TEXTURE_SWIZZLE_RGBA, glSwizzle.data());
}

void tr::Texture::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_TEXTURE, _id.get(), label.size(), label.data());
}

tr::Texture1D::Texture1D(int size, int mipmaps, TextureFormat format)
	: Texture {GL_TEXTURE_1D}
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
	auto [format, type] {bitmapToGLFormat(bitmap.format())};
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTextureSubImage1D(_id.get(), 0, offset, bitmap.size().x, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}

tr::ArrayTexture1D::ArrayTexture1D(int size, int layers, int mipmaps, TextureFormat format)
	: Texture {GL_TEXTURE_1D_ARRAY}
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
	: ArrayTexture1D {bitmap.size().x, bitmap.size().y, mipmaps, format}
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
	RectI2 bounds {size(), layers()};
	assert(bounds.contains(tl + bitmap.size()));
#endif
	auto [format, type] {bitmapToGLFormat(bitmap.format())};
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage2D(_id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}

tr::Texture2D::Texture2D(glm::ivec2 size, int mipmaps, TextureFormat format)
	: Texture {GL_TEXTURE_2D}
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
	: Texture2D {bitmap.size(), mipmaps, format}
{
	setRegion({0, 0}, bitmap);
}

glm::ivec2 tr::Texture2D::size() const noexcept
{
	return {Texture::width(), Texture::height()};
}

void tr::Texture2D::setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert(RectI2 {size()}.contains(tl + bitmap.size()));
	auto [format, type] {bitmapToGLFormat(bitmap.format())};
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage2D(_id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type, bitmap.data());
	glGenerateTextureMipmap(_id.get());
}

tr::ArrayTexture2D::ArrayTexture2D(glm::ivec2 size, int layers, int mipmaps, TextureFormat format)
	: Texture {GL_TEXTURE_2D_ARRAY}
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
	: ArrayTexture2D {determineArrayTextureSize(layers), int(layers.size()), mipmaps, format}
{
	for (int i = 0; i < layers.size(); ++i) {
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
	assert(RectI2 {size()}.contains(tl + bitmap.size()));
	auto [format, type] {bitmapToGLFormat(bitmap.format())};
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage3D(
		_id.get(),
		0,
		tl.x,
		tl.y,
		layer,
		bitmap.size().x,
		bitmap.size().y,
		1,
		format,
		type,
		bitmap.data()
	);
	glGenerateTextureMipmap(_id.get());
}

tr::Texture3D::Texture3D(glm::ivec3 size, int mipmaps, TextureFormat format)
	: Texture {GL_TEXTURE_3D}
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
	return {Texture::width(), Texture::height(), Texture::depth()};
}

void tr::Texture3D::setLayerRegion(glm::ivec3 tl, SubBitmap bitmap) noexcept
{
	assert(tl.z <= size().z);
	assert(RectI2 {glm::ivec2(size())}.contains(glm::ivec2(tl) + bitmap.size()));
	auto [format, type] {bitmapToGLFormat(bitmap.format())};
	glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	glTextureSubImage3D(
		_id.get(),
		0,
		tl.x,
		tl.y,
		tl.z,
		bitmap.size().x,
		bitmap.size().y,
		1,
		format,
		type,
		bitmap.data()
	);
	glGenerateTextureMipmap(_id.get());
}
