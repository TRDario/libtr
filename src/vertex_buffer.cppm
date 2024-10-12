module;
#include <cassert>
#include <magic_enum/magic_enum.hpp>

export module tr:vertex_buffer;

import std;
import :gl_buffer;

export namespace tr {
    class VertexBuffer {
    public:
        // Creates an empty vertex buffer.
        VertexBuffer() noexcept;
        // Creates an uninitialized vertex buffer.
        // The label is an internal identifier for debugging.
		VertexBuffer(std::size_t capacity);
		// Creates a vertex buffer initialized to some data.
        // The label is an internal identifier for debugging.
		VertexBuffer(std::span<const std::byte> data);

        // Gets whether the buffer is empty.
        bool empty() const noexcept;
        // Gets the size of the buffer contents.
        std::size_t size() const noexcept;
        // Gets the capacity of the buffer.
        std::size_t capacity() const noexcept;

        // Clears the contents of the buffer.
        void clear() noexcept;
        // Sets the contents of the buffer.
        // A reallocation will be performed if the data is larger than the capacity of the buffer.
        void set(std::span<const std::byte> data);
        // Sets a subsection of the buffer.
		void setRegion(std::size_t offset, std::span<const std::byte> data) noexcept;

		bool mapped() const noexcept;
        // Maps the buffer, invalidating the previous contents in the process and resetting its size.
        // A reallocation will be performed if the new size is larger than the capacity of the buffer.
        GLBufferMap mapNew(std::size_t size);
		// Maps a region of the buffer.
		GLBufferMap mapRegion(std::size_t offset, std::size_t size);

        void setLabel(std::string label) noexcept;
    private:
        std::optional<GLBuffer> _buffer;
        std::size_t             _size; // Size of the allocated portion of the buffer.
        std::string             _label;

        // Resizes the buffer, reallocating if needed.
        void resize(std::size_t newSize);

        friend class GLContext;
    };
}

// IMPLEMENTATION

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
    assert(!data.empty());
    resize(data.size());
    _buffer->setRegion(0, data);
}

void tr::VertexBuffer::setRegion(std::size_t offset, std::span<const std::byte> data) noexcept
{
    assert(!data.empty());
    assert(offset + data.size() <= _size);
    _buffer->setRegion(offset, data);
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