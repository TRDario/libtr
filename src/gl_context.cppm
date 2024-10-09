module;
#include <cassert>
#include <SDL2/SDL.h>
#include <EnumBitmask.hpp>
#include <GL/glew.h>

export module tr:gl_context;

import std;
import :color;
import :framebuffer;
import :geometry;
import :handle;
import :index_buffer;
import :integer;
import :sampler;
import :sdl;
import :shader;
import :vertex_buffer;
import :vertex_format;
import :window;

export namespace tr {
    // Enum representing a stencil face.
	enum class StencilFace {
		FRONT = 0x404, // The front face of a polygon.
		BACK  = 0x405, // The back face of the polygon.
		BOTH  = 0x408  // Both faces of the polygon.
	};
	// Enum representing a stencil operation.
	enum class StencilOperation {
		ZERO,             // Stets the stencil buffer value to 0.
		KEEP    = 0x1E00, // Keeps the current value.
		REPLACE,          // Sets the stencil buffer value to comp.
		INC,              // Increments the current stencil buffer value up to the maximum.
		DEC,              // Decrements the current stencil buffer value down to 0.
		INVERT  = 0x150A, // Bitwise inverts the current stencil buffer value.
		WRAPINC = 0x8507, // Increments the current stencil buffer value, wrapping to 0 on overflow.
		WRAPDEC           // Decrements the current stencil buffer value, wrapping to the maximum on underflow.
	};

	// Enum representing a blending function.
	enum class BlendFunction {
		ADD      = 0x8006, // The source and destination colors are added together.
		MIN,               // The minimum channel values of the source and destination colors are selected.
		MAX,               // The maximum channel values of the source and destination colors are selected.
		SUBTRACT = 0x800A, // The source and destination colors are subtracted in the form (src - dst).
		REVERSE_SUBTRACT   // The source and destination colors are subtracted in the form (dst - src).
	};
	// Enum representing a blending parameter.
	enum class BlendParameter {
		ZERO,                     // The parameter is multiplied by 0.
		ONE,                      // The parameter is multiplied by 1.
		SRC_COLOR = 0x300,        // The parameter is multiplied by the source color.
		ONE_MINUS_SRC_COLOR,      // The parameter is multiplied by 1 - the source color.
		SRC_ALPHA,                // The parameter is multiplied by the source alpha.
		ONE_MINUS_SRC_ALPHA,      // The parameter is multiplied by 1 - the source alpha.
		DST_ALPHA,                // The parameter is multiplied by the destination alpha.
		ONE_MINUS_DST_ALPHA,      // The parameter is multiplied by 1 - the destination alpha.
		DST_COLOR,                // The parameter is multiplied by the destination color.
		ONE_MINUS_DST_COLOR,      // The parameter is multiplied by 1 - the destination color.
		SRC_ALPHA_SATURATE,       // ???
		CONSTANT_COLOR = 0x8001,  // The parameter is multiplied by the blend color.
		ONE_MINUS_CONSTANT_COLOR, // The parameter is multiplied by 1 - the blend color.
		CONSTANT_ALPHA,           // The parameter is multiplied by the blend color alpha.
		ONE_MINUS_CONSTANT_ALPHA  // The parameter is multiplied by 1 - the blend color alpha.
	};
	// Struct representing a blending mode.
	struct BlendMode {
		BlendParameter rgbSrc;   // The parameter the source color is multiplied with.
		BlendFunction  rgbFn;    // The operation applied with the source and destination colors.
		BlendParameter rgbDst;   // The parameter the destination color is multiplied with.
		BlendParameter alphaSrc; // The parameter the source alpha is multiplied with.
		BlendFunction  alphaFn;  // The operation applied with the source and destination alpha.
		BlendParameter alphaDst; // The parameter the destination alpha is multiplied with.
	};

	// Blending mode resulting in alpha blending.
	inline constexpr BlendMode ALPHA_BLENDING {
		BlendParameter::SRC_ALPHA, BlendFunction::ADD, BlendParameter::ONE_MINUS_SRC_ALPHA,
		BlendParameter::ONE,       BlendFunction::ADD, BlendParameter::ZERO
	};
	// Blending mode resulting in premultiplied alpha blending.
	inline constexpr BlendMode PREMUL_ALPHA_BLENDING {
		BlendParameter::ONE, BlendFunction::ADD, BlendParameter::ONE_MINUS_SRC_ALPHA,
		BlendParameter::ONE, BlendFunction::ADD, BlendParameter::ONE_MINUS_SRC_ALPHA
	};

	// Enum representing a clearable component of the render target.
	enum class Clear {
		DEPTH = 0b0000000100000000,
		STENCIL = 0b0000010000000000,
		COLOR = 0b0100000000000000,
		ALL = 0b0100010100000000
	};
	DEFINE_BITMASK_OPERATORS(Clear);

    // Enum representing vsync behavior.
	enum class VSync : Si8 {
		ADAPTIVE = -1, // Vsync is enabled, but late swaps happen immediately instead of waiting for the next retrace.
		DISABLED,      // Vsync is disabled.
		ENABLED        // Vsync is enabled.
	};

