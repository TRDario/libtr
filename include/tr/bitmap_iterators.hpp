#pragma once
#include "bitmap.hpp"

namespace tr {
	/** @addtogroup bitmap
	 *  @{
	 */

	/******************************************************************************************************************
	 * Read-only reference to a bitmap pixel.
	 ******************************************************************************************************************/
	class SubBitmap::PixelRef {
	  public:
		/**************************************************************************************************************
		 * Gets the pixel's color value.
		 **************************************************************************************************************/
		operator RGBA8() const noexcept;

	  private:
		const std::byte*              _impl;   // A pointer to the pixel data.
		const SDL_PixelFormatDetails* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(const std::byte* ptr, const SDL_PixelFormatDetails* format) noexcept;

		friend class SubBitmap;
		friend class Iterator;
	};

	/******************************************************************************************************************
	 * Immutable bitmap pixel iterator.
	 *
	 * The iterator fulfills the @em RandomAccessIterator requirements.
	 ******************************************************************************************************************/
	class SubBitmap::Iterator {
	  public:
		/**************************************************************************************************************
		 * @em RandomAccessIterator typedef requirement.
		 **************************************************************************************************************/
		using value_type = PixelRef;

		/**************************************************************************************************************
		 * @em RandomAccessIterator typedef requirement.
		 **************************************************************************************************************/
		using pointer = const value_type*;

		/**************************************************************************************************************
		 * @em RandomAccessIterator typedef requirement.
		 **************************************************************************************************************/
		using difference_type = int;

		/**************************************************************************************************************
		 * Default-constructs an iterator.
		 *
		 * An iterator constructed in this manner is in an non-dereferencable state until a valid value is assigned to
		 * it.
		 **************************************************************************************************************/
		Iterator() noexcept = default;

		/**************************************************************************************************************
		 * Constructs an iterator to a bitmap pixel.
		 *
		 * @param[in] bitmap The sub-bitmap.
		 * @param[in] pos The position of the pixel within the sub-bitmap.
		 **************************************************************************************************************/
		Iterator(SubBitmap bitmap, glm::ivec2 pos) noexcept;

		/**************************************************************************************************************
		 * Three-way comparison operator.
		 **************************************************************************************************************/
		std::partial_ordering operator<=>(const Iterator&) const noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		bool operator==(const Iterator&) const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator.
		 *
		 * @return The pixel reference.
		 **************************************************************************************************************/
		value_type operator*() const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator with a subscript.
		 *
		 * @param[in] diff The pixel relative to the iterator to access.
		 *
		 * @return The pixel reference.
		 **************************************************************************************************************/
		value_type operator[](difference_type diff) const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator with a subscript.
		 *
		 * @param[in] diff The pixel relative to the iterator to access (in vector form).
		 *
		 * @return The pixel reference.
		 **************************************************************************************************************/
		value_type operator[](glm::ivec2 diff) const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator through a pointer.
		 *
		 * @return A pointer to the pixel reference.
		 **************************************************************************************************************/
		pointer operator->() const noexcept;

		/**************************************************************************************************************
		 * Increments the iterator.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		Iterator& operator++() noexcept;

		/**************************************************************************************************************
		 * Post-increments the iterator.
		 *
		 * @return An iterator with the prior state of the incremented iterator.
		 **************************************************************************************************************/
		Iterator operator++(int) noexcept;

		/**************************************************************************************************************
		 * Advances the iterator.
		 *
		 * @param[in] diff The number of pixels to move forward.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		Iterator& operator+=(difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Advances an iterator.
		 *
		 * @param[in] diff The number of pixels to move forward (in vector form).
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		Iterator& operator+=(glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move forward.
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator+(const Iterator& it, difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] diff The number of pixels to move forward.
		 * @param[in] it The iterator to add to.
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator+(difference_type diff, const Iterator& it) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move forward (in vector form).
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator+(const Iterator& it, glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] diff The number of pixels to move forward (in vector form).
		 * @param[in] it The iterator to add to.num
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator+(glm::ivec2 diff, const Iterator& it) noexcept;

		/**************************************************************************************************************
		 * Decrements the iterator.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		Iterator& operator--() noexcept;

		/**************************************************************************************************************
		 * Post-decrements the iterator.
		 *
		 * @return An iterator with the prior state of the decremented iterator.
		 **************************************************************************************************************/
		Iterator operator--(int) noexcept;

		/**************************************************************************************************************
		 * Moves the iterator back.
		 *
		 * @param[in] diff The number of pixels to move backward.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		Iterator& operator-=(difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Moves the iterator back.
		 *
		 * @param[in] diff The number of pixels to move backward (in vector form).
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		Iterator& operator-=(glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] diff The number of pixels to move backward.
		 * @param[in] it The iterator to add to.
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator-(const Iterator& it, difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move backward.
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator-(difference_type diff, const Iterator& it) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] diff The number of pixels to move backward (in vector form).
		 * @param[in] it The iterator to add to.
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator-(const Iterator& it, glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move backward (in vector form).
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend Iterator operator-(glm::ivec2 diff, const Iterator& it) noexcept;

		/**************************************************************************************************************
		 * Gets the distance between two iterators.
		 *
		 * This function only makes sense if two iterators are pointing to the same bitmap.
		 *
		 * @param[in] l, r Iterators to the same bitmap.
		 *
		 * @return The distance between the iterators.
		 **************************************************************************************************************/
		friend difference_type operator-(const Iterator& l, const Iterator& r) noexcept;

	  private:
		PixelRef   _pixel;
		glm::ivec2 _bitmapSize;
		int        _pitch;
		glm::ivec2 _pos;
	};

	/******************************************************************************************************************
	 * Mutable reference to a bitmap pixel.
	 ******************************************************************************************************************/
	class Bitmap::PixelRef {
	  public:
		/**************************************************************************************************************
		 * Gets the color value of the pixel.
		 **************************************************************************************************************/
		operator RGBA8() const noexcept;

		/**************************************************************************************************************
		 * Sets the pixel's color value.
		 *
		 * @param[in] color The color to set the pixel to.
		 *
		 * @return A reference to the pixel being assigned.
		 **************************************************************************************************************/
		PixelRef& operator=(RGBA8 color) noexcept;

	  private:
		std::byte*                    _impl;   // A pointer to the pixel data.
		const SDL_PixelFormatDetails* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(std::byte* ptr, const SDL_PixelFormatDetails* format) noexcept;

		friend class MutIt;
	};

	/******************************************************************************************************************
	 * Mutable pixel iterator.
	 *
	 * The iterator fulfills the @em RandomAccessIterator requirements.
	 ******************************************************************************************************************/
	class Bitmap::MutIt {
	  public:
		/**************************************************************************************************************
		 * @em RandomAccessIterator typedef requirement.
		 **************************************************************************************************************/
		using value_type = PixelRef;

		/**************************************************************************************************************
		 * @em RandomAccessIterator typedef requirement.
		 **************************************************************************************************************/
		using pointer = const value_type*;

		/**************************************************************************************************************
		 * @em RandomAccessIterator typedef requirement.
		 **************************************************************************************************************/
		using difference_type = int;

		/**************************************************************************************************************
		 * Default-constructs an iterator.
		 *
		 * An iterator constructed in this manner is in an non-dereferencable state until a valid value is assigned to
		 * it.
		 **************************************************************************************************************/
		MutIt() noexcept = default;

		/**************************************************************************************************************
		 * Constructs an iterator to a bitmap pixel.
		 *
		 * @param[in] bitmap The sub-bitmap.
		 * @param[in] pos The position of the pixel within the sub-bitmap.
		 **************************************************************************************************************/
		MutIt(Bitmap& bitmap, glm::ivec2 pos) noexcept;

		/**************************************************************************************************************
		 * Three-way comparison operator.
		 **************************************************************************************************************/
		std::partial_ordering operator<=>(const MutIt&) const noexcept;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 **************************************************************************************************************/
		bool operator==(const MutIt&) const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator.
		 *
		 * @return The pixel reference.
		 **************************************************************************************************************/
		value_type operator*() const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator with a subscript.
		 *
		 * @param[in] diff The pixel relative to the iterator to access.
		 *
		 * @return The pixel reference.
		 **************************************************************************************************************/
		value_type operator[](difference_type diff) const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator with a subscript.
		 *
		 * @param[in] diff The pixel relative to the iterator to access (in vector form).
		 *
		 * @return The pixel reference.
		 **************************************************************************************************************/
		value_type operator[](glm::ivec2 diff) const noexcept;

		/**************************************************************************************************************
		 * Dereferences the iterator through a pointer.
		 *
		 * @return A pointer to the pixel reference.
		 **************************************************************************************************************/
		pointer operator->() const noexcept;

		/**************************************************************************************************************
		 * Increments the iterator.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		MutIt& operator++() noexcept;

		/**************************************************************************************************************
		 * Post-increments the iterator.
		 *
		 * @return An iterator with the prior state of the incremented iterator.
		 **************************************************************************************************************/
		MutIt operator++(int) noexcept;

		/**************************************************************************************************************
		 * Advances the iterator.
		 *
		 * @param[in] diff The number of pixels to move forward.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		MutIt& operator+=(difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Advances the iterator.
		 *
		 * @param[in] diff The number of pixels to move forward (in vector form).
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		MutIt& operator+=(glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move forward.
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator+(const MutIt& it, difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] diff The number of pixels to move forward.
		 * @param[in] it The iterator to add to.
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator+(difference_type diff, const MutIt& it) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move forward (in vector form).
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator+(const MutIt& it, glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Adds to an iterator.
		 *
		 * @param[in] diff The number of pixels to move forward (in vector form).
		 * @param[in] it The iterator to add to.
		 *
		 * @return An iterator equal to @em it advanced by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator+(glm::ivec2 diff, const MutIt& it) noexcept;

		/**************************************************************************************************************
		 * Decrements the iterator.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		MutIt& operator--() noexcept;

		/**************************************************************************************************************
		 * Post-decrements the iterator.
		 *
		 * @return An iterator with the prior state of the decremented iterator.
		 **************************************************************************************************************/
		MutIt operator--(int) noexcept;

		/**************************************************************************************************************
		 * Moves the iterator back.
		 *
		 * @param[in] diff The number of pixels to move backward.
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		MutIt& operator-=(difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Moves the iterator back.
		 *
		 * @param[in] diff The number of pixels to move backward (in vector form).
		 *
		 * @return A reference to the iterator.
		 **************************************************************************************************************/
		MutIt& operator-=(glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] diff The number of pixels to move backward.
		 * @param[in] it The iterator to add to.
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator-(const MutIt& it, difference_type diff) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move backward.
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator-(difference_type diff, const MutIt& it) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] diff The number of pixels to move backward (in vector form).
		 * @param[in] it The iterator to add to.
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator-(const MutIt& it, glm::ivec2 diff) noexcept;

		/**************************************************************************************************************
		 * Subtracts from an iterator.
		 *
		 * @param[in] it The iterator to add to.
		 * @param[in] diff The number of pixels to move backward (in vector form).
		 *
		 * @return An iterator equal to @em it moved back by @em diff pixels.
		 **************************************************************************************************************/
		friend MutIt operator-(glm::ivec2 diff, const MutIt& it) noexcept;

		/**************************************************************************************************************
		 * Gets the distance between two iterators.
		 *
		 * This function only makes sense if two iterators are pointing to the same bitmap.
		 *
		 * @param[in] l, r Iterators to the same bitmap.
		 *
		 * @return The distance between the iterators.
		 **************************************************************************************************************/
		friend difference_type operator-(const MutIt& l, const MutIt& r) noexcept;

	  private:
		PixelRef   _pixel;
		Bitmap*    _bitmap;
		glm::ivec2 _pos;
	};

	/// @}
} // namespace tr