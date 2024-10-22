/**
 * @file gl_buffer.cppm
 * @brief Provides basic OpenGL buffer functionality.
 */

module;
#include <cassert>
#include <GL/glew.h>

export module tr:gl_buffer;

import std;
import :function;
import :handle;

export namespace tr {
    class GLBufferMap;

	/******************************************************************************************************************
	 * Error thrown when allocating a GPU buffer fails.
	 ******************************************************************************************************************/
    struct GLBufferBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        constexpr virtual const char* what() const noexcept;
    };

	/******************************************************************************************************************
	 * Error thrown when mapping a GPU buffer fails.
	 ******************************************************************************************************************/
    struct GLMapBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        constexpr virtual const char* what() const noexcept;
    };


    /******************************************************************************************************************
	 * Base OpenGL buffer class.
	 *
	 * This class is not meant to be used directly.
	 ******************************************************************************************************************/
    class GLBuffer {
    public:
		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
        friend bool operator==(const GLBuffer&, const GLBuffer&) noexcept;
    protected:
		/// @cond IMPLEMENTATION
		// Enum representing an OpenGL buffer target.
		enum class Target : std::uint32_t {
			ARRAY_BUFFER 				= 0x8892,
			ATOMIC_COUNTER_BUFFER 		= 0x92C0,
			COPY_READ_BUFFER 			= 0x8F36,
			COPY_WRITE_BUFFER 			= 0x8F37,
			DISPATCH_INDIRECT_BUFFER 	= 0x90EE,
			DRAW_INDIRECT_BUFFER 		= 0x8F3F,
			ELEMENT_ARRAY_BUFFER 		= 0x8893,
			PIXEL_PACK_BUFFER 			= 0x88EB,
			PIXEL_UNPACK_BUFFER 		= 0x88EC,
			QUERY_BUFFER 				= 0x9192,
			SHADER_STORAGE_BUFFER		= 0x90D2,
			TEXTURE_BUFFER         		= 0x8C2A,
			TRANSFORM_FEEDBACK_BUFFER	= 0x8C8E,
			UNIFORM_BUFFER				= 0x8A11
		};
		// Enum representing buffer creation flags.
		enum class Flag : std::uint32_t {
			NONE 			= 0x0,
			READABLE 		= 0x1,
			WRITABLE 		= 0x2,
			PERSISTENT 		= 0x40,
			COHERENT 		= 0x80,
			DYNAMIC_STORAGE = 0x100,
			CPU_STORAGE 	= 0x200
		};
		// Enum representing a buffer format.
		enum class Format {
			R8 			= 0x8229,
			R16 		= 0x822A,
			R_FP16 		= 0x822D,
			R_FP32 		= 0x822E,
			R_SI8 		= 0x8231,
			R_UI8 		= 0x8232,
			R_SI16 		= 0x8233,
			R_UI16 		= 0x8234,
			R_SI32 		= 0x8235,
			R_UI32 		= 0x8236,
			RG8 		= 0x822B,
			RG16 		= 0x822C,
			RG_FP16 	= 0x822F,
			RG_FP32 	= 0x8230,
			RG_SI8 		= 0x8237,
			RG_UI8 		= 0x8238,
			RG_SI16 	= 0x8239,
			RG_UI16 	= 0x823A,
			RG_SI32 	= 0x823B,
			RG_UI32 	= 0x823C,
			RGB_FP32 	= 0x8815,
			RGB_SI32 	= 0x8D83,
			RGB_UI32 	= 0x8D71,
			RGBA8 		= 0x8058,
			RGBA16 		= 0x805B,
			RGBA_FP16 	= 0x881A,
			RGBA_FP32 	= 0x8814,
			RGBA_SI8 	= 0x8D8E,
			RGBA_UI8 	= 0x8D7C,
			RGBA_SI16 	= 0x8D88,
			RGBA_UI16 	= 0x8D76,
			RGBA_SI32 	= 0x8D82,
			RGBA_UI32 	= 0x8D70,
		};
		// Enum representing a mapping flag.
		enum class MapFlag {
			NONE 				= 0x0,
			READABLE 			= 0x1,
			WRITABLE 			= 0x2,
			INVALIDATE_RANGE 	= 0x4,
			INVALIDATE_BUFFER 	= 0x8,
			EXPLICIT_FLUSH 		= 0x10,
			UNSYNCHRONIZED 		= 0x20,
			PERSISTENT 			= 0x40,
			COHERENT 			= 0x80
		};

		Handle<GLuint, 0, decltype([] (GLuint id) { glDeleteBuffers(1, &id); })> _id;
		Target                     												 _target;
		std::size_t                												 _size;

        // Creates an uninitialized buffer.
		GLBuffer(Target target, std::size_t size, Flag flags);
		// Creates an initialized buffer.
		GLBuffer(Target target, std::span<const std::byte> data, Flag flags);

		std::size_t size() const noexcept;

		std::vector<std::byte> copyRegion(std::size_t offset, std::size_t size) const;

        void setRegion(std::size_t offset, std::span<const std::byte> data) noexcept;

		bool mapped() const noexcept;
		GLBufferMap mapRegion(std::size_t offset, std::size_t size, MapFlag flags);

        // Resets the buffer's target binding point.
		void resetTarget(Target newTarget) noexcept;
		// Binds the buffer to a target (or std::nullopt to bind to the default target).
		void bind(std::optional<Target> target = std::nullopt) const noexcept;
		// Binds the buffer to an indexed target (or std::nullopt to bind to the default target).
		void bindIndexed(std::optional<Target> target, std::uint32_t index) const noexcept;
		// Binds a range of the buffer to an indexed target (or std::nullopt to bind to the default target).
		void bindIndexedRange(std::optional<Target> target, std::uint32_t index, std::size_t offset, std::size_t size) const noexcept;

		void setLabel(std::string_view label) noexcept;
		/// @endcond

		friend class Shader;
		friend class GLContext;
		friend class VertexBuffer;
		friend class IndexBuffer;
    };

    /******************************************************************************************************************
	 * RAII wrapper over an OpenGL buffer map.
	 *
	 * The buffer is automatically unmapped once the map goes out of scope.
	 ******************************************************************************************************************/
    class GLBufferMap {
    public:
		/**************************************************************************************************************
		 * Casts the map into a regular byte span.
		 **************************************************************************************************************/
        operator std::span<std::byte>() const noexcept;

		/**************************************************************************************************************
		 * Casts the map into a regular byte span.
		 *
		 * @return A span of bytes. Despite not being const-qualified, they may be read-only dpeending on buffer access.
		 **************************************************************************************************************/
        std::span<std::byte> span() const noexcept;
    private:
        Handle<GLuint, 0, decltype([] (GLuint id) { glUnmapNamedBuffer(id); })> _buffer;
        std::span<std::byte> 													_span;

        GLBufferMap(GLuint buffer, std::span<std::byte> span) noexcept;

        friend class GLBuffer;
    };
}

/// @cond IMPLEMENTATION

constexpr const char* tr::GLBufferBadAlloc::what() const noexcept {
	return "OpenGL buffer allocation error";
}

constexpr const char* tr::GLMapBadAlloc::what() const noexcept {
	return "OpenGL buffer map allocation error";
}

tr::GLBuffer::GLBuffer(Target target, std::size_t size, Flag flags)
    : _target { target }
    , _size { size }
{
	assert(size != 0);

    GLuint id;
    glCreateBuffers(1, &id);
    _id.reset(id);

    glNamedBufferStorage(id, size, nullptr, GLenum(flags));
    if (glGetError() == GL_OUT_OF_MEMORY) {
        throw GLBufferBadAlloc {};
    }
}

tr::GLBuffer::GLBuffer(Target target, std::span<const std::byte> data, Flag flags)
    : _target { target }
    , _size ( data.size() )
{
	assert(data.size() != 0);

    GLuint id;
    glCreateBuffers(1, &id);
    _id.reset(id);

    glNamedBufferStorage(id, _size, data.data(), GLenum(flags));
    if (glGetError() == GL_OUT_OF_MEMORY) {
        throw GLBufferBadAlloc {};
    }
}

bool tr::operator==(const GLBuffer& l, const GLBuffer& r) noexcept
{
    return l._id == r._id;
}

void tr::GLBuffer::setLabel(std::string_view label) noexcept
{
    glObjectLabel(GL_BUFFER, _id.get(), label.size(), label.data());
}

std::size_t tr::GLBuffer::size() const noexcept
{
    return _size;
}

std::vector<std::byte> tr::GLBuffer::copyRegion(std::size_t offset, std::size_t size) const
{
	if (size == 0) {
		return {};
	}
    assert(offset + size <= _size);
    std::vector<std::byte> data(size);
    glGetNamedBufferSubData(_id.get(), offset, size, data.data());
    return data;
}

void tr::GLBuffer::setRegion(std::size_t offset, std::span<const std::byte> data) noexcept
{
    assert(offset + data.size() <= _size);
    glNamedBufferSubData(_id.get(), offset, data.size(), data.data());
}

bool tr::GLBuffer::mapped() const noexcept
{
    int mapped;
    glGetNamedBufferParameteriv(_id.get(), GL_BUFFER_MAPPED, &mapped);
    return mapped;
}

tr::GLBufferMap tr::GLBuffer::mapRegion(std::size_t offset, std::size_t size, MapFlag flags)
{
    assert(!mapped());
	assert(size > 0);
    assert(offset + size <= _size);
    auto ptr { (std::byte*)(glMapNamedBufferRange(_id.get(), offset, size, GLenum(flags))) };
    if (glGetError() == GL_OUT_OF_MEMORY) {
        throw GLMapBadAlloc {};
    }
    return { _id.get(), std::span(ptr, size) };
}

void tr::GLBuffer::resetTarget(Target newTarget) noexcept
{
    _target = newTarget;
}

void tr::GLBuffer::bind(std::optional<Target> target) const noexcept
{
    glBindBuffer(GLenum(target.value_or(_target)), _id.get());
}

void tr::GLBuffer::bindIndexed(std::optional<Target> target, std::uint32_t index) const noexcept
{
    glBindBufferBase(GLenum(target.value_or(_target)), index, _id.get());
}

void tr::GLBuffer::bindIndexedRange(std::optional<Target> target, std::uint32_t index, std::size_t offset, std::size_t size) const noexcept
{
    glBindBufferRange(GLenum(target.value_or(_target)), index, _id.get(), offset, size);
}

tr::GLBufferMap::GLBufferMap(GLuint buffer, std::span<std::byte> span) noexcept
    : _buffer { buffer }
    , _span { span }
{}

tr::GLBufferMap::operator std::span<std::byte>() const noexcept
{
    return _span;
}

std::span<std::byte> tr::GLBufferMap::span() const noexcept
{
    return _span;
}

/// @endcond