#pragma once
#include "geometry.hpp"

namespace tr {
	class Bitmap;
	class BitmapFormat;
	class ColorTexture1D;
	class ColorTexture2D;
	class ColorTexture3D;
	class ArrayColorTexture1D;
	class ArrayColorTexture2D;
	class Window;

	/** @ingroup graphics
	 *  @defgroup framebuffer Framebuffers
	 *  Framebuffer classes and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Base framebuffer class.
	 *
	 * @note This class cannot be instantiated directly.
	 ******************************************************************************************************************/
	class BasicFramebuffer {
	  public:
		/**************************************************************************************************************
		 * Gets the size of the framebuffer.
		 *
		 * @return The size of the framebuffer in pixels.
		 **************************************************************************************************************/
		virtual glm::ivec2 size() const noexcept = 0;

		/**************************************************************************************************************
		 * Reads pixels into a bitmap.
		 *
		 * The image is copied from the color attachments bound to the framebuffer.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] rect The rect of the framebuffer to copy.
		 * @param[in] format The format to use for the bitmap pixels.
		 *
		 * @return A bitmap containing the pixel data.
		 **************************************************************************************************************/
		Bitmap readRegion(const RectI2& rect, BitmapFormat format) const;

		/**************************************************************************************************************
		 * Copies pixels to a texture.
		 *
		 * The image is copied from the color attachments bound to the framebuffer.
		 *
		 * @param[in] rect The rect of the framebuffer to copy.
		 * @param[out] texture The texture to copy the pixels to.
		 * @param[in] textureTL The coordinates of the top-left corner of the rect within the texture.
		 **************************************************************************************************************/
		void copyRegion(const RectI2& rect, ColorTexture2D& texture, glm::ivec2 textureTL) const noexcept;

	  protected:
		unsigned int _id;

		BasicFramebuffer(unsigned int id) noexcept;

		// Binds the framebuffer for reading.
		void bindRead() const noexcept;
		// Binds the framebuffer for writing.
		void bindWrite() const noexcept;

		friend class GraphicsContext;
	};

	/******************************************************************************************************************
	 * User framebuffer.
	 ******************************************************************************************************************/
	class Framebuffer : public BasicFramebuffer {
	  public:
		/**************************************************************************************************************
		 * Constructs an empty framebuffer.
		 **************************************************************************************************************/
		Framebuffer() noexcept;

		/**************************************************************************************************************
		 * Move-constructs a framebuffer.
		 *
		 * @param move The framebuffer to be moved.
		 **************************************************************************************************************/
		Framebuffer(Framebuffer&& move) noexcept;

		/**************************************************************************************************************
		 * Destroys the framebuffer.
		 **************************************************************************************************************/
		~Framebuffer() noexcept;

		/**************************************************************************************************************
		 * Move-assigns a framebuffer.
		 *
		 * @param move The framebuffer to be moved.
		 *
		 * @return A reference to the assigned framebuffer.
		 **************************************************************************************************************/
		Framebuffer& operator=(Framebuffer&& move) noexcept;

		glm::ivec2 size() const noexcept override;

		/**************************************************************************************************************
		 * Attaches a 1D texture to a slot on the framebuffer.
		 *
		 * @note The viewport of the framebuffer will be clamped to the minimum size of all attachments.
		 *
		 * @param tex The texture to attach.
		 * @param slot
		 * @parblock
		 * The slot to attach the texture to.
		 *
		 * @pre @em slot must be a value in the range [0, 8).
		 * @endparblock
		 **************************************************************************************************************/
		void attach(ColorTexture1D& tex, int slot) noexcept;

		/**************************************************************************************************************
		 * Attaches a layer of a 1D array texture to a slot on the framebuffer.
		 *
		 * @note The viewport of the framebuffer will be clamped to the minimum size of all attachments.
		 *
		 * @param tex The texture to attach.
		 * @param[in] layer The layer of the texture to attach.
		 * @param slot
		 * @parblock
		 * The slot to attach the texture to.
		 *
		 * @pre @em slot must be a value in the range [0, 8).
		 * @endparblock
		 **************************************************************************************************************/
		void attach(ArrayColorTexture1D& tex, int layer, int slot) noexcept;

		/**************************************************************************************************************
		 * Attaches a 2D texture to a slot on the framebuffer.
		 *
		 * @note The viewport of the framebuffer will be clamped to the minimum size of all attachments.
		 *
		 * @param tex The texture to attach.
		 * @param slot
		 * @parblock
		 * The slot to attach the texture to.
		 *
		 * @pre @em slot must be a value in the range [0, 8).
		 * @endparblock
		 **************************************************************************************************************/
		void attach(ColorTexture2D& tex, int slot) noexcept;

		/**************************************************************************************************************
		 * Attaches a layer of a 2D array texture to a slot on the framebuffer.
		 *
		 * @note The viewport of the framebuffer will be clamped to the minimum size of all attachments.
		 *
		 * @param tex The texture to attach.
		 * @param[in] layer The layer of the texture to attach.
		 * @param slot
		 * @parblock
		 * The slot to attach the texture to.
		 *
		 * @pre @em slot must be a value in the range [0, 8).
		 * @endparblock
		 **************************************************************************************************************/
		void attach(ArrayColorTexture2D& tex, int layer, int slot) noexcept;

		/**************************************************************************************************************
		 * Attaches a layer of a 3D texture to a slot on the framebuffer.
		 *
		 * @note The viewport of the framebuffer will be clamped to the minimum size of all attachments.
		 *
		 * @param tex The texture to attach.
		 * @param z The z layer of the texture to attach.
		 * @param slot
		 * @parblock
		 * The slot to attach the texture to.
		 *
		 * @pre @em slot must be a value in the range [0, 8).
		 * @endparblock
		 **************************************************************************************************************/
		void attach(ColorTexture3D& tex, int z, int slot) noexcept;

		/**************************************************************************************************************
		 * Clears a slot of attachments.
		 *
		 * @note The viewport of the framebuffer will be clamped to the minimum size of all attachments.
		 *
		 * @param slot
		 * @parblock
		 * The slot to attach the texture to.
		 *
		 * @pre @em slot must be a value in the range [0, 8).
		 * @endparblock
		 **************************************************************************************************************/
		void clear(int slot) noexcept;

		/**************************************************************************************************************
		 * Sets the debug label of the framebuffer.
		 *
		 * @param[in] label The new label of the framebuffer.
		 **************************************************************************************************************/
		void setLabel(std::string_view label) noexcept;

	  private:
		std::array<glm::i16vec2, 8> _attachSizes; // The sizes of the framebuffer attachments.
		glm::ivec2                  _size;

		// Recalculates the size of the framebuffer.
		glm::ivec2 calcSize() noexcept;

		friend class GraphicsContext;
	};

	/******************************************************************************************************************
	 * Special window backbuffer.
	 *
	 * @note This class cannot be instantiated directly.
	 ******************************************************************************************************************/
	class Backbuffer : public BasicFramebuffer {
	  public:
		glm::ivec2 size() const noexcept override;

	  private:
		Backbuffer() noexcept;

		friend class Window;
	};

	/// @}
} // namespace tr
