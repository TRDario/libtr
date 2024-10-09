module;
#include <magic_enum/magic_enum.hpp>
#include <cassert>

export module tr:shader_buffer;

import std;
import :gl_buffer;
import :handle;
import :integer;

export namespace tr {
    class ShaderBuffer : public GLBuffer {
    public:
        // The type of access allowed for the buffer.
		enum class Access : Ui32 {
			// Read-only buffer.
			READ_ONLY = Ui32(Flag::READABLE),
			// Write-only buffer.
			WRITE_ONLY = Ui32(Flag::WRITABLE),
			// Readable and writable buffer.
			READ_WRITE = Ui32(Flag::READABLE) | Ui32(Flag::WRITABLE)
		};

        // Creates an uninitialized shader buffer.
		// headerSize - The size of the fixed block in front of the dynamic array.
		// capacity - The maximum size of the dynamic array.
		ShaderBuffer(Size headerSize, Size capacity, Access access);

        // Gets the size of the fixed buffer header block.
		Size headerSize() const noexcept;
		// Gets the size of the dynamic array.
		Size arraySize() const noexcept;
		// Gets the maximum supported dynamic array size.
		Size arrayCapacity() const noexcept;

		// Copies the fixed header block to a vector.
		std::vector<Byte> copyHeader() const;
		// Copies the dynamic array to a vector.
		std::vector<Byte> copyArray() const;

		// Sets the fixed header block.
		void setHeader(std::span<const Byte> data) noexcept;
		// Sets the dynamic array.
		void setArray(std::span<const Byte> data) noexcept;
		// Resizes the dynamic array.
		void resizeArray(Size size) noexcept;

		using GLBuffer::mapped;
		// Maps the fixed header block of the buffer.
		GLBufferMap mapHeader() noexcept;
		// Maps the dynamic array.
		GLBufferMap mapArray() noexcept;
		// Maps the entire buffer.
		GLBufferMap map() noexcept;

		using GLBuffer::setLabel;
    private:
		Access 							  _access;
        Size                              _headerSize;
		Size                              _size;

        friend class Shader;
    };
}

// IMPLEMENTATION

using namespace magic_enum::bitwise_operators;

tr::ShaderBuffer::ShaderBuffer(Size headerSize, Size capacity, Access access)
    : GLBuffer { Target::SHADER_STORAGE_BUFFER, headerSize + capacity, Flag(access) | Flag::DYNAMIC_STORAGE }
    , _headerSize { headerSize }
    , _size { 0 }
{}

tr::ShaderBuffer::Size tr::ShaderBuffer::headerSize() const noexcept
{
    return _headerSize;
}

tr::ShaderBuffer::Size tr::ShaderBuffer::arraySize() const noexcept
{
    return _size;
}

tr::ShaderBuffer::Size tr::ShaderBuffer::arrayCapacity() const noexcept
{
    return GLBuffer::size() - _headerSize;
}

std::vector<tr::Byte> tr::ShaderBuffer::copyHeader() const
{
    return GLBuffer::copyRegion(0, _headerSize);
}

std::vector<tr::Byte> tr::ShaderBuffer::copyArray() const
{
    return GLBuffer::copyRegion(_headerSize, _size);
}

void tr::ShaderBuffer::setHeader(std::span<const Byte> data) noexcept
{
    assert(data.size() == headerSize());
    GLBuffer::setRegion(0, data);
}

void tr::ShaderBuffer::setArray(std::span<const Byte> data) noexcept
{
    assert(data.size() <= arrayCapacity());
    GLBuffer::setRegion(_headerSize, data);
    _size = data.size();
}

void tr::ShaderBuffer::resizeArray(Size size) noexcept
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