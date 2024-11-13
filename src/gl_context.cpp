#include "../include/tr/gl_context.hpp"

#include "../include/tr/function.hpp"

#include <GL/glew.h>
#include <SDL2/SDL.h>

namespace tr {
	// Initializes the GL context and GLEW.
	SDL_GLContext createContext(SDL_Window* window);
}

SDL_GLContext tr::createContext(SDL_Window* window)
{
	std::unique_ptr<void, FunctionCaller<&SDL_GL_DeleteContext>> context {SDL_GL_CreateContext(window)};
	if (context == nullptr) {
		throw GLContextCreationError {std::format("Failed to create OpenGL context: {}", SDL_GetError())};
	}

	glewExperimental = true;
	GLenum glewError;
	if ((glewError = glewInit()) != GLEW_OK) {
		throw GLContextCreationError {
			std::format("Failed to initialize GLEW: {}", (const char*)(glewGetErrorString(glewError)))
		};
	}

	return context.release();
}

tr::VSyncError::VSyncError()
	: SDLError {"Failed to set V-sync mode"}
{}

tr::GLContext::GLContext(Window& window)
	: _impl {createContext(window._impl)}
	, backbuffer {window}
{}

void tr::GLContext::Deleter::operator()(SDL_GLContext ptr) const noexcept
{
	SDL_GL_DeleteContext(ptr);
}

const char* tr::GLContext::vendorStr() const noexcept
{
	return (const char*)(glGetString(GL_VENDOR));
}

const char* tr::GLContext::rendererStr() const noexcept
{
	return (const char*)(glGetString(GL_RENDERER));
}

const char* tr::GLContext::versionStr() const noexcept
{
	return (const char*)(glGetString(GL_VERSION));
}

tr::VSync tr::GLContext::vsync() const noexcept
{
	return VSync(SDL_GL_GetSwapInterval());
}

void tr::GLContext::setVSync(VSync vsync)
{
	if (SDL_GL_SetSwapInterval(int(vsync)) < 0) {
		throw VSyncError {};
	}
}

void tr::GLContext::setFramebuffer(BasicFramebuffer& framebuffer) noexcept
{
	framebuffer.bindWrite();
}

void tr::GLContext::setShaderPipeline(const ShaderPipeline& pipeline) noexcept
{
	pipeline.bind();
}

void tr::GLContext::useFaceCulling(bool use) noexcept
{
	use ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void tr::GLContext::useScissorTest(bool use) noexcept
{
	use ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}

void tr::GLContext::setScissorBox(RectI2 rect) noexcept
{
	glScissor(rect.tl.x, rect.tl.y, rect.size.x, rect.size.y);
}

void tr::GLContext::useStencilTest(bool use) noexcept
{
	use ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

void tr::GLContext::setStencilTest(StencilFace face, Compare func, int comp, std::uint32_t mask) noexcept
{
	glStencilFuncSeparate(GLenum(face), GLenum(func), comp, mask);
}

void tr::GLContext::setStencilOperation(
	StencilFace      face,
	StencilOperation sfail,
	StencilOperation dfail,
	StencilOperation dpass
) noexcept
{
	glStencilOpSeparate(GLenum(face), GLenum(sfail), GLenum(dfail), GLenum(dpass));
}

void tr::GLContext::setStencilMask(StencilFace face, std::uint32_t mask) noexcept
{
	glStencilMaskSeparate(GLenum(face), mask);
}

void tr::GLContext::useDepthTest(bool use) noexcept
{
	use ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void tr::GLContext::setDepthTest(Compare func) noexcept
{
	glDepthFunc(GLenum(func));
}

void tr::GLContext::useBlending(bool use) noexcept
{
	use ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void tr::GLContext::setBlendingMode(BlendMode blendMode) noexcept
{
	glBlendEquationSeparate(GLenum(blendMode.rgbFn), GLenum(blendMode.alphaFn));
	glBlendFuncSeparate(
		GLenum(blendMode.rgbSrc),
		GLenum(blendMode.rgbDst),
		GLenum(blendMode.alphaSrc),
		GLenum(blendMode.alphaDst)
	);
}

void tr::GLContext::setBlendingColor(RGBAF clr)
{
	glBlendColor(clr.r, clr.g, clr.b, clr.a);
}

void tr::GLContext::setColorMask(bool red, bool green, bool blue, bool alpha) noexcept
{
	glColorMask(red, green, blue, alpha);
}

void tr::GLContext::setClearColor(RGBAF clr) noexcept
{
	glClearColor(clr.r, clr.g, clr.b, clr.a);
}

void tr::GLContext::setClearDepth(float depth) noexcept
{
	glClearDepth(depth);
}

void tr::GLContext::setClearStencil(int stencil) noexcept
{
	glClearStencil(stencil);
}

void tr::GLContext::clear(Clear components) noexcept
{
	glClear(GLbitfield(components));
}

void tr::GLContext::setVertexFormat(const VertexFormat& format) noexcept
{
	format.bind();
}

void tr::GLContext::setVertexBuffer(const VertexBuffer& buffer, std::size_t offset, std::size_t vertexStride) noexcept
{
	assert(buffer._buffer.has_value());
	assert(offset < buffer.size());
	glBindVertexBuffer(0, buffer._buffer->_id.get(), offset, vertexStride);
}

void tr::GLContext::setIndexBuffer(const IndexBuffer& buffer) noexcept
{
	assert(buffer._buffer.has_value());
	buffer._buffer->bind();
}

void tr::GLContext::draw(Primitive type, std::size_t offset, std::size_t vertices) noexcept
{
	glDrawArrays(GLenum(type), offset, vertices);
}

void tr::GLContext::drawInstances(Primitive type, std::size_t offset, std::size_t vertices, int instances) noexcept
{
	glDrawArraysInstanced(GLenum(type), offset, vertices, instances);
}

void tr::GLContext::drawIndexed(Primitive type, std::size_t offset, std::size_t indices) noexcept
{
	glDrawElements(GLenum(type), indices, GL_UNSIGNED_SHORT, (const void*)(offset));
}

void tr::GLContext::drawIndexedInstances(
	Primitive   type,
	std::size_t offset,
	std::size_t indices,
	int         instances
) noexcept
{
	glDrawElementsInstanced(GLenum(type), indices, GL_UNSIGNED_SHORT, (const void*)(offset), instances);
}
