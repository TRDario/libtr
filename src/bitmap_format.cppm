module;
#include <SDL2/SDL.h>

export module tr:bitmap_format;

export namespace tr {
	class BitmapFormat {
	public:
		enum class Type {
			UNKNOWN,
			RGBA32,
			ARGB32,
			BGRA32,
			ABGR32,
			INDEX_1LSB   = SDL_PIXELFORMAT_INDEX1LSB,
			INDEX_1MSB   = SDL_PIXELFORMAT_INDEX1MSB,
			INDEX_4LSB   = SDL_PIXELFORMAT_INDEX4LSB,
			INDEX_4MSB   = SDL_PIXELFORMAT_INDEX4MSB,
			INDEX_8      = SDL_PIXELFORMAT_INDEX8,
			RGB_332      = SDL_PIXELFORMAT_RGB332,
			XRGB_4444    = SDL_PIXELFORMAT_XRGB4444,
			XRGB_1555    = SDL_PIXELFORMAT_XRGB1555,
			XBGR_1555    = SDL_PIXELFORMAT_XBGR1555,
			ARGB_4444    = SDL_PIXELFORMAT_ARGB4444,
			RGBA_4444    = SDL_PIXELFORMAT_RGBA4444,
			ABGR_4444    = SDL_PIXELFORMAT_ABGR4444,
			BGRA_4444    = SDL_PIXELFORMAT_BGRA4444,
			ARGB_1555    = SDL_PIXELFORMAT_ARGB1555,
			RGBA_5551    = SDL_PIXELFORMAT_RGBA5551,
			ABGR_1555    = SDL_PIXELFORMAT_ABGR1555,
			BGRA_5551    = SDL_PIXELFORMAT_BGRA5551,
			RGB_565      = SDL_PIXELFORMAT_RGB565,
			BGR_565      = SDL_PIXELFORMAT_BGR565,
			RGB_24       = SDL_PIXELFORMAT_RGB24,
			BGR_24       = SDL_PIXELFORMAT_BGR24,
			XRGB_8888    = SDL_PIXELFORMAT_XRGB8888,
			RGBX_8888    = SDL_PIXELFORMAT_RGBX8888,
			XBGR_8888    = SDL_PIXELFORMAT_XBGR8888,
			BGRX_8888    = SDL_PIXELFORMAT_BGRX8888,
			ARGB_8888    = SDL_PIXELFORMAT_ARGB8888,
			RGBA_8888    = SDL_PIXELFORMAT_RGBA8888,
			ABGR_8888    = SDL_PIXELFORMAT_ABGR8888,
			BGRA_8888    = SDL_PIXELFORMAT_BGRA8888,
			ARGB_2101010 = SDL_PIXELFORMAT_ARGB2101010
		};
		using enum Type;
		// Enum representing an order of elements in a pixel format.
		enum class Order {
			BITMAPPED_NONE = 0,
			BITMAPPED_4321,
			BITMAPPED_1234,

			PACKED_NONE,
			PACKED_XRGB,
			PACKED_RGBX,
			PACKED_ARGB,
			PACKED_RGBA,
			PACKED_XBGR,
			PACKED_BGRX,
			PACKED_ABGR,
			PACKED_BGRA,

			ARRAY_NONE,
			ARRAY_RGB,
			ARRAY_RGBA,
			ARRAY_ARGB,
			ARRAY_BGR,
			ARRAY_BGRA,
			ARRAY_ABGR
		};
		// Enum representing a packed pixel format channel layout.
		enum class Layout {
			NONE,
			I8_332,
			I16_4444,
			I16_1555,
			I16_5551,
			I16_565,
			I32_8888,
			I32_2101010,
			I32_1010102
		};

		BitmapFormat(Type type) noexcept;

		operator Type() const noexcept;

		const char* name      () const noexcept;
		Order 	    order     () const noexcept;
		Layout 	    layout    () const noexcept;
		int 		pixelBits () const noexcept;
		int 		pixelBytes() const noexcept;
		bool 		isIndexed () const noexcept;
		bool 		isPacked  () const noexcept;
		bool 		isArray   () const noexcept;
		bool 		hasAlpha  () const noexcept;
	private:
		Type _type;
	};
}

// IMPLEMENTATION

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