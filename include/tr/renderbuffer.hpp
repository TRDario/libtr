/**
 * @file renderbuffer.hpp
 * @brief Provides a renderbuffer class.
 */

#pragma once
#include "texture.hpp"

namespace tr {
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
     * Wrapper over an OpenGL renderbuffer. An OpenGL context must be open to instantiate and use objects of this type.
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
		bool       operator==(const Renderbuffer& r) const noexcept;

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
		void       setLabel(std::string_view label) noexcept;

	private:
		struct Deleter {
			void operator()(unsigned int id) const noexcept;
		};

		Handle<unsigned int, 0, Deleter> _id;
		glm::ivec2                       _size;

		void                             bind() const noexcept;

		friend class Framebuffer;
	};
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::RenderbufferBadAlloc::what() const noexcept
{
	return "failed renderbuffer allocation";
}

/// @endcond
