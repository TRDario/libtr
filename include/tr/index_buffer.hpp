#pragma once
#include "graphics_buffer.hpp"
#include <string>

namespace tr {
	/** @ingroup graphics
	 *  @defgroup index_buffer Index Buffer
	 *  Graphics index buffer class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * RAII wrapper over an index buffer map.
	 *
	 * The buffer is automatically unmapped once the map goes out of scope.
	 ******************************************************************************************************************/
	class IndexBufferMap : private GraphicsBufferMap {
	  public:
		/**************************************************************************************************************
		 * Casts the map into a regular index span.
		 **************************************************************************************************************/
		operator std::span<std::uint16_t>() const noexcept;

		/**************************************************************************************************************
		 * Casts the map into a regular index span.
		 *
		 * @return A span of bytes.
		 **************************************************************************************************************/
		std::span<std::uint16_t> span() const noexcept;

	  private:
		IndexBufferMap(GraphicsBufferMap base) noexcept;

		friend class IndexBuffer;
	};

	/******************************************************************************************************************
	 * GPU index buffer class.
	 ******************************************************************************************************************/
	class IndexBuffer {
	  public:
		/**************************************************************************************************************
		 * Constructs an empty index buffer.
		 **************************************************************************************************************/
		IndexBuffer() noexcept;

		/**************************************************************************************************************
		 * Allocates an index buffer.
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
		 * The capacity of the buffer in indices.
		 *
		 * @pre @em capacity must be greater than 0.
		 * @endparblock
		 **************************************************************************************************************/
		IndexBuffer(std::size_t capacity);

		/**************************************************************************************************************
		 * Allocates an index buffer and fills it with data.
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
		IndexBuffer(std::span<const std::uint16_t> data);

		/**************************************************************************************************************
		 * Gets whether the index buffer is empty.
		 *
		 * @return True if the buffer has size 0.
		 **************************************************************************************************************/
		bool empty() const noexcept;

		/**************************************************************************************************************
		 * Gets the size of the index buffer contents.
		 *
		 * @return The size of the index buffer contents in indices.
		 **************************************************************************************************************/
		std::size_t size() const noexcept;

		/**************************************************************************************************************
		 * Gets the capacity of the index buffer.
		 *
		 * @note The buffer can be resized past this capacity, but it will trigger a reallocation.
		 *
		 * @return The capacity of the index buffer in indices.
		 **************************************************************************************************************/
		std::size_t capacity() const noexcept;

		/**************************************************************************************************************
		 * Sets the size of the index buffer to 0.
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
		void set(std::span<const std::uint16_t> data);

		/**************************************************************************************************************
		 * Sets a region of the buffer.
		 *
		 * @remark Unlike set(), a call to this function will never cause a reallocation.
		 *
		 * @pre The buffer cannot be mapped when this function is called.
		 *
		 * @param[in] offset The starting offset within the buffer in elements.
		 * @param[in] data
		 * @parblock
		 * The new data of the buffer.
		 *
		 * @pre `offset + data.size() <= capacity()` must hold true.
		 * @endparblock
		 **************************************************************************************************************/
		void setRegion(std::size_t offset, std::span<const std::uint16_t> data) noexcept;

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
		 * The new size of the buffer in elements.
		 *
		 * @pre @em size must be greater than 0.
		 * @endparblock
		 *
		 * @return A map object.
		 **************************************************************************************************************/
		IndexBufferMap mapNew(std::size_t size);

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
		 * @param[in] offset The starting offset within the buffer in elements.
		 * @param[in] size
		 * @parblock
		 * The size of the mapped region in elements.
		 *
		 * @pre @em size must be greater than 0, and `offset + data.size() <= capacity()` must hold true.
		 * @endparblock
		 *
		 * @return A map object.
		 **************************************************************************************************************/
		IndexBufferMap mapRegion(std::size_t offset, std::size_t size);

		/**************************************************************************************************************
		 * Sets the debug label of the vertex buffer.
		 *
		 * @param[in] label The new label of the index buffer.
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
