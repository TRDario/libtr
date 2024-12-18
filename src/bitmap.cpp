#include "../include/tr/bitmap.hpp"
#include "../include/tr/bitmap_iterators.hpp"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL.h>
#include <format>

#ifdef _WIN32
#include <windows.h>
#endif

namespace tr {
	template <class T> T checkNotNull(T ptr);
	void                 saveBitmap(SDL_Surface* bitmap, const std::filesystem::path& path);
} // namespace tr

template <class T> T tr::checkNotNull(T ptr)
{
	if (ptr == nullptr) {
		throw BitmapBadAlloc{};
	}
	return ptr;
}

void tr::saveBitmap(SDL_Surface* bitmap, const std::filesystem::path& path)
{
	assert(bitmap != nullptr);

#ifdef _WIN32
	if (!IMG_Init(IMG_INIT_PNG) || IMG_SavePNG_RW(bitmap, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true) < 0) {
#else
	if (!IMG_Init(IMG_INIT_PNG) || IMG_SavePNG(bitmap, path.c_str()) < 0) {
#endif
		throw BitmapSaveError{path};
	}
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

tr::SubBitmap::SubBitmap(const Bitmap& bitmap, const RectI2& rect) noexcept
	: _bitmap{bitmap._impl.get()}, _rect{rect}
{
}

tr::SubBitmap::SubBitmap(const BitmapView& bitmap, const RectI2& rect) noexcept
	: _bitmap{bitmap._impl.get()}, _rect{rect}
{
}

glm::ivec2 tr::SubBitmap::size() const noexcept
{
	return _rect.size;
}

tr::SubBitmap tr::SubBitmap::sub(const RectI2& rect) noexcept
{
	assert(_rect.contains(rect.tl + rect.size));
	return {_bitmap, {_rect.tl + rect.tl, rect.size}};
}

tr::SubBitmap::PixelRef tr::SubBitmap::operator[](glm::ivec2 pos) const noexcept
{
	return *(begin() + pos);
}

tr::SubBitmap::Iterator tr::SubBitmap::begin() const noexcept
{
	return {*this, {}};
}

tr::SubBitmap::Iterator tr::SubBitmap::cbegin() const noexcept
{
	return begin();
}

tr::SubBitmap::Iterator tr::SubBitmap::end() const noexcept
{
	return {*this, {0, size().y}};
}

tr::SubBitmap::Iterator tr::SubBitmap::cend() const noexcept
{
	return end();
}

const std::byte* tr::SubBitmap::data() const noexcept
{
	return (const std::byte*)(_bitmap->pixels) + pitch() * _rect.tl.y + format().pixelBytes() * _rect.tl.x;
}

tr::BitmapFormat tr::SubBitmap::format() const noexcept
{
	return tr::BitmapFormat::Type(_bitmap->format->format);
}

int tr::SubBitmap::rowLength() const noexcept
{
	return _bitmap->w;
}

int tr::SubBitmap::pitch() const noexcept
{
	return _bitmap->pitch;
}

int tr::operator-(const tr::SubBitmap::Iterator& l, const tr::SubBitmap::Iterator& r) noexcept
{
	return (l._pos.y * l._bitmapSize.x + l._pos.x) - (r._pos.y * r._bitmapSize.x + r._pos.x);
}

tr::BitmapView::BitmapView(std::span<const std::byte> rawData, glm::ivec2 size, BitmapFormat format)
	: BitmapView(rawData.data(), size.x * format.pixelBytes(), size, format)
{
	assert(rawData.size() == size.x * size.y * format.pixelBytes());
}

tr::BitmapView::BitmapView(const std::byte* rawDataStart, int pitch, glm::ivec2 size, BitmapFormat format)
	: _impl{checkNotNull(SDL_CreateRGBSurfaceWithFormatFrom((std::byte*)(rawDataStart), size.x, size.y,
															format.pixelBits(), pitch,
															std::uint32_t(BitmapFormat::Type(format))))}
{
}

void tr::BitmapView::Deleter::operator()(SDL_Surface* ptr) const noexcept
{
	SDL_FreeSurface(ptr);
}

glm::ivec2 tr::BitmapView::size() const noexcept
{
	return {_impl.get()->w, _impl.get()->h};
}

tr::BitmapView::PixelRef tr::BitmapView::operator[](glm::ivec2 pos) const noexcept
{
	return *(begin() + pos);
}

tr::BitmapView::Iterator tr::BitmapView::begin() const noexcept
{
	return cbegin();
}

tr::BitmapView::Iterator tr::BitmapView::cbegin() const noexcept
{
	assert(_impl != nullptr);
	return SubBitmap(*this).begin();
}

tr::BitmapView::Iterator tr::BitmapView::end() const noexcept
{
	assert(_impl != nullptr);
	return cend();
}

tr::BitmapView::Iterator tr::BitmapView::cend() const noexcept
{
	assert(_impl != nullptr);
	return SubBitmap(*this).end();
}

tr::BitmapView::operator SubBitmap() const noexcept
{
	return sub({{}, size()});
}

tr::SubBitmap tr::BitmapView::sub(const RectI2& rect) const noexcept
{
	return SubBitmap{*this, rect};
}

const std::byte* tr::BitmapView::data() const noexcept
{
	assert(_impl != nullptr);
	return (const std::byte*)(_impl.get()->pixels);
}

tr::BitmapFormat tr::BitmapView::format() const noexcept
{
	assert(_impl != nullptr);
	assert(_impl.get()->format);
	return BitmapFormat::Type(_impl.get()->format->format);
}

int tr::BitmapView::pitch() const noexcept
{
	assert(_impl != nullptr);
	return _impl.get()->pitch;
}

void tr::BitmapView::save(const std::filesystem::path& path) const
{
	saveBitmap(_impl.get(), path);
}

tr::Bitmap::Bitmap(SDL_Surface* ptr)
	: _impl{checkNotNull(ptr)}
{
}

tr::Bitmap::Bitmap(glm::ivec2 size, BitmapFormat format)
	: _impl{checkNotNull(SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, format.pixelBits(),
														SDL_PixelFormatEnum(BitmapFormat::Type(format))))}
{
}

tr::Bitmap::Bitmap(std::span<const std::byte> embeddedFile)
	: _impl{checkNotNull(IMG_Load_RW(SDL_RWFromConstMem(embeddedFile.data(), embeddedFile.size()), true))}
{
}

tr::Bitmap::Bitmap(const std::filesystem::path& path)
{
	if (!is_regular_file(path)) {
		throw FileNotFound{path};
	}

#ifdef _WIN32
	_impl.reset(IMG_Load_RW(SDL_RWFromFP(_wfopen(path.c_str(), L"r"), true), true));
#else
	_impl.reset(IMG_Load(path.c_str()));
#endif
	if (_impl == nullptr) {
		throw BitmapLoadError{path};
	}
}

tr::Bitmap::Bitmap(const Bitmap& bitmap, BitmapFormat format)
	: _impl{checkNotNull(SDL_ConvertSurfaceFormat(bitmap._impl.get(), std::uint32_t(BitmapFormat::Type(format)), 0))}
{
}

tr::Bitmap::Bitmap(const BitmapView& view, BitmapFormat format)
	: _impl{checkNotNull(SDL_ConvertSurfaceFormat(view._impl.get(), std::uint32_t(BitmapFormat::Type(format)), 0))}
{
}

tr::Bitmap::Bitmap(const SubBitmap& source, BitmapFormat format)
	: Bitmap{source.size(), format}
{
	blit({}, source);
}

glm::ivec2 tr::Bitmap::size() const noexcept
{
	return {_impl.get()->w, _impl.get()->h};
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
	return {*this, {}};
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
	return {*this, {0, size().y}};
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

void tr::Bitmap::blit(glm::ivec2 tl, const SubBitmap& source) noexcept
{
	assert(_impl != nullptr);
	assert(RectI2{size()}.contains(tl + source.size()));
	SDL_Rect sdlSource{source._rect.tl.x, source._rect.tl.y, source.size().x, source.size().y};
	SDL_Rect sdlDest{tl.x, tl.y};
	SDL_BlitSurface(source._bitmap, &sdlSource, _impl.get(), &sdlDest);
}

void tr::Bitmap::fill(const RectI2& rect, RGBA8 color) noexcept
{
	assert(_impl != nullptr);
	assert(RectI2{size()}.contains(rect.tl + rect.size));
	SDL_Rect sdlRect{rect.tl.x, rect.tl.y, rect.size.x, rect.size.y};
	SDL_FillRect(_impl.get(), &sdlRect, SDL_MapRGBA(_impl.get()->format, color.r, color.g, color.b, color.a));
}

tr::Bitmap::operator SubBitmap() const noexcept
{
	return sub({{}, size()});
}

tr::SubBitmap tr::Bitmap::sub(const RectI2& rect) const noexcept
{
	return SubBitmap{*this, rect};
}

std::byte* tr::Bitmap::data() noexcept
{
	assert(_impl != nullptr);
	return (std::byte*)(_impl.get()->pixels);
}

const std::byte* tr::Bitmap::data() const noexcept
{
	assert(_impl != nullptr);
	return (const std::byte*)(_impl.get()->pixels);
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

void tr::Bitmap::save(const std::filesystem::path& path) const
{
	saveBitmap(_impl.get(), path);
}

tr::Bitmap tr::createCheckerboard(glm::ivec2 size)
{
	constexpr RGBA8  BLACK{0, 0, 0, 255};
	constexpr RGBA8  MAGENTA{255, 0, 255, 255};
	const glm::ivec2 halfSize{size / 2};

	Bitmap bitmap{size, BitmapFormat::RGB_332};
	bitmap.fill({{0, 0}, halfSize}, BLACK);
	bitmap.fill({{halfSize.x, 0}, halfSize}, MAGENTA);
	bitmap.fill({{0, halfSize.x}, halfSize}, MAGENTA);
	bitmap.fill({halfSize, halfSize}, BLACK);
	return bitmap;
}