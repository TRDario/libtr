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
		 *
		 * This function can be called before creating a GraphicsContext.
		 **************************************************************************************************************/
		VertexBuffer() noexcept;

		/**************************************************************************************************************
		 * Allocates a vertex buffer.
		 *
		 * The buffer will be of size 0 and capacity @em capacity after construction.
		 *
		 * This function can't be called before creating a GraphicsContext.
		 *
		 * @exception GraphicsBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param capacity The capacity of the buffer in bytes. Must be greater than 0.
		 **************************************************************************************************************/
		VertexBuffer(std::size_t capacity);

		/**************************************************************************************************************
		 * Allocates a vertex buffer and fills it with data.
		 *
		 * The buffer will be of size and capacity @em data.size() after construction.
		 *
		 * This function can't be called before creating a GraphicsContext.
		 *
		 * @exception GraphicsBufferBadAlloc If allocating the buffer fails.
		 *
		 * @param[in] data The data to be uploaded to be buffer. Must not be empty.
		 **************************************************************************************************************/
		VertexBuffer(std::span<const std::byte> data);

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
		 * The buffer can be resized past this capacity, but it will trigger a reallocation.
		 *
		 * @return The capacity of the vertex buffer in bytes.
		 **************************************************************************************************************/
		std::size_t capacity() const noexcept;

		/**************************************************************************************************************
		 * Sets the size of the vertex buffer to 0.
		 *
		 * The buffer cannot be mapped when this function is called.
		 *
		 * This does not affect the capacity of the buffer.
		 **************************************************************************************************************/
		void clear() noexcept;

		/**************************************************************************************************************
		 * Sets the contents of the buffer.
		 *
		 * If data.size() is greater than the capacity of the buffer, a reallocation will be done. This voids any
		 *previous bindings of the buffer to the context, and so it must be rebound. setRegion() will never cause a
		 * reallocation, so may be used in cases where that's a requirement.
		 *
		 * The buffer cannot be mapped when this function is called.
		 *
		 * @exception GraphicsBufferBadAlloc If a reallocation is triggered and reallocating the buffer fails.
		 *
		 * @param[in] data The new data of the buffer.
		 **************************************************************************************************************/
		void set(std::span<const std::byte> data);

		/**************************************************************************************************************
		 * Sets a region of the buffer.
		 *
		 * Unlike set(), a call to this function will never cause a reallocation, but an assertion may fail if a span
		 * too large is passed to it.
		 *
		 * The buffer cannot be mapped when this function is called.
		 *
		 * @param[in] offset The starting offset within the buffer in bytes.
		 * @param[in] data The new data of the buffer. `offset + data.size() <= capacity()` must hold true.
		 **************************************************************************************************************/
		void setRegion(std::size_t offset, std::span<const std::byte> data) noexcept;

		/**************************************************************************************************************
		 * Gets whether the buffer is mapped.
		 *
		 * @return True if the buffer is mapped, and false otherwise.
		 **************************************************************************************************************/
		bool mapped() const noexcept;

		/**************************************************************************************************************
		 * Maps the buffer, invalidating the previous contents in the process and resetting its size.
		 *
		 * If @em size is greater than the capacity of the buffer, a reallocation will be done. This voids any previous
		 * bindings of the buffer to the context, and so it must be rebound. mapRegion() will never cause a
		 * reallocation, so may be used in cases where that's a requirement.
		 *
		 * @exception GraphicsBufferBadAlloc If a reallocation was triggered and reallocating the buffer failed.
		 * @exception GraphicsBufferMapBadAlloc If mapping the buffer failed.
		 *
		 * @param[in] size The new size of the buffer in bytes. Must be greater than 0, otherwise a failed assertion may
		 *                 be triggered.
		 *
		 * @return A map object.
		 **************************************************************************************************************/
		GraphicsBufferMap mapNew(std::size_t size);

		/**************************************************************************************************************
		 * Maps a region of the buffer, invalidating the previous contents in the process.
		 *
		 * Unlike mapNew(), a call to this function will never cause a reallocation, but an assertion may fail if an
		 * out-of-bounds map is requested.
		 *
		 * @exception GraphicsBufferMapBadAlloc If mapping the buffer failed.
		 *
		 * @param[in] offset The starting offset within the buffer in bytes.
		 * @param[in] size The size of the mapped region in bytes. Must be greater than 0, and `offset + data.size() <=
		 *                 capacity()` must hold true, otherwise a failed assertion may be triggered.
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
