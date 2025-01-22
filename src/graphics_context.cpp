#include "../include/tr/graphics_context.hpp"
#include "../include/tr/window.hpp"
#include "gl_call.hpp"
#include <SDL3/SDL.h>
#include <format>

namespace tr {
	// Initializes the GL context and GLEW.
	SDL_GLContext createContext(SDL_Window* window);
} // namespace tr

SDL_GLContext tr::createContext(SDL_Window* window)
{
	SDL_GLContext context{SDL_GL_CreateContext(window)};
	if (context == nullptr) {
		throw WindowOpenError{std::format("Failed to create OpenGL context: {}", SDL_GetError())};
	}
	else if (!gladLoadGLLoader((void* (*)(const char*))(SDL_GL_GetProcAddress))) {
		throw WindowOpenError{"Failed to load OpenGL 4.6."};
	}
	return context;
}

tr::GraphicsContext::GraphicsContext(SDL_Window* window)
	: _impl{createContext(window)}
{
}

void tr::GraphicsContext::Deleter::operator()(SDL_GLContext ptr) const noexcept
{
	SDL_GL_DestroyContext(ptr);
}

const char* tr::GraphicsContext::vendorInfo() const noexcept
{
	return (const char*)(TR_RETURNING_GL_CALL(glGetString, GL_VENDOR));
}

const char* tr::GraphicsContext::rendererInfo() const noexcept
{
	return (const char*)(TR_RETURNING_GL_CALL(glGetString, GL_RENDERER));
}

const char* tr::GraphicsContext::versionInfo() const noexcept
{
	return (const char*)(TR_RETURNING_GL_CALL(glGetString, GL_VERSION));
}

void tr::GraphicsContext::setViewport(const RectI2& viewport) noexcept
{
	TR_GL_CALL(glViewport, viewport.tl.x, viewport.tl.y, viewport.size.x, viewport.size.y);
}

void tr::GraphicsContext::setDepthRange(double min, double max) noexcept
{
	TR_GL_CALL(glDepthRange, min, max);
}

void tr::GraphicsContext::setFramebuffer(BasicFramebuffer& framebuffer) noexcept
{
	framebuffer.bindWrite();
}

void tr::GraphicsContext::setShaderPipeline(const ShaderPipeline& pipeline) noexcept
{
	pipeline.bind();
}

void tr::GraphicsContext::useFaceCulling(bool use) noexcept
{
	if (use) {
		TR_GL_CALL(glEnable, GL_CULL_FACE);
	}
	else {
		TR_GL_CALL(glDisable, GL_CULL_FACE);
	}
}

void tr::GraphicsContext::useScissorTest(bool use) noexcept
{
	if (use) {
		TR_GL_CALL(glEnable, GL_SCISSOR_TEST);
	}
	else {
		TR_GL_CALL(glDisable, GL_SCISSOR_TEST);
	}
}

void tr::GraphicsContext::setScissorBox(const RectI2& rect) noexcept
{
	TR_GL_CALL(glScissor, rect.tl.x, rect.tl.y, rect.size.x, rect.size.y);
}

void tr::GraphicsContext::useStencilTest(bool use) noexcept
{
	if (use) {
		TR_GL_CALL(glEnable, GL_STENCIL_TEST);
	}
	else {
		TR_GL_CALL(glDisable, GL_STENCIL_TEST);
	}
}

void tr::GraphicsContext::setStencilTest(StencilFace face, Compare func, int comp, std::uint32_t mask) noexcept
{
	TR_GL_CALL(glStencilFuncSeparate, GLenum(face), GLenum(func), comp, mask);
}

void tr::GraphicsContext::setStencilOperation(StencilFace face, StencilOperation sfail, StencilOperation dfail,
											  StencilOperation dpass) noexcept
{
	TR_GL_CALL(glStencilOpSeparate, GLenum(face), GLenum(sfail), GLenum(dfail), GLenum(dpass));
}

void tr::GraphicsContext::setStencilMask(StencilFace face, std::uint32_t mask) noexcept
{
	TR_GL_CALL(glStencilMaskSeparate, GLenum(face), mask);
}

void tr::GraphicsContext::useDepthTest(bool use) noexcept
{
	if (use) {
		TR_GL_CALL(glEnable, GL_DEPTH_TEST);
	}
	else {
		TR_GL_CALL(glDisable, GL_DEPTH_TEST);
	}
}

void tr::GraphicsContext::setDepthTest(Compare func) noexcept
{
	TR_GL_CALL(glDepthFunc, GLenum(func));
}

void tr::GraphicsContext::useBlending(bool use) noexcept
{
	if (use) {
		TR_GL_CALL(glEnable, GL_BLEND);
	}
	else {
		TR_GL_CALL(glDisable, GL_BLEND);
	}
}

void tr::GraphicsContext::setBlendingMode(BlendMode blendMode) noexcept
{
	TR_GL_CALL(glBlendEquationSeparate, GLenum(blendMode.rgbFn), GLenum(blendMode.alphaFn));
	TR_GL_CALL(glBlendFuncSeparate, GLenum(blendMode.rgbSrc), GLenum(blendMode.rgbDst), GLenum(blendMode.alphaSrc),
			   GLenum(blendMode.alphaDst));
}

void tr::GraphicsContext::setBlendingColor(RGBAF clr)
{
	TR_GL_CALL(glBlendColor, clr.r, clr.g, clr.b, clr.a);
}

void tr::GraphicsContext::setColorMask(bool red, bool green, bool blue, bool alpha) noexcept
{
	TR_GL_CALL(glColorMask, red, green, blue, alpha);
}

void tr::GraphicsContext::setClearColor(RGBAF clr) noexcept
{
	TR_GL_CALL(glClearColor, clr.r, clr.g, clr.b, clr.a);
}

void tr::GraphicsContext::setClearDepth(float depth) noexcept
{
	TR_GL_CALL(glClearDepth, depth);
}

void tr::GraphicsContext::setClearStencil(int stencil) noexcept
{
	TR_GL_CALL(glClearStencil, stencil);
}

void tr::GraphicsContext::clear(Clear components) noexcept
{
	TR_GL_CALL(glClear, GLbitfield(components));
}

void tr::GraphicsContext::setVertexFormat(const VertexFormat& format) noexcept
{
	format.bind();
}

void tr::GraphicsContext::setVertexBuffer(const VertexBuffer& buffer, std::size_t offset,
										  std::size_t vertexStride) noexcept
{
	assert(buffer._buffer.has_value());
	assert(offset < buffer.size());
	TR_GL_CALL(glBindVertexBuffer, 0, buffer._buffer->_id.get(), offset, vertexStride);
}

void tr::GraphicsContext::setIndexBuffer(const IndexBuffer& buffer) noexcept
{
	assert(buffer._buffer.has_value());
	buffer._buffer->bind();
}

void tr::GraphicsContext::draw(Primitive type, std::size_t offset, std::size_t vertices) noexcept
{
	TR_GL_CALL(glDrawArrays, GLenum(type), offset, vertices);
}

void tr::GraphicsContext::drawInstances(Primitive type, std::size_t offset, std::size_t vertices,
										int instances) noexcept
{
	TR_GL_CALL(glDrawArraysInstanced, GLenum(type), offset, vertices, instances);
}

void tr::GraphicsContext::drawIndexed(Primitive type, std::size_t offset, std::size_t indices) noexcept
{
	TR_GL_CALL(glDrawElements, GLenum(type), indices, GL_UNSIGNED_SHORT, (const void*)(offset * sizeof(std::uint16_t)));
}

void tr::GraphicsContext::drawIndexedInstances(Primitive type, std::size_t offset, std::size_t indices,
											   int instances) noexcept
{
	TR_GL_CALL(glDrawElementsInstanced, GLenum(type), indices, GL_UNSIGNED_SHORT,
			   (const void*)(offset * sizeof(std::uint16_t)), instances);
}

void tr::GraphicsContext::swap() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_GL_SwapWindow(window()._impl.get());
}