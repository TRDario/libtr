/**
 * @file framebuffer.hpp
 * @brief Provides OpenGL framebuffer classes.
 */

#pragma once
#include "texture.hpp"
#include "renderbuffer.hpp"
#include "window.hpp"

namespace tr {
    /******************************************************************************************************************
	 * Framebuffer depth range.
	 ******************************************************************************************************************/
	struct DepthRange {
        /**************************************************************************************************************
	     * Minimum depth value.
	     **************************************************************************************************************/
        double min;
        
        /**************************************************************************************************************
	     * Maximum depth value.
	     **************************************************************************************************************/
        double max;
    };


    /******************************************************************************************************************
	 * Base framebuffer class.
     *
     * This class cannot be constructed directly.
	 ******************************************************************************************************************/
	class BasicFramebuffer {
	public:
        /**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		bool operator==(const BasicFramebuffer&) const noexcept;


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
         * @exception BitmapBadAlloc If allocating the bitmap failed.
         *
         * @param rect The rect of the framebuffer to copy.
         * @param format The format to use for the bitmap pixels.
         *
         * @return A bitmap containing the pixel data.
	     **************************************************************************************************************/
		Bitmap readRegion(RectI2 rect, BitmapFormat format) const;

		/**************************************************************************************************************
	     * Copies pixels to a texture.
         *
         * The image is copied from the color attachments bound to the framebuffer.
         *
         * @param rect The rect of the framebuffer to copy.
         * @param texture The texture to copy the pixels to.
         * @param textureTL The coordinates of the top-left corner of the rect within the texture.
	     **************************************************************************************************************/
		void copyRegion(RectI2 rect, Texture2D& texture, glm::ivec2 textureTL) const noexcept;


        /**************************************************************************************************************
	     * Gets the viewport rect of the framebuffer.
         *
         * @return The viewport rect.
	     **************************************************************************************************************/
		RectI2 viewport() const noexcept;

        /**************************************************************************************************************
	     * Sets the viewport rect of the framebuffer.
         *
         * @param viewport The new viewport.
	     **************************************************************************************************************/
		void setViewport(RectI2 viewport) noexcept;


		/**************************************************************************************************************
	     * Gets the depth range of the framebuffer.
         *
         * @return The depth range.
	     **************************************************************************************************************/
		DepthRange depthRange() const noexcept;

        /**************************************************************************************************************
	     * Sets the depth range of the framebuffer.
         *
         * @param range The new depth range.
	     **************************************************************************************************************/
		void setDepthRange(DepthRange range) noexcept;
	protected:
        /// @cond IMPLEMENTATION
		unsigned int _id;
		RectI2 	     _viewport;
		DepthRange   _depthRange;

		BasicFramebuffer(unsigned int id, RectI2 viewport, DepthRange depthRange) noexcept;

		// Binds the framebuffer for reading.
		void bindRead() const noexcept;
		// Binds the framebuffer for writing.
		void bindWrite() const noexcept;
        /// @endcond

		friend class GLContext;
	};

	/******************************************************************************************************************
	 * Custom framebuffer.
     *
     * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	class Framebuffer : public BasicFramebuffer {
	public:
		/**************************************************************************************************************
	     * Framebuffer attachment slots.
	     **************************************************************************************************************/
		enum class Slot {
            /**********************************************************************************************************
	         * Color attachment slot 0.
	         **********************************************************************************************************/
			COLOR0,

            /**********************************************************************************************************
	         * Color attachment slot 1.
	         **********************************************************************************************************/
			COLOR1,

            /**********************************************************************************************************
	         * Color attachment slot 2.
	         **********************************************************************************************************/
			COLOR2,

            /**********************************************************************************************************
	         * Color attachment slot 3.
	         **********************************************************************************************************/
			COLOR3,

            /**********************************************************************************************************
	         * Color attachment slot 4.
	         **********************************************************************************************************/
			COLOR4,

            /**********************************************************************************************************
	         * Color attachment slot 5.
	         **********************************************************************************************************/
			COLOR5,

            /**********************************************************************************************************
	         * Color attachment slot 6.
	         **********************************************************************************************************/
			COLOR6,

            /**********************************************************************************************************
	         * Color attachment slot 7.
	         **********************************************************************************************************/
			COLOR7,

            /**********************************************************************************************************
	         * Depth attachment slot.
	         **********************************************************************************************************/
			DEPTH,

            /**********************************************************************************************************
	         * Stencil attachment slot.
	         **********************************************************************************************************/
			STENCIL
		};


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


		virtual glm::ivec2 size() const noexcept;


        /**************************************************************************************************************
	     * Attaches a 1D texture to a slot on the framebuffer.
         *
         * The viewport of the framebuffer will be clamped to the minimum size of all attachments.
         *
         * @param tex The texture to attach.
         * @param slot The slot to attach the texture to.
	     **************************************************************************************************************/
		void attach(Texture1D& tex, Slot slot) noexcept;

        /**************************************************************************************************************
	     * Attaches a layer of a 1D array texture to a slot on the framebuffer.
         *
         * The viewport of the framebuffer will be clamped to the minimum size of all attachments.
         *
         * @param tex The texture to attach.
         * @param layer The layer of the texture to attach.
         * @param slot The slot to attach the texture to.
	     **************************************************************************************************************/
		void attach(ArrayTexture1D& tex, int layer, Slot slot) noexcept;

        /**************************************************************************************************************
	     * Attaches a 2D texture to a slot on the framebuffer.
         *
         * The viewport of the framebuffer will be clamped to the minimum size of all attachments.
         *
         * @param tex The texture to attach.
         * @param slot The slot to attach the texture to.
	     **************************************************************************************************************/
		void attach(Texture2D& tex, Slot slot) noexcept;

        /**************************************************************************************************************
	     * Attaches a layer of a 2D array texture to a slot on the framebuffer.
         *
         * The viewport of the framebuffer will be clamped to the minimum size of all attachments.
         *
         * @param tex The texture to attach.
         * @param layer The layer of the texture to attach.
         * @param slot The slot to attach the texture to.
	     **************************************************************************************************************/
		void attach(ArrayTexture2D& tex, int layer, Slot slot) noexcept;

        /**************************************************************************************************************
	     * Attaches a layer of a 3D texture to a slot on the framebuffer.
         *
         * The viewport of the framebuffer will be clamped to the minimum size of all attachments.
         *
         * @param tex The texture to attach.
         * @param z The z layer of the texture to attach.
         * @param slot The slot to attach the texture to.
	     **************************************************************************************************************/
		void attach(Texture3D& tex, int z, Slot slot) noexcept;

        /**************************************************************************************************************
	     * Attaches a renderbuffer to a slot on the framebuffer.
         *
         * The viewport of the framebuffer will be clamped to the minimum size of all attachments.
         *
         * @param buffer The renderbuffer to attach.
         * @param slot The slot to attach the texture to.
	     **************************************************************************************************************/
		void attach(Renderbuffer& buffer, Slot slot) noexcept;

        /**************************************************************************************************************
	     * Clears a slot of attachments.
         *
         * The viewport of the framebuffer will be clamped to the minimum size of all attachments.
         *
         * @param slot The slot to clear.
	     **************************************************************************************************************/
		void clear(Slot slot) noexcept;


        /**************************************************************************************************************
	     * Sets the debug label of the framebuffer.
         *
         * @param label The new label of the framebuffer.
	     **************************************************************************************************************/
        void setLabel(std::string_view label) noexcept;
	private:
		static constexpr glm::ivec2 EMPTY_ATTACHMENT { -1, -1 };
        std::array<glm::ivec2, 10> _attachSizes; // The sizes of the framebuffer attachments.
		glm::ivec2 				   _size;

		// Recalculates the size of the framebuffer.
		glm::ivec2 calcSize() noexcept;
		// Clamps the viewport to a valid rect of the framebuffer.
		void clampViewport() noexcept;

		friend class GLContext;
	};

	/******************************************************************************************************************
	 * Special context backbuffer.
	 ******************************************************************************************************************/
	class Backbuffer : public BasicFramebuffer {
	public:
		virtual glm::ivec2 size() const noexcept;
	private:
		const Window& _window;
		
		Backbuffer(const Window& window) noexcept;

		friend class GLContext;
	};
}