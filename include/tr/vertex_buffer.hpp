#pragma once
#include "graphics_buffer.hpp"
#include <string>

namespace tr {
	/** @ingroup graphics
	 *  @defgroup vertex_buffer Vertex Buffer
	 *  Graphics vertex buffer class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * GPU vertex buffer class.
	 ******************************************************************************************************************/
	class VertexBuffer {
	  public:
		/**************************************************************************************************************
		 * Constructs an empty vertex buffer.
		 **************************************************************************************************************/
		VertexBuffer() noexcept;

		/**************************************************************************************************************
		 * Allocates a vertex buffer.
		 *
		 * The buffer will be of size 0 and capacity @em capacity after construction.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception GraphicsBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] capacity
		 * @parblock
		 * The capacity of the buffer in bytes.
		 *
		 * @pre @em capacity must be greater than 0.
		 * @endparblock
		 **************************************************************************************************************/
		VertexBuffer(std::size_t capacity);

		/**************************************************************************************************************
		 * Allocates a vertex buffer and fills it with data.
		 *
		 * The buffer will be of size and capacity @em data.size() after construction.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception GraphicsBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] data
		 * @parblock
		 * The data to be uploaded to be buffer.
		 *
		 * @pre @em data cannot be empty.
		 * @endparblock
		 **************************************************************************************************************/
		VertexBuffer(std::span<const std::byte> data);

		/**************************************************************************************************************
		 * Allocates a vertex buffer and fills it with data.
		 *
		 * The buffer will be of size and capacity @em data.size() after construction.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception GraphicsBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] range
		 * @parblock
		 * The data to be uploaded to be buffer.
		 *
		 * @pre @em range cannot be empty.
		 * @endparblock
		 **************************************************************************************************************/
		template <std::ranges::contiguous_range T>
		VertexBuffer(T&& range)
			: VertexBuffer{std::span<const std::byte>(rangeBytes(range))}
		{
		}

		/**************************************************************************************************************
		 * Gets whether the vertex buffer is empty.
		 *
		 * @return True if the buffer has size 0.
		 **************************************************************************************************************/
		bool empty() const noexcept;

		/**************************************************************************************************************
		 * Gets the size of the vertex buffer contents.
		 *
		 * @return The size of the vertex buffer contents in bytes.
		 **************************************************************************************************************/
		std::size_t size() const noexcept;

		/**************************************************************************************************************
		 * Gets the capacity of the vertex buffer.
		 *
		 * @note The buffer can be resized past this capacity, but it will trigger a reallocation.
		 *
		 * @return The capacity of the vertex buffer in indices.
		 **************************************************************************************************************/
		std::size_t capacity() const noexcept;

		/**************************************************************************************************************
		 * Sets the size of the vertex buffer to 0.
		 *
		 * This does not affect the capacity of the buffer.
		 *
		 * @pre The buffer cannot be mapped when this function is called.
		 **************************************************************************************************************/
		void clear() noexcept;

		/**************************************************************************************************************
		 * Sets the contents of the buffer.
		 *
		 * @note If data.size() is greater than the capacity of the buffer, a reallocation will be done. This voids any
		 * previous bindings of the buffer to the context, and so it must be rebound. setRegion() will never cause a
		 * reallocation, so may be used in cases where that's a requirement.
		 *
		 * @pre The buffer cannot be mapped when this function is called.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception GraphicsBufferBadAlloc If a reallocation is triggered and reallocating the buffer fails.
		 *
		 * @param[in] data The new data of the buffer.
		 **************************************************************************************************************/
		void set(std::span<const std::byte> data);

		/**************************************************************************************************************
		 * Sets the contents of the buffer.
		 *
		 * @note If data.size() is greater than the capacity of the buffer, a reallocation will be done. This voids any
		 * previous bindings of the buffer to the context, and so it must be rebound. setRegion() will never cause a
		 * reallocation, so may be used in cases where that's a requirement.
		 *
		 * @pre The buffer cannot be mapped when this function is called.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception GraphicsBufferBadAlloc If a reallocation is triggered and reallocating the buffer fails.
		 *
		 * @param[in] range The new data of the buffer.
		 **************************************************************************************************************/
		template <std::ranges::contiguous_range T> void set(T&& range)
		{
			set(std::span<const std::byte>{rangeBytes(range)});
		}