    // Enum representing a type of primitive.
    enum class Primitive {
        POINTS,     // The vertices are drawn as individual points.
        LINES,      // The vertices are drawn in pairs as lines.
        LINE_LOOP,  // The vertices are drawn as a continuous line loop.
        LINE_STRIP, // The vertices are drawn as a continuous line strip.
        TRIS,       // The vertices are drawn in groups of three as triangles.
        TRI_STRIP,  // The vertices are drawn as a continuous triangle strip.
        TRI_FAN     // The vertices are drawn as a continuous triangle fan.
    };

    // OpenGL context class.
    class GLContext {
    public:
        GLContext(Window& window);

        // Gets the context's vendor string.
        const char* vendorStr() const noexcept;
        // Gets the context's renderer string.
        const char* rendererStr() const noexcept;
        // Gets the context's version string.
        const char* versionStr() const noexcept;

        // Gets the V-sync mode for the context.
        VSync vsync() const noexcept;
        // Sets the V-sync mode of the context.
        bool setVSync(VSync vsync) noexcept;

        // Sets the target framebuffer.
		void setFramebuffer(BasicFramebuffer& framebuffer) noexcept;

		// Sets the context's active vertex shader.
		void setVertexShader(Shader& shader) noexcept;
		// Sets the context's active fragment shader.
		void setFragmentShader(Shader& shader) noexcept;

        // Sets the context's active vertex format.
        void setVertexFormat(const VertexFormat& format) noexcept;

        // Sets the context's active vertex buffer.
        void setVertexBuffer(VertexBuffer& buffer, VertexBuffer::Size offset, Size vertexStride) noexcept;
        // Sets the context's active index buffer.
        void setIndexBuffer(IndexBuffer& buffer) noexcept;

		// Sets whether face culling is performed.
		void useFaceCulling(bool use) noexcept;

		// Sets whether the scissor test is performed.
		void useScissorTest(bool use) noexcept;
		// Sets the box used in the scissor test.
		void setScissorBox(RectI2 rect) noexcept;

		// Sets whether the stencil test is performed.
		void useStencilTest(bool use) noexcept;
		// Sets the function used for the stencil test.
		// face - The face(s) this function applies to.
		// func - The function used for the depth test.
		// comp - The value that will be compared against in the function.
		// mask - The value of the stencil mask.
		void setStencilTest(StencilFace face, Compare func, int comp, Ui32 mask) noexcept;
		// Sets the operation performed on the stencil buffer in various circumstances. 
		// face - The face(s) this function applies to.
		// sfail - The operation used in case of a stencil test fail.
		// dfail - The operation used in case of a depth test fail.
		// dpass - The operation used in case of a depth test success.
		void setStencilOperation(StencilFace face, StencilOperation sfail, StencilOperation dfail, StencilOperation dpass) noexcept;
		// Sets the stencil mask that enables and disables writing of individual bits in stencil buffers.
		// face - The face(s) this mask applies to.
		// mask - A bitmask where 0 means the bit cannot be written and 1 means it can be written.
		void setStencilMask(StencilFace face, Ui32 mask) noexcept;

		// Sets whether the depth test is performed.
		void useDepthTest(bool use) noexcept;
		// Sets the function used for the depth test.
		void setDepthTest(Compare func) noexcept;

		// Sets whether blending is performed.
		void useBlending(bool use) noexcept;
		// Sets the blending mode.
		void setBlendingMode(BlendMode BlendMode) noexcept;
		// Sets the constant blending color.
		void setBlendingColor(RGBAF color);
		// Sets the color mask that determines the writing of frame buffer color components.
		void setColorMask(bool red, bool green, bool blue, bool alpha) noexcept;

		// Sets the clear color.
		void setClearColor(RGBAF color) noexcept;
		// Sets the clear depth.
		void setClearDepth(float depth) noexcept;
		// Sets the clear stencil.
		void setClearStencil(int stencil) noexcept;
		// Clears the render target.
		// components - The components to be cleared, multiple may be ORed together.
		void clear(Clear components) noexcept;

        void draw(Primitive type, VertexBuffer::Size offset, VertexBuffer::Size vertices);
        void drawInstances(Primitive type, VertexBuffer::Size offset, VertexBuffer::Size vertices, int instances);
        void drawIndexed(Primitive type, IndexBuffer::Size offset, IndexBuffer::Size indices);
        void drawIndexedInstances(Primitive type, IndexBuffer::Size offset, IndexBuffer::Size indices, int instances);
    private:
        struct Deleter { void operator()(SDL_GLContext context) noexcept; };
        struct PipelineDeleter { void operator()(unsigned int id) noexcept; };
        std::unique_ptr<void, Deleter>           _impl;
        Handle<unsigned int, 0, PipelineDeleter> _pipeline;
    public:
        Backbuffer backbuffer;
    };
}

// IMPLEMENTATION

