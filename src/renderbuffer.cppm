module;
#include <cassert>
#include <GL/glew.h>

export module tr:renderbuffer;

import std;
import glm;
import :handle;
import :texture;

export namespace tr {
    struct RenderbufferBadAlloc : std::bad_alloc {
        RenderbufferBadAlloc() noexcept = default;
		constexpr virtual const char* what() const noexcept { return "failed renderbuffer allocation"; };
    };

	class Renderbuffer {
	public:
		// Creates a blank renderbuffer.
		Renderbuffer(glm::ivec2 size, TextureFormat format);

		friend bool operator==(const Renderbuffer& lhs, const Renderbuffer& rhs) noexcept;

		// Gets the size of the renderbuffer.
		glm::ivec2 size() const noexcept;

        void setLabel(std::string_view label) noexcept;
	private:
		struct Deleter { void operator()(GLuint id) noexcept; };

		Handle<GLuint, 0, Deleter> _id;
		glm::ivec2 				   _size;

		void bind() const noexcept;

		friend class Framebuffer;
	};
}

// IMPLEMENTATION

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


bool tr::operator==(const Renderbuffer& lhs, const Renderbuffer& rhs) noexcept
{
    return lhs._id == rhs._id;
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