#pragma once
#include "bitmap_format.hpp"
#include "color.hpp"
#include "geometry.hpp"
#include "iostream.hpp"

struct SDL_Surface;
struct SDL_PixelFormatDetails;

namespace tr {
	class Bitmap;
	class BitmapView;

	/** @ingroup system
	 *  @defgroup bitmap Bitmaps
	 *  Bitmap classes and related functionality.
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
		 * @param[in] rect
		 * @parblock
		 * The region of the sub-bitmap.
		 *
		 * @pre @em rect is not allowed to strech outside the bitmap bounds.
		 * @endparblock
		 **************************************************************************************************************/
		SubBitmap(const Bitmap& bitmap, const RectI2& rect) noexcept;

		/**************************************************************************************************************
		 * Constructs a sub-bitmap.
		 *
		 * @param[in] view The parent bitmap view.
		 * @param[in] rect
		 * @parblock
		 * The region of the sub-bitmap.
		 *
		 * @pre @em rect is not allowed to strech outside the bitmap bounds.
		 * @endparblock
		 **************************************************************************************************************/
		SubBitmap(const BitmapView& view, const RectI2& rect) noexcept;

		/**************************************************************************************************************
		 * Gets the size of the sub-bitmap.
		 *
		 * @return The size of the sub-bitmap in pixels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Creates a sub-bitmap of the sub-bitmap.
		 *
		 * @param[in] rect
		 * @parblock
		 * The region of the sub-bitmap.
		 *
		 * @pre @em rect is not allowed to strech outside the parent sub-bitmap bounds.
		 * @endparblock
		 *
		 * @return The new sub-bitmap.
		 **************************************************************************************************************/
		SubBitmap sub(const RectI2& rect) noexcept;

		/**************************************************************************************************************
		 * Gets immutable access to a pixel of the bitmap.
		 *
		 * @param[in] pos
		 * @parblock
		 * The position of the pixel within the sub-bitmap.
		 *
		 * @pre @em pos must be within the bounds of the sub-bitmap.
		 * @endparblock
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
		 * @return
		 * A pointer to the data of the sub-bitmap.
		 * @remark This data is not necessarily contiguous, the distance between rows is pitch() bytes and may differ
		 *         from `size().x * format().pixelBytes()`.
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
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap view's internals fails.
		 *
		 * @param[in] rawData
		 * @parblock
		 * The pixel data span.
		 *
		 * @pre The size of the span must match `size.x * size.y * [pixel bytes]`.
		 * @endparblock
		 * @param[in] size The size of the bitmap.
		 * @param[in] format The format of the bitmap.
		 **************************************************************************************************************/
		BitmapView(std::span<const std::byte> rawData, glm::ivec2 size, BitmapFormat format);

		/**************************************************************************************************************
		 * Creates a bitmap view over a range of pixel data.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap view's internals fails.
		 *
		 * @param[in] range
		 * @parblock
		 * A contiguous range of pixel data.
		 *
		 * @pre The size of the range must match `size.x * size.y * [pixel bytes]`.
		 * @endparblock
		 * @param[in] size The size of the bitmap.
		 * @param[in] format The format of the bitmap.
		 **************************************************************************************************************/
		template <std::ranges::contiguous_range T>
		BitmapView(T&& range, glm::ivec2 size, BitmapFormat format)
			: BitmapView{std::span<const std::byte>{rangeBytes(range)}, size, format}
		{
		}

		/**************************************************************************************************************
		 * Creates a bitmap view over pixel data.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
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
		 * @param[in] pos
		 * @parblock
		 * The position of the pixel within the view.
		 *
		 * @pre @em pos must be within the bounds of the view.
		 * @endparblock
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
		 * @param[in] rect
		 * @parblock
		 * The region of the sub-bitmap.
		 *
		 * @pre @em rect is not allowed to strech outside the view bounds.
		 * @endparblock
		 *
		 * @return The sub-bitmap.
		 **************************************************************************************************************/
		SubBitmap sub(const RectI2& rect) const noexcept;

		/**************************************************************************************************************
		 * Gets the raw data of the bitmap.
		 *
		 * @return
		 * A pointer to the data of the sub-bitmap.
		 * @remark This data is not necessarily contiguous, the distance between rows is pitch() bytes and may differ
		 *         from `size().x * format().pixelBytes()`.
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
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
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
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] size The size of the bitmap.
		 * @param[in] format The format of the bitmap.
		 **************************************************************************************************************/
		Bitmap(glm::ivec2 size, BitmapFormat format = BitmapFormat::RGBA_8888);

		/**************************************************************************************************************
		 * Clones a bitmap.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] bitmap The bitmap to clone.
		 * @param[in] format The format of the new bitmap.
		 **************************************************************************************************************/
		explicit Bitmap(const Bitmap& bitmap, BitmapFormat format = BitmapFormat::RGBA_8888);

		/**************************************************************************************************************
		 * Clones a bitmap view.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] view The bitmap view to clone.
		 * @param[in] format The format of the new bitmap.
		 **************************************************************************************************************/
		explicit Bitmap(const BitmapView& view, BitmapFormat format = BitmapFormat::RGBA_8888);

		/**************************************************************************************************************
		 * Clones a sub-bitmap.
		 *
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
		 *
		 * @exception BitmapBadAlloc If allocating the bitmap fails.
		 *
		 * @param[in] source The sub-bitmap to clone.
		 * @param[in] format The format of the new bitmap.
		 **************************************************************************************************************/
		explicit Bitmap(const SubBitmap& source, BitmapFormat format = BitmapFormat::RGBA_8888);

		Bitmap(Bitmap&& bitmap) noexcept = default;

		Bitmap& operator=(Bitmap&& r) noexcept = default;

		/**************************************************************************************************************
		 * Gets the size of the bitmap.
		 *
		 * @return The size of the bitmap in pixels.
		 **************************************************************************************************************/
		glm::ivec2 size() const noexcept;

		/**************************************************************************************************************
		 * Gets mutable access to a pixel of the bitmap.
		 *
		 * @param[in] pos
		 * @parblock
		 * The position of the pixel within the bitmap.
		 *
		 * @pre @em pos must be within the bounds of the bitmap.
		 * @endparblock
		 *
		 * @return A mutable reference to a pixel of the bitmap.
		 **************************************************************************************************************/
		PixelRef operator[](glm::ivec2 pos) noexcept;

		/**************************************************************************************************************
		 * Gets immutable access to a pixel of the bitmap.
		 *
		 * @param[in] pos
		 * @parblock
		 * The position of the pixel within the bitmap.
		 *
		 * @pre @em pos must be within the bounds of the bitmap.
		 * @endparblock
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
		 * @param[in] tl
		 * @parblock
		 * The top-left corner of the region to blit to.
		 *
		 * @pre @em tl is not allowed to stretch outside the bitmap bounds.
		 * @endparblock
		 * @param[in] source The source sub-bitmap.
		 **************************************************************************************************************/
		void blit(glm::ivec2 tl, const SubBitmap& source) noexcept;

		/**************************************************************************************************************
		 * Fills a region of the bitmap with a solid color.
		 *
		 * @param[in] rect
		 * @parblock
		 * The region to fill.
		 *
		 * @pre @em rect is not allowed to stretch outside the bitmap bounds.
		 * @endparblock
		 * @param[in] color The fill color.
		 **************************************************************************************************************/
		void fill(const RectI2& rect, RGBA8 color) noexcept;

		/**************************************************************************************************************
		 * Creates a sub-bitmap spanning the entire bitmap.
		 **************************************************************************************************************/
		operator SubBitmap() const noexcept;

		/**************************************************************************************************************
		 * Creates a sub-bitmap of the bitmap.
		 *
		 * @param[in] rect
		 * @parblock
		 * The region of the sub-bitmap.
		 *
		 * @pre @em rect is not allowed to strech outside the bitmap bounds.
		 * @endparblock
		 *
		 * @return The sub-bitmap.
		 **************************************************************************************************************/
		SubBitmap sub(const RectI2& rect) const noexcept;

		/**************************************************************************************************************
		 * Gets the raw data of the bitmap.
		 *
		 * @return
		 * A pointer to the data of the sub-bitmap.
		 * @remark This data is not necessarily contiguous, the distance between rows is pitch() bytes and may differ
		 *         from `size().x * format().pixelBytes()`.
		 **************************************************************************************************************/
		std::byte* data() noexcept;

		/**************************************************************************************************************
		 * Gets the raw data of the bitmap.
		 *
		 * @return
		 * A pointer to the data of the sub-bitmap.
		 * @remark This data is not necessarily contiguous, the distance between rows is pitch() bytes and may differ
		 *         from `size().x * format().pixelBytes()`.
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
		 * @par Exception Safety
		 *
		 * Strong exception guarantee.
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

		friend Bitmap loadEmbeddedBitmap(std::span<const std::byte> data);
		friend Bitmap loadBitmapFile(const std::filesystem::path& path);
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

	/******************************************************************************************************************
	 * Loads an embedded bitmap file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception BitmapBadAlloc If allocating the bitmap fails.
	 *
	 * @param[in] data
	 * @parblock
	 * The embedded file data.
	 *
	 * @pre @em data is assumed to always be a valid bitmap file.
	 * @endparblock
	 *
	 * @return A bitmap loaded with data from the embedded file.
	 ******************************************************************************************************************/
	Bitmap loadEmbeddedBitmap(std::span<const std::byte> data);

	/******************************************************************************************************************
	 * Loads an embedded bitmap file.
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception BitmapBadAlloc If allocating the bitmap fails.
	 *
	 * @param[in] range
	 * @parblock
	 * The embedded file data.
	 *
	 * @pre @em range is assumed to always be a valid bitmap file.
	 * @endparblock
	 *
	 * @return A bitmap loaded with data from the embedded file.
	 ******************************************************************************************************************/
	template <std::ranges::contiguous_range Range> Bitmap loadEmbeddedBitmap(Range&& range)
	{
		return loadEmbeddedBitmap(std::span<const std::byte>{rangeBytes(range)});
	}

	/******************************************************************************************************************
	 * Loads a bitmap from file (BMP/PNG/JPG/QOI).
	 *
	 * @par Exception Safety
	 *
	 * Strong exception guarantee.
	 *
	 * @exception FileNotFound If the file isn't found.
	 * @exception BitmapLoadError If loading the bitmap fails.
	 *
	 * @param[in] path The path to the bitmap file.
	 *
	 * @return A bitmap loaded with data from the file.
	 ******************************************************************************************************************/
	Bitmap loadBitmapFile(const std::filesystem::path& path);

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

constexpr const char* tr::BitmapBadAlloc::what() const noexcept
{
	return "failed bitmap allocation";
}

/// @endcond