		/**************************************************************************************************************
		 * Sets a region of the buffer.
		 *
		 * @remark Unlike set(), a call to this function will never cause a reallocation.
		 *
		 * @pre The buffer cannot be mapped when this function is called.
		 *
		 * @param[in] offset The starting offset within the buffer in bytes.
		 * @param[in] data
		 * @parblock
		 * The new data of the buffer.
		 *
		 * @pre `offset + data.size() <= capacity()` must hold true.
		 * @endparblock
		 **************************************************************************************************************/
		void setRegion(std::size_t offset, std::span<const std::byte> data) noexcept;

		/**************************************************************************************************************
		 * Sets a region of the buffer.
		 *
		 * @remark Unlike set(), a call to this function will never cause a reallocation.
		 *
		 * @pre The buffer cannot be mapped when this function is called.
		 *
		 * @param[in] offset The starting offset within the buffer in bytes.
		 * @param[in] range
		 * @parblock
		 * The new data of the buffer.
		 *
		 * @pre `offset + range.size() <= capacity()` must hold true.
		 * @endparblock
		 **************************************************************************************************************/
		template <std::ranges::contiguous_range T> void setRegion(std::size_t offset, T&& range) noexcept
		{
			setRegion(offset, std::span<const std::byte>{rangeBytes(range)});
		}

		/**************************************************************************************************************
		 * Gets whether the buffer is mapped.
		 *
		 * @return True if the buffer is mapped, and false otherwise.
		 **************************************************************************************************************/
		bool mapped() const noexcept;

		/**************************************************************************************************************
		 * Maps the buffer, invalidating the previous contents in the process and resetting its size.
		 *
		 * @note If @em size is greater than the capacity of the buffer, a reallocation will be done. This voids any
		 * previous bindings of the buffer to the context, and so it must be rebound. mapRegion() will never cause a
		 * reallocation, so may be used in cases where that's a requirement.
		 *
		 * @par Exception Safety
		 *
		 * Weak exception guarantee, if mapping the buffer after reallocation fails, the reallocation is not reverted.
		 *
		 * @exception GraphicsBufferBadAlloc If a reallocation was triggered and reallocating the buffer fails.
		 * @exception GraphicsBufferMapBadAlloc If mapping the buffer fails.
		 *
		 * @param[in] size
		 * @parblock
		 * The new size of the buffer in bytes.
		 *
		 * @pre @em size must be greater than 0.
		 * @endparblock
		 *
		 * @return A map object.
		 **************************************************************************************************************/
		GraphicsBufferMap mapNew(std::size_t size);

		/**************************************************************************************************************
		 * Maps a region of the buffer, invalidating the previous contents in the process.
		 *
		 * @remark Unlike mapNew(), a call to this function will never cause a reallocation.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception GraphicsBufferMapBadAlloc If mapping the buffer failed.
		 *
		 * @param[in] offset The starting offset within the buffer in bytes.
		 * @param[in] size
		 * @parblock
		 * The size of the mapped region in bytes.
		 *
		 * @pre @em size must be greater than 0, and `offset + data.size() <= capacity()` must hold true.
		 * @endparblock
		 *
		 * @return A map object.
		 **************************************************************************************************************/
		GraphicsBufferMap mapRegion(std::size_t offset, std::size_t size);

		/**************************************************************************************************************
		 * Sets the debug label of the vertex buffer.
		 *
		 * @param[in] label The new label of the vertex buffer.
		 **************************************************************************************************************/
		void setLabel(std::string label) noexcept;

	  private:
		std::optional<GraphicsBuffer> _buffer;
		std::size_t                   _size; // Size of the allocated portion of the buffer.
		std::string                   _label;

		// Resizes the buffer, reallocating if needed.
		void resize(std::size_t newSize);

		friend class GraphicsContext;
	};

	/// @}
} // namespace tr