namespace tr {
    // Initializes the GL context and GLEW.
    SDL_GLContext createContext(SDL_Window* window);
    // Creates a programmable pipeline object.
    unsigned int createPipeline() noexcept;
}

SDL_GLContext tr::createContext(SDL_Window* window)
{
    SDL_GLContext context { SDL_GL_CreateContext(window) };
    if (context == nullptr) {
        throw SDLError { "Failed to create OpenGL context" };
    }

    glewExperimental = true;
    GLenum glewError;
    if ((glewError = glewInit()) != GLEW_OK) {
        throw std::runtime_error { std::format("Failed to initialize GLEW ({}).", (const char*)(glewGetErrorString(glewError))) };
    }

    return context;
}

unsigned int tr::createPipeline() noexcept
{
    unsigned int id;
    glGenProgramPipelines(1, &id);
    glBindProgramPipeline(id);
    #ifndef NDEBUG
    constexpr std::string_view PIPELINE_NAME { "(tr) Program Pipeline" };
    glObjectLabel(GL_PROGRAM_PIPELINE, id, PIPELINE_NAME.size(), PIPELINE_NAME.data());
    #endif
    return id;
}

tr::GLContext::GLContext(Window& window)
    : _impl { createContext(window._impl) }
    , _pipeline { createPipeline() }
    , backbuffer { window }
{}

void tr::GLContext::Deleter::operator()(SDL_GLContext context) noexcept
{
    SDL_GL_DeleteContext(context);
}

void tr::GLContext::PipelineDeleter::operator()(unsigned int id) noexcept
{
    glDeleteProgramPipelines(1, &id);
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

bool tr::GLContext::setVSync(VSync vsync) noexcept
{
    if (SDL_GL_SetSwapInterval(int(vsync)) < 0) {
        // Fallback to regular VSync if adaptive VSync doesn't work.
        if (vsync == VSync::ADAPTIVE) {
            return setVSync(VSync::ENABLED);
        }
        return false;
    }
    return true;
}

void tr::GLContext::setFramebuffer(BasicFramebuffer& framebuffer) noexcept
{
    framebuffer.bindWrite();
}

void tr::GLContext::setVertexShader(Shader& shader) noexcept
{
    assert(shader.type() == Shader::Type::VERTEX);
    glUseProgramStages(_pipeline.get(), GL_VERTEX_SHADER_BIT, shader._id.get());
}

void tr::GLContext::setFragmentShader(Shader& shader) noexcept
{
    assert(shader.type() == Shader::Type::FRAGMENT);
    glUseProgramStages(_pipeline.get(), GL_FRAGMENT_SHADER_BIT, shader._id.get());
}

void useFaceCulling(bool use) noexcept
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

void tr::GLContext::setStencilTest(StencilFace face, Compare func, int comp, Ui32 mask) noexcept
{
    glStencilFuncSeparate(GLenum(face), GLenum(func), comp, mask);
}

void tr::GLContext::setStencilOperation(StencilFace face, StencilOperation sfail, StencilOperation dfail, StencilOperation dpass) noexcept
{
    glStencilOpSeparate(GLenum(face), GLenum(sfail), GLenum(dfail), GLenum(dpass));
}

void tr::GLContext::setStencilMask(StencilFace face, Ui32 mask) noexcept
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

void tr::GLContext::setBlendingMode(BlendMode BlendMode) noexcept
{
    glBlendEquationSeparate(GLenum(BlendMode.rgbFn), GLenum(BlendMode.alphaFn));
    glBlendFuncSeparate(GLenum(BlendMode.rgbSrc), GLenum(BlendMode.rgbDst), GLenum(BlendMode.alphaSrc), GLenum(BlendMode.alphaDst));
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

void tr::GLContext::setVertexBuffer(VertexBuffer& buffer, VertexBuffer::Size offset, Size vertexStride) noexcept
{
	assert(buffer._buffer.has_value());
    glBindVertexBuffer(0, buffer._buffer->_id.get(), offset, vertexStride);
}

void tr::GLContext::setIndexBuffer(IndexBuffer& buffer) noexcept
{
	assert(buffer._buffer.has_value());
    buffer._buffer->bind();
}

void tr::GLContext::draw(Primitive type, VertexBuffer::Size offset, VertexBuffer::Size vertices)
{
    glDrawArrays(GLenum(type), offset, vertices);
}

void tr::GLContext::drawInstances(Primitive type, VertexBuffer::Size offset, VertexBuffer::Size vertices, int instances)
{
    glDrawArraysInstanced(GLenum(type), offset, vertices, instances);
}

void tr::GLContext::drawIndexed(Primitive type, IndexBuffer::Size offset, IndexBuffer::Size indices)
{
    glDrawElements(GLenum(type), indices, GL_UNSIGNED_SHORT, (const void*)(offset));
}

void tr::GLContext::drawIndexedInstances(Primitive type, IndexBuffer::Size offset, IndexBuffer::Size indices, int instances)
{
    glDrawElementsInstanced(GLenum(type), indices, GL_UNSIGNED_SHORT, (const void*)(offset), instances);
}