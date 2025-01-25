#include "../include/tr/shader_buffer.hpp"
#include <magic_enum/magic_enum.hpp>

using namespace magic_enum::bitwise_operators;

tr::ShaderBuffer::ShaderBuffer(std::size_t headerSize, std::size_t capacity, Access access)
	: GraphicsBuffer{Target::SHADER_STORAGE_BUFFER, headerSize + capacity,
					 static_cast<Flag>(access) | Flag::DYNAMIC_STORAGE}
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
	return GraphicsBuffer::size() - _headerSize;
}

void tr::ShaderBuffer::setHeader(std::span<const std::byte> data) noexcept
{
	assert(data.size() == headerSize());

	GraphicsBuffer::setRegion(0, data);
}

void tr::ShaderBuffer::setArray(std::span<const std::byte> data) noexcept
{
	assert(data.size() <= arrayCapacity());

	GraphicsBuffer::setRegion(_headerSize, data);
	_size = data.size();
}

void tr::ShaderBuffer::resizeArray(std::size_t size) noexcept
{
	assert(size <= arrayCapacity());

	_size = size;
}

tr::GraphicsBufferMap tr::ShaderBuffer::mapHeader() noexcept
{
	return GraphicsBuffer::mapRegion(0, _headerSize, static_cast<MapFlag>(_access));
}

tr::GraphicsBufferMap tr::ShaderBuffer::mapArray() noexcept
{
	return GraphicsBuffer::mapRegion(_headerSize, _size, static_cast<MapFlag>(_access));
}

tr::GraphicsBufferMap tr::ShaderBuffer::map() noexcept
{
	return GraphicsBuffer::mapRegion(0, _headerSize + _size, static_cast<MapFlag>(_access));
}
