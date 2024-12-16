#include "../include/tr/graphics_context.hpp"
#include "../include/tr/window.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
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

	glewExperimental = true;
	GLenum glewError;
	if ((glewError = glewInit()) != GLEW_OK) {
		SDL_GL_DeleteContext(context);
		auto error{(const char*)(glewGetErrorString(glewError))};
		throw WindowOpenError{std::format("Failed to initialize GLEW: {}", error)};
	}

	return context;
}

tr::GraphicsContext::GraphicsContext(SDL_Window* window)
	: _impl{createContext(window)}
{
}

void tr::GraphicsContext::Deleter::operator()(SDL_GLContext ptr) const noexcept
{
	SDL_GL_DeleteContext(ptr);
}

const char* tr::GraphicsContext::vendorInfo() const noexcept
{
	return (const char*)(glGetString(GL_VENDOR));
}

const char* tr::GraphicsContext::rendererInfo() const noexcept
{
	return (const char*)(glGetString(GL_RENDERER));
}

const char* tr::GraphicsContext::versionInfo() const noexcept
{
	return (const char*)(glGetString(GL_VERSION));
}

tr::VSync tr::GraphicsContext::vsync() const noexcept
{
	return VSync(SDL_GL_GetSwapInterval());
}

void tr::GraphicsContext::setVSync(VSync vsync)
{
	if (SDL_GL_SetSwapInterval(int(vsync)) < 0) {
		throw WindowError{"Failed to set V-sync mode"};
	}
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
	use ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void tr::GraphicsContext::useScissorTest(bool use) noexcept
{
	use ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}

void tr::GraphicsContext::setScissorBox(RectI2 rect) noexcept
{
	glScissor(rect.tl.x, rect.tl.y, rect.size.x, rect.size.y);
}

void tr::GraphicsContext::useStencilTest(bool use) noexcept
{
	use ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

void tr::GraphicsContext::setStencilTest(StencilFace face, Compare func, int comp, std::uint32_t mask) noexcept
{
	glStencilFuncSeparate(GLenum(face), GLenum(func), comp, mask);
}

void tr::GraphicsContext::setStencilOperation(StencilFace face, StencilOperation sfail, StencilOperation dfail,
											  StencilOperation dpass) noexcept
{
	glStencilOpSeparate(GLenum(face), GLenum(sfail), GLenum(dfail), GLenum(dpass));
}

void tr::GraphicsContext::setStencilMask(StencilFace face, std::uint32_t mask) noexcept
{
	glStencilMaskSeparate(GLenum(face), mask);
}

void tr::GraphicsContext::useDepthTest(bool use) noexcept
{
	use ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void tr::GraphicsContext::setDepthTest(Compare func) noexcept
{
	glDepthFunc(GLenum(func));
}

void tr::GraphicsContext::useBlending(bool use) noexcept
{
	use ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void tr::GraphicsContext::setBlendingMode(BlendMode blendMode) noexcept
{
	glBlendEquationSeparate(GLenum(blendMode.rgbFn), GLenum(blendMode.alphaFn));
	glBlendFuncSeparate(GLenum(blendMode.rgbSrc), GLenum(blendMode.rgbDst), GLenum(blendMode.alphaSrc),
						GLenum(blendMode.alphaDst));
}

void tr::GraphicsContext::setBlendingColor(RGBAF clr)
{
	glBlendColor(clr.r, clr.g, clr.b, clr.a);
}

void tr::GraphicsContext::setColorMask(bool red, bool green, bool blue, bool alpha) noexcept
{
	glColorMask(red, green, blue, alpha);
}

void tr::GraphicsContext::setClearColor(RGBAF clr) noexcept
{
	glClearColor(clr.r, clr.g, clr.b, clr.a);
}

void tr::GraphicsContext::setClearDepth(float depth) noexcept
{
	glClearDepth(depth);
}

void tr::GraphicsContext::setClearStencil(int stencil) noexcept
{
	glClearStencil(stencil);
}

void tr::GraphicsContext::clear(Clear components) noexcept
{
	glClear(GLbitfield(components));
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
	glBindVertexBuffer(0, buffer._buffer->_id.get(), offset, vertexStride);
}

void tr::GraphicsContext::setIndexBuffer(const IndexBuffer& buffer) noexcept
{
	assert(buffer._buffer.has_value());
	buffer._buffer->bind();
}

void tr::GraphicsContext::draw(Primitive type, std::size_t offset, std::size_t vertices) noexcept
{
	glDrawArrays(GLenum(type), offset, vertices);
}

void tr::GraphicsContext::drawInstances(Primitive type, std::size_t offset, std::size_t vertices,
										int instances) noexcept
{
	glDrawArraysInstanced(GLenum(type), offset, vertices, instances);
}

void tr::GraphicsContext::drawIndexed(Primitive type, std::size_t offset, std::size_t indices) noexcept
{
	glDrawElements(GLenum(type), indices, GL_UNSIGNED_SHORT, (const void*)(offset));
}

void tr::GraphicsContext::drawIndexedInstances(Primitive type, std::size_t offset, std::size_t indices,
											   int instances) noexcept
{
	glDrawElementsInstanced(GLenum(type), indices, GL_UNSIGNED_SHORT, (const void*)(offset), instances);
}

void tr::GraphicsContext::swap() noexcept
{
	assert(_impl.get() != nullptr);
	SDL_GL_SwapWindow(window()._impl.get());
}