#pragma once
#include "framebuffer.hpp"
#include "index_buffer.hpp"
#include "shader_pipeline.hpp"
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"

struct SDL_Window;

namespace tr {
	/** @ingroup system
	 *  @defgroup graphics Graphics
	 *  Graphics context manipulation and drawing functionality.
	 *  @{
	 *  @}
	 */

	/** @ingroup graphics
	 *  @defgroup graphics_context Graphics Context
	 *  Graphics context manager and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Graphics context properties.
	 ******************************************************************************************************************/
	struct GraphicsProperties {
		/**************************************************************************************************************
		 * Whether a debug context should be used.
		 **************************************************************************************************************/
		bool debugContext = false;

		/**************************************************************************************************************
		 * The number of bits to use for backbuffer depth.
		 **************************************************************************************************************/
		std::uint8_t depthBits = 0;

		/**************************************************************************************************************
		 * The number of bits to use for backbuffer stencil.
		 **************************************************************************************************************/
		std::uint8_t stencilBits = 0;

		/**************************************************************************************************************
		 * The number of samples used around a pixel for multisampled anti-aliasing.
		 **************************************************************************************************************/
		std::uint8_t multisamples = 0;
	};

	/******************************************************************************************************************
	 * Stencil faces.
	 ******************************************************************************************************************/
	enum class StencilFace {
		/**************************************************************************************************************
		 * The front face of a polygon.
		 **************************************************************************************************************/
		FRONT = 0x4'04,

		/**************************************************************************************************************
		 * The back face of a polygon.
		 **************************************************************************************************************/
		BACK = 0x4'05,

		/**************************************************************************************************************
		 * Both faces of a polygon.
		 **************************************************************************************************************/
		BOTH = 0x4'08
	};

	/******************************************************************************************************************
	 * Stencil operations.
	 ******************************************************************************************************************/
	enum class StencilOperation {
		/**************************************************************************************************************
		 * Sets the stencil value to 0.
		 **************************************************************************************************************/
		ZERO,

		/**************************************************************************************************************
		 * Keeps the existing stencil value.
		 **************************************************************************************************************/
		KEEP = 0x1E'00,

		/**************************************************************************************************************
		 * Unconditionallz replaces the stencil value.
		 **************************************************************************************************************/
		REPLACE,

		/**************************************************************************************************************
		 * Increments the stencil value towards the maximum.
		 **************************************************************************************************************/
		INC,

		/**************************************************************************************************************
		 * Decrements the stencil value towards zero.
		 **************************************************************************************************************/
		DEC,

		/**************************************************************************************************************
		 * Inverts the bits of the stencil value.
		 **************************************************************************************************************/
		INVERT = 0x15'0A,

		/**************************************************************************************************************
		 * Increments the stencil value, wrapping around to 0,
		 **************************************************************************************************************/
		WRAPINC = 0x85'07,

		/**************************************************************************************************************
		 * Decrements the stencil value, wrapping around to the maximum.
		 **************************************************************************************************************/
		WRAPDEC
	};

	/******************************************************************************************************************
	 * Blending functions.
	 ******************************************************************************************************************/
	enum class BlendFunction {
		/**************************************************************************************************************
		 * The source and destination colors are added together.
		 **************************************************************************************************************/
		ADD = 0x80'06,

		/**************************************************************************************************************
		 * The minimum channel values of the source and destination colors are selected.
		 **************************************************************************************************************/
		MIN,

		/**************************************************************************************************************
		 * The maximum channel values of the source and destination colors are selected.
		 **************************************************************************************************************/
		MAX,

		/**************************************************************************************************************
		 * The source and destination colors are subtracted in the form (src - dst).
		 **************************************************************************************************************/
		SUBTRACT = 0x80'0A,

		/**************************************************************************************************************
		 * The source and destination colors are subtracted in the form (dst - src).
		 **************************************************************************************************************/
		REVERSE_SUBTRACT
	};

	/******************************************************************************************************************
	 * Blending parameter multipliers.
	 ******************************************************************************************************************/
	enum class BlendMultiplier {
		/**************************************************************************************************************
		 * The parameter is multiplied by 0.
		 **************************************************************************************************************/
		ZERO,

		/**************************************************************************************************************
		 * The parameter is multiplied by 1.
		 **************************************************************************************************************/
		ONE,

		/**************************************************************************************************************
		 * The parameter is multiplied by the source color.
		 **************************************************************************************************************/
		SRC_COLOR = 0x3'00,

		/**************************************************************************************************************
		 * The parameter is multiplied by 1 - the source color.
		 **************************************************************************************************************/
		ONE_MINUS_SRC_COLOR,

		/**************************************************************************************************************
		 * The parameter is multiplied by the source alpha.
		 **************************************************************************************************************/
		SRC_ALPHA,

		/**************************************************************************************************************
		 * The parameter is multiplied by 1 - the source alpha.
		 **************************************************************************************************************/
		ONE_MINUS_SRC_ALPHA,

		/**************************************************************************************************************
		 * The parameter is multiplied by the destination alpha.
		 **************************************************************************************************************/
		DST_ALPHA,

		/**************************************************************************************************************
		 * The parameter is multiplied by 1 - the destination alpha.
		 **************************************************************************************************************/
		ONE_MINUS_DST_ALPHA,

		/**************************************************************************************************************
		 * The parameter is multiplied by the destination color.
		 **************************************************************************************************************/
		DST_COLOR,

		/**************************************************************************************************************
		 * The parameter is multiplied by 1 - the destination color.
		 **************************************************************************************************************/
		ONE_MINUS_DST_COLOR,

		/**************************************************************************************************************
		 * TO-DO: figure out what this is.
		 **************************************************************************************************************/
		SRC_ALPHA_SATURATE,

		/**************************************************************************************************************
		 * The parameter is multiplied by the blending color constant.
		 **************************************************************************************************************/
		CONSTANT_COLOR = 0x80'01,

		/**************************************************************************************************************
		 * The parameter is multiplied by 1 - the blending color constant.
		 **************************************************************************************************************/
		ONE_MINUS_CONSTANT_COLOR,

		/**************************************************************************************************************
		 * The parameter is multiplied by the blending alpha constant.
		 **************************************************************************************************************/
		CONSTANT_ALPHA,

		/**************************************************************************************************************
		 * The parameter is multiplied by 1 - the blending alpha constant.
		 **************************************************************************************************************/
		ONE_MINUS_CONSTANT_ALPHA
	};

	/******************************************************************************************************************
	 * Blending mode information.
	 ******************************************************************************************************************/
	struct BlendMode {
		/**************************************************************************************************************
		 * The multiplier used for the source color.
		 **************************************************************************************************************/
		BlendMultiplier rgbSrc;

		/**************************************************************************************************************
		 * The operation applied with the source and destination colors.
		 **************************************************************************************************************/
		BlendFunction rgbFn;

		/**************************************************************************************************************
		 * The multiplier used for the destination color.
		 **************************************************************************************************************/
		BlendMultiplier rgbDst; // The parameter the destination color is multiplied with.

		/**************************************************************************************************************
		 * The multiplier used for the source alpha.
		 **************************************************************************************************************/
		BlendMultiplier alphaSrc;

		/**************************************************************************************************************
		 * The operation applied with the source and destination alpha.
		 **************************************************************************************************************/
		BlendFunction alphaFn;

		/**************************************************************************************************************
		 * The multiplier used for the destination alpha.
		 **************************************************************************************************************/
		BlendMultiplier alphaDst;
	};

	/******************************************************************************************************************
	 * Alpha blending mode.
	 ******************************************************************************************************************/
	inline constexpr BlendMode ALPHA_BLENDING{
		BlendMultiplier::SRC_ALPHA, BlendFunction::ADD, BlendMultiplier::ONE_MINUS_SRC_ALPHA,
		BlendMultiplier::ONE,       BlendFunction::ADD, BlendMultiplier::ZERO};

	/******************************************************************************************************************
	 * Premultiplied alpha blending mode.
	 ******************************************************************************************************************/
	inline constexpr BlendMode PREMUL_ALPHA_BLENDING{
		BlendMultiplier::ONE, BlendFunction::ADD, BlendMultiplier::ONE_MINUS_SRC_ALPHA,
		BlendMultiplier::ONE, BlendFunction::ADD, BlendMultiplier::ONE_MINUS_SRC_ALPHA};

	/******************************************************************************************************************
	 * Clearable components of the render target.
	 ******************************************************************************************************************/
	enum class Clear {
		/**************************************************************************************************************
		 * Clear the depth values.
		 **************************************************************************************************************/
		DEPTH = 0b00000001'00000000,

		/**************************************************************************************************************
		 * Clear the stencil values.
		 **************************************************************************************************************/
		STENCIL = 0b00000100'00000000,

		/**************************************************************************************************************
		 * Clear the color values.
		 **************************************************************************************************************/
		COLOR = 0b01000000'00000000,

		/**************************************************************************************************************
		 * Clear all values.
		 **************************************************************************************************************/
		ALL = 0b01000101'00000000
	};

	/// @cond IMPLEMENTATION
	DEFINE_BITMASK_OPERATORS(Clear);
	/// @endcond

	/******************************************************************************************************************
	 * Rendering primitives.
	 ******************************************************************************************************************/
	enum class Primitive {
		/**************************************************************************************************************
		 * The vertices are drawn as individual points.
		 **************************************************************************************************************/
		POINTS,

		/**************************************************************************************************************
		 * The vertices are drawn in pairs as lines.
		 **************************************************************************************************************/
		LINES,

		/**************************************************************************************************************
		 * The vertices are drawn as a continuous line loop.
		 **************************************************************************************************************/
		LINE_LOOP,

		/**************************************************************************************************************
		 * The vertices are drawn as a continuous line strip.
		 **************************************************************************************************************/
		LINE_STRIP,

		/**************************************************************************************************************
		 * The vertices are drawn in groups of three as triangles.
		 **************************************************************************************************************/
		TRIS,

		/**************************************************************************************************************
		 * The vertices are drawn as a continuous triangle strip.
		 **************************************************************************************************************/
		TRI_STRIP,

		/**************************************************************************************************************
		 * The vertices are drawn as a continuous triangle fan.
		 **************************************************************************************************************/
		TRI_FAN
	};

	/******************************************************************************************************************
	 * Graphics context.
	 *
	 * This class cannot be directly instantiated.
	 ******************************************************************************************************************/
	class GraphicsContext {
	  public:
		/**************************************************************************************************************
		 * Gets the context's vendor string.
		 *
		 * @return A vendor information C-string.
		 **************************************************************************************************************/
		const char* vendorInfo() const noexcept;

		/**************************************************************************************************************
		 * Gets the context's renderer string.
		 *
		 * @return A renderer information C-string.
		 **************************************************************************************************************/
		const char* rendererInfo() const noexcept;

		/**************************************************************************************************************
		 * Gets the context's version string.
		 *
		 * @return A version information C-string.
		 **************************************************************************************************************/
		const char* versionInfo() const noexcept;

		/**************************************************************************************************************
		 * Sets the target framebuffer.
		 *
		 * @param[in] framebuffer The new target framebuffer (user-defined or backbuffer).
		 **************************************************************************************************************/
		void setFramebuffer(BasicFramebuffer& framebuffer) noexcept;

		/**************************************************************************************************************
		 * Sets the active shader pipeline.
		 *
		 * @param[in] pipeline The new active shader pipeline.
		 **************************************************************************************************************/
		void setShaderPipeline(const ShaderPipeline& pipeline) noexcept;

		/**************************************************************************************************************
		 * Sets the active vertex format.
		 *
		 * @param[in] format The new active vertex format.
		 **************************************************************************************************************/
		void setVertexFormat(const VertexFormat& format) noexcept;

		/**************************************************************************************************************
		 * Sets the active vertex buffer.
		 *
		 * @param[in] buffer The new active vertex buffer.
		 * @param[in] offset The starting offset within the buffer (an out of bounds offset may trigger an assertion).
		 * @param[in] vertexStride The distance between consecutive vertices.
		 **************************************************************************************************************/
		void setVertexBuffer(const VertexBuffer& buffer, std::size_t offset, std::size_t vertexStride) noexcept;

		/**************************************************************************************************************
		 * Sets the active index buffer.
		 *
		 * @param[in] buffer The new active index buffer.
		 **************************************************************************************************************/
		void setIndexBuffer(const IndexBuffer& buffer) noexcept;

		/**************************************************************************************************************
		 * Sets whether face culling is performed.
		 *
		 * @param[in] use Whether face culling should be performed or not.
		 **************************************************************************************************************/
		void useFaceCulling(bool use) noexcept;

		/**************************************************************************************************************
		 * Sets whether the scissor test is performed.
		 *
		 * @param[in] use Whether the scissor test should be performed or not.
		 **************************************************************************************************************/
		void useScissorTest(bool use) noexcept;

		/**************************************************************************************************************
		 * Sets the box used in the scissor test.
		 *
		 * @param[in] rect The rectangle outside which to discard fragments.
		 **************************************************************************************************************/
		void setScissorBox(RectI2 rect) noexcept;

		/**************************************************************************************************************
		 * Sets whether the stencil test is performed.
		 *
		 * @param[in] use Whether the stencil test should be performed or not.
		 **************************************************************************************************************/
		void useStencilTest(bool use) noexcept;

		/**************************************************************************************************************
		 * Sets the function used for the stencil test.
		 *
		 * @param[in] face The pace(s) this function applies to.
		 * @param[in] func The function to be used for the depth test.
		 * @param[in] comp The value that will be compared against in the function.
		 * @param[in] mask The value of the stencil mask.
		 **************************************************************************************************************/
		void setStencilTest(StencilFace face, Compare func, int comp, std::uint32_t mask) noexcept;

		/**************************************************************************************************************
		 * Sets the operation performed on the stencil buffer in various circumstances.
		 *
		 * @param[in] face The pace(s) this function applies to.
		 * @param[in] sfail The operation used in case of a stencil test fail.
		 * @param[in] dfail The operation used in case of a depth test fail.
		 * @param[in] dpass The operation used in case of a depth test success.
		 **************************************************************************************************************/
		void setStencilOperation(StencilFace face, StencilOperation sfail, StencilOperation dfail,
								 StencilOperation dpass) noexcept;

