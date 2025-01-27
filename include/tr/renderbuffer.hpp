#pragma once
#include "texture.hpp"

namespace tr {
	/** @ingroup graphics
	 *  @defgroup renderbuffer Renderbuffer
	 *  Renderbuffer class and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown if a failed renderbuffer allocation fails.
	 ******************************************************************************************************************/
	struct RenderbufferBadAlloc : std::bad_alloc {
		/**************************************************************************************************************
		 * Gets an error message.
		 *
		 * @return An explanatory error message.
		 **************************************************************************************************************/
		constexpr const char* what() const noexcept override;
	};

	/******************************************************************************************************************
	 * Image buffer optimized for use as a render target in a framebuffer.
	 ******************************************************************************************************************/
	class Renderbuffer {
	  public:
		/**************************************************************************************************************
		 * Allocates a blank renderbuffer.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception RenderbufferBadAlloc If allocating the renderbuffer fails.
		 *
		 * @param[in] size The size of the renderbuffer in pixels.
		 * @param[in] format The pixel format of the renderbuffer.
		 **************************************************************************************************************/
		Renderbuffer(glm::ivec2 size, TextureFormat format = TextureFormat::RGBA8);

		/**************************************************************************************************************
		 * Gets the size of the renderbuffer.
		 *
		 * @return The size of the renderbuffer in pixels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the renderbuffer.
		 *
		 * @param[in] label The new label of the renderbuffer.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;
		glm::ivec2                       _size;

		void bind() const noexcept;

		friend class Framebuffer;
	};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::RenderbufferBadAlloc::what() const noexcept
{
	return "failed renderbuffer allocation";
}

/// @endcond
