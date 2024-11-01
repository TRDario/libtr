#include "../include/bitmap_format.hpp"
#include <SDL2/SDL.h>


tr::BitmapFormat::BitmapFormat(Type type) noexcept
    : _type { type }
{
    switch (type) {
    case RGBA32:
        _type = Type(SDL_PIXELFORMAT_RGBA32);
        break;
    case ARGB32:
        _type = Type(SDL_PIXELFORMAT_ARGB32);
        break;
    case BGRA32:
        _type = Type(SDL_PIXELFORMAT_BGRA32);
        break;
    case ABGR32:
        _type = Type(SDL_PIXELFORMAT_ABGR32);
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
    return SDL_GetPixelFormatName(SDL_PixelFormatEnum(_type));
}

tr::BitmapFormat::Order tr::BitmapFormat::order() const noexcept
{
    int order { SDL_PIXELORDER(SDL_PixelFormatEnum(_type)) };
    if (isPacked()) {
        order += int(Order::PACKED_NONE);
    }
    else if (isArray()) {
        order += int(Order::ARRAY_NONE);
    }
    return Order(order);
}

tr::BitmapFormat::Layout tr::BitmapFormat::layout() const noexcept
{
    return Layout(SDL_PIXELLAYOUT(SDL_PixelFormatEnum(_type)));
}

int tr::BitmapFormat::pixelBits() const noexcept
{
    return SDL_BITSPERPIXEL(SDL_PixelFormatEnum(_type));
}

int tr::BitmapFormat::pixelBytes() const noexcept
{
    return SDL_BYTESPERPIXEL(SDL_PixelFormatEnum(_type));
}

bool tr::BitmapFormat::isIndexed() const noexcept
{
    return SDL_ISPIXELFORMAT_INDEXED(SDL_PixelFormatEnum(_type));
}

bool tr::BitmapFormat::isPacked() const noexcept
{
    return SDL_ISPIXELFORMAT_PACKED(SDL_PixelFormatEnum(_type));
}

bool tr::BitmapFormat::isArray() const noexcept
{
    return SDL_ISPIXELFORMAT_ARRAY(SDL_PixelFormatEnum(_type));
}

bool tr::BitmapFormat::hasAlpha() const noexcept
{
    return SDL_ISPIXELFORMAT_ALPHA(SDL_PixelFormatEnum(_type));
}