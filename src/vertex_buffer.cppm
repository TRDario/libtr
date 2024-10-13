module;
#include <cassert>
#include <magic_enum/magic_enum.hpp>

export module tr:vertex_buffer;

import std;
import :gl_buffer;

export namespace tr {
    /******************************************************************************************************************
	 * GPU vertex buffer class.
	 ******************************************************************************************************************/
    class VertexBuffer {
    public:
        /**************************************************************************************************************
	     * Constructs an empty vertex buffer.
         *
         * This function can be called before creating a GLContext, but most operations on the buffer still require it.
	     **************************************************************************************************************/
        VertexBuffer() noexcept;
        
        /**************************************************************************************************************
	     * Allocates a vertex buffer.
         *
         * The buffer will be of size 0 and capacity @em capacity after construction.
         *
         * This function can't be called before creating a GLContext.
         *
         * @exception GLBufferBadAlloc If allocating the buffer failed.
         *
         * @param capacity The capacity of the buffer in bytes. Must be greater than 0, otherwise a failed assertion may
         *                 be triggered.
	     **************************************************************************************************************/
		VertexBuffer(std::size_t capacity);
		
        /**************************************************************************************************************
	     * Allocates a vertex buffer and fills it with data.
         *
         * The buffer will be of size and capacity @em data.size() after construction.
         *
         * This function can't be called before creating a GLContext.
         *
         * @exception GLBufferBadAlloc If allocating the buffer failed.
         *
         * @param data The data to be uploaded to be buffer. Must not be empty, otherwise a failed assertion
         *             may be triggered.
	     **************************************************************************************************************/
		VertexBuffer(std::span<const std::byte> data);


        /**************************************************************************************************************
	     * Gets whether the vertex buffer is empty.
         *
         * @return True if the buffer has size 0.
	     **************************************************************************************************************/
        bool empty() const noexcept;
        
        /**************************************************************************************************************
	     * Gets the size of the vertex buffer contents.
         *
         * @return The size of the vertex buffer contents in bytes.
	     **************************************************************************************************************/
        std::size_t size() const noexcept;
        
        /**************************************************************************************************************
	     * Gets the capacity of the vertex buffer.
         *
         * The buffer can be resized past this capacity, but it will trigger a reallocation.
         *
         * @return The capacity of the vertex buffer in bytes.
	     **************************************************************************************************************/
        std::size_t capacity() const noexcept;


        /**************************************************************************************************************
	     * Sets the size of the vertex buffer to 0.
         *
         * The buffer cannot be mapped when this function is called.
         *
         * This does not affect the capacity of the buffer.
	     **************************************************************************************************************/
        void clear() noexcept;
        
        /**************************************************************************************************************
	     * Sets the contents of the buffer.
         *
         * If data.size() is greater than the capacity of the buffer, a reallocation will be done. This voids any previous
         * bindings of the buffer to the context, and so it must be rebound. setRegion() will never cause a
         * reallocation, so may be used in cases where that's a requirement.
         *
         * The buffer cannot be mapped when this function is called.
         *
         * @exception GLBufferBadAlloc If a reallocation was triggered and reallocating the buffer failed.
         *
         * @param data The new data of the buffer.
	     **************************************************************************************************************/
        void set(std::span<const std::byte> data);
        
        /**************************************************************************************************************
	     * Sets a region of the buffer.
         *
         * Unlike set(), a call to this function will never cause a reallocation, but an assertion may fail if a span
         * too large is passed to it.
         *
         * The buffer cannot be mapped when this function is called.
         *
         * @param offset The starting offset within the buffer in bytes.
         * @param data The new data of the buffer. `offset + data.size() <= capacity()` must hold true, otherwise a failed
         *             assertion may be triggered.
	     **************************************************************************************************************/
		void setRegion(std::size_t offset, std::span<const std::byte> data) noexcept;


        /**************************************************************************************************************
	     * Gets whether the buffer is mapped.
         *
         * @return True if the buffer is mapped, and false otherwise.
	     **************************************************************************************************************/
		bool mapped() const noexcept;

        /**************************************************************************************************************
	     * Maps the buffer, invalidating the previous contents in the process and resetting its size.
         *
         * If @em size is greater than the capacity of the buffer, a reallocation will be done. This voids any previous
         * bindings of the buffer to the context, and so it must be rebound. mapRegion() will never cause a
         * reallocation, so may be used in cases where that's a requirement.
         *
         * @exception GLBufferBadAlloc If a reallocation was triggered and reallocating the buffer failed.
         * @exception GLMapBadAlloc If mapping the buffer failed.
         *
         * @param size The new size of the buffer in bytes. Must be greater than 0, otherwise a failed assertion may
         *             be triggered.
         *
         * @return A map object.
	     **************************************************************************************************************/
        GLBufferMap mapNew(std::size_t size);
		
        /**************************************************************************************************************
	     * Maps a region of the buffer, invalidating the previous contents in the process.
         *
         * Unlike mapNew(), a call to this function will never cause a reallocation, but an assertion may fail if an
         * out-of-bounds map is requested.
         *
         * @exception GLMapBadAlloc If mapping the buffer failed.
         *
         * @param offset The starting offset within the buffer in bytes.
         * @param size The size of the mapped region in bytes. Must be greater than 0, and `offset + data.size() <= capacity()`
         *             must hold true, otherwise a failed assertion may be triggered. 
         *
         * @return A map object.
	     **************************************************************************************************************/
		GLBufferMap mapRegion(std::size_t offset, std::size_t size);


        /**************************************************************************************************************
	     * Sets the debug label of the vertex buffer.
         *
         * @param label The new label of the buffer vertex.
	     **************************************************************************************************************/
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