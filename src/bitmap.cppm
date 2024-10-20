/**
 * @file bitmap.cppm
 * @brief Provides a bitmap class and related functionality.
 */

module;
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <EnumBitmask.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

export module tr:bitmap;

import std;
import glm;
import :color;
import :geometry;
import :handle;
import :iostream;
import :bitmap_format;
import :sdl;

export namespace tr {
	class Bitmap;

    /******************************************************************************************************************
	 * Error thrown when bitmap allocation failed.
	 ******************************************************************************************************************/
    struct BitmapBadAlloc : std::bad_alloc {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
		constexpr virtual const char* what() const noexcept;
	};

    /******************************************************************************************************************
	 * Error thrown when bitmap loading failed.
	 ******************************************************************************************************************/
	struct BitmapLoadError : FileError {
        using FileError::FileError;

        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
		virtual const char* what() const noexcept;
	};

    /******************************************************************************************************************
	 * Error thrown when bitmap saving failed.
	 ******************************************************************************************************************/
    struct BitmapSaveError : FileError {
        using FileError::FileError;

        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
		virtual const char* what() const noexcept;
	};


    /******************************************************************************************************************
	 * Supported image formats (bitmask).
	 ******************************************************************************************************************/
	enum class ImageFormat {
        BMP  = 0x0,
		JPG  = 0x1,
		PNG  = 0x2
	};
    /// @cond IMPLEMENTATION
	DEFINE_BITMASK_OPERATORS(ImageFormat);
    /// @endcond

    /******************************************************************************************************************
	 * Image format handler.
	 ******************************************************************************************************************/
	class SDL_Image {
	public:
        /**************************************************************************************************************
	     * Initializes the image format handler.
         *
         * @param formats A bitmask of formats to support.
	     **************************************************************************************************************/
		explicit SDL_Image(ImageFormat formats) noexcept;


        /**************************************************************************************************************
	     * Determines whether a particular format or set of formats is supported.
         *
         * @return True if the formats are supported, and false otherwise.
	     **************************************************************************************************************/
		static bool has(ImageFormat format) noexcept;

        /**************************************************************************************************************
	     * Gets the version of the image format handler library.
         *
         * @return The version of the image format handler library.
	     **************************************************************************************************************/
		static Version version() noexcept;
	private:
		struct Deleter { void operator()(bool) noexcept; /**< @private */ };
        Handle<bool, false, Deleter> _handle { true };
	};


    /******************************************************************************************************************
	 * Bitmap region reference.
	 ******************************************************************************************************************/
	class SubBitmap {
	public:
		class PixelRef;
		class Iterator;


        /**************************************************************************************************************
	     * Constructs a sub-bitmap.
         *
         * @param bitmap The parent bitmap.
         * @param rect The region of the sub-bitmap. If the region stretches outside of the bitmap bounds, a failed
         *             assertion may be triggered.
	     **************************************************************************************************************/
		SubBitmap(const Bitmap& bitmap, RectI2 rect) noexcept;


        /**************************************************************************************************************
	     * Gets the size of the sub-bitmap.
         *
         * @return The size of the sub-bitmap in pixels.
	     **************************************************************************************************************/
		glm::ivec2 size() const noexcept;


        /**************************************************************************************************************
	     * Creates a sub-bitmap of the sub-bitmap.
         *
         * @param rect The region of the sub-bitmap. If the region stretches outside of the sub-bitmap bounds, a failed
         *             assertion may be triggered.
         *
         * @return The new sub-bitmap.
	     **************************************************************************************************************/
		SubBitmap sub(RectI2 rect) noexcept;


        /**************************************************************************************************************
	     * Gets immutable access to a pixel of the bitmap.
         *
         * @param pos The position of the pixel within the sub-bitmap. Accessing an out-of-bounds pixel may trigger
         *            a failed assertion.
         *
         * @return An immutable reference to a pixel of the bitmap.
	     **************************************************************************************************************/
		PixelRef operator[](glm::ivec2 pos) const noexcept;


        /**************************************************************************************************************
	     * Gets an immutable iterator to the beginning of the sub-bitmap.
         *
         * @return An immutable iterator to the beginning of the sub-bitmap.
	     **************************************************************************************************************/
		Iterator begin() const noexcept;

        /**************************************************************************************************************
	     * Gets an immutable iterator to the beginning of the sub-bitmap.
         *
         * @return An immutable iterator to the beginning of the sub-bitmap.
	     **************************************************************************************************************/
		Iterator cbegin() const noexcept;

        /**************************************************************************************************************
	     * Gets an immutable iterator to one past the end of the sub-bitmap.
         *
         * @return An immutable iterator to one past the end of the sub-bitmap.
	     **************************************************************************************************************/
		Iterator end() const noexcept;

        /**************************************************************************************************************
	     * Gets an immutable iterator to one past the end of the sub-bitmap.
         *
         * @return An immutable iterator to one past the end of the sub-bitmap.
	     **************************************************************************************************************/
		Iterator cend() const noexcept;


        /**************************************************************************************************************
	     * Gets the raw data of the sub-bitmap.
         *
         * @return A pointer to the data of the sub-bitmap. This data is not necessarily contiguous,
         *         the distance between rows is pitch() bytes and may differ from `size().x * format().pixelBytes()`.
	     **************************************************************************************************************/
		const void* data() const noexcept;
		
        /**************************************************************************************************************
	     * Gets the format of the bitmap.
         *
         * @return The format of the bitmap.
	     **************************************************************************************************************/
		BitmapFormat format() const noexcept;

        /**************************************************************************************************************
	     * Gets the row length of the underlying bitmap.
         *
         * @return The row length of the underlying bitmap in pixels.
	     **************************************************************************************************************/
		int rowLength() const noexcept;

        /**************************************************************************************************************
	     * Gets the pitch of the bitmap.
         *
         * The pitch is the length of a row of pixels in bytes.
         *
         * @return The pitch of the bitmap in bytes.
	     **************************************************************************************************************/
		int pitch() const noexcept;
	private:
		std::reference_wrapper<const Bitmap> _bitmap;
		RectI2		                         _rect;

		friend class Bitmap;
	};

    /******************************************************************************************************************
	 * Abstracted read-only reference to a bitmap pixel.
	 ******************************************************************************************************************/
	class SubBitmap::PixelRef {
	public:
        /**************************************************************************************************************
	     * Casts the pixel to an RGBA8 color.
	     **************************************************************************************************************/
		operator RGBA8() const noexcept;
	private:
		const void*      _impl; 	  // A pointer to the pixel data.
		SDL_PixelFormat* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(const void* ptr, SDL_PixelFormat* format) noexcept;

		friend class SubBitmap;
		friend class Iterator;
	};

	/******************************************************************************************************************
	 * Immutable pixel iterator.
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
         * An iterator constructed in this manner is in an non-dereferencable state until a valid value is assigned to it.
         **************************************************************************************************************/
		Iterator() noexcept = default;

        /**************************************************************************************************************
         * Constructs an iterator to a bitmap pixel.
         *
         * @param bitmap The sub-bitmap.
         * @param pos The position of the pixel within the sub-bitmap.
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
         * @param diff The pixel relative to the iterator to access.
         *
         * @return The pixel reference.
         **************************************************************************************************************/
		value_type operator[](difference_type diff) const noexcept;

        /**************************************************************************************************************
         * Dereferences the iterator with a subscript.
         *
         * @param diff The pixel relative to the iterator to access (in vector form).
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
         * @param diff The number of pixels to move forward.
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		Iterator& operator+=(difference_type diff) noexcept;

        /**************************************************************************************************************
         * Advances the iterator.
         *
         * @param diff The number of pixels to move forward (in vector form).
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		Iterator& operator+=(glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move forward.
         *
         * @return An iterator equal to @em it advanced by @em diff pixels.
         **************************************************************************************************************/
		friend Iterator operator+(const Iterator& it, difference_type diff) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param diff The number of pixels to move forward.
         * @param it The iterator to add to.
         *
         * @return An iterator equal to @em it advanced by @em diff pixels.
         **************************************************************************************************************/
		friend Iterator operator+(difference_type diff, const Iterator& it) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move forward (in vector form).
         *
         * @return An iterator equal to @em it advanced by @em diff pixels.
         **************************************************************************************************************/
		friend Iterator operator+(const Iterator& it, glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param diff The number of pixels to move forward (in vector form).
         * @param it The iterator to add to.
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
         * @param diff The number of pixels to move backward.
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		Iterator& operator-=(difference_type diff) noexcept;

        /**************************************************************************************************************
         * Moves the iterator back.
         *
         * @param diff The number of pixels to move backward (in vector form).
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		Iterator& operator-=(glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param diff The number of pixels to move backward.
         * @param it The iterator to add to.
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend Iterator operator-(const Iterator& it, difference_type diff) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move backward.
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend Iterator operator-(difference_type diff, const Iterator& it) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param diff The number of pixels to move backward (in vector form).
         * @param it The iterator to add to.
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend Iterator operator-(const Iterator& it, glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move backward (in vector form).
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend Iterator operator-(glm::ivec2 diff, const Iterator& it) noexcept;

        /**************************************************************************************************************
         * Gets the distance between two iterators.
         *
         * This function only makes sense if two iterators are pointing to the same bitmap.
         *
         * @param lhs, rhs Iterators to the same bitmap.
         *
         * @return The distance between the iterators.
         **************************************************************************************************************/
		friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) noexcept;
	private:
		PixelRef   _pixel;
		glm::ivec2 _bitmapSize;
		int		   _pitch;
		glm::ivec2 _pos;
	};

	// Class representing a bitmap stored in memory.
	class Bitmap {
	public:
        class PixelRef;
		class MutIt;


        /**************************************************************************************************************
         * Immutable pixel reference.
         **************************************************************************************************************/
		using PixelCref = tr::SubBitmap::PixelRef;

        /**************************************************************************************************************
         * Immutable iterator.
         **************************************************************************************************************/
        using ConstIt = tr::SubBitmap::Iterator;


        /**************************************************************************************************************
	     * Creates a blank bitmap.
         *
         * @exception BitmapBadAlloc If allocating the bitmap failed.
         *
         * @param size The size of the bitmap.
         * @param format The format of the bitmap.
	     **************************************************************************************************************/
		Bitmap(glm::ivec2 size, BitmapFormat format);

        /**************************************************************************************************************
	     * Creates a bitmap from raw pixel data.
         *
         * @exception BitmapBadAlloc If allocating the bitmap failed.
         *
         * @param size The size of the bitmap.
         * @param pixelData The pixel data.
         * @param dataFormat The format of the pixel data.
	     **************************************************************************************************************/
		Bitmap(glm::ivec2 size, std::span<const std::byte> pixelData, BitmapFormat dataFormat);

        /**************************************************************************************************************
	     * Loads an embedded bitmap file.
         *
         * @exception BitmapBadAlloc If allocating the bitmap failed.
         *
         * @param embeddedFile The embedded file data.
	     **************************************************************************************************************/
		explicit Bitmap(std::span<const std::byte> embeddedFile);
		
        /**************************************************************************************************************
	     * Loads an bitmap from file.
         *
         * Make sure to initialize support for any file formats you plan to use.
         *
         * @exception FileNotFound If the file could not be found.
         * @exception BitmapLoadError If loading the bitmap failed.
         *
         * @param path The path to the bitmap file.
	     **************************************************************************************************************/
		explicit Bitmap(const std::filesystem::path& path);

		/**************************************************************************************************************
	     * Clones a bitmap.
         *
         * @exception BitmapBadAlloc If allocating the bitmap failed.
         *
         * @param bitmap The bitmap to clone.
         * @param format The format of the new bitmap.
	     **************************************************************************************************************/
		Bitmap(const Bitmap& bitmap, BitmapFormat format);
		
        /**************************************************************************************************************
	     * Clones a sub-bitmap.
         *
         * @exception BitmapBadAlloc If allocating the bitmap failed.
         *
         * @param source The sub-bitmap to clone.
         * @param format The format of the new bitmap.
	     **************************************************************************************************************/
		Bitmap(SubBitmap source, BitmapFormat format);


        /**************************************************************************************************************
	     * Equality comparison operator.
	     **************************************************************************************************************/
		bool operator==(const Bitmap&) const noexcept = default;


        /**************************************************************************************************************
	     * Gets the size of the bitmap.
         *
         * @return The size of the bitmap in pixels.
	     **************************************************************************************************************/
		glm::ivec2 size() const noexcept;


		/**************************************************************************************************************
	     * Gets mutable access to a pixel of the bitmap.
         *
         * @param pos The position of the pixel within the bitmap. Accessing an out-of-bounds pixel may trigger
         *            a failed assertion.
         *
         * @return An immutable reference to a pixel of the bitmap.
	     **************************************************************************************************************/
		PixelRef operator[](glm::ivec2 pos) noexcept;
		
        /**************************************************************************************************************
	     * Gets immutable access to a pixel of the bitmap.
         *
         * @param pos The position of the pixel within the sub-bitmap. Accessing an out-of-bounds pixel may trigger
         *            a failed assertion.
         *
         * @return An immutable reference to a pixel of the bitmap.
	     **************************************************************************************************************/
		PixelCref operator[](glm::ivec2 pos) const noexcept;


        /**************************************************************************************************************
	     * Gets a mutable iterator to the beginning of the sub-bitmap.
         *
         * @return A mutable iterator to the beginning of the sub-bitmap.
	     **************************************************************************************************************/
		MutIt begin() noexcept;

        /**************************************************************************************************************
	     * Gets an immutable iterator to the beginning of the sub-bitmap.
         *
         * @return An immutable iterator to the beginning of the sub-bitmap.
	     **************************************************************************************************************/
		ConstIt begin() const noexcept;

        /**************************************************************************************************************
	     * Gets an immutable iterator to the beginning of the sub-bitmap.
         *
         * @return An immutable iterator to the beginning of the sub-bitmap.
	     **************************************************************************************************************/
		ConstIt cbegin() const noexcept;

        /**************************************************************************************************************
	     * Gets a mutable iterator to one past the end of the sub-bitmap.
         *
         * @return A mutable iterator to one past the end of the sub-bitmap.
	     **************************************************************************************************************/
		MutIt end() noexcept;

        /**************************************************************************************************************
	     * Gets an immutable iterator to one past the end of the sub-bitmap.
         *
         * @return An immutable iterator to one past the end of the sub-bitmap.
	     **************************************************************************************************************/
		ConstIt end() const noexcept;

        /**************************************************************************************************************
	     * Gets an immutable iterator to one past the end of the sub-bitmap.
         *
         * @return An immutable iterator to one past the end of the sub-bitmap.
	     **************************************************************************************************************/
		ConstIt cend() const noexcept;


        /**************************************************************************************************************
	     * Blits a sub-bitmap to the bitmap.
         *
         * @param tl The top-left corner of the region to blit to.
         *           If the region stretches outside of the sub-bitmap bounds, a failed assertion may be triggered.
         * @param source The source sub-bitmap.
	     **************************************************************************************************************/
		void blit(glm::ivec2 tl, SubBitmap source) noexcept;
		
        /**************************************************************************************************************
	     * Fills a region of the bitmap with a solid color.
         *
         * @param rect The rect to fill.
         *             If the region stretches outside of the sub-bitmap bounds, a failed assertion may be triggered.
         * @param color The fill color.
	     **************************************************************************************************************/
		void fill(RectI2 rect, RGBA8 color) noexcept;


        /**************************************************************************************************************
	     * Creates a sub-bitmap spanning the entire bitmap.
	     **************************************************************************************************************/
		operator SubBitmap() const noexcept;
		
        /**************************************************************************************************************
	     * Creates a sub-bitmap of the bitmap.
         *
         * @param rect The region of the sub-bitmap. If the region stretches outside of the bitmap bounds, a failed
         *             assertion may be triggered.
         *
         * @return The sub-bitmap.
	     **************************************************************************************************************/
		SubBitmap sub(RectI2 rect) const noexcept;


        /**************************************************************************************************************
	     * Gets the raw data of the bitmap.
         *
         * @return A pointer to the data of the bitmap. This data is not necessarily contiguous,
         *         the distance between rows is pitch() bytes and may differ from `size().x * format().pixelBytes()`.
	     **************************************************************************************************************/
		void* data() noexcept;

        /**************************************************************************************************************
	     * Gets the raw data of the bitmap.
         *
         * @return A pointer to the data of the bitmap. This data is not necessarily contiguous,
         *         the distance between rows is pitch() bytes and may differ from `size().x * format().pixelBytes()`.
	     **************************************************************************************************************/
		const void* data() const noexcept;

        /**************************************************************************************************************
	     * Gets the format of the bitmap.
         *
         * @return The format of the bitmap.
	     **************************************************************************************************************/
		BitmapFormat format() const noexcept;

        /**************************************************************************************************************
	     * Gets the pitch of the bitmap.
         *
         * The pitch is the length of a row of pixels in bytes.
         *
         * @return The pitch of the bitmap in bytes.
	     **************************************************************************************************************/
		int pitch() const noexcept;


        /**************************************************************************************************************
	     * Saves the bitmap to file.
         *
         * @exception BitmapSaveError If saving the bitmap failed.
         *
         * @param path The path to the file.
         * @param format The image format.
	     **************************************************************************************************************/
		void save(const std::filesystem::path& path, ImageFormat format);
	private:
		struct Deleter { void operator()(SDL_Surface* bitmap) noexcept; /**< @private */ };
		std::unique_ptr<SDL_Surface, Deleter> _impl;

		Bitmap(SDL_Surface* ptr);

		friend class SubBitmap;
		friend class tr::SubBitmap::Iterator;
		friend class MutIt;
		friend class Cursor;
		friend class TTFont;
		friend class WindowView;
	};

    /******************************************************************************************************************
	 * Abstracted mutable reference to a bitmap pixel.
	 ******************************************************************************************************************/
	class Bitmap::PixelRef {
	public:
		/**************************************************************************************************************
	     * Casts the pixel to an RGBA8 color.
	     **************************************************************************************************************/
		operator RGBA8() const noexcept;

		/**************************************************************************************************************
	     * Sets the pixel to an RGBA color.
         *
         * @param color The color to set the pixel to.
         *
         * @return A reference to the pixel being assigned.
	     **************************************************************************************************************/
		PixelRef& operator=(RGBA8 color) noexcept;
	private:
		void*            _impl;   // A pointer to the pixel data.
		SDL_PixelFormat* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(void* ptr, SDL_PixelFormat* format) noexcept;

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
         * An iterator constructed in this manner is in an non-dereferencable state until a valid value is assigned to it.
         **************************************************************************************************************/
		MutIt() noexcept = default;

        /**************************************************************************************************************
         * Constructs an iterator to a bitmap pixel.
         *
         * @param bitmap The sub-bitmap.
         * @param pos The position of the pixel within the sub-bitmap.
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
         * @param diff The pixel relative to the iterator to access.
         *
         * @return The pixel reference.
         **************************************************************************************************************/
		value_type operator[](difference_type diff) const noexcept;

        /**************************************************************************************************************
         * Dereferences the iterator with a subscript.
         *
         * @param diff The pixel relative to the iterator to access (in vector form).
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
         * @param diff The number of pixels to move forward.
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		MutIt& operator+=(difference_type diff) noexcept;

        /**************************************************************************************************************
         * Advances the iterator.
         *
         * @param diff The number of pixels to move forward (in vector form).
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		MutIt& operator+=(glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move forward.
         *
         * @return An iterator equal to @em it advanced by @em diff pixels.
         **************************************************************************************************************/
		friend MutIt operator+(const MutIt& it, difference_type diff) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param diff The number of pixels to move forward.
         * @param it The iterator to add to.
         *
         * @return An iterator equal to @em it advanced by @em diff pixels.
         **************************************************************************************************************/
		friend MutIt operator+(difference_type diff, const MutIt& it) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move forward (in vector form).
         *
         * @return An iterator equal to @em it advanced by @em diff pixels.
         **************************************************************************************************************/
		friend MutIt operator+(const MutIt& it, glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Adds to the iterator.
         *
         * @param diff The number of pixels to move forward (in vector form).
         * @param it The iterator to add to.
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
         * @param diff The number of pixels to move backward.
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		MutIt& operator-=(difference_type diff) noexcept;

        /**************************************************************************************************************
         * Moves the iterator back.
         *
         * @param diff The number of pixels to move backward (in vector form).
         *
         * @return A reference to the iterator.
         **************************************************************************************************************/
		MutIt& operator-=(glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param diff The number of pixels to move backward.
         * @param it The iterator to add to.
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend MutIt operator-(const MutIt& it, difference_type diff) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move backward.
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend MutIt operator-(difference_type diff, const MutIt& it) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param diff The number of pixels to move backward (in vector form).
         * @param it The iterator to add to.
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend MutIt operator-(const MutIt& it, glm::ivec2 diff) noexcept;

        /**************************************************************************************************************
         * Subtracts from the iterator.
         *
         * @param it The iterator to add to.
         * @param diff The number of pixels to move backward (in vector form).
         *
         * @return An iterator equal to @em it moved back by @em diff pixels.
         **************************************************************************************************************/
		friend MutIt operator-(glm::ivec2 diff, const MutIt& it) noexcept;

        /**************************************************************************************************************
         * Gets the distance between two iterators.
         *
         * This function only makes sense if two iterators are pointing to the same bitmap.
         *
         * @param lhs, rhs Iterators to the same bitmap.
         *
         * @return The distance between the iterators.
         **************************************************************************************************************/
		friend difference_type operator-(const MutIt& lhs, const MutIt& rhs) noexcept;
	private:
		PixelRef   _pixel;
		Bitmap*    _bitmap;
		glm::ivec2 _pos;
	};


    /******************************************************************************************************************
     * Creates a bitmap with the missing texture checkerboard pattern.
     *
     * @exception BitmapBadAlloc If allocating the bitmap failed.
     *
     * @param size The size of the bitmap.
     *
     * @return A bitmap with the missing texture checkerboard pattern.
     ******************************************************************************************************************/
	Bitmap createCheckerboard(glm::ivec2 size);
}

/// @cond IMPLEMENTATION

namespace tr {
    template <class T> T checkNotNull(T ptr);
    RGBA8 getPixelColor(const void* data, SDL_PixelFormat* format);
}

template <class T>
T tr::checkNotNull(T ptr)
{
    if (ptr == nullptr) {
        throw BitmapBadAlloc {};
    }
    return ptr;
}

tr::RGBA8 tr::getPixelColor(const void* data, SDL_PixelFormat* format)
{
    std::uint32_t value;
    switch (format->BytesPerPixel) {
    case 1:
        value = *((const std::uint8_t*)(data));
    case 2:
        value = *((const std::uint16_t*)(data));
    case 3:
        value = ((const std::uint8_t*)(data))[0] << 16 |
                ((const std::uint8_t*)(data))[1] << 8  |
                ((const std::uint8_t*)(data))[2];
    case 4:
        value = *((const std::uint32_t*)(data));
    }

    RGBA8 color;
    SDL_GetRGBA(value, format, &color.r, &color.g, &color.b, &color.a);
    return color;
}

constexpr const char* tr::BitmapBadAlloc::what() const noexcept 
{
    return "failed bitmap allocation";
}

const char* tr::BitmapLoadError::what() const noexcept
{
    static std::string str;
    str.clear();
    format_to(back_inserter(str), "Failed to load bitmap file ({}): '{}'", SDL_GetError(), path());
    return str.c_str();
}

const char* tr::BitmapSaveError::what() const noexcept
{
    static std::string str;
    str.clear();
    format_to(back_inserter(str), "Failed to save bitmap file ({}): '{}'", IMG_GetError(), path());
    return str.c_str();
}

void tr::SDL_Image::Deleter::operator()(bool) noexcept
{
    IMG_Quit();
}

tr::SDL_Image::SDL_Image(ImageFormat formats) noexcept
{
    IMG_Init(int(formats));
}

bool tr::SDL_Image::has(ImageFormat format) noexcept
{
    return format == ImageFormat::BMP ? true : IMG_Init(0) & int(format);
}

tr::Version tr::SDL_Image::version() noexcept
{
    return std::bit_cast<Version>(*IMG_Linked_Version());
}

tr::SubBitmap::SubBitmap(const Bitmap& bitmap, RectI2 rect) noexcept
    : _bitmap { bitmap }
    , _rect { rect }
{}

glm::ivec2 tr::SubBitmap::size() const noexcept
{
    return _rect.size;
}

tr::SubBitmap tr::SubBitmap::sub(RectI2 rect) noexcept
{
    assert(_rect.contains(rect.tl + rect.size));
    return { _bitmap, { _rect.tl + rect.tl, rect.size } };
}

tr::SubBitmap::PixelRef tr::SubBitmap::operator[](glm::ivec2 pos) const noexcept
{
    return *(begin() + pos);
}

tr::SubBitmap::Iterator tr::SubBitmap::begin() const noexcept
{
    return { *this, {} };
}

tr::SubBitmap::Iterator tr::SubBitmap::cbegin() const noexcept
{
    return begin();
}

tr::SubBitmap::Iterator tr::SubBitmap::end() const noexcept
{
    return { *this, { 0, size().y } };
}

tr::SubBitmap::Iterator tr::SubBitmap::cend() const noexcept
{
    return end();
}

const void* tr::SubBitmap::data() const noexcept
{
    return (const std::byte*)(_bitmap.get().data()) + pitch() * _rect.tl.y + format().pixelBytes() * _rect.tl.x;
}

tr::BitmapFormat tr::SubBitmap::format() const noexcept
{
    return _bitmap.get().format();
}

int tr::SubBitmap::rowLength() const noexcept
{
    return _bitmap.get().size().x;
}

int tr::SubBitmap::pitch() const noexcept
{
    return _bitmap.get().pitch();
}

tr::SubBitmap::PixelRef::PixelRef(const void* ptr, SDL_PixelFormat* format) noexcept
    : _impl { ptr }
    , _format { format }
{}

tr::SubBitmap::PixelRef::operator RGBA8() const noexcept
{
    return getPixelColor(_impl, _format);
}

tr::SubBitmap::Iterator::Iterator(SubBitmap bitmap, glm::ivec2 pos) noexcept
    : _pixel { (const std::byte*)(bitmap.data()) + bitmap.pitch() * pos.y + bitmap.format().pixelBytes() * pos.x, bitmap._bitmap.get()._impl.get()->format }
    , _bitmapSize { bitmap.size() }
    , _pitch { bitmap.pitch() }
    , _pos { pos }
{}

std::partial_ordering tr::SubBitmap::Iterator::operator<=>(const Iterator& rhs) const noexcept
{
    if (_pixel._impl != nullptr && rhs._pixel._impl != nullptr) {
        return _pixel._impl <=> rhs._pixel._impl;
    }
    else if (_pixel._impl == nullptr && rhs._pixel._impl == nullptr) { 
        return std::partial_ordering::equivalent;
    }
    else {
        return std::partial_ordering::unordered;
    }
}

bool tr::SubBitmap::Iterator::operator==(const Iterator& rhs) const noexcept
{
    return *this <=> rhs == std::strong_ordering::equal;
}

tr::SubBitmap::Iterator::value_type tr::SubBitmap::Iterator::operator*() const noexcept
{
    assert(_pixel._impl != nullptr && RectI2 { _bitmapSize }.contains(_pos));
    return _pixel;
}

tr::SubBitmap::Iterator::value_type tr::SubBitmap::Iterator::operator[](int diff) const noexcept
{
    return *(*this + diff);
}

tr::SubBitmap::Iterator::value_type tr::SubBitmap::Iterator::operator[](glm::ivec2 diff) const noexcept
{
    return *(*this + diff);
}

tr::SubBitmap::Iterator::pointer tr::SubBitmap::Iterator::operator->() const noexcept
{
    return &_pixel;
}

tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator++() noexcept
{
    return *this += 1;
}

tr::SubBitmap::Iterator tr::SubBitmap::Iterator::operator++(int) noexcept
{
    Iterator copy { *this };
    ++*this;
    return copy;
}

tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator+=(int diff) noexcept
{
    auto lines { diff / _bitmapSize.x };
    diff %= _bitmapSize.x;
    if (diff + _pos.x >= _bitmapSize.x) {
        ++lines;
        diff -= _bitmapSize.x;
    }
    else if (diff + _pos.x < 0) {
        --lines;
        diff += _bitmapSize.x;
    }
    _pixel._impl = (const std::byte*)(_pixel._impl) + _pitch * lines + _pixel._format->BytesPerPixel * diff;
    return *this;
}

tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator+=(glm::ivec2 diff) noexcept
{
    return *this += (diff.y * _bitmapSize.x + diff.x);
}

tr::SubBitmap::Iterator tr::operator+(const tr::SubBitmap::Iterator& it, int diff) noexcept
{
    tr::SubBitmap::Iterator copy { it };
    copy += diff;
    return copy;
}

tr::SubBitmap::Iterator tr::operator+(int diff, const tr::SubBitmap::Iterator& it) noexcept
{
    return it + diff;
}

tr::SubBitmap::Iterator tr::operator+(const tr::SubBitmap::Iterator& it, glm::ivec2 diff) noexcept
{
    tr::SubBitmap::Iterator copy { it };
    copy += diff;
    return copy;
}

tr::SubBitmap::Iterator tr::operator+(glm::ivec2 diff, const tr::SubBitmap::Iterator& it) noexcept
{
    return it + diff;
}


tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator--() noexcept
{
    return *this -= 1;
}

tr::SubBitmap::Iterator tr::SubBitmap::Iterator::operator--(int) noexcept
{
    Iterator copy { *this };
    --*this;
    return copy;
}

tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator-=(int diff) noexcept
{
    return *this += -diff;
}

tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator-=(glm::ivec2 diff) noexcept
{
    return *this += -diff;
}

tr::SubBitmap::Iterator tr::operator-(const tr::SubBitmap::Iterator& it, int diff) noexcept
{
    tr::SubBitmap::Iterator copy { it };
    copy -= diff;
    return copy;
}

tr::SubBitmap::Iterator tr::operator-(int diff, const tr::SubBitmap::Iterator& it) noexcept
{
    return it - diff;
}

tr::SubBitmap::Iterator tr::operator-(const tr::SubBitmap::Iterator& it, glm::ivec2 diff) noexcept
{
    tr::SubBitmap::Iterator copy { it };
    copy -= diff;
    return copy;
}

tr::SubBitmap::Iterator tr::operator-(glm::ivec2 diff, const tr::SubBitmap::Iterator& it) noexcept
{
    return it - diff;
}

int tr::operator-(const tr::SubBitmap::Iterator& lhs, const tr::SubBitmap::Iterator& rhs) noexcept
{
    return (lhs._pos.y * lhs._bitmapSize.x + lhs._pos.x) - (rhs._pos.y * rhs._bitmapSize.x + rhs._pos.x);
}

tr::Bitmap::Bitmap(SDL_Surface* ptr)
    : _impl { checkNotNull(ptr) }
{}

tr::Bitmap::Bitmap(glm::ivec2 size, BitmapFormat format)
    : _impl { checkNotNull(SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, format.pixelBits(), SDL_PixelFormatEnum(BitmapFormat::Type(format)))) }
{}

tr::Bitmap::Bitmap(glm::ivec2 size, std::span<const std::byte> pixelData, BitmapFormat dataFormat)
    : _impl { checkNotNull(SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, dataFormat.pixelBits(), std::uint32_t(BitmapFormat::Type(dataFormat)))) }
{}

tr::Bitmap::Bitmap(std::span<const std::byte> embeddedFile)
    : _impl { checkNotNull(IMG_Load_RW(SDL_RWFromConstMem(embeddedFile.data(), embeddedFile.size()), true)) }
{}

tr::Bitmap::Bitmap(const std::filesystem::path& path)
{
    if (!is_regular_file(path)) {
        throw FileNotFound { path };
    }

    #ifdef _WIN32
    _impl.reset(IMG_Load_RW(SDL_RWFromFP(_wfopen(path.c_str(), L"r"), true), true));
    #else
    _impl.reset(IMG_Load(path.c_str()));
    #endif
    if (_impl == nullptr) {
        throw BitmapLoadError { path };
    }
}

tr::Bitmap::Bitmap(const Bitmap& bitmap, BitmapFormat format)
    : _impl { checkNotNull(SDL_ConvertSurfaceFormat(bitmap._impl.get(), std::uint32_t(BitmapFormat::Type(format)), 0)) }
{}

tr::Bitmap::Bitmap(SubBitmap source, BitmapFormat format)
    : Bitmap { source.size(), format } 
{
    blit({}, source);
}

void tr::Bitmap::Deleter::operator()(SDL_Surface* bitmap) noexcept
{
    SDL_FreeSurface(bitmap);
}

glm::ivec2 tr::Bitmap::size() const noexcept
{
    return { _impl.get()->w, _impl.get()->h };
}

tr::Bitmap::PixelRef tr::Bitmap::operator[](glm::ivec2 pos) noexcept
{
    return *(begin() + pos);
}

tr::Bitmap::PixelCref tr::Bitmap::operator[](glm::ivec2 pos) const noexcept
{
    return *(begin() + pos);
}

tr::Bitmap::MutIt tr::Bitmap::begin() noexcept
{
    assert(_impl != nullptr);
    return { *this, {} };
}

tr::Bitmap::ConstIt tr::Bitmap::begin() const noexcept
{
    return cbegin();
}

tr::Bitmap::ConstIt tr::Bitmap::cbegin() const noexcept
{
    assert(_impl != nullptr);
    return SubBitmap(*this).begin();
}

tr::Bitmap::MutIt tr::Bitmap::end() noexcept
{
    assert(_impl != nullptr);
    return { *this, { 0, size().y } };
}

tr::Bitmap::ConstIt tr::Bitmap::end() const noexcept
{
    assert(_impl != nullptr);
    return cend();
}

tr::Bitmap::ConstIt tr::Bitmap::cend() const noexcept
{
    assert(_impl != nullptr);
    return SubBitmap(*this).end();
}

void tr::Bitmap::blit(glm::ivec2 tl, SubBitmap source) noexcept
{
    assert(_impl != nullptr);
    assert(RectI2 { size() }.contains(tl + source.size()));
    SDL_Rect sdlSource { source._rect.tl.x, source._rect.tl.y, source.size().x, source.size().y };
    SDL_Rect sdlDest { tl.x, tl.y };
    SDL_BlitSurface((SDL_Surface*)(source._bitmap.get()._impl.get()), &sdlSource, _impl.get(), &sdlDest);
}

void tr::Bitmap::fill(RectI2 rect, RGBA8 color) noexcept
{
    assert(_impl != nullptr);
    assert(RectI2 { size() }.contains(rect.tl + rect.size));
    SDL_Rect sdlRect { rect.tl.x, rect.tl.y, rect.size.x, rect.size.y };
    SDL_FillRect(_impl.get(), &sdlRect, SDL_MapRGBA(_impl.get()->format, color.r, color.g, color.b, color.a));
}

tr::Bitmap::operator SubBitmap() const noexcept
{
    return sub({ {}, size() });
}

tr::SubBitmap tr::Bitmap::sub(RectI2 rect) const noexcept
{
    return SubBitmap { *this, rect };
}

void* tr::Bitmap::data() noexcept
{
    assert(_impl != nullptr);
    return _impl.get()->pixels;
}

const void* tr::Bitmap::data() const noexcept
{
    assert(_impl != nullptr);
    return _impl.get()->pixels;
}

tr::BitmapFormat tr::Bitmap::format() const noexcept
{
    assert(_impl != nullptr);
    assert(_impl.get()->format);
    return BitmapFormat::Type(_impl.get()->format->format);
}

int tr::Bitmap::pitch() const noexcept
{
    assert(_impl != nullptr);
    return _impl.get()->pitch;
}

void tr::Bitmap::save(const std::filesystem::path& path, ImageFormat format)
{
    assert(_impl != nullptr);

    auto surface { _impl.get() };
    switch (format) {
    case ImageFormat::BMP:
        #ifdef _WIN32
        if (SDL_SaveBMP_RW(surface, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true) < 0) {
        #else
        if (SDL_SaveBMP(surface, path.c_str()) < 0) {
        #endif
            throw BitmapSaveError { path };
        }
    break;
    case ImageFormat::PNG:
        #ifdef _WIN32
        if (IMG_SavePNG_RW(surface, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true) < 0) {
        #else
        if (IMG_SavePNG(surface, path.c_str()) < 0) {
        #endif
             throw BitmapSaveError { path };
        }
    break;
    case ImageFormat::JPG:
        #ifdef _WIN32
        if (IMG_SaveJPG_RW(surface, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true, 70) < 0) {
        #else
        if (IMG_SaveJPG(surface, path.c_str(), 70) < 0) {
        #endif
             throw BitmapSaveError { path };
        }
    break;
    default:
        assert(false);
    }
};

tr::Bitmap tr::createCheckerboard(glm::ivec2 size)
{
    constexpr RGBA8 BLACK { 0, 0, 0, 255 };
    constexpr RGBA8 MAGENTA { 255, 0, 255, 255 };
    const glm::ivec2 halfSize { size / 2 };

    Bitmap bitmap { size, BitmapFormat::RGB_332 };
    bitmap.fill({ { 0, 0 }, halfSize }, BLACK);
    bitmap.fill({ { halfSize.x, 0 }, halfSize }, MAGENTA);
    bitmap.fill({ { 0, halfSize.x }, halfSize }, MAGENTA);
    bitmap.fill({ halfSize, halfSize }, BLACK);
    return bitmap;
}

tr::Bitmap::PixelRef::PixelRef(void* ptr, SDL_PixelFormat* format) noexcept
    : _impl { ptr }
    , _format { format }
{}

tr::Bitmap::PixelRef::operator RGBA8() const noexcept
{
    return getPixelColor(_impl, _format);
}

tr::Bitmap::PixelRef& tr::Bitmap::PixelRef::operator=(RGBA8 color) noexcept
{
    auto formatted { SDL_MapRGBA(_format, color.r, color.g, color.b, color.a) };
    switch (_format->BytesPerPixel) {
    case 1:
        *((std::uint8_t*)(_impl)) = formatted;
        break;
    case 2:
        *(std::uint16_t*)(_impl) = formatted;
        break;
    case 3:
        ((std::uint8_t*)(_impl))[0] = std::uint8_t(formatted >> 16);
        ((std::uint8_t*)(_impl))[1] = std::uint8_t(formatted >> 8);
        ((std::uint8_t*)(_impl))[2] = std::uint8_t(formatted);
        break;
    case 4:
        *(std::uint32_t*)(_impl) = formatted;
    }
    return *this;
}

tr::Bitmap::MutIt::MutIt(Bitmap& bitmap, glm::ivec2 pos) noexcept
    : _pixel { (std::byte*)(bitmap.data()) + bitmap.pitch() * pos.y + bitmap.format().pixelBytes() * pos.x, bitmap._impl.get()->format }
    , _bitmap { &bitmap }
    , _pos { pos }
{}

std::partial_ordering tr::Bitmap::MutIt::operator<=>(const tr::Bitmap::MutIt& rhs) const noexcept
{
    if (_pixel._impl != nullptr && rhs._pixel._impl != nullptr) {
        return _pixel._impl <=> rhs._pixel._impl;
    }
    else if (_pixel._impl == nullptr && rhs._pixel._impl == nullptr) { 
        return std::partial_ordering::equivalent;
    }
    else {
        return std::partial_ordering::unordered;
    }
}

bool tr::Bitmap::MutIt::operator==(const MutIt& rhs) const noexcept
{
    return *this <=> rhs == std::strong_ordering::equal;
}

tr::Bitmap::MutIt::value_type tr::Bitmap::MutIt::operator*() const noexcept
{
    assert(_pixel._impl != nullptr && RectI2 { _bitmap->size() }.contains(_pos));
    return _pixel;
}

tr::Bitmap::MutIt::value_type tr::Bitmap::MutIt::operator[](int diff) const noexcept
{
    return *(*this + diff);
}

tr::Bitmap::MutIt::value_type tr::Bitmap::MutIt::operator[](glm::ivec2 diff) const noexcept
{
    return *(*this + diff);
}

tr::Bitmap::MutIt::pointer tr::Bitmap::MutIt::operator->() const noexcept
{
    return &_pixel;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator++() noexcept
{
    return *this += 1;
}

tr::Bitmap::MutIt tr::Bitmap::MutIt::operator++(int) noexcept
{
    MutIt copy { *this };
    ++*this;
    return copy;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator+=(int diff) noexcept
{
    assert(_pixel._impl != nullptr);

    const glm::ivec2 bitmapSize { _bitmap->size() };
    auto lines { diff / bitmapSize.x };
    diff %= bitmapSize.x;
    if (diff + _pos.x >= bitmapSize.x) {
        ++lines;
        diff -= bitmapSize.x;
    }
    else if (diff + _pos.x < 0) {
        --lines;
        diff += bitmapSize.x;
    }
    _pixel._impl = (std::byte*)(_pixel._impl) + _bitmap->pitch() * lines + _pixel._format->BytesPerPixel * diff;
    _pos += glm::ivec2 { diff, lines };
    return *this;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator+=(glm::ivec2 diff) noexcept
{
    return *this += (diff.y * _bitmap->size().x + diff.x);
}

tr::Bitmap::MutIt tr::operator+(const tr::Bitmap::MutIt& it, int diff) noexcept
{
    tr::Bitmap::MutIt copy { it };
    copy += diff;
    return copy;
}

tr::Bitmap::MutIt tr::operator+(int diff, const tr::Bitmap::MutIt& it) noexcept
{
    return it + diff;
}

tr::Bitmap::MutIt tr::operator+(const tr::Bitmap::MutIt& it, glm::ivec2 diff) noexcept
{
    tr::Bitmap::MutIt copy { it };
    copy += diff;
    return copy;
}

tr::Bitmap::MutIt tr::operator+(glm::ivec2 diff, const tr::Bitmap::MutIt& it) noexcept
{
    return it + diff;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator--() noexcept
{
    return *this -= 1;
}

tr::Bitmap::MutIt tr::Bitmap::MutIt::operator--(int) noexcept
{
    MutIt copy { *this };
    --*this;
    return copy;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator-=(int diff) noexcept
{
    return *this += -diff;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator-=(glm::ivec2 diff) noexcept
{
    return *this += -diff;
}

tr::Bitmap::MutIt tr::operator-(const tr::Bitmap::MutIt& it, int diff) noexcept
{
    tr::Bitmap::MutIt copy { it };
    copy -= diff;
    return copy;
}

tr::Bitmap::MutIt tr::operator-(int diff, const tr::Bitmap::MutIt& it) noexcept
{
    return it - diff;
}

tr::Bitmap::MutIt tr::operator-(const tr::Bitmap::MutIt& it, glm::ivec2 diff) noexcept
{
    tr::Bitmap::MutIt copy { it };
    copy -= diff;
    return copy;
}

tr::Bitmap::MutIt tr::operator-(glm::ivec2 diff, const tr::Bitmap::MutIt& it) noexcept
{
    return it - diff;
}

int tr::operator-(const tr::Bitmap::MutIt& lhs, const tr::Bitmap::MutIt& rhs) noexcept
{
    assert(lhs._bitmap == rhs._bitmap && lhs._bitmap != nullptr);
    return (lhs._pos.y * lhs._bitmap->size().x + lhs._pos.x) - (rhs._pos.y * rhs._bitmap->size().x + rhs._pos.x);
}

/// @endcond