#include "../include/tr/index_buffer.hpp"

#include "../include/tr/dependencies/magic_enum.hpp"
#include "../include/tr/ranges.hpp"

using namespace magic_enum::bitwise_operators;

tr::IndexBufferMap::IndexBufferMap(GLBufferMap base) noexcept
	: GLBufferMap{std::move(base)}
{
}

tr::IndexBufferMap::operator std::span<std::uint16_t>() const noexcept
{
	return span();
}

std::span<std::uint16_t> tr::IndexBufferMap::span() const noexcept
{
	return asMutObjects<std::uint16_t>(GLBufferMap::span());
}

tr::IndexBuffer::IndexBuffer() noexcept
	: _size{0}
{
}

tr::IndexBuffer::IndexBuffer(std::size_t capacity)
	: _buffer{{GLBuffer::Target::ELEMENT_ARRAY_BUFFER, std::size_t(capacity * sizeof(std::uint16_t)),
			   GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE}},
	  _size{0}
{
}

tr::IndexBuffer::IndexBuffer(std::span<const std::uint16_t> data)
	: _buffer{{GLBuffer::Target::ELEMENT_ARRAY_BUFFER, rangeBytes(data),
			   GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE}},
	  _size{std::size_t(data.size())}
{
}

bool tr::IndexBuffer::empty() const noexcept
{
	return _size == 0;
}

std::size_t tr::IndexBuffer::size() const noexcept
{
	return _size;
}

std::size_t tr::IndexBuffer::capacity() const noexcept
{
	return _buffer.has_value() ? _buffer->size() : 0;
}

void tr::IndexBuffer::clear() noexcept
{
	resize(0);
}

void tr::IndexBuffer::set(std::span<const std::uint16_t> data)
{
	assert(!data.empty());
	resize(data.size());
	_buffer->setRegion(0, rangeBytes(data));
}

void tr::IndexBuffer::setRegion(std::size_t offset, std::span<const std::uint16_t> data) noexcept
{
	assert(!data.empty());
	assert(offset + data.size() <= _size);
	_buffer->setRegion(offset * sizeof(std::uint16_t), rangeBytes(data));
}

bool tr::IndexBuffer::mapped() const noexcept
{
	return _buffer.has_value() && _buffer->mapped();
}

tr::IndexBufferMap tr::IndexBuffer::mapNew(std::size_t size)
{
	resize(size);
	return _buffer->mapRegion(0, _size * sizeof(std::uint16_t),
							  GLBuffer::MapFlag::WRITABLE | GLBuffer::MapFlag::INVALIDATE_BUFFER);
}

tr::IndexBufferMap tr::IndexBuffer::mapRegion(std::size_t offset, std::size_t size)
{
	assert(offset + size <= _size);
	return _buffer->mapRegion(offset * sizeof(std::uint16_t), size * sizeof(std::uint16_t),
							  GLBuffer::MapFlag::WRITABLE);
}

void tr::IndexBuffer::resize(std::size_t newSize)
{
	assert(!mapped());
	if (newSize > capacity()) {
		_buffer = GLBuffer{GLBuffer::Target::ELEMENT_ARRAY_BUFFER, std::size_t(newSize * sizeof(std::uint16_t)),
						   GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE};
		if (!_label.empty()) {
			_buffer->setLabel(_label);
		}
	}
	_size = newSize;
}

void tr::IndexBuffer::setLabel(std::string label) noexcept
{
	_label = std::move(label);
	if (_buffer.has_value()) {
		_buffer->setLabel(_label);
	}
}
