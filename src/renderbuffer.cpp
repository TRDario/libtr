#include "../include/tr/renderbuffer.hpp"
#include "gl_call.hpp"

tr::Renderbuffer::Renderbuffer(glm::ivec2 size, TextureFormat format)
	: _size{size}
{
	GLuint id;
	TR_GL_CALL(glCreateRenderbuffers, 1, &id);
	_id.reset(id);

	TR_GL_CALL(glNamedRenderbufferStorage, id, static_cast<GLenum>(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw RenderbufferBadAlloc{};
	}
}

void tr::Renderbuffer::Deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteRenderbuffers, 1, &id);
}

glm::ivec2 tr::Renderbuffer::size() const noexcept
{
	return _size;
}

void tr::Renderbuffer::bind() const noexcept
{
	TR_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, _id.get());
}

void tr::Renderbuffer::setLabel(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_RENDERBUFFER, _id.get(), label.size(), label.data());
}
