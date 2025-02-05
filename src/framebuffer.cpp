#include "../include/tr/bitmap.hpp"
#include "../include/tr/framebuffer.hpp"
#include "../include/tr/texture.hpp"
#include "../include/tr/window.hpp"
#include "bitmap_to_gl_format.hpp"
#include "gl_call.hpp"
#include <SDL2/SDL.h>

namespace tr {
	inline constexpr glm::i16vec2 EMPTY_ATTACHMENT{-1, -1};

	GLuint createFramebuffer() noexcept;
} // namespace tr

GLuint tr::createFramebuffer() noexcept
{
	GLuint id;
	TR_GL_CALL(glCreateFramebuffers, 1, &id);
	return id;
}

tr::BasicFramebuffer::BasicFramebuffer(GLuint id) noexcept
	: _id{id}
{
}

tr::Bitmap tr::BasicFramebuffer::readRegion(const RectI2& rect, BitmapFormat format) const
{
	auto [glFormat, glType]{bitmapToGLFormat(format)};
	Bitmap bitmap{rect.size, format};
	bindRead();
	TR_GL_CALL(glReadPixels, rect.tl.x, rect.tl.y, rect.size.x, rect.size.y, glFormat, glType, bitmap.data());
	return bitmap;
}

void tr::BasicFramebuffer::copyRegion(const RectI2& rect, ColorTexture2D& texture, glm::ivec2 textureTL) const noexcept
{
	bindRead();
	TR_GL_CALL(glCopyTextureSubImage2D, static_cast<Texture&>(texture)._id.get(), 0, textureTL.x, textureTL.y,
			   rect.tl.x, rect.tl.y, rect.size.x, rect.size.y);
}

void tr::BasicFramebuffer::bindRead() const noexcept
{
	TR_GL_CALL(glBindFramebuffer, GL_READ_FRAMEBUFFER, _id);
}

void tr::BasicFramebuffer::bindWrite() const noexcept
{
	TR_GL_CALL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, _id);
}

tr::Framebuffer::Framebuffer() noexcept
	: BasicFramebuffer{createFramebuffer()}, _size{0, 0}
{
	_attachSizes.fill(EMPTY_ATTACHMENT);
}

tr::Framebuffer::Framebuffer(Framebuffer&& r) noexcept
	: BasicFramebuffer{r._id}, _attachSizes{r._attachSizes}, _size{r._size}
{
	r._id = 0;
}

tr::Framebuffer& tr::Framebuffer::operator=(Framebuffer&& r) noexcept
{
	std::ignore = Framebuffer{std::move(*this)};
	std::swap(_id, r._id);
	_attachSizes = r._attachSizes;
	_size        = r._size;
	return *this;
}

tr::Framebuffer::~Framebuffer() noexcept
{
	TR_GL_CALL(glDeleteFramebuffers, 1, &_id);
}

glm::ivec2 tr::Framebuffer::size() const noexcept
{
	return _size;
}

void tr::Framebuffer::attach(ColorTexture1D& tex, int slot) noexcept
{
	assert(slot >= 0 && slot < 8);

	TR_GL_CALL(glNamedFramebufferTexture, _id, GL_COLOR_ATTACHMENT0 + slot, static_cast<Texture&>(tex)._id.get(), 0);
	_attachSizes[slot] = {tex.size(), 1};
	calcSize();
}

void tr::Framebuffer::attach(ArrayColorTexture1D& tex, int layer, int slot) noexcept
{
	assert(slot >= 0 && slot < 8);
	assert(layer >= 0 && layer < tex.layers());

	TR_GL_CALL(glNamedFramebufferTextureLayer, _id, GL_COLOR_ATTACHMENT0 + slot, static_cast<Texture&>(tex)._id.get(),
			   0, layer);
	_attachSizes[slot] = {tex.size(), 1};
	calcSize();
}

void tr::Framebuffer::attach(ColorTexture2D& tex, int slot) noexcept
{
	assert(slot >= 0 && slot < 8);

	TR_GL_CALL(glNamedFramebufferTexture, _id, GL_COLOR_ATTACHMENT0 + slot, static_cast<Texture&>(tex)._id.get(), 0);
	_attachSizes[slot] = tex.size();
	calcSize();
}

void tr::Framebuffer::attach(ArrayColorTexture2D& tex, int layer, int slot) noexcept
{
	assert(slot >= 0 && slot < 8);
	assert(layer >= 0 && layer < tex.layers());

	TR_GL_CALL(glNamedFramebufferTextureLayer, _id, GL_COLOR_ATTACHMENT0 + slot, static_cast<Texture&>(tex)._id.get(),
			   0, layer);
	_attachSizes[slot] = tex.size();
	calcSize();
}

void tr::Framebuffer::attach(ColorTexture3D& tex, int z, int slot) noexcept
{
	assert(slot >= 0 && slot < 8);
	assert(z >= 0 && z < tex.size().z);

	TR_GL_CALL(glNamedFramebufferTextureLayer, _id, GL_COLOR_ATTACHMENT0 + slot, static_cast<Texture&>(tex)._id.get(),
			   0, z);
	_attachSizes[slot] = {tex.size().x, tex.size().y};
	calcSize();
}

void tr::Framebuffer::clear(int slot) noexcept
{
	assert(slot >= 0 && slot < 8);

	TR_GL_CALL(glNamedFramebufferTexture, _id, GL_COLOR_ATTACHMENT0 + slot, 0, 0);
	_attachSizes[slot] = EMPTY_ATTACHMENT;
	calcSize();
}

void tr::Framebuffer::setLabel(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_FRAMEBUFFER, _id, label.size(), label.data());
}

glm::ivec2 tr::Framebuffer::calcSize() noexcept
{
	auto attachments{std::views::filter(_attachSizes, [](glm::i16vec2 size) { return size != EMPTY_ATTACHMENT; })};
	if (attachments.empty()) {
		_size = {0, 0};
	}
	else {
		_size = *attachments.begin();
		for (const glm::i16vec2& attach : attachments) {
			_size = {std::min<int>(_size.x, attach.x), std::min<int>(_size.y, attach.y)};
		}
	}
	return _size;
}

tr::Backbuffer::Backbuffer() noexcept
	: BasicFramebuffer{0}
{
}

glm::ivec2 tr::Backbuffer::size() const noexcept
{
	glm::ivec2 size;
	SDL_GL_GetDrawableSize(window()._impl.get(), &size.x, &size.y);
	return size;
}
