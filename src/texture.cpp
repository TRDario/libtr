#include "../include/tr/bitmap.hpp"
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

void tr::Texture::setMinFilter(MinFilter filter) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
}

void tr::Texture::setMagFilter(MagFilter filter) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
}

void tr::Texture::setWrap(Wrap wrap) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap));
}

void tr::Texture::setLabel(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_TEXTURE, _id.get(), label.size(), label.data());
}

void tr::Texture::copyRegion(const glm::ivec3& offset, const Texture& src, const RectI3& rect) noexcept
{
	TR_GL_CALL(glCopyImageSubData, src._id.get(), src._target, 0, rect.tl.x, rect.tl.y, rect.tl.z, _id.get(), _target,
			   0, offset.x, offset.y, offset.z, rect.size.x, rect.size.y, rect.size.z);
}

void tr::ColorTexture::setSwizzle(Swizzle r, Swizzle g, Swizzle b, Swizzle a) noexcept
{
	std::array<int, 4> glSwizzle{static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a)};
	TR_GL_CALL(glTextureParameteriv, _id.get(), GL_TEXTURE_SWIZZLE_RGBA, glSwizzle.data());
}

void tr::ColorTexture::setBorderColor(RGBAF color) noexcept
{
	TR_GL_CALL(glTextureParameterfv, _id.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::ColorTexture::clear(const RGBAF& color) noexcept
{
	TR_GL_CALL(glClearTexImage, _id.get(), 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::ColorTexture::clearRegion(const RectI3& region, const RGBAF& color) noexcept
{
	TR_GL_CALL(glClearTexSubImage, _id.get(), 0, region.tl.x, region.tl.y, region.tl.z, region.size.x, region.size.y,
			   region.size.z, GL_RGBA, GL_FLOAT, &color);
}

// void tr::Texture::disableComparison() noexcept
// {
// 	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_COMPARE_MODE, GL_NONE);
// }

// void tr::Texture::setComparisonMode(Compare op) noexcept
// {
// 	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
// 	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_COMPARE_FUNC, static_cast<GLint>(op));
// }

tr::ColorTexture1D::ColorTexture1D(int size, bool mipmapped, ColorTextureFormat format)
	: ColorTexture{GL_TEXTURE_1D}
{
	assert(size > 0);

	TR_GL_CALL(glTextureStorage1D, _id.get(), mipmapped ? std::floor(std::log2(size)) + 1 : 1,
			   static_cast<GLenum>(format), size);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::ColorTexture1D::ColorTexture1D(SubBitmap bitmap, bool mipmapped, ColorTextureFormat format)
	: ColorTexture1D{bitmap.size().x, mipmapped, format}
{
	setRegion(0, bitmap);
}

int tr::ColorTexture1D::size() const noexcept
{
	return Texture::width();
}

void tr::ColorTexture1D::clearRegion(int offset, int size, const tr::RGBAF& color) noexcept
{
	assert(offset + size <= this->size());

	ColorTexture::clearRegion({{offset, 0, 0}, {size, 1, 1}}, color);
}

void tr::ColorTexture1D::copyRegion(int offset, const ColorTexture1D& src, int srcOffset, int size) noexcept
{
	assert(srcOffset + size <= src.size());
	assert(offset + size <= this->size());

	Texture::copyRegion({offset, 0, 0}, src, {{srcOffset, 0, 0}, {size, 1, 1}});
}

void tr::ColorTexture1D::setRegion(int offset, SubBitmap bitmap) noexcept
{
	assert(bitmap.size().y == 1);
	assert(offset + bitmap.size().x <= size());

	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, 0);
	TR_GL_CALL(glTextureSubImage1D, _id.get(), 0, offset, bitmap.size().x, format, type, bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::ArrayColorTexture1D::ArrayColorTexture1D(int size, int layers, bool mipmapped, ColorTextureFormat format)
	: ColorTexture{GL_TEXTURE_1D_ARRAY}
{
	assert(size > 0 && layers > 0);

	TR_GL_CALL(glTextureStorage2D, _id.get(), mipmapped ? std::floor(std::log2(size)) + 1 : 1,
			   static_cast<GLenum>(format), size, layers);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::ArrayColorTexture1D::ArrayColorTexture1D(SubBitmap bitmap, bool mipmapped, ColorTextureFormat format)
	: ArrayColorTexture1D{bitmap.size().x, bitmap.size().y, mipmapped, format}
{
	setRegion({0, 0}, bitmap);
}

int tr::ArrayColorTexture1D::size() const noexcept
{
	return Texture::width();
}

int tr::ArrayColorTexture1D::layers() const noexcept
{
	return Texture::height();
}

void tr::ArrayColorTexture1D::clearRegion(const RectI2& rect, const tr::RGBAF& color) noexcept
{
#ifndef NDEBUG
	const RectI2 bounds{{this->size(), layers()}};
	assert(bounds.contains(rect.tl + rect.size));
#endif

	ColorTexture::clearRegion({{rect.tl, 0}, {rect.size, 1}}, color);
}

void tr::ArrayColorTexture1D::copyRegion(glm::ivec2 tl, const ArrayColorTexture1D& src, const RectI2& rect) noexcept
{
#ifndef NDEBUG
	const RectI2 lbounds{{size(), layers()}};
	const RectI2 rbounds{{src.size(), src.layers()}};
	assert(lbounds.contains(tl + rect.size));
	assert(rbounds.contains(rect.tl + rect.size));
#endif

	Texture::copyRegion({tl, 0}, src, {{rect.tl, 0}, {rect.size, 1}});
}

void tr::ArrayColorTexture1D::setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept
{
#ifndef NDEBUG
	const RectI2 bounds{{size(), layers()}};
	assert(bounds.contains(tl + bitmap.size()));
#endif

	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage2D, _id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::ColorTexture2D::ColorTexture2D(glm::ivec2 size, bool mipmapped, ColorTextureFormat format)
	: ColorTexture{GL_TEXTURE_2D}
{
	assert(size.x > 0 && size.y > 0);

	glTextureStorage2D(_id.get(), mipmapped ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : 1,
					   static_cast<GLenum>(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::ColorTexture2D::ColorTexture2D(SubBitmap bitmap, bool mipmapped, ColorTextureFormat format)
	: ColorTexture2D{bitmap.size(), mipmapped, format}
{
	setRegion({0, 0}, bitmap);
}

glm::ivec2 tr::ColorTexture2D::size() const noexcept
{
	return {Texture::width(), Texture::height()};
}

void tr::ColorTexture2D::clearRegion(const RectI2& rect, const tr::RGBAF& color) noexcept
{
#ifndef NDEBUG
	const RectI2 bounds{{size()}};
	assert(bounds.contains(rect.tl + rect.size));
#endif

	ColorTexture::clearRegion({{rect.tl, 0}, {rect.size, 1}}, color);
}

void tr::ColorTexture2D::copyRegion(glm::ivec2 tl, const ColorTexture2D& src, const RectI2& rect) noexcept
{
#ifndef NDEBUG
	const RectI2 lbounds{{size()}};
	const RectI2 rbounds{{src.size()}};
	assert(lbounds.contains(tl + rect.size));
	assert(rbounds.contains(rect.tl + rect.size));
#endif

	Texture::copyRegion({tl, 0}, src, {{rect.tl, 0}, {rect.size, 1}});
}

void tr::ColorTexture2D::setRegion(glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert(RectI2{size()}.contains(tl + bitmap.size()));
	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage2D, _id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::ArrayColorTexture2D::ArrayColorTexture2D(glm::ivec2 size, int layers, bool mipmapped, ColorTextureFormat format)
	: ColorTexture{GL_TEXTURE_2D_ARRAY}
{
	assert(size.x > 0 && size.y > 0 && layers > 0);

	TR_GL_CALL(glTextureStorage3D, _id.get(), mipmapped ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : 1,
			   static_cast<GLenum>(format), size.x, size.y, layers);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

tr::ArrayColorTexture2D::ArrayColorTexture2D(std::span<SubBitmap> layers, bool mipmapped, ColorTextureFormat format)
	: ArrayColorTexture2D{determineArrayTextureSize(layers), static_cast<int>(layers.size()), mipmapped, format}
{
	for (std::size_t i = 0; i < layers.size(); ++i) {
		setLayerRegion(i, {}, layers[i]);
	}
}

glm::ivec2 tr::ArrayColorTexture2D::size() const noexcept
{
	return {Texture::width(), Texture::height()};
}

int tr::ArrayColorTexture2D::layers() const noexcept
{
	return Texture::depth();
}

void tr::ArrayColorTexture2D::clearRegion(const RectI3& rect, const tr::RGBAF& color) noexcept
{
#ifndef NDEBUG
	const RectI3 bounds{{size(), layers()}};
	assert(bounds.contains(rect.tl + rect.size));
#endif

	ColorTexture::clearRegion(rect, color);
}

void tr::ArrayColorTexture2D::copyRegion(const glm::ivec3& tl, const ArrayColorTexture2D& src,
										 const RectI3& rect) noexcept
{
#ifndef NDEBUG
	const RectI3 lbounds{{size(), layers()}};
	const RectI3 rbounds{{src.size(), src.layers()}};
	assert(lbounds.contains(tl + rect.size));
	assert(rbounds.contains(rect.tl + rect.size));
#endif

	Texture::copyRegion(tl, src, rect);
}

void tr::ArrayColorTexture2D::setLayerRegion(int layer, glm::ivec2 tl, SubBitmap bitmap) noexcept
{
	assert(layer <= layers());
	assert(RectI2{size()}.contains(tl + bitmap.size()));
	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage3D, _id.get(), 0, tl.x, tl.y, layer, bitmap.size().x, bitmap.size().y, 1, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

tr::ColorTexture3D::ColorTexture3D(glm::ivec3 size, bool mipmapped, ColorTextureFormat format)
	: ColorTexture{GL_TEXTURE_3D}
{
	assert(size.x > 0 && size.y > 0 && size.z > 0);

	TR_GL_CALL(glTextureStorage3D, _id.get(), mipmapped ? std::floor(std::log2(std::max(size.x, size.y))) + 1 : 1,
			   static_cast<GLenum>(format), size.x, size.y, size.z);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw TextureBadAlloc{};
	}
}

glm::ivec3 tr::ColorTexture3D::size() const noexcept
{
	return {Texture::width(), Texture::height(), Texture::depth()};
}

void tr::ColorTexture3D::clearRegion(const RectI3& rect, const tr::RGBAF& color) noexcept
{
#ifndef NDEBUG
	const RectI3 bounds{size()};
	assert(bounds.contains(rect.tl + rect.size));
#endif

	ColorTexture::clearRegion(rect, color);
}

void tr::ColorTexture3D::copyRegion(const glm::ivec3& tl, const ColorTexture3D& src, const RectI3& rect) noexcept
{
#ifndef NDEBUG
	const RectI3 lbounds{size()};
	const RectI3 rbounds{src.size()};
	assert(lbounds.contains(tl + rect.size));
	assert(rbounds.contains(rect.tl + rect.size));
#endif

	Texture::copyRegion(tl, src, rect);
}

void tr::ColorTexture3D::setLayerRegion(const glm::ivec3& tl, SubBitmap bitmap) noexcept
{
	assert(tl.z <= size().z);
	assert(RectI2{static_cast<glm::ivec2>(size())}.contains(static_cast<glm::ivec2>(tl) + bitmap.size()));
	auto [format, type]{bitmapToGLFormat(bitmap.format())};
	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / bitmap.format().pixelBytes());
	TR_GL_CALL(glTextureSubImage3D, _id.get(), 0, tl.x, tl.y, tl.z, bitmap.size().x, bitmap.size().y, 1, format, type,
			   bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}