		/**************************************************************************************************************
		 * Sets the stencil mask that enables and disables writing of individual bits in stencil buffers.
		 *
		 * @param[in] face The pace(s) this mask applies to.
		 * @param[in] mask A bitmask where 0 means the bit cannot be written and 1 means it can be written.
		 **************************************************************************************************************/
		void setStencilMask(StencilFace face, std::uint32_t mask) noexcept;

		/**************************************************************************************************************
		 * Sets whether the depth test is performed.
		 *
		 * @param[in] use Whether the depth test should be performed or not.
		 **************************************************************************************************************/
		void useDepthTest(bool use) noexcept;

		/**************************************************************************************************************
		 * Sets the function used for the depth test.
		 *
		 * @param[in] func The function to use for the depth test.
		 **************************************************************************************************************/
		void setDepthTest(Compare func) noexcept;

		/**************************************************************************************************************
		 * Sets whether blending is performed.
		 *
		 * @param[in] use Whether blending should be performed or not.
		 **************************************************************************************************************/
		void useBlending(bool use) noexcept;

		/**************************************************************************************************************
		 * Sets the active blending mode.
		 *
		 * @param[in] blendMode the blending mode to use.
		 **************************************************************************************************************/
		void setBlendingMode(BlendMode blendMode) noexcept;

		/**************************************************************************************************************
		 * Sets the constant blending color.
		 *
		 * @param[in] color the blending color to use.
		 **************************************************************************************************************/
		void setBlendingColor(RGBAF color);

		/**************************************************************************************************************
		 * Sets the color mask that determines the writing of frame buffer color components.
		 *
		 * @param[in] red, green, blue, alpha Whether writing to a color channel should be enabled.
		 **************************************************************************************************************/
		void setColorMask(bool red, bool green, bool blue, bool alpha) noexcept;

		/**************************************************************************************************************
		 * Sets the clear color value.
		 *
		 * @param[in] color The new clear color.
		 **************************************************************************************************************/
		void setClearColor(RGBAF color) noexcept;

		/**************************************************************************************************************
		 * Sets the clear depth value.
		 *
		 * @param[in] depth The new clear depth.
		 **************************************************************************************************************/
		void setClearDepth(float depth) noexcept;

		/**************************************************************************************************************
		 * Sets the clear stencil value.
		 *
		 * @param[in] stencil The new clear stencil.
		 **************************************************************************************************************/
		void setClearStencil(int stencil) noexcept;

		/**************************************************************************************************************
		 * Clears the current render target.
		 *
		 * @param[in] components The drawing components to clear (by default ALL).
		 **************************************************************************************************************/
		void clear(Clear components = Clear::ALL) noexcept;

		/**************************************************************************************************************
		 * Draws a mesh from a vertex buffer.
		 *
		 * @param[in] type The type of primitive to draw.
		 * @param[in] offset The offset within the vertex buffer.
		 * @param[in] vertices The number of vertices to draw.
		 **************************************************************************************************************/
		void draw(Primitive type, std::size_t offset, std::size_t vertices) noexcept;

		/**************************************************************************************************************
		 * Draws an instanced mesh from a vertex buffer.
		 *
		 * @param[in] type The type of primitive to draw.
		 * @param[in] offset The offset within the vertex buffer.
		 * @param[in] vertices The number of vertices to draw.
		 * @param[in] instances The number of instances to draw.
		 **************************************************************************************************************/
		void drawInstances(Primitive type, std::size_t offset, std::size_t vertices, int instances) noexcept;

		/**************************************************************************************************************
		 * Draws an indexed mesh.
		 *
		 * @param[in] type The type of primitive to draw.
		 * @param[in] offset The offset within the index buffer.
		 * @param[in] indices The number of indices to draw.
		 **************************************************************************************************************/
		void drawIndexed(Primitive type, std::size_t offset, std::size_t indices) noexcept;

		/**************************************************************************************************************
		 * Draws an instanced indexed mesh.
		 *
		 * @param[in] type The type of primitive to draw.
		 * @param[in] offset The offset within the index buffer.
		 * @param[in] indices The number of indices to draw.
		 * @param[in] instances The number of instances to draw.
		 **************************************************************************************************************/
		void drawIndexedInstances(Primitive type, std::size_t offset, std::size_t indices, int instances) noexcept;

		/**************************************************************************************************************
		 * Swaps the display's front and back buffers.
		 **************************************************************************************************************/
		void swap() noexcept;

	  private:
		struct Deleter {
			void operator()(void* ptr) const noexcept;
		};

		std::unique_ptr<void, Deleter> _impl;

		GraphicsContext(SDL_Window* window);

		friend class Window;
	};

	/// @}
} // namespace tr
