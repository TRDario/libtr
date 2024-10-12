module;
#include <cassert>
#include <magic_enum/magic_enum.hpp>

export module tr:index_buffer;

import std;
import :gl_buffer;
import :integer;
import :ranges;

export namespace tr {
    class IndexBufferMap : private GLBufferMap {
    public:
        operator std::span<Ui16>() const noexcept;
        std::span<Ui16> span() const noexcept;
    private:
        IndexBufferMap(GLBufferMap base) noexcept;
        friend class IndexBuffer;
    };

    class IndexBuffer {
    public:
        using Size = GLBuffer::Size;

        // Creates an empty index buffer.
        IndexBuffer() noexcept;
        // Creates an uninitialized vertex buffer.
        // The label is an internal identifier for debugging.
		IndexBuffer(Size capacity);
		// Creates a vertex buffer initialized to some data.
        // The label is an internal identifier for debugging.
		IndexBuffer(std::span<const Ui16> data);

        // Gets whether the buffer is empty.
        bool empty() const noexcept;
        // Gets the size of the buffer contents.
        Size size() const noexcept;
        // Gets the capacity of the buffer.
        Size capacity() const noexcept;

        // Clears the contents of the buffer.
        void clear() noexcept;
        // Sets the contents of the buffer.
        // A reallocation will be performed if the data is larger than the capacity of the buffer.
        void set(std::span<const Ui16> data);
        // Sets a subsection of the buffer.
		void setRegion(Size offset, std::span<const Ui16> data) noexcept;

		bool mapped() const noexcept;
        // Maps the buffer, invalidating the previous contents in the process and resetting its size.
        // A reallocation will be performed if the new size is larger than the capacity of the buffer.
        IndexBufferMap mapNew(Size size);
		// Maps a region of the buffer.
		IndexBufferMap mapRegion(Size offset, Size size);

        void setLabel(std::string label) noexcept;
    private:
        std::optional<GLBuffer> _buffer;
        Size                    _size; // Size of the allocated portion of the buffer.
        std::string             _label;

        // Resizes the buffer, reallocating if needed.
        void resize(Size newSize);

        friend class GLContext;
    };
}

// IMPLEMENTATION

using namespace magic_enum::bitwise_operators;

tr::IndexBufferMap::IndexBufferMap(GLBufferMap base) noexcept
    : GLBufferMap { std::move(base) }
{}

tr::IndexBufferMap::operator std::span<Ui16>() const noexcept
{
    return span();
}

std::span<tr::Ui16> tr::IndexBufferMap::span() const noexcept
{
    return asObjects<Ui16>(GLBufferMap::span());
}

tr::IndexBuffer::IndexBuffer() noexcept
    : _size { 0 }
{}

tr::IndexBuffer::IndexBuffer(Size capacity)
    : _buffer { { GLBuffer::Target::ELEMENT_ARRAY_BUFFER, Size(capacity * sizeof(Ui16)), GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE } }
    , _size { 0 }
{}

tr::IndexBuffer::IndexBuffer(std::span<const Ui16> data)
    : _buffer { { GLBuffer::Target::ELEMENT_ARRAY_BUFFER, asBytes(data), GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE } }
    , _size { Size(data.size()) }
{}

bool tr::IndexBuffer::empty() const noexcept
{
    return _size == 0;
}

tr::IndexBuffer::Size tr::IndexBuffer::size() const noexcept
{
    return _size;
}

tr::IndexBuffer::Size tr::IndexBuffer::capacity() const noexcept
{
    return _buffer.has_value() ? _buffer->size() : 0;
}

void tr::IndexBuffer::clear() noexcept
{
    resize(0);
}

void tr::IndexBuffer::set(std::span<const Ui16> data)
{
    assert(!data.empty());
    resize(data.size());
    _buffer->setRegion(0, asBytes(data));
}

void tr::IndexBuffer::setRegion(Size offset, std::span<const Ui16> data) noexcept
{
    assert(!data.empty());
    assert(offset + data.size() <= _size);
    _buffer->setRegion(offset * sizeof(Ui16), asBytes(data));
}

bool tr::IndexBuffer::mapped() const noexcept
{
    return _buffer.has_value() && _buffer->mapped();
}

tr::IndexBufferMap tr::IndexBuffer::mapNew(Size size)
{
    resize(size);
    return _buffer->mapRegion(0, _size * sizeof(Ui16), GLBuffer::MapFlag::WRITABLE | GLBuffer::MapFlag::INVALIDATE_BUFFER);
}

tr::IndexBufferMap tr::IndexBuffer::mapRegion(Size offset, Size size)
{
    assert(offset + size <= _size);
    return _buffer->mapRegion(offset * sizeof(Ui16), size * sizeof(Ui16), GLBuffer::MapFlag::WRITABLE);
}

void tr::IndexBuffer::resize(Size newSize)
{
    if (newSize > capacity()) {
        _buffer = GLBuffer { GLBuffer::Target::ELEMENT_ARRAY_BUFFER, Size(newSize * sizeof(Ui16)), GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE };
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