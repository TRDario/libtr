/**
 * @file index_buffer.hpp
 * @brief Provides a GPU index buffer class.
 */

#pragma once
#include "gl_buffer.hpp"

#include <string>

namespace tr {
	/******************************************************************************************************************
	 * RAII wrapper over an index buffer map.
	 *
	 * The buffer is automatically unmapped once the map goes out of scope.
	 ******************************************************************************************************************/
	class IndexBufferMap : private GLBufferMap {
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
		IndexBufferMap(GLBufferMap base) noexcept;

		friend class IndexBuffer;
	};

	/******************************************************************************************************************
	 * GPU index buffer class.
	 *
	 * An OpenGL context must be open to instantiate and use objects of this type.
	 ******************************************************************************************************************/
	class IndexBuffer {
	  public:
		/**************************************************************************************************************
		 * Constructs an empty index buffer.
		 *
		 * This function can be called before creating a GLContext, but most operations on the buffer still require it.
		 **************************************************************************************************************/
		IndexBuffer() noexcept;

		/**************************************************************************************************************
		 * Allocates an index buffer.
		 *
		 * The buffer will be of size 0 and capacity @em capacity after construction.
		 *
		 * This function can't be called before creating a GLContext.
		 *
		 * @exception GLBufferBadAlloc If allocating the buffer failed.
		 *
		 * @param capacity The capacity of the buffer in indices. Must be greater than 0, otherwise a failed assertion
		 *                 may be triggered.
		 **************************************************************************************************************/
		IndexBuffer(std::size_t capacity);

		/**************************************************************************************************************
		 * Allocates an index buffer and fills it with data.
		 *
		 * The buffer will be of size and capacity @em data.size() after construction.
		 *
		 * This function can't be called before creating a GLContext.
		 *
		 * @exception GLBufferBadAlloc If allocating the buffer failed.
		 *
		 * @param data The data to be uploaded to be buffer. Must not be empty, otherwise a failed assertion
		 *             may be triggered.
		 **************************************************************************************************************/
		IndexBuffer(std::span<const std::uint16_t> data);

		/**************************************************************************************************************
		 * Gets whether the vertex buffer is empty.
		 *
		 * @return True if the buffer has size 0.
		 **************************************************************************************************************/
		bool empty() const noexcept;

		/**************************************************************************************************************
		 * Gets the size of the vertex buffer contents.
		 *
		 * @return The size of the vertex buffer contents in indices.
		 **************************************************************************************************************/
		std::size_t size() const noexcept;

		/**************************************************************************************************************
		 * Gets the capacity of the vertex buffer.
		 *
		 * The buffer can be resized past this capacity, but it will trigger a reallocation.
		 *
		 * @return The capacity of the vertex buffer in indices.
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
		 * @exception GLBufferBadAlloc If a reallocation was triggered and reallocating the buffer failed.
		 *
		 * @param data The new data of the buffer.
		 **************************************************************************************************************/
		void set(std::span<const std::uint16_t> data);

		/**************************************************************************************************************
		 * Sets a region of the buffer.
		 *
		 * Unlike set(), a call to this function will never cause a reallocation, but an assertion may fail if a span
		 * too large is passed to it.
		 *
		 * The buffer cannot be mapped when this function is called.
		 *
		 * @param offset The starting offset within the buffer in indices.
		 * @param data The new data of the buffer. `offset + data.size() <= capacity()` must hold true, otherwise a
		 *failed assertion may be triggered.
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
		 * If @em size is greater than the capacity of the buffer, a reallocation will be done. This voids any previous
		 * bindings of the buffer to the context, and so it must be rebound. mapRegion() will never cause a
		 * reallocation, so may be used in cases where that's a requirement.
		 *
		 * @exception GLBufferBadAlloc If a reallocation was triggered and reallocating the buffer failed.
		 * @exception GLMapBadAlloc If mapping the buffer failed.
		 *
		 * @param size The new size of the buffer in indices. Must be greater than 0, otherwise a failed assertion may
		 *             be triggered.
		 *
		 * @return A map object.
		 **************************************************************************************************************/
		IndexBufferMap mapNew(std::size_t size);

		/**************************************************************************************************************
		 * Maps a region of the buffer, invalidating the previous contents in the process.
		 *
		 * Unlike mapNew(), a call to this function will never cause a reallocation, but an assertion may fail if an
		 * out-of-bounds map is requested.
		 *
		 * @exception GLMapBadAlloc If mapping the buffer failed.
		 *
		 * @param offset The starting offset within the buffer in indices.
		 * @param size The size of the mapped region in indices. Must be greater than 0, and `offset + data.size() <=
		 *capacity()` must hold true, otherwise a failed assertion may be triggered.
		 *
		 * @return A map object.
		 **************************************************************************************************************/
		IndexBufferMap mapRegion(std::size_t offset, std::size_t size);

		/**************************************************************************************************************
		 * Sets the debug label of the vertex buffer.
		 *
		 * @param label The new label of the index buffer.
		 **************************************************************************************************************/
		void setLabel(std::string label) noexcept;

	  private:
		std::optional<GLBuffer> _buffer;
		std::size_t             _size; // Size of the allocated portion of the buffer.
		std::string             _label;

		// Resizes the buffer, reallocating if needed.
		void resize(std::size_t newSize);

		friend class GLContext;
	};
} // namespace tr
