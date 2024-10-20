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

	enum class ImageFormat {
		JPG  = 0x1,
		PNG  = 0x2,
		TIF  = 0x4,
		WEBP = 0x8,
		JXL  = 0x10,
		AVIF = 0x20
	};
	DEFINE_BITMASK_OPERATORS(ImageFormat);

	// Image format handler.
	class SDL_Image {
	public:
		explicit SDL_Image(ImageFormat formats) noexcept;

		static bool has(ImageFormat format) noexcept;

		Version version() const noexcept;
	private:
		struct Deleter { void operator()(bool) noexcept; };
        Handle<bool, false, Deleter> _handle { true };
	};
	
	enum class BitmapSaveResult {
		SUCCESS,
		FAILED,
		UNSUPPORTED_EXTENSION
	};

	// Bitmap sub-region reference.
	class SubBitmap {
	public:
		class PixelRef;
		class Iterator;

		SubBitmap(const Bitmap& bitmap, RectI2 rect) noexcept;

		glm::ivec2 size() const noexcept;

		SubBitmap sub(RectI2 rect) noexcept;

		// Gets immutable access to a pixel of the bitmap.
		PixelRef operator[](glm::ivec2 pos) const noexcept;

		Iterator begin() const noexcept;
		Iterator cbegin() const noexcept;
		Iterator end() const noexcept;
		Iterator cend() const noexcept;

		const void* data() const noexcept;
		BitmapFormat format() const noexcept;
		// Gets the row length of the underlying bitmap.
		int rowLength() const noexcept;
		// Gets the pitch of the bitmap (the length of a row of pixels in bytes).
		int pitch() const noexcept;
	private:
		std::reference_wrapper<const Bitmap> _bitmap;
		RectI2		                         _rect;

		friend class Bitmap;
	};
	// Abstracted read-only reference to a bitmap pixel.
	class tr::SubBitmap::PixelRef {
	public:
		// Casts the pixel to an RGBA8 color.
		operator RGBA8() const noexcept;
	private:
		const void*      _impl; 	  // A pointer to the pixel data.
		SDL_PixelFormat* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(const void* ptr, SDL_PixelFormat* format) noexcept;

		friend class SubBitmap;
		friend class Iterator;
	};
	// Read-only pixel iterator.
	class tr::SubBitmap::Iterator {
	public:
		using value_type = PixelRef;
		using pointer = const value_type*;
		using difference_type = int;

		Iterator() noexcept = default;
		Iterator(SubBitmap bitmap, glm::ivec2 pos) noexcept;

		std::partial_ordering operator<=>(const Iterator& rhs) const noexcept;
		bool operator==(const Iterator& rhs) const noexcept;

		value_type operator*() const noexcept;
		value_type operator[](difference_type diff) const noexcept;
		value_type operator[](glm::ivec2 diff) const noexcept;
		pointer operator->() const noexcept;

		Iterator& operator++() noexcept;
		Iterator operator++(int) noexcept;
		Iterator& operator+=(difference_type diff) noexcept;
		Iterator& operator+=(glm::ivec2 diff) noexcept;
		friend Iterator operator+(const Iterator& it, difference_type diff) noexcept;
		friend Iterator operator+(difference_type diff, const Iterator& it) noexcept;
		friend Iterator operator+(const Iterator& it, glm::ivec2 diff) noexcept;
		friend Iterator operator+(glm::ivec2 diff, const Iterator& it) noexcept;
		Iterator& operator--() noexcept;
		Iterator operator--(int) noexcept;
		Iterator& operator-=(difference_type diff) noexcept;
		Iterator& operator-=(glm::ivec2 diff) noexcept;
		friend Iterator operator-(const Iterator& it, difference_type diff) noexcept;
		friend Iterator operator-(difference_type diff, const Iterator& it) noexcept;
		friend Iterator operator-(const Iterator& it, glm::ivec2 diff) noexcept;
		friend Iterator operator-(glm::ivec2 diff, const Iterator& it) noexcept;
		friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) noexcept;
	private:
		PixelRef   _pixel;
		glm::ivec2 _bitmapSize;
		int		   _pitch;
		glm::ivec2 _pos;
	};

	struct BitmapBadAlloc : std::bad_alloc {
		BitmapBadAlloc() noexcept = default;
		constexpr virtual const char* what() const noexcept { return "failed bitmap allocation"; };
	};
	struct BitmapLoadError : FileError {
        using FileError::FileError;
		virtual const char* what() const noexcept;
	};

	// Class representing a bitmap stored in memory.
	class Bitmap {
	public:
		using PixelCref = tr::SubBitmap::PixelRef;
		class PixelRef;
		using ConstIt = tr::SubBitmap::Iterator;
		class MutIt;

		// Creates a blank bitmap.
		Bitmap(glm::ivec2 size, BitmapFormat format);
		// Creates a bitmap from raw pixel data.
		Bitmap(glm::ivec2 size, std::span<const std::byte> pixelData, BitmapFormat dataFormat);
		// Loads an embedded bitmap file.
		Bitmap(std::span<const std::byte> embeddedFile);
		// Loads a bitmap from file.
		Bitmap(const std::filesystem::path& path);
		// Clones a bitmap.
		Bitmap(const Bitmap& bitmap, BitmapFormat format);
		// Clones a sub-bitmap.
		Bitmap(SubBitmap source, BitmapFormat format);

		bool operator==(const Bitmap&) const noexcept = default;

		glm::ivec2 size() const noexcept;

		// Gets mutable access to a pixel of the bitmap.
		PixelRef operator[](glm::ivec2 pos) noexcept;
		// Gets immutable access to a pixel of the bitmap.
		PixelCref operator[](glm::ivec2 pos) const noexcept;

		MutIt begin() noexcept;
		ConstIt begin() const noexcept;
		ConstIt cbegin() const noexcept;
		MutIt end() noexcept;
		ConstIt end() const noexcept;
		ConstIt cend() const noexcept;

		// Blits a sub-bitmap to the bitmap.
		void blit(glm::ivec2 tl, SubBitmap source) noexcept;
		// Fills a rect with a color.
		void fill(RectI2 rect, RGBA8 color) noexcept;

		operator SubBitmap() const noexcept;
		// Creates a sub-bitmap view.
		SubBitmap sub(RectI2 rect) const noexcept;

		void* data() noexcept;
		const void* data() const noexcept;
		BitmapFormat format() const noexcept;
		// Gets the pitch of the bitmap (the length of a row of pixels in bytes).
		int pitch() const noexcept;

		// Saves the bitmap to file.
		BitmapSaveResult save(const std::filesystem::path& path) noexcept;
	private:
		struct Deleter { void operator()(SDL_Surface* bitmap) noexcept; };
		std::unique_ptr<SDL_Surface, Deleter> _impl;

		Bitmap(SDL_Surface* ptr);

		friend class SubBitmap;
		friend class tr::SubBitmap::Iterator;
		friend class MutIt;
		friend class Cursor;
		friend class TTFont;
		friend class WindowView;

		friend std::optional<Bitmap> tryLoadingBitmap(const std::filesystem::path& path);
	};
	// Abstracted mutable reference to a bitmap pixel.
	class tr::Bitmap::PixelRef {
	public:
		// Casts the pixel to an RGBA8 color.
		operator RGBA8() const noexcept;
		// Sets the pixel to a color.
		PixelRef& operator=(RGBA8 color) noexcept;
	private:
		void*            _impl; 	  // A pointer to the pixel data.
		SDL_PixelFormat* _format; // The format of the pixel.

		PixelRef() noexcept = default;
		PixelRef(void* ptr, SDL_PixelFormat* format) noexcept;

		friend class MutIt;
	};
	// Mutable pixel iterator.
	class tr::Bitmap::MutIt {
	public:
		using value_type = PixelRef;
		using pointer = const value_type*;
		using difference_type = int;

		MutIt() noexcept = default;
		MutIt(Bitmap& bitmap, glm::ivec2 pos) noexcept;

		std::partial_ordering operator<=>(const MutIt& rhs) const noexcept;
		bool operator==(const MutIt& rhs) const noexcept;

		value_type operator*() const noexcept;
		value_type operator[](difference_type diff) const noexcept;
		value_type operator[](glm::ivec2 diff) const noexcept;
		pointer operator->() const noexcept;

		MutIt& operator++() noexcept;
		MutIt operator++(int) noexcept;
		MutIt& operator+=(difference_type diff) noexcept;
		MutIt& operator+=(glm::ivec2 diff) noexcept;
		friend MutIt operator+(const MutIt& it, difference_type diff) noexcept;
		friend MutIt operator+(difference_type diff, const MutIt& it) noexcept;
		friend MutIt operator+(const MutIt& it, glm::ivec2 diff) noexcept;
		friend MutIt operator+(glm::ivec2 diff, const MutIt& it) noexcept;
		MutIt& operator--() noexcept;
		MutIt operator--(int) noexcept;
		MutIt& operator-=(difference_type diff) noexcept;
		MutIt& operator-=(glm::ivec2 diff) noexcept;
		friend MutIt operator-(const MutIt& it, difference_type diff) noexcept;
		friend MutIt operator-(difference_type diff, const MutIt& it) noexcept;
		friend MutIt operator-(const MutIt& it, glm::ivec2 diff) noexcept;
		friend MutIt operator-(glm::ivec2 diff, const MutIt& it) noexcept;
		friend difference_type operator-(const MutIt& lhs, const MutIt& rhs) noexcept;
	private:
		PixelRef   _pixel;
		Bitmap*    _bitmap;
		glm::ivec2 _pos;
	};

	// Tries to load a bitmap from file.
	std::optional<Bitmap> tryLoadingBitmap(const std::filesystem::path& path);
	// Creates a bitmap with the missing texture checkerboard pattern.
	Bitmap createCheckerboard(glm::ivec2 size);
	// Loads a bitmap from file or creates a missing texture checkerboard.
	Bitmap loadBitmapOrCheckerboard(const std::filesystem::path& path, glm::ivec2 checkerboardSize);
}

// IMPLEMENTATION

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

const char* tr::BitmapLoadError::what() const noexcept
{
    static std::string str;
    str.clear();
    format_to(back_inserter(str), "Failed to load bitmap file ({}): '{}'", SDL_GetError(), path());
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
    return IMG_Init(0) & int(format);
}

tr::Version tr::SDL_Image::version() const noexcept
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

tr::BitmapSaveResult tr::Bitmap::save(const std::filesystem::path& path) noexcept
{
    assert(_impl != nullptr);

    auto surface { _impl.get() };
    auto extension { path.extension() };

    if (extension == ".bmp") {
        #ifdef _WIN32
        return SDL_SaveBMP_RW(surface, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true) == 0 ? BitmapSaveResult::SUCCESS : BitmapSaveResult::FAILED;
        #else
        return SDL_SaveBMP(surface, path.c_str()) == 0 ? BitmapSaveResult::SUCCESS : BitmapSaveResult::FAILED;
        #endif
    }
    else if (extension == ".jpg" && SDL_Image::has(ImageFormat::JPG)) {
        #ifdef _WIN32
        return IMG_SaveJPG_RW(surface, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true, 70) == 0 ? BitmapSaveResult::SUCCESS : BitmapSaveResult::FAILED;
        #else
        return IMG_SaveJPG(surface, path.c_str(), 70) == 0 ? BitmapSaveResult::SUCCESS : BitmapSaveResult::FAILED;
        #endif
    }
    else if (extension == ".png" && SDL_Image::has(ImageFormat::PNG)) {
        #ifdef _WIN32
        return IMG_SavePNG_RW(surface, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true) == 0 ? BitmapSaveResult::SUCCESS : BitmapSaveResult::FAILED;
        #else
        return IMG_SavePNG(surface, path.c_str()) == 0 ? BitmapSaveResult::SUCCESS : BitmapSaveResult::FAILED;
        #endif
    }
    else {
        return BitmapSaveResult::UNSUPPORTED_EXTENSION;
    }
};

std::optional<tr::Bitmap> tr::tryLoadingBitmap(const std::filesystem::path& path)
{
    #ifdef _WIN32
        auto ptr { IMG_Load_RW(SDL_RWFromFP(_wfopen(path.c_str(), L"r"), true), true) };
    #else
        auto ptr { IMG_Load(path.c_str()) };
    #endif
    return ptr != nullptr ? std::optional<Bitmap> { { ptr } } : std::nullopt;
}

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

tr::Bitmap tr::loadBitmapOrCheckerboard(const std::filesystem::path& path, glm::ivec2 checkerboardSize)
{
    std::optional<Bitmap> bitmap { tryLoadingBitmap(path) };
    return bitmap.has_value() ? *std::move(bitmap) : createCheckerboard(checkerboardSize);
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