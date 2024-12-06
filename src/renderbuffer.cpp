#include "../include/tr/renderbuffer.hpp"
#include <GL/glew.h>

tr::Renderbuffer::Renderbuffer(glm::ivec2 size, TextureFormat format)
	: _size{size}
{
	GLuint id;
	glCreateRenderbuffers(1, &id);
	_id.reset(id);

	glNamedRenderbufferStorage(id, GLenum(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw RenderbufferBadAlloc{};
	}
}

void tr::Renderbuffer::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteRenderbuffers(1, &id);
}

bool tr::Renderbuffer::operator==(const Renderbuffer& r) const noexcept
{
	return _id == r._id;
}

glm::ivec2 tr::Renderbuffer::size() const noexcept
{
	return _size;
}

void tr::Renderbuffer::bind() const noexcept
{
	glBindRenderbuffer(GL_RENDERBUFFER, _id.get());
}

void tr::Renderbuffer::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_RENDERBUFFER, _id.get(), label.size(), label.data());
}
