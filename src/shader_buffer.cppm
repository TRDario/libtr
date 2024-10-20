/**
 * @file shader_buffer.cppm
 * @brief Provides a GPU shader buffer class.
 */

module;
#include <magic_enum/magic_enum.hpp>
#include <cassert>

export module tr:shader_buffer;

import std;
import :gl_buffer;
import :handle;

export namespace tr {
	/******************************************************************************************************************
	 * GPU buffer accessable by a shader.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 *
	 * The API is designed as a dynamic-length array with a fixed-length header in front.
	 * Passing 0 to the size/capacity of either component disables them, and they cannot be accessed.
	 ******************************************************************************************************************/
    class ShaderBuffer : public GLBuffer {
    public:
        /**************************************************************************************************************
		 * Shader buffer map access type.
		 **************************************************************************************************************/
		enum class Access : std::uint32_t {
			/**********************************************************************************************************
			 * Buffer maps are read-only.
			 **********************************************************************************************************/
			READ_ONLY = std::uint32_t(Flag::READABLE),
			
			/**********************************************************************************************************
			 * Buffer maps are write-only.
			 **********************************************************************************************************/
			WRITE_ONLY = std::uint32_t(Flag::WRITABLE),
			
			/**********************************************************************************************************
			 * Buffer maps are readable and writable.
			 **********************************************************************************************************/
			READ_WRITE = std::uint32_t(Flag::READABLE) | std::uint32_t(Flag::WRITABLE)
		};


        /******************************************************************************************************************
		 * Allocates an uninitialized shader buffer.
		 *
		 * @exception GLBufferBadAlloc If allocating the GPU buffer failed.
		 *
		 * @param headerSize The size of the fixed header block in bytes (may be 0).
		 * @param capacity The maximum capacity of the dynamic array in bytes (may be 0).
		 * @param access The access permissions of buffer maps.
		 ******************************************************************************************************************/
		ShaderBuffer(std::size_t headerSize, std::size_t capacity, Access access);


        /******************************************************************************************************************
		 * Gets the size of the fixed header block.
		 *
		 * @return The size of the header in bytes.
		 ******************************************************************************************************************/
		std::size_t headerSize() const noexcept;

		/******************************************************************************************************************
		 * Gets the size of the dynamic array.
		 *
		 * @return The size of the dynamic array in bytes.
		 ******************************************************************************************************************/
		std::size_t arraySize() const noexcept;

		/******************************************************************************************************************
		 * Gets the maximum capacity of the dynamic array.
		 *
		 * @return The maximum capacity of the dynamic array in bytes.
		 ******************************************************************************************************************/
		std::size_t arrayCapacity() const noexcept;


		/******************************************************************************************************************
		 * Copies the data of the fixed header to a vector.
		 *
		 * @exception std::bad_alloc If allocating the vector failed.
		 *
		 * @return A vector containing a copy of the header's data.
		 ******************************************************************************************************************/
		std::vector<std::byte> copyHeader() const;

		/******************************************************************************************************************
		 * Copies the data of the dynamic array to a vector.
		 *
		 * @exception std::bad_alloc If allocating the vector failed.
		 *
		 * @return A vector containing a copy of the dynamic array's data.
		 ******************************************************************************************************************/
		std::vector<std::byte> copyArray() const;


		/******************************************************************************************************************
		 * Sets the data of the header.
		 *
		 * @param data The new data of the header. The span must be the same size as the header, otherwise a failed
		 *             assertion may be raised.
		 ******************************************************************************************************************/
		void setHeader(std::span<const std::byte> data) noexcept;

		/******************************************************************************************************************
		 * Sets the data of the dynamic array.
		 *
		 * @param data The new data of the array. The span must be smaller or have the same size as the array capacity,
		 *			   otherwise a failed assertion may be raised.
		 ******************************************************************************************************************/
		void setArray(std::span<const std::byte> data) noexcept;

		/******************************************************************************************************************
		 * Resizes the dynamic array.
		 *
		 * @param size The new size of the array in bytes. Must be less than the capacity, otherwise a failed assertion
		 *             may be raised.
		 ******************************************************************************************************************/
		void resizeArray(std::size_t size) noexcept;


		using GLBuffer::mapped;

		/******************************************************************************************************************
		 * Maps the fixed header.
		 *
		 * The buffer must have a fixed header block, otherwise a failed assertion may be raised.
		 *
		 * @return The mapped fixed header.
		 ******************************************************************************************************************/
		GLBufferMap mapHeader() noexcept;

		/******************************************************************************************************************
		 * Maps the dynamic array.
		 *
		 * The buffer must have a dynamic array block, otherwise a failed assertion may be raised.
		 *
		 * @return The mapped dynamic array.
		 ******************************************************************************************************************/
		GLBufferMap mapArray() noexcept;

		/******************************************************************************************************************
		 * Maps the entire buffer.
		 *
		 * @return The mapped buffer.
		 ******************************************************************************************************************/
		GLBufferMap map() noexcept;


		using GLBuffer::setLabel;
    private:
		Access 		_access;
        std::size_t _headerSize;
		std::size_t _size;

        friend class Shader;
    };
}

// IMPLEMENTATION

using namespace magic_enum::bitwise_operators;

tr::ShaderBuffer::ShaderBuffer(std::size_t headerSize, std::size_t capacity, Access access)
    : GLBuffer { Target::SHADER_STORAGE_BUFFER, headerSize + capacity, Flag(access) | Flag::DYNAMIC_STORAGE }
    , _headerSize { headerSize }
    , _size { 0 }
{}

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

std::vector<std::byte> tr::ShaderBuffer::copyHeader() const
{
    return GLBuffer::copyRegion(0, _headerSize);
}

std::vector<std::byte> tr::ShaderBuffer::copyArray() const
{
    return GLBuffer::copyRegion(_headerSize, _size);
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