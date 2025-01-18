#include "../include/tr/graphics_buffer.hpp"
#include "gl_call.hpp"

tr::GraphicsBuffer::GraphicsBuffer(Target target, std::size_t size, Flag flags)
	: _target{target}, _size{size}
{
	assert(size != 0);

	GLuint id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	_id.reset(id);

	TR_GL_CALL(glNamedBufferStorage, id, size, nullptr, GLenum(flags));
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw GraphicsBufferBadAlloc{};
	}
}

tr::GraphicsBuffer::GraphicsBuffer(Target target, std::span<const std::byte> data, Flag flags)
	: _target{target}, _size(data.size())
{
	assert(data.size() != 0);

	GLuint id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	_id.reset(id);

	TR_GL_CALL(glNamedBufferStorage, id, _size, data.data(), GLenum(flags));
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw GraphicsBufferBadAlloc{};
	}
}

void tr::GraphicsBuffer::Deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

void tr::GraphicsBuffer::setLabel(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, _id.get(), label.size(), label.data());
}

std::size_t tr::GraphicsBuffer::size() const noexcept
{
	return _size;
}

void tr::GraphicsBuffer::copyRegionToBase(void* ptr, std::size_t offset, std::size_t size) const noexcept
{
	if (size == 0) {
		return;
	}
	assert(offset + size <= _size);
	TR_GL_CALL(glGetNamedBufferSubData, _id.get(), offset, size, ptr);
}

void tr::GraphicsBuffer::setRegion(std::size_t offset, std::span<const std::byte> data) noexcept
{
	assert(offset + data.size() <= _size);
	TR_GL_CALL(glNamedBufferSubData, _id.get(), offset, data.size(), data.data());
}

bool tr::GraphicsBuffer::mapped() const noexcept
{
	int mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, _id.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::GraphicsBufferMap tr::GraphicsBuffer::mapRegion(std::size_t offset, std::size_t size, MapFlag flags)
{
	assert(!mapped());
	assert(size > 0);
	assert(offset + size <= _size);
	std::byte* ptr{(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, _id.get(), offset, size, GLenum(flags)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw GraphicsBufferMapBadAlloc{};
	}
	return {_id.get(), std::span(ptr, size)};
}

void tr::GraphicsBuffer::resetTarget(Target newTarget) noexcept
{
	_target = newTarget;
}

void tr::GraphicsBuffer::bind(std::optional<Target> target) const noexcept
{
	TR_GL_CALL(glBindBuffer, GLenum(target.value_or(_target)), _id.get());
}

void tr::GraphicsBuffer::bindIndexed(std::optional<Target> target, std::uint32_t index) const noexcept
{
	TR_GL_CALL(glBindBufferBase, GLenum(target.value_or(_target)), index, _id.get());
}

void tr::GraphicsBuffer::bindIndexedRange(std::optional<Target> target, std::uint32_t index, std::size_t offset,
										  std::size_t size) const noexcept
{
	TR_GL_CALL(glBindBufferRange, GLenum(target.value_or(_target)), index, _id.get(), offset, size);
}

tr::GraphicsBufferMap::GraphicsBufferMap(unsigned int buffer, std::span<std::byte> span) noexcept
	: _buffer{buffer}, _span{span}
{
}

void tr::GraphicsBufferMap::Deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glUnmapNamedBuffer, id);
}

tr::GraphicsBufferMap::operator std::span<std::byte>() const noexcept
{
	return _span;
}

std::span<std::byte> tr::GraphicsBufferMap::span() const noexcept
{
	return _span;
}
