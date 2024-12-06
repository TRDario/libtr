#include "../include/tr/shader_buffer.hpp"
#include <magic_enum/magic_enum.hpp>

using namespace magic_enum::bitwise_operators;

tr::ShaderBuffer::ShaderBuffer(std::size_t headerSize, std::size_t capacity, Access access)
	: GLBuffer{Target::SHADER_STORAGE_BUFFER, headerSize + capacity, Flag(access) | Flag::DYNAMIC_STORAGE}
	, _headerSize{headerSize}
	, _size{0}
{
}

std::size_t tr::ShaderBuffer::headerSize() const noexcept
{
	return _headerSize;
}

std::size_t tr::ShaderBuffer::arraySize() const noexcept
{
	return _size;
}

std::size_t tr::ShaderBuffer::arrayCapacity() const noexcept
{
	return GLBuffer::size() - _headerSize;
}

void tr::ShaderBuffer::setHeader(std::span<const std::byte> data) noexcept
{
	assert(data.size() == headerSize());
	GLBuffer::setRegion(0, data);
}

void tr::ShaderBuffer::setArray(std::span<const std::byte> data) noexcept
{
	assert(data.size() <= arrayCapacity());
	GLBuffer::setRegion(_headerSize, data);
	_size = data.size();
}

void tr::ShaderBuffer::resizeArray(std::size_t size) noexcept
{
	assert(size <= arrayCapacity());
	_size = size;
}

tr::GLBufferMap tr::ShaderBuffer::mapHeader() noexcept
{
	return GLBuffer::mapRegion(0, _headerSize, MapFlag(_access));
}

tr::GLBufferMap tr::ShaderBuffer::mapArray() noexcept
{
	return GLBuffer::mapRegion(_headerSize, _size, MapFlag(_access));
}

tr::GLBufferMap tr::ShaderBuffer::map() noexcept
{
	return GLBuffer::mapRegion(0, _headerSize + _size, MapFlag(_access));
}
