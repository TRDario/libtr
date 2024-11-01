#include "../include/vertex_buffer.hpp"
#include <magic_enum/magic_enum.hpp>

using namespace magic_enum::bitwise_operators;


tr::VertexBuffer::VertexBuffer() noexcept
    : _size { 0 }
{}

tr::VertexBuffer::VertexBuffer(std::size_t capacity)
    : _buffer { { GLBuffer::Target::ARRAY_BUFFER, capacity, GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE } }
    , _size { 0 }
{}

tr::VertexBuffer::VertexBuffer(std::span<const std::byte> data)
    : _buffer { { GLBuffer::Target::ARRAY_BUFFER, data, GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE } }
    , _size { std::size_t(data.size()) }
{}

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

tr::GLBufferMap tr::VertexBuffer::mapNew(std::size_t size)
{
    resize(size);
    return _buffer->mapRegion(0, _size, GLBuffer::MapFlag::WRITABLE | GLBuffer::MapFlag::INVALIDATE_BUFFER);
}

tr::GLBufferMap tr::VertexBuffer::mapRegion(std::size_t offset, std::size_t size)
{
    assert(offset + size <= _size);
    return _buffer->mapRegion(offset, size, GLBuffer::MapFlag::WRITABLE);
}

void tr::VertexBuffer::resize(std::size_t newSize)
{
    assert(!mapped());
    if (newSize > capacity()) {
        _buffer = GLBuffer { GLBuffer::Target::ARRAY_BUFFER, newSize, GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE };
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