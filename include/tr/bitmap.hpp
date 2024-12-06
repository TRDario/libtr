#pragma once
#include "bitmap_format.hpp"
#include "color.hpp"
#include "geometry.hpp"
#include "iostream.hpp"
#include <EnumBitmask.hpp>

struct SDL_Surface;
struct SDL_PixelFormat;

namespace tr {
	class Bitmap;
	class BitmapView;

	/** @defgroup bitmap Bitmaps
	 * Functionality for manipulating bitmaps.
	 *
	 *  An instance of Window must be created before any functionality from this section can be used.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Error thrown when bitmap allocation fails.
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
	 * Error thrown when bitmap loading fails.
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
	 * Error thrown when bitmap saving fails.
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
	 * View over a rectangular region of a bitmap.
	 ******************************************************************************************************************/
	class SubBitmap {
	  public:
		class PixelRef;
		class Iterator;

		/**************************************************************************************************************
		 * Constructs a sub-bitmap.
		 *
		 * @param[in] bitmap The parent bitmap.
		 * @param[in] rect The region of the sub-bitmap. The region is not allowed to strech outside the bitmap bounds.
		 **************************************************************************************************************/
		SubBitmap(const Bitmap& bitmap, RectI2 rect) noexcept;

		/**************************************************************************************************************
		 * Constructs a sub-bitmap.
		 *
		 * @param[in] view The parent bitmap view.
		 * @param[in] rect The region of the sub-bitmap. The region is not allowed to strech outside the bitmap bounds.
		 **************************************************************************************************************/
		SubBitmap(const BitmapView& view, RectI2 rect) noexcept;

		/**************************************************************************************************************
		 * Gets the size of the sub-bitmap.
		 *
		 * @return The size of the sub-bitmap in pixels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Creates a sub-bitmap of the sub-bitmap.
		 *
		 * @param[in] rect The region of the sub-bitmap. The region is not allowed to strech outside the sub-bitmap
		 *                 bounds.
		 *
		 * @return The new sub-bitmap.
		 **************************************************************************************************************/
		SubBitmap sub(RectI2 rect) noexcept;

		/**************************************************************************************************************
		 * Gets immutable access to a pixel of the bitmap.
		 *
		 * @param[in] pos The position of the pixel within the sub-bitmap. Accessing an out-of-bounds pixel is not
		 *                allowed.
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
	 * Non-owning view over bitmap data.
	 *
	 * This class cannot be instantiated before the window is opened.
	 ******************************************************************************************************************/
	class BitmapView {
	  public:
		/**************************************************************************************************************
		 * Immutable pixel reference.
		 **************************************************************************************************************/
		using PixelRef = tr::SubBitmap::PixelRef;

		/**************************************************************************************************************
		 * Immutable iterator.
		 **************************************************************************************************************/
		using Iterator = tr::SubBitmap::Iterator;

		/**************************************************************************************************************
		 * Creates a bitmap view over contiguous pixel data.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap view's internals fails.
		 *
		 * @param[in] rawData The pixel data span. The size of the span must match size.x * size.y * [pixel bytes].
		 * @param[in] size The size of the bitmap.
		 * @param[in] format The format of the bitmap.
		 **************************************************************************************************************/
		BitmapView(std::span<const std::byte> rawData, glm::ivec2 size, BitmapFormat format);

		/**************************************************************************************************************
		 * Creates a bitmap view over pixel data.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap view's internals fails.
		 *
		 * @param[in] rawDataStart A pointer to the start of the pixel data.
		 * @param[in] pitch The distance between rows of pixels in bytes.
		 * @param[in] size The size of the bitmap.
		 * @param[in] format The format of the bitmap.
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
		 * @param[in] pos The position of the pixel within the bitmap. Accessing an out-of-bounds pixel is not allowed.
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
		 * Creates a sub-bitmap spanning the entire bitmap view.
		 **************************************************************************************************************/
		operator SubBitmap() const noexcept;

		/**************************************************************************************************************
		 * Creates a sub-bitmap of the bitmap.
		 *
		 * @param[in] rect The region of the sub-bitmap. The region is not allowed to strech outside the view bounds.
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
		 * Saves the bitmap to a .png file.
		 *
		 * @exception BitmapSaveError If saving the bitmap fails.
		 *
		 * @param[in] path The path to the file.
		 **************************************************************************************************************/
		void save(const std::filesystem::path& path) const;

	  private:
		struct Deleter {
			void operator()(SDL_Surface* ptr) const noexcept;
		};

		std::unique_ptr<SDL_Surface, Deleter> _impl;

		friend class Bitmap;
		friend class SubBitmap;
		friend class Cursor;
		friend class Window;
	};

	/******************************************************************************************************************
	 * Class containing owned bitmap data.
	 *
	 * This class cannot be instantiated before the window is opened.
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
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] size The size of the bitmap.
		 * @param[in] format The format of the bitmap.
		 **************************************************************************************************************/
		Bitmap(glm::ivec2 size, BitmapFormat format);

		/**************************************************************************************************************
		 * Loads an embedded bitmap file.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] embeddedFile The embedded file data.
		 **************************************************************************************************************/
		explicit Bitmap(std::span<const std::byte> embeddedFile);

		/**************************************************************************************************************
		 * Loads a bitmap from file (BMP/QOI).
		 *
		 * @exception FileNotFound If the file isn't found.
		 * @exception BitmapLoadError If loading the bitmap fails.
		 *
		 * @param[in] path The path to the bitmap file.
		 **************************************************************************************************************/
		explicit Bitmap(const std::filesystem::path& path);

		/**************************************************************************************************************
		 * Clones a bitmap.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] bitmap The bitmap to clone.
		 * @param[in] format The format of the new bitmap.
		 **************************************************************************************************************/
		Bitmap(const Bitmap& bitmap, BitmapFormat format);

		/**************************************************************************************************************
		 * Clones a bitmap view.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] view The bitmap view to clone.
		 * @param[in] format The format of the new bitmap.
		 **************************************************************************************************************/
		Bitmap(const BitmapView& view, BitmapFormat format);

		/**************************************************************************************************************
		 * Clones a sub-bitmap.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] source The sub-bitmap to clone.
		 * @param[in] format The format of the new bitmap.
		 **************************************************************************************************************/
		Bitmap(SubBitmap source, BitmapFormat format);

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
		 * @param[in] pos The position of the pixel within the bitmap. Accessing an out-of-bounds pixel is not allowed.
		 *
		 * @return A mutable reference to a pixel of the bitmap.
		 **************************************************************************************************************/
		PixelRef operator[](glm::ivec2 pos) noexcept;

		/**************************************************************************************************************
		 * Gets immutable access to a pixel of the bitmap.
		 *
		 * @param[in] pos The position of the pixel within the bitmap. Accessing an out-of-bounds pixel is not allowed.
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
		 * @param[in] tl The top-left corner of the region to blit to.
		 *               The region is not allowed to stretch outside the bitmap bounds.
		 * @param[in] source The source sub-bitmap.
		 **************************************************************************************************************/
		void blit(glm::ivec2 tl, SubBitmap source) noexcept;

		/**************************************************************************************************************
		 * Fills a region of the bitmap with a solid color.
		 *
		 * @param[in] rect The region to fill. The region is not allowed to stretch outside the bitmap bounds.
		 * @param[in] color The fill color.
		 **************************************************************************************************************/
		void fill(RectI2 rect, RGBA8 color) noexcept;

		/**************************************************************************************************************
		 * Creates a sub-bitmap spanning the entire bitmap.
		 **************************************************************************************************************/
		operator SubBitmap() const noexcept;

		/**************************************************************************************************************
		 * Creates a sub-bitmap of the bitmap.
		 *
		 * @param[in] rect The region of the sub-bitmap. The region is not allowed to stretch outside the bitmap bounds.
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
		 * Saves the bitmap to a .png file.
		 *
		 * @exception BitmapSaveError If saving the bitmap fails.
		 *
		 * @param[in] path The path to the file.
		 **************************************************************************************************************/
		void save(const std::filesystem::path& path) const;

	  private:
		std::unique_ptr<SDL_Surface, BitmapView::Deleter> _impl;

		Bitmap(SDL_Surface* ptr);

		friend class SubBitmap;
		friend class tr::SubBitmap::Iterator;
		friend class MutIt;
		friend class Cursor;
		friend class TTFont;
		friend class Window;
	};

	/******************************************************************************************************************
	 * Creates a bitmap with the missing texture checkerboard pattern.
	 *
	 * @exception BitmapBadAlloc If allocating the bitmap fails.
	 *
	 * @param[in] size The size of the bitmap.
	 *
	 * @return A bitmap with the missing texture checkerboard pattern.
	 ******************************************************************************************************************/
	Bitmap createCheckerboard(glm::ivec2 size);

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::BitmapBadAlloc::what() const noexcept
{
	return "failed bitmap allocation";
}

/// @endcond
