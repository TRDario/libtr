#include "../include/tr/gl_buffer.hpp"

#include <GL/glew.h>

tr::GLBuffer::GLBuffer(Target target, std::size_t size, Flag flags)
	: _target{target}, _size{size}
{
	assert(size != 0);

	GLuint id;
	glCreateBuffers(1, &id);
	_id.reset(id);

	glNamedBufferStorage(id, size, nullptr, GLenum(flags));
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw GLBufferBadAlloc{};
	}
}

tr::GLBuffer::GLBuffer(Target target, std::span<const std::byte> data, Flag flags)
	: _target{target}, _size(data.size())
{
	assert(data.size() != 0);

	GLuint id;
	glCreateBuffers(1, &id);
	_id.reset(id);

	glNamedBufferStorage(id, _size, data.data(), GLenum(flags));
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw GLBufferBadAlloc{};
	}
}

void tr::GLBuffer::Deleter::operator()(unsigned int id) const noexcept
{
	glDeleteBuffers(1, &id);
}

bool tr::operator==(const GLBuffer& l, const GLBuffer& r) noexcept
{
	return l._id == r._id;
}

void tr::GLBuffer::setLabel(std::string_view label) noexcept
{
	glObjectLabel(GL_BUFFER, _id.get(), label.size(), label.data());
}

std::size_t tr::GLBuffer::size() const noexcept
{
	return _size;
}

std::vector<std::byte> tr::GLBuffer::copyRegion(std::size_t offset, std::size_t size) const
{
	if (size == 0) {
		return {};
	}
	assert(offset + size <= _size);
	std::vector<std::byte> data(size);
	glGetNamedBufferSubData(_id.get(), offset, size, data.data());
	return data;
}

void tr::GLBuffer::setRegion(std::size_t offset, std::span<const std::byte> data) noexcept
{
	assert(offset + data.size() <= _size);
	glNamedBufferSubData(_id.get(), offset, data.size(), data.data());
}

bool tr::GLBuffer::mapped() const noexcept
{
	int mapped;
	glGetNamedBufferParameteriv(_id.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::GLBufferMap tr::GLBuffer::mapRegion(std::size_t offset, std::size_t size, MapFlag flags)
{
	assert(!mapped());
	assert(size > 0);
	assert(offset + size <= _size);
	auto ptr{(std::byte*)(glMapNamedBufferRange(_id.get(), offset, size, GLenum(flags)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw GLMapBadAlloc{};
	}
	return {_id.get(), std::span(ptr, size)};
}

void tr::GLBuffer::resetTarget(Target newTarget) noexcept
{
	_target = newTarget;
}

void tr::GLBuffer::bind(std::optional<Target> target) const noexcept
{
	glBindBuffer(GLenum(target.value_or(_target)), _id.get());
}

void tr::GLBuffer::bindIndexed(std::optional<Target> target, std::uint32_t index) const noexcept
{
	glBindBufferBase(GLenum(target.value_or(_target)), index, _id.get());
}

void tr::GLBuffer::bindIndexedRange(std::optional<Target> target, std::uint32_t index, std::size_t offset,
									std::size_t size) const noexcept
{
	glBindBufferRange(GLenum(target.value_or(_target)), index, _id.get(), offset, size);
}

tr::GLBufferMap::GLBufferMap(unsigned int buffer, std::span<std::byte> span) noexcept
	: _buffer{buffer}, _span{span}
{
}

void tr::GLBufferMap::Deleter::operator()(unsigned int id) const noexcept
{
	glUnmapNamedBuffer(id);
}

tr::GLBufferMap::operator std::span<std::byte>() const noexcept
{
	return _span;
}

std::span<std::byte> tr::GLBufferMap::span() const noexcept
{
	return _span;
}