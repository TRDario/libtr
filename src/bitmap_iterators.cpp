#include "../include/tr/bitmap_iterators.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace tr {
	RGBA8 getPixelColor(const void* data, const SDL_PixelFormatDetails* format);
}

tr::RGBA8 tr::getPixelColor(const void* data, const SDL_PixelFormatDetails* format)
{
	std::uint32_t value;
	switch (format->bytes_per_pixel) {
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
	SDL_GetRGBA(value, format, nullptr, &color.r, &color.g, &color.b, &color.a);
	return color;
}

tr::SubBitmap::PixelRef::PixelRef(const std::byte* ptr, const SDL_PixelFormatDetails* format) noexcept
	: _impl{ptr}, _format{format}
{
}

tr::SubBitmap::PixelRef::operator RGBA8() const noexcept
{
	return getPixelColor(_impl, _format);
}

tr::SubBitmap::Iterator::Iterator(SubBitmap bitmap, glm::ivec2 pos) noexcept
	: _pixel{bitmap.data() + bitmap.pitch() * pos.y + bitmap.format().pixelBytes() * pos.x,
			 SDL_GetPixelFormatDetails(bitmap._bitmap->format)}
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
	_pixel._impl = _pixel._impl + _pitch * lines + _pixel._format->bytes_per_pixel * diff;
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

tr::Bitmap::PixelRef::PixelRef(std::byte* ptr, const SDL_PixelFormatDetails* format) noexcept
	: _impl{ptr}, _format{format}
{
}

tr::Bitmap::PixelRef::operator RGBA8() const noexcept
{
	return getPixelColor(_impl, _format);
}

tr::Bitmap::PixelRef& tr::Bitmap::PixelRef::operator=(RGBA8 color) noexcept
{
	auto formatted{SDL_MapRGBA(_format, nullptr, color.r, color.g, color.b, color.a)};
	switch (_format->bytes_per_pixel) {
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
	: _pixel{bitmap.data() + bitmap.pitch() * pos.y + bitmap.format().pixelBytes() * pos.x,
			 SDL_GetPixelFormatDetails(bitmap._impl.get()->format)}
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
	_pixel._impl = _pixel._impl + _bitmap->pitch() * lines + _pixel._format->bytes_per_pixel * diff;
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
