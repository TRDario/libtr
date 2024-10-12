module;
#include <cassert>
#include <GL/glew.h>

export module tr:renderbuffer;

import std;
import glm;
import :handle;
import :texture;

export namespace tr {
    /******************************************************************************************************************
	 * Error thrown on a failed renderbuffer bad allocation.
	 ******************************************************************************************************************/
    struct RenderbufferBadAlloc : std::bad_alloc {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
    };

    /******************************************************************************************************************
	 * Image buffer optimized for use as a render target in a framebuffer.
     *
     * Wrapper over an OpenGL renderbuffer.
	 ******************************************************************************************************************/
	class Renderbuffer {
	public:
		/**************************************************************************************************************
        * Allocates a blank renderbuffer.
        *
        * @exception RenderbufferBadAlloc If allocating the renderbuffer failed.
        *
        * @param size The size of the renderbuffer in pixels.
        * @param format The pixel format of the renderbuffer.
        **************************************************************************************************************/
		Renderbuffer(glm::ivec2 size, TextureFormat format);

        /**************************************************************************************************************
        * Equality comparison operator.
        **************************************************************************************************************/
		bool operator==(const Renderbuffer& rhs) const noexcept;


		/**************************************************************************************************************
        * Gets the size of the renderbuffer.
        *
        * @return The size of the renderbuffer in pixels.
        **************************************************************************************************************/
		glm::ivec2 size() const noexcept;


        /**************************************************************************************************************
        * Sets the debug label of the renderbuffer.
        *
        * @param label The new label of the renderbuffer.
        **************************************************************************************************************/
        void setLabel(std::string_view label) noexcept;
	private:
		struct Deleter { void operator()(GLuint id) noexcept; /**< @private */ };

		Handle<GLuint, 0, Deleter> _id;
		glm::ivec2 				   _size;

		void bind() const noexcept;

		friend class Framebuffer;
	};
}

// IMPLEMENTATION

constexpr const char* tr::RenderbufferBadAlloc::what() const noexcept
{
    return "failed renderbuffer allocation";
}

tr::Renderbuffer::Renderbuffer(glm::ivec2 size, TextureFormat format)
    : _size { size }
{
    GLuint id;
    glCreateRenderbuffers(1, &id);
    _id.reset(id);

    glNamedRenderbufferStorage(id, GLenum(format), size.x, size.y);
    if (glGetError() == GL_OUT_OF_MEMORY) {
        throw RenderbufferBadAlloc {};
    }
}

void tr::Renderbuffer::Deleter::operator()(GLuint id) noexcept
{
    glDeleteRenderbuffers(1, &id);
}


bool tr::Renderbuffer::operator==(const Renderbuffer& rhs) const noexcept
{
    return _id == rhs._id;
}

glm::ivec2 tr::Renderbuffer::size() const noexcept
{
    return _size;
}

void tr::Renderbuffer::bind() const noexcept
{
    glBindRenderbuffer(GL_RENDERBUFFER, _id.get());
}

void tr::Renderbuffer::setLabel(std::string_view label) noexcept
{
    glObjectLabel(GL_RENDERBUFFER, _id.get(), label.size(), label.data());
}