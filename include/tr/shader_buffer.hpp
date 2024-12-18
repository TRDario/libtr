#pragma once
#include "graphics_buffer.hpp"

namespace tr {
	/** @ingroup graphics
	 *  @defgroup shader_buffer Shader Buffer
	 *  Graphics shader buffer class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * GPU buffer accessable by a shader.
	 *
	 * The API is designed as a dynamic-length array with a fixed-length header in front.
	 * Passing 0 to the size/capacity of either component disables them, and they cannot be accessed.
	 ******************************************************************************************************************/
	class ShaderBuffer : public GraphicsBuffer {
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
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception GraphicsBufferBadAlloc If allocating the buffer fails.
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
		template <GraphicsBufferCopyOutputIterator It> void copyHeaderTo(It out) const noexcept
		{
			copyRegionTo(out, 0, _headerSize);
		}

		/******************************************************************************************************************
		 * Copies the data of the dynamic array to an output iterator.
		 *
		 * @param[out] out An iterator to the beginning of the range to copy the header to.
		 ******************************************************************************************************************/
		template <GraphicsBufferCopyOutputIterator It> void copyArrayTo(It out) const noexcept
		{
			copyRegionTo(out, _headerSize, _size);
		}

		/******************************************************************************************************************
		 * Sets the data of the header.
		 *
		 * @param[in] data
		 * @parblock
		 * The new data of the header.
		 *
		 * @pre The span must be the same size as the header.
		 * @endparblock
		 ******************************************************************************************************************/
		void setHeader(std::span<const std::byte> data) noexcept;

		/******************************************************************************************************************
		 * Sets the data of the header.
		 *
		 * @param[in] range
		 * @parblock
		 * The new data of the header.
		 *
		 * @pre The range must be the same size as the header.
		 * @endparblock
		 ******************************************************************************************************************/
		template <std::ranges::contiguous_range Range> void setHeader(Range&& range) noexcept
		{
			setHeader(std::span<const std::byte>{rangeBytes(range)});
		}

		/******************************************************************************************************************
		 * Sets the data of the header.
		 *
		 * @param[in] value
		 * @parblock
		 * The new data of the header.
		 *
		 * @pre The value must be the same size as the header.
		 * @endparblock
		 ******************************************************************************************************************/
		template <class T> void setHeader(const T& value) noexcept
		{
			setHeader(std::span<const std::byte>{asBytes(value)});
		}

		/******************************************************************************************************************
		 * Sets the data of the dynamic array.
		 *
		 * @param[in] data
		 * @parblock
		 * The new data of the array.
		 *
		 * @pre @em data must be smaller or have the same size as the array capacity.
		 * @endparblock
		 ******************************************************************************************************************/
		void setArray(std::span<const std::byte> data) noexcept;

		/******************************************************************************************************************
		 * Sets the data of the dynamic array.
		 *
		 * @param[in] range
		 * @parblock
		 * The new data of the array.
		 *
		 * @pre @em range must be smaller or have the same size as the array capacity.
		 * @endparblock
		 ******************************************************************************************************************/
		template <std::ranges::contiguous_range Range> void setArray(Range&& range) noexcept
		{
			setArray(std::span<const std::byte>{rangeBytes(range)});
		}

		/******************************************************************************************************************
		 * Resizes the dynamic array.
		 *
		 * @param[in] size The new size of the array in bytes. Must be less than the capacity.
		 ******************************************************************************************************************/
		void resizeArray(std::size_t size) noexcept;

		using GraphicsBuffer::mapped;

		/******************************************************************************************************************
		 * Maps the fixed header.
		 *
		 * @pre This function cannot be called if the buffer doesn't have a fixed header block.
		 *
		 * @return The mapped fixed header.
		 ******************************************************************************************************************/
		GraphicsBufferMap mapHeader() noexcept;

		/******************************************************************************************************************
		 * Maps the dynamic array.
		 *
		 * @pre This function cannot be called if the buffer doesn't have a dynamic array block.
		 *
		 * @return The mapped dynamic array.
		 ******************************************************************************************************************/
		GraphicsBufferMap mapArray() noexcept;

		/******************************************************************************************************************
		 * Maps the entire buffer.
		 *
		 * @return The mapped buffer.
		 ******************************************************************************************************************/
		GraphicsBufferMap map() noexcept;

		using GraphicsBuffer::setLabel;

	  private:
		Access      _access;
		std::size_t _headerSize;
		std::size_t _size;

		friend class Shader;
	};

	/// @}
} // namespace tr
