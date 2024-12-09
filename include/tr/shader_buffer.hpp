#pragma once
#include "gl_buffer.hpp"

namespace tr {
	/** @addtogroup graphics
	 *  @{
	 */

	/******************************************************************************************************************
	 * GPU buffer accessable by a shader.
	 *
	 * The API is designed as a dynamic-length array with a fixed-length header in front.
	 * Passing 0 to the size/capacity of either component disables them, and they cannot be accessed.
	 ******************************************************************************************************************/
	class ShaderBuffer : public GLBuffer {
	  public:
		/**************************************************************************************************************
		 * Shader buffer map access type.
		 **************************************************************************************************************/
		enum class Access : std::uint32_t {
			/**********************************************************************************************************
			 * Buffer maps are read-only.
			 **********************************************************************************************************/
			READ_ONLY = std::uint32_t(Flag::READABLE),

			/**********************************************************************************************************
			 * Buffer maps are write-only.
			 **********************************************************************************************************/
			WRITE_ONLY = std::uint32_t(Flag::WRITABLE),

			/**********************************************************************************************************
			 * Buffer maps are readable and writable.
			 **********************************************************************************************************/
			READ_WRITE = std::uint32_t(Flag::READABLE) | std::uint32_t(Flag::WRITABLE)
		};

		/******************************************************************************************************************
		 * Allocates an uninitialized shader buffer.
		 *
		 * @exception GLBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] headerSize The size of the fixed header block in bytes (may be 0).
		 * @param[in] capacity The maximum capacity of the dynamic array in bytes (may be 0).
		 * @param[in] access The access permissions of buffer maps.
		 ******************************************************************************************************************/
		ShaderBuffer(std::size_t headerSize, std::size_t capacity, Access access);

		/******************************************************************************************************************
		 * Gets the size of the fixed header block.
		 *
		 * @return The size of the header in bytes.
		 ******************************************************************************************************************/
		std::size_t headerSize() const noexcept;

		/******************************************************************************************************************
		 * Gets the size of the dynamic array.
		 *
		 * @return The size of the dynamic array in bytes.
		 ******************************************************************************************************************/
		std::size_t arraySize() const noexcept;

		/******************************************************************************************************************
		 * Gets the maximum capacity of the dynamic array.
		 *
		 * @return The maximum capacity of the dynamic array in bytes.
		 ******************************************************************************************************************/
		std::size_t arrayCapacity() const noexcept;

		/******************************************************************************************************************
		 * Copies the data of the fixed header to an output iterator.
		 *
		 * @param[out] out An iterator to the beginning of the range to copy the header to.
		 ******************************************************************************************************************/
		template <GLCopyOutputIterator It> void copyHeaderTo(It out) const noexcept
		{
			copyRegionTo(out, 0, _headerSize);
		}

		/******************************************************************************************************************
		 * Copies the data of the dynamic array to an output iterator.
		 *
		 * @param[out] out An iterator to the beginning of the range to copy the header to.
		 ******************************************************************************************************************/
		template <GLCopyOutputIterator It> void copyArrayTo(It out) const noexcept
		{
			copyRegionTo(out, _headerSize, _size);
		}

		/******************************************************************************************************************
		 * Sets the data of the header.
		 *
		 * @param[in] data The new data of the header. The span must be the same size as the header.
		 ******************************************************************************************************************/
		void setHeader(std::span<const std::byte> data) noexcept;

		/******************************************************************************************************************
		 * Sets the data of the dynamic array.
		 *
		 * @param[in] data The new data of the array. The span must be smaller or have the same size as the array
		 *                 capacity.
		 ******************************************************************************************************************/
		void setArray(std::span<const std::byte> data) noexcept;

		/******************************************************************************************************************
		 * Resizes the dynamic array.
		 *
		 * @param[in] size The new size of the array in bytes. Must be less than the capacity.
		 ******************************************************************************************************************/
		void resizeArray(std::size_t size) noexcept;

		using GLBuffer::mapped;

		/******************************************************************************************************************
		 * Maps the fixed header.
		 *
		 * This function cannot be called if the buffer doesn't have a fixed header block.
		 *
		 * @return The mapped fixed header.
		 ******************************************************************************************************************/
		GLBufferMap mapHeader() noexcept;

		/******************************************************************************************************************
		 * Maps the dynamic array.
		 *
		 * This function cannot be called if the buffer doesn't have a dynamic array block.
		 *
		 * @return The mapped dynamic array.
		 ******************************************************************************************************************/
		GLBufferMap mapArray() noexcept;

		/******************************************************************************************************************
		 * Maps the entire buffer.
		 *
		 * @return The mapped buffer.
		 ******************************************************************************************************************/
		GLBufferMap map() noexcept;

		using GLBuffer::setLabel;

	  private:
		Access      _access;
		std::size_t _headerSize;
		std::size_t _size;

		friend class Shader;
	};

	/// @}
} // namespace tr
