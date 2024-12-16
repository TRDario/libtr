#include "../include/tr/vertex_buffer.hpp"
#include <magic_enum/magic_enum.hpp>

using namespace magic_enum::bitwise_operators;

tr::VertexBuffer::VertexBuffer() noexcept
	: _size{0}
{
}

tr::VertexBuffer::VertexBuffer(std::size_t capacity)
	: _buffer{{GraphicsBuffer::Target::ARRAY_BUFFER, capacity,
			   GraphicsBuffer::Flag::DYNAMIC_STORAGE | GraphicsBuffer::Flag::WRITABLE}}
	, _size{0}
{
}

tr::VertexBuffer::VertexBuffer(std::span<const std::byte> data)
	: _buffer{{GraphicsBuffer::Target::ARRAY_BUFFER, data,
			   GraphicsBuffer::Flag::DYNAMIC_STORAGE | GraphicsBuffer::Flag::WRITABLE}}
	, _size{std::size_t(data.size())}
{
}

bool tr::VertexBuffer::empty() const noexcept
{
	return _size == 0;
}

std::size_t tr::VertexBuffer::size() const noexcept
{
	return _size;
}

std::size_t tr::VertexBuffer::capacity() const noexcept
{
	return _buffer.has_value() ? _buffer->size() : 0;
}

void tr::VertexBuffer::clear() noexcept
{
	resize(0);
}

void tr::VertexBuffer::set(std::span<const std::byte> data)
{
	if (data.empty()) {
		clear();
	}
	else {
		resize(data.size());
		_buffer->setRegion(0, data);
	}
}

void tr::VertexBuffer::setRegion(std::size_t offset, std::span<const std::byte> data) noexcept
{
	if (!data.empty()) {
		assert(!mapped());
		assert(offset + data.size() <= _size);
		_buffer->setRegion(offset, data);
	}
}

bool tr::VertexBuffer::mapped() const noexcept
{
	return _buffer.has_value() && _buffer->mapped();
}

tr::GraphicsBufferMap tr::VertexBuffer::mapNew(std::size_t size)
{
	resize(size);
	return _buffer->mapRegion(0, _size, GraphicsBuffer::MapFlag::WRITABLE | GraphicsBuffer::MapFlag::INVALIDATE_BUFFER);
}

tr::GraphicsBufferMap tr::VertexBuffer::mapRegion(std::size_t offset, std::size_t size)
{
	assert(offset + size <= _size);
	return _buffer->mapRegion(offset, size, GraphicsBuffer::MapFlag::WRITABLE);
}

void tr::VertexBuffer::resize(std::size_t newSize)
{
	assert(!mapped());
	if (newSize > capacity()) {
		_buffer = GraphicsBuffer{GraphicsBuffer::Target::ARRAY_BUFFER, newSize,
								 GraphicsBuffer::Flag::DYNAMIC_STORAGE | GraphicsBuffer::Flag::WRITABLE};
		if (!_label.empty()) {
			_buffer->setLabel(_label);
		}
	}
	_size = newSize;
}

void tr::VertexBuffer::setLabel(std::string label) noexcept
{
	_label = std::move(label);
	if (_buffer.has_value()) {
		_buffer->setLabel(_label);
	}
}
