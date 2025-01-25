#include "../include/tr/bitmap_format.hpp"
#include <SDL2/SDL.h>

tr::BitmapFormat::BitmapFormat(Type type) noexcept
	: _type{type}
{
	switch (type) {
	case RGBA32:
		_type = static_cast<Type>(SDL_PIXELFORMAT_RGBA32);
		break;
	case ARGB32:
		_type = static_cast<Type>(SDL_PIXELFORMAT_ARGB32);
		break;
	case BGRA32:
		_type = static_cast<Type>(SDL_PIXELFORMAT_BGRA32);
		break;
	case ABGR32:
		_type = static_cast<Type>(SDL_PIXELFORMAT_ABGR32);
		break;
	default:
		break;
	}
}

tr::BitmapFormat::operator Type() const noexcept
{
	return _type;
}

const char* tr::BitmapFormat::name() const noexcept
{
	return SDL_GetPixelFormatName(static_cast<SDL_PixelFormatEnum>(_type));
}

tr::BitmapFormat::Order tr::BitmapFormat::order() const noexcept
{
	int order{SDL_PIXELORDER(static_cast<SDL_PixelFormatEnum>(_type))};
	if (isPacked()) {
		order += static_cast<int>(Order::PACKED_NONE);
	}
	else if (isArray()) {
		order += static_cast<int>(Order::ARRAY_NONE);
	}
	return static_cast<Order>(order);
}

tr::BitmapFormat::Layout tr::BitmapFormat::layout() const noexcept
{
	return Layout(SDL_PIXELLAYOUT(static_cast<SDL_PixelFormatEnum>(_type)));
}

int tr::BitmapFormat::pixelBits() const noexcept
{
	return SDL_BITSPERPIXEL(static_cast<SDL_PixelFormatEnum>(_type));
}

int tr::BitmapFormat::pixelBytes() const noexcept
{
	return SDL_BYTESPERPIXEL(static_cast<SDL_PixelFormatEnum>(_type));
}

bool tr::BitmapFormat::isIndexed() const noexcept
{
	return SDL_ISPIXELFORMAT_INDEXED(static_cast<SDL_PixelFormatEnum>(_type));
}

bool tr::BitmapFormat::isPacked() const noexcept
{
	return SDL_ISPIXELFORMAT_PACKED(static_cast<SDL_PixelFormatEnum>(_type));
}

bool tr::BitmapFormat::isArray() const noexcept
{
	return SDL_ISPIXELFORMAT_ARRAY(static_cast<SDL_PixelFormatEnum>(_type));
}

bool tr::BitmapFormat::hasAlpha() const noexcept
{
	return SDL_ISPIXELFORMAT_ALPHA(static_cast<SDL_PixelFormatEnum>(_type));
}
