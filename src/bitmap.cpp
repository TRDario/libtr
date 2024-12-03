#include "../include/tr/bitmap.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace tr {
	template <class T> T checkNotNull(T ptr);
	RGBA8                getPixelColor(const void* data, SDL_PixelFormat* format);
	void                 saveBitmap(SDL_Surface* bitmap, const std::filesystem::path& path, ImageFormat format);
} // namespace tr

template <class T> T tr::checkNotNull(T ptr)
{
	if (ptr == nullptr) {
		throw BitmapBadAlloc{};
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
		value = ((const std::uint8_t*)(data))[0] << 16 | ((const std::uint8_t*)(data))[1] << 8 |
				((const std::uint8_t*)(data))[2];
	case 4:
		value = *((const std::uint32_t*)(data));
	}

	RGBA8 color;
	SDL_GetRGBA(value, format, &color.r, &color.g, &color.b, &color.a);
	return color;
}

void tr::saveBitmap(SDL_Surface* bitmap, const std::filesystem::path& path, ImageFormat format)
{
	assert(bitmap != nullptr);

	switch (format) {
	case ImageFormat::BMP:
#ifdef _WIN32
		if (SDL_SaveBMP_RW(bitmap, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true) < 0) {
#else
		if (SDL_SaveBMP(bitmap, path.c_str()) < 0) {
#endif
			throw BitmapSaveError{path};
		}
		break;
	case ImageFormat::PNG:
#ifdef _WIN32
		if (IMG_SavePNG_RW(bitmap, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true) < 0) {
#else
		if (IMG_SavePNG(bitmap, path.c_str()) < 0) {
#endif
			throw BitmapSaveError{path};
		}
		break;
	case ImageFormat::JPG:
#ifdef _WIN32
		if (IMG_SaveJPG_RW(bitmap, SDL_RWFromFP(_wfopen(path.c_str(), L"w"), true), true, 70) < 0) {
#else
		if (IMG_SaveJPG(bitmap, path.c_str(), 70) < 0) {
#endif
			throw BitmapSaveError{path};
		}
		break;
	default:
		assert(false);
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

tr::SDL_Image::SDL_Image(ImageFormat formats) noexcept
{
	IMG_Init(int(formats));
}

void tr::SDL_Image::Deleter::operator()(bool) const noexcept
{
	IMG_Quit();
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
	: _bitmap{bitmap._impl.get()}, _rect{rect}
{
}

tr::SubBitmap::SubBitmap(const BitmapView& bitmap, RectI2 rect) noexcept
	: _bitmap{bitmap._impl.get()}, _rect{rect}
{
}

glm::ivec2 tr::SubBitmap::size() const noexcept
{
	return _rect.size;
}

tr::SubBitmap tr::SubBitmap::sub(RectI2 rect) noexcept
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

tr::SubBitmap::PixelRef::PixelRef(const std::byte* ptr, SDL_PixelFormat* format) noexcept
	: _impl{ptr}, _format{format}
{
}

tr::SubBitmap::PixelRef::operator RGBA8() const noexcept
{
	return getPixelColor(_impl, _format);
}

tr::SubBitmap::Iterator::Iterator(SubBitmap bitmap, glm::ivec2 pos) noexcept
	: _pixel{bitmap.data() + bitmap.pitch() * pos.y + bitmap.format().pixelBytes() * pos.x, bitmap._bitmap->format}
	, _bitmapSize{bitmap.size()}
	, _pitch{bitmap.pitch()}
	, _pos{pos}
{
}

std::partial_ordering tr::SubBitmap::Iterator::operator<=>(const Iterator& r) const noexcept
{
	if (_pixel._impl != nullptr && r._pixel._impl != nullptr) {
		return _pixel._impl <=> r._pixel._impl;
	}
	else if (_pixel._impl == nullptr && r._pixel._impl == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::SubBitmap::Iterator::operator==(const Iterator& r) const noexcept
{
	return *this <=> r == std::strong_ordering::equal;
}

tr::SubBitmap::Iterator::value_type tr::SubBitmap::Iterator::operator*() const noexcept
{
	assert(_pixel._impl != nullptr && RectI2{_bitmapSize}.contains(_pos));
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
	Iterator copy{*this};
	++*this;
	return copy;
}

tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator+=(int diff) noexcept
{
	auto lines{diff / _bitmapSize.x};
	diff %= _bitmapSize.x;
	if (diff + _pos.x >= _bitmapSize.x) {
		++lines;
		diff -= _bitmapSize.x;
	}
	else if (diff + _pos.x < 0) {
		--lines;
		diff += _bitmapSize.x;
	}
	_pixel._impl = _pixel._impl + _pitch * lines + _pixel._format->BytesPerPixel * diff;
	return *this;
}

tr::SubBitmap::Iterator& tr::SubBitmap::Iterator::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * _bitmapSize.x + diff.x);
}

tr::SubBitmap::Iterator tr::operator+(const tr::SubBitmap::Iterator& it, int diff) noexcept
{
	tr::SubBitmap::Iterator copy{it};
	copy += diff;
	return copy;
}

tr::SubBitmap::Iterator tr::operator+(int diff, const tr::SubBitmap::Iterator& it) noexcept
{
	return it + diff;
}

tr::SubBitmap::Iterator tr::operator+(const tr::SubBitmap::Iterator& it, glm::ivec2 diff) noexcept
{
	tr::SubBitmap::Iterator copy{it};
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
	Iterator copy{*this};
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
	tr::SubBitmap::Iterator copy{it};
	copy -= diff;
	return copy;
}

tr::SubBitmap::Iterator tr::operator-(int diff, const tr::SubBitmap::Iterator& it) noexcept
{
	return it - diff;
}

tr::SubBitmap::Iterator tr::operator-(const tr::SubBitmap::Iterator& it, glm::ivec2 diff) noexcept
{
	tr::SubBitmap::Iterator copy{it};
	copy -= diff;
	return copy;
}

tr::SubBitmap::Iterator tr::operator-(glm::ivec2 diff, const tr::SubBitmap::Iterator& it) noexcept
{
	return it - diff;
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

tr::SubBitmap tr::BitmapView::sub(RectI2 rect) const noexcept
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

void tr::BitmapView::save(const std::filesystem::path& path, ImageFormat format) const
{
	saveBitmap(_impl.get(), path, format);
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

tr::Bitmap::Bitmap(SubBitmap source, BitmapFormat format)
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

void tr::Bitmap::blit(glm::ivec2 tl, SubBitmap source) noexcept
{
	assert(_impl != nullptr);
	assert(RectI2{size()}.contains(tl + source.size()));
	SDL_Rect sdlSource{source._rect.tl.x, source._rect.tl.y, source.size().x, source.size().y};
	SDL_Rect sdlDest{tl.x, tl.y};
	SDL_BlitSurface(source._bitmap, &sdlSource, _impl.get(), &sdlDest);
}

void tr::Bitmap::fill(RectI2 rect, RGBA8 color) noexcept
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

tr::SubBitmap tr::Bitmap::sub(RectI2 rect) const noexcept
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

void tr::Bitmap::save(const std::filesystem::path& path, ImageFormat format) const
{
	saveBitmap(_impl.get(), path, format);
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

tr::Bitmap::PixelRef::PixelRef(std::byte* ptr, SDL_PixelFormat* format) noexcept
	: _impl{ptr}, _format{format}
{
}

tr::Bitmap::PixelRef::operator RGBA8() const noexcept
{
	return getPixelColor(_impl, _format);
}

tr::Bitmap::PixelRef& tr::Bitmap::PixelRef::operator=(RGBA8 color) noexcept
{
	auto formatted{SDL_MapRGBA(_format, color.r, color.g, color.b, color.a)};
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
	: _pixel{bitmap.data() + bitmap.pitch() * pos.y + bitmap.format().pixelBytes() * pos.x, bitmap._impl.get()->format}
	, _bitmap{&bitmap}
	, _pos{pos}
{
}

std::partial_ordering tr::Bitmap::MutIt::operator<=>(const tr::Bitmap::MutIt& r) const noexcept
{
	if (_pixel._impl != nullptr && r._pixel._impl != nullptr) {
		return _pixel._impl <=> r._pixel._impl;
	}
	else if (_pixel._impl == nullptr && r._pixel._impl == nullptr) {
		return std::partial_ordering::equivalent;
	}
	else {
		return std::partial_ordering::unordered;
	}
}

bool tr::Bitmap::MutIt::operator==(const MutIt& r) const noexcept
{
	return *this <=> r == std::strong_ordering::equal;
}

tr::Bitmap::MutIt::value_type tr::Bitmap::MutIt::operator*() const noexcept
{
	assert(_pixel._impl != nullptr && RectI2{_bitmap->size()}.contains(_pos));
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
	MutIt copy{*this};
	++*this;
	return copy;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator+=(int diff) noexcept
{
	assert(_pixel._impl != nullptr);

	const glm::ivec2 bitmapSize{_bitmap->size()};
	auto             lines{diff / bitmapSize.x};
	diff %= bitmapSize.x;
	if (diff + _pos.x >= bitmapSize.x) {
		++lines;
		diff -= bitmapSize.x;
	}
	else if (diff + _pos.x < 0) {
		--lines;
		diff += bitmapSize.x;
	}
	_pixel._impl = _pixel._impl + _bitmap->pitch() * lines + _pixel._format->BytesPerPixel * diff;
	_pos += glm::ivec2{diff, lines};
	return *this;
}

tr::Bitmap::MutIt& tr::Bitmap::MutIt::operator+=(glm::ivec2 diff) noexcept
{
	return *this += (diff.y * _bitmap->size().x + diff.x);
}

tr::Bitmap::MutIt tr::operator+(const tr::Bitmap::MutIt& it, int diff) noexcept
{
	tr::Bitmap::MutIt copy{it};
	copy += diff;
	return copy;
}

tr::Bitmap::MutIt tr::operator+(int diff, const tr::Bitmap::MutIt& it) noexcept
{
	return it + diff;
}

tr::Bitmap::MutIt tr::operator+(const tr::Bitmap::MutIt& it, glm::ivec2 diff) noexcept
{
	tr::Bitmap::MutIt copy{it};
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
	MutIt copy{*this};
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
	tr::Bitmap::MutIt copy{it};
	copy -= diff;
	return copy;
}

tr::Bitmap::MutIt tr::operator-(int diff, const tr::Bitmap::MutIt& it) noexcept
{
	return it - diff;
}

tr::Bitmap::MutIt tr::operator-(const tr::Bitmap::MutIt& it, glm::ivec2 diff) noexcept
{
	tr::Bitmap::MutIt copy{it};
	copy -= diff;
	return copy;
}

tr::Bitmap::MutIt tr::operator-(glm::ivec2 diff, const tr::Bitmap::MutIt& it) noexcept
{
	return it - diff;
}

int tr::operator-(const tr::Bitmap::MutIt& l, const tr::Bitmap::MutIt& r) noexcept
{
	assert(l._bitmap == r._bitmap && l._bitmap != nullptr);
	return (l._pos.y * l._bitmap->size().x + l._pos.x) - (r._pos.y * r._bitmap->size().x + r._pos.x);
}
