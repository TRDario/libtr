module;
#include <magic_enum/magic_enum.hpp>
#include <cassert>

export module tr:shader_buffer;

import std;
import :gl_buffer;
import :handle;

export namespace tr {
    class ShaderBuffer : public GLBuffer {
    public:
        // The type of access allowed for the buffer.
		enum class Access : std::uint32_t {
			// Read-only buffer.
			READ_ONLY = std::uint32_t(Flag::READABLE),
			// Write-only buffer.
			WRITE_ONLY = std::uint32_t(Flag::WRITABLE),
			// Readable and writable buffer.
			READ_WRITE = std::uint32_t(Flag::READABLE) | std::uint32_t(Flag::WRITABLE)
		};

        // Creates an uninitialized shader buffer.
		// headerSize - The size of the fixed block in front of the dynamic array.
		// capacity - The maximum size of the dynamic array.
		ShaderBuffer(std::size_t headerSize, std::size_t capacity, Access access);

        // Gets the size of the fixed buffer header block.
		std::size_t headerSize() const noexcept;
		// Gets the size of the dynamic array.
		std::size_t arraySize() const noexcept;
		// Gets the maximum supported dynamic array size.
		std::size_t arrayCapacity() const noexcept;

		// Copies the fixed header block to a vector.
		std::vector<std::byte> copyHeader() const;
		// Copies the dynamic array to a vector.
		std::vector<std::byte> copyArray() const;

		// Sets the fixed header block.
		void setHeader(std::span<const std::byte> data) noexcept;
		// Sets the dynamic array.
		void setArray(std::span<const std::byte> data) noexcept;
		// Resizes the dynamic array.
		void resizeArray(std::size_t size) noexcept;

		using GLBuffer::mapped;
		// Maps the fixed header block of the buffer.
		GLBufferMap mapHeader() noexcept;
		// Maps the dynamic array.
		GLBufferMap mapArray() noexcept;
		// Maps the entire buffer.
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