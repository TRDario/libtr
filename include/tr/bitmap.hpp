/**
 * @file bitmap.hpp
 * @brief Provides a bitmap class and related functionality.
 */

#pragma once
#include "bitmap_format.hpp"
#include "color.hpp"
#include "dependencies/EnumBitmask.hpp"
#include "geometry.hpp"
#include "iostream.hpp"
#include "sdl.hpp"

struct SDL_Surface;
struct SDL_PixelFormat;

namespace tr {
	class Bitmap;
	class BitmapView;

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
		BMP = 0x0,
		JPG = 0x1,
		PNG = 0x2
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
		struct Deleter {
			/// @private
			void operator()(bool) const noexcept;
		};

		Handle<bool, false, Deleter> _handle{true};
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
		 * Constructs a sub-bitmap.
		 *
		 * @param view The parent bitmap view.
		 * @param rect The region of the sub-bitmap. If the region stretches outside of the bitmap bounds, a failed
		 *             assertion may be triggered.
		 **************************************************************************************************************/
		SubBitmap(const BitmapView& bitmap, RectI2 rect) noexcept;

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
		const std::byte* data() const noexcept;

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
		SDL_Surface* _bitmap;
		RectI2       _rect;

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
		const std::byte* _impl;   // A pointer to the pixel data.
		SDL_PixelFormat* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(const std::byte* ptr, SDL_PixelFormat* format) noexcept;

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
		 * An iterator constructed in this manner is in an non-dereferencable state until a valid value is assigned to
		 *it.
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
		 * @param l, r Iterators to the same bitmap.
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
	 * Non-owning view over bitmap data.
	 *
	 * No instances of this class can be created before initializing SDL.
	 ******************************************************************************************************************/
	class BitmapView {
	  public:
		using PixelRef = SubBitmap::PixelRef;
		using Iterator = SubBitmap::Iterator;

		/**************************************************************************************************************
		 * Creates a bitmap view over contiguous pixel data.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap view's internals failed.
		 *
		 * @param rawData The pixel data span. The size of the span must match size.x * size.y * [pixel bytes],
		 *                otherwise a failed assertion may be triggered.
		 * @param size The size of the bitmap.
		 * @param format The format of the bitmap.
		 **************************************************************************************************************/
		BitmapView(std::span<const std::byte> rawData, glm::ivec2 size, BitmapFormat format);

		/**************************************************************************************************************
		 * Creates a bitmap view over pixel data.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap view's internals failed.
		 *
		 * @param rawDataStart A pointer to the start of the pixel data.
		 * @param pitch The distance between rows of pixels in bytes.
		 * @param size The size of the bitmap.
		 * @param format The format of the bitmap.
		 **************************************************************************************************************/
		BitmapView(const std::byte* rawDataStart, int pitch, glm::ivec2 size, BitmapFormat format);

		/**************************************************************************************************************
		 * Gets the size of the bitmap.
		 *
		 * @return The size of the bitmap in pixels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

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
		 * Gets an immutable iterator to the beginning of the bitmap.
		 *
		 * @return An immutable iterator to the beginning of the bitmap.
		 **************************************************************************************************************/
		Iterator begin() const noexcept;

		/**************************************************************************************************************
		 * Gets an immutable iterator to the beginning of the bitmap.
		 *
		 * @return An immutable iterator to the beginning of the bitmap.
		 **************************************************************************************************************/
		Iterator cbegin() const noexcept;

		/**************************************************************************************************************
		 * Gets an immutable iterator to one past the end of the bitmap.
		 *
		 * @return An immutable iterator to one past the end of the bitmap.
		 **************************************************************************************************************/
		Iterator end() const noexcept;

		/**************************************************************************************************************
		 * Gets an immutable iterator to one past the end of the bitmap.
		 *
		 * @return An immutable iterator to one past the end of the bitmap.
		 **************************************************************************************************************/
		Iterator cend() const noexcept;

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
		const std::byte* data() const noexcept;

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
		void save(const std::filesystem::path& path, ImageFormat format) const;

	  private:
		struct Deleter {
			/// @private
			void operator()(SDL_Surface* ptr) const noexcept;
		};

		std::unique_ptr<SDL_Surface, Deleter> _impl;

		friend class Bitmap;
		friend class SubBitmap;
		friend class Cursor;
		friend class WindowView;
	};

	/******************************************************************************************************************
	 * Class representing a bitmap stored in memory.
	 *
	 * No instances of this class can be created before initializing SDL.
	 ******************************************************************************************************************/
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
		 * Loads an embedded bitmap file.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap failed.
		 *
		 * @param embeddedFile The embedded file data.
		 **************************************************************************************************************/
		explicit Bitmap(std::span<const std::byte> embeddedFile);

		/**************************************************************************************************************
		 * Loads a bitmap from file.
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
		 * Clones a bitmap view.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap failed.
		 *
		 * @param view The bitmap view to clone.
		 * @param format The format of the new bitmap.
		 **************************************************************************************************************/
		Bitmap(const BitmapView& view, BitmapFormat format);

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
		friend bool operator==(const Bitmap&, const Bitmap&) noexcept = default;

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
		 * Gets a mutable iterator to the beginning of the bitmap.
		 *
		 * @return A mutable iterator to the beginning of the bitmap.
		 **************************************************************************************************************/
		MutIt begin() noexcept;

		/**************************************************************************************************************
		 * Gets an immutable iterator to the beginning of the bitmap.
		 *
		 * @return An immutable iterator to the beginning of the bitmap.
		 **************************************************************************************************************/
		ConstIt begin() const noexcept;

		/**************************************************************************************************************
		 * Gets an immutable iterator to the beginning of the bitmap.
		 *
		 * @return An immutable iterator to the beginning of the bitmap.
		 **************************************************************************************************************/
		ConstIt cbegin() const noexcept;

		/**************************************************************************************************************
		 * Gets a mutable iterator to one past the end of the bitmap.
		 *
		 * @return A mutable iterator to one past the end of the bitmap.
		 **************************************************************************************************************/
		MutIt end() noexcept;

		/**************************************************************************************************************
		 * Gets an immutable iterator to one past the end of the bitmap.
		 *
		 * @return An immutable iterator to one past the end of the bitmap.
		 **************************************************************************************************************/
		ConstIt end() const noexcept;

		/**************************************************************************************************************
		 * Gets an immutable iterator to one past the end of the bitmap.
		 *
		 * @return An immutable iterator to one past the end of the bitmap.
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
		std::byte* data() noexcept;

		/**************************************************************************************************************
		 * Gets the raw data of the bitmap.
		 *
		 * @return A pointer to the data of the bitmap. This data is not necessarily contiguous,
		 *         the distance between rows is pitch() bytes and may differ from `size().x * format().pixelBytes()`.
		 **************************************************************************************************************/
		const std::byte* data() const noexcept;

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
		void save(const std::filesystem::path& path, ImageFormat format) const;

	  private:
		std::unique_ptr<SDL_Surface, BitmapView::Deleter> _impl;

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
		std::byte*       _impl;   // A pointer to the pixel data.
		SDL_PixelFormat* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(std::byte* ptr, SDL_PixelFormat* format) noexcept;

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
		 *it.
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
		 * @param l, r Iterators to the same bitmap.
		 *
		 * @return The distance between the iterators.
		 **************************************************************************************************************/
		friend difference_type operator-(const MutIt& l, const MutIt& r) noexcept;

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
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::BitmapBadAlloc::what() const noexcept
{
	return "failed bitmap allocation";
}

/// @endcond
