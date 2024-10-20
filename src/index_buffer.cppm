/**
 * @file index_buffer.cppm
 * @brief Provides a GPU index buffer class.
 */

module;
#include <cassert>
#include <magic_enum/magic_enum.hpp>

export module tr:index_buffer;

import std;
import :gl_buffer;
import :ranges;

export namespace tr {
    /******************************************************************************************************************
	 * RAII wrapper over an index buffer map.
	 *
	 * The buffer is automatically unmapped once the map goes out of scope.
	 ******************************************************************************************************************/
    class IndexBufferMap : private GLBufferMap {
    public:
        /**************************************************************************************************************
		 * Casts the map into a regular index span.
		 **************************************************************************************************************/
        operator std::span<std::uint16_t>() const noexcept;

        /**************************************************************************************************************
		 * Casts the map into a regular index span.
		 *
		 * @return A span of bytes.
		 **************************************************************************************************************/
        std::span<std::uint16_t> span() const noexcept;
    private:
        IndexBufferMap(GLBufferMap base) noexcept;

        friend class IndexBuffer;
    };

    /******************************************************************************************************************
	 * GPU index buffer class.
     *
     * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
    class IndexBuffer {
    public:
        /**************************************************************************************************************
	     * Constructs an empty index buffer.
         *
         * This function can be called before creating a GLContext, but most operations on the buffer still require it.
	     **************************************************************************************************************/
        IndexBuffer() noexcept;
        
        /**************************************************************************************************************
	     * Allocates an index buffer.
         *
         * The buffer will be of size 0 and capacity @em capacity after construction.
         *
         * This function can't be called before creating a GLContext.
         *
         * @exception GLBufferBadAlloc If allocating the buffer failed.
         *
         * @param capacity The capacity of the buffer in indices. Must be greater than 0, otherwise a failed assertion
         *                 may be triggered.
	     **************************************************************************************************************/
		IndexBuffer(std::size_t capacity);
		
        /**************************************************************************************************************
	     * Allocates an index buffer and fills it with data.
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
		IndexBuffer(std::span<const std::uint16_t> data);


        /**************************************************************************************************************
	     * Gets whether the vertex buffer is empty.
         *
         * @return True if the buffer has size 0.
	     **************************************************************************************************************/
        bool empty() const noexcept;
        
        /**************************************************************************************************************
	     * Gets the size of the vertex buffer contents.
         *
         * @return The size of the vertex buffer contents in indices.
	     **************************************************************************************************************/
        std::size_t size() const noexcept;
        
        /**************************************************************************************************************
	     * Gets the capacity of the vertex buffer.
         *
         * The buffer can be resized past this capacity, but it will trigger a reallocation.
         *
         * @return The capacity of the vertex buffer in indices.
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
        void set(std::span<const std::uint16_t> data);
        
        /**************************************************************************************************************
	     * Sets a region of the buffer.
         *
         * Unlike set(), a call to this function will never cause a reallocation, but an assertion may fail if a span
         * too large is passed to it.
         *
         * The buffer cannot be mapped when this function is called.
         *
         * @param offset The starting offset within the buffer in indices.
         * @param data The new data of the buffer. `offset + data.size() <= capacity()` must hold true, otherwise a failed
         *             assertion may be triggered.
	     **************************************************************************************************************/
		void setRegion(std::size_t offset, std::span<const std::uint16_t> data) noexcept;


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
         * @param size The new size of the buffer in indices. Must be greater than 0, otherwise a failed assertion may
         *             be triggered.
         *
         * @return A map object.
	     **************************************************************************************************************/
        IndexBufferMap mapNew(std::size_t size);

        /**************************************************************************************************************
	     * Maps a region of the buffer, invalidating the previous contents in the process.
         *
         * Unlike mapNew(), a call to this function will never cause a reallocation, but an assertion may fail if an
         * out-of-bounds map is requested.
         *
         * @exception GLMapBadAlloc If mapping the buffer failed.
         *
         * @param offset The starting offset within the buffer in indices.
         * @param size The size of the mapped region in indices. Must be greater than 0, and `offset + data.size() <= capacity()`
         *             must hold true, otherwise a failed assertion may be triggered. 
         *
         * @return A map object.
	     **************************************************************************************************************/
		IndexBufferMap mapRegion(std::size_t offset, std::size_t size);


        /**************************************************************************************************************
	     * Sets the debug label of the vertex buffer.
         *
         * @param label The new label of the index buffer.
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

tr::IndexBufferMap::IndexBufferMap(GLBufferMap base) noexcept
    : GLBufferMap { std::move(base) }
{}

tr::IndexBufferMap::operator std::span<std::uint16_t>() const noexcept
{
    return span();
}

std::span<std::uint16_t> tr::IndexBufferMap::span() const noexcept
{
    return asObjects<std::uint16_t>(GLBufferMap::span());
}

tr::IndexBuffer::IndexBuffer() noexcept
    : _size { 0 }
{}

tr::IndexBuffer::IndexBuffer(std::size_t capacity)
    : _buffer { { GLBuffer::Target::ELEMENT_ARRAY_BUFFER, std::size_t(capacity * sizeof(std::uint16_t)), GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE } }
    , _size { 0 }
{}

tr::IndexBuffer::IndexBuffer(std::span<const std::uint16_t> data)
    : _buffer { { GLBuffer::Target::ELEMENT_ARRAY_BUFFER, rangeBytes(data), GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE } }
    , _size { std::size_t(data.size()) }
{}

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
    return _buffer->mapRegion(0, _size * sizeof(std::uint16_t), GLBuffer::MapFlag::WRITABLE | GLBuffer::MapFlag::INVALIDATE_BUFFER);
}

tr::IndexBufferMap tr::IndexBuffer::mapRegion(std::size_t offset, std::size_t size)
{
    assert(offset + size <= _size);
    return _buffer->mapRegion(offset * sizeof(std::uint16_t), size * sizeof(std::uint16_t), GLBuffer::MapFlag::WRITABLE);
}

void tr::IndexBuffer::resize(std::size_t newSize)
{
    assert(!mapped());
    if (newSize > capacity()) {
        _buffer = GLBuffer { GLBuffer::Target::ELEMENT_ARRAY_BUFFER, std::size_t(newSize * sizeof(std::uint16_t)), GLBuffer::Flag::DYNAMIC_STORAGE | GLBuffer::Flag::WRITABLE };
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