module;
#include <cassert>
#include <SDL2/SDL.h>

export module tr:mouse;

import std;
import glm;
import :bitmap;
import :geometry;
import :integer;
import :sdl;

export namespace tr {
	// Enum representing a system cursor.
	enum class SysCursor {
		ARROW,
		IBEAM,
		WAIT,
		CROSSHAIR,
		WAIT_ARROW,
		SIZE_NWSE,
		SIZE_NESW,
		SIZE_WE,
		SIZE_NS,
		SIZE_ALL,
		NO,
		HAND
	};

    struct CursorBadAlloc : std::bad_alloc {
        virtual const char* what() const noexcept { return "failed cursor allocation"; }
    };

    class Cursor;
    namespace mouse {
        // Sets the mouse cursor.
        void setCursor(const Cursor& cursor) noexcept;
    }
	// Class representing a mouse cursor graphic.
	class Cursor {
	public:
		// Creates the default mouse cursor.
		Cursor();
		// Creates a system cursor.
		Cursor(SysCursor icon);
		// Creates a cursor from a bitmap.
		// focus - The focus point of the cursor (where the actual mouse position is relative to the graphic).
		Cursor(const Bitmap& bitmap, glm::ivec2 focus);
		// Constructs a simple black-and-white cursor.
		// color - A span of bits where 0 is white and 1 black.
		// mask - A span of bits determining the mask: 1 is opaque, 0 + white = transparent, 0 + black = inverted color.
		// size - The size of the cursor graphic, must be multiples of 8.
		// focus - The focus point of the cursor (where the actual mouse position is relative to the graphic).
		Cursor(std::span<const Byte> color, std::span<const Byte> mask, glm::ivec2 size, glm::ivec2 focus);
	private:
		struct Deleter { void operator()(SDL_Cursor* ptr) noexcept; };
		std::unique_ptr<SDL_Cursor, Deleter> _impl;

		friend void mouse::setCursor(const Cursor& cursor) noexcept;
	};

	// Enum representing a mouse button. Expressions like Button { [5 - 32] } are allowed.
	enum class MouseButton {
		LEFT = 1,
		MIDDLE,
		RIGHT,
		X1,
		X2
	};
	// Enum representing a mouse button mask. Opaque, meant to be interacted with with tr::held.
	enum class MouseButtonMask : unsigned int {};

	namespace mouse {
        // Gets the position of the mouse (relative to the top left corner of the window).
        glm::ivec2 pos() noexcept;
        // Gets the mouse position delta between the last call to this function and now.
        glm::ivec2 delta() noexcept;

        // Gets the mouse button state in the form of a button mask.
        MouseButtonMask buttons() noexcept;
        // Gets whether a mouse button is held.
        // button - A mouse button. May be any of the named ones or in the range [1, 32].
        bool held(MouseButton button) noexcept;
        // Gets whether a mouse button is held for a given mask.
        // button - A mouse button. May be any of the named ones or in the range [1, 32].
        bool held(MouseButton button, MouseButtonMask mask) noexcept;

        // Gets if relative mouse mode is enabled.
        bool inRelativeMode() noexcept;
        // Sets whether relative mode (only deltas are reported, mouse position isn't changed) is enabled.
        bool setRelativeMode(bool relative) noexcept;

        // Sets whether the mouse is captured (this application's window gets all the mouse events, good for dragging and such but not for long periods of time).
        bool setCaptured(bool captured) noexcept;

        // Gets whether the cursor is visible.
        bool cursorShown() noexcept;
        // Sets whether the cursor is visible.
        void showCursor(bool show) noexcept;
    }
}

// IMPLEMENTATION

namespace tr {
    SDL_Cursor* checkNotNull(SDL_Cursor* ptr);
}

SDL_Cursor* tr::checkNotNull(SDL_Cursor* ptr)
{
    if (ptr == nullptr) {
        throw CursorBadAlloc {};
    }
    return ptr;
}

glm::ivec2 tr::mouse::pos() noexcept
{
    glm::ivec2 pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

glm::ivec2 tr::mouse::delta() noexcept
{
    glm::ivec2 delta;
    SDL_GetRelativeMouseState(&delta.x, &delta.y);
    return delta;
}

tr::MouseButtonMask tr::mouse::buttons() noexcept
{
    return MouseButtonMask(SDL_GetMouseState(nullptr, nullptr));
}

bool tr::mouse::held(MouseButton button) noexcept
{
    return held(button, buttons());
}

bool tr::mouse::held(MouseButton button, MouseButtonMask mask) noexcept
{
    return Ui32(mask) & SDL_BUTTON(Ui32(button));
}

bool tr::mouse::inRelativeMode() noexcept
{
    return SDL_GetRelativeMouseMode();
}

bool tr::mouse::setRelativeMode(bool relative) noexcept
{
    return !SDL_SetRelativeMouseMode(SDL_bool(relative));
}

bool tr::mouse::setCaptured(bool captured) noexcept
{
    return !SDL_CaptureMouse(SDL_bool(captured));
}

tr::Cursor::Cursor()
    : _impl { checkNotNull(SDL_GetDefaultCursor()) }
{}

tr::Cursor::Cursor(SysCursor icon)
    : _impl { checkNotNull(SDL_CreateSystemCursor(SDL_SystemCursor(icon))) }
{}

tr::Cursor::Cursor(const Bitmap& bitmap, glm::ivec2 focus)
    : _impl { checkNotNull(SDL_CreateColorCursor(bitmap._impl.get(), focus.x, focus.y)) }
{}

tr::Cursor::Cursor(std::span<const Byte> color, std::span<const Byte> mask, glm::ivec2 size, glm::ivec2 focus)
{
    assert(color.size() == mask.size() && color.size() == size.x * size.y / 64);
    _impl.reset(checkNotNull(SDL_CreateCursor((const Ui8*)(color.data()), (const Ui8*)(mask.data()), size.x, size.y, focus.x, focus.y)));
}

void tr::Cursor::Deleter::operator()(SDL_Cursor* ptr) noexcept
{
    SDL_FreeCursor(ptr);
}

void tr::mouse::setCursor(const Cursor& cursor) noexcept
{
    SDL_SetCursor(cursor._impl.get());
}

bool tr::mouse::cursorShown() noexcept
{
    return SDL_ShowCursor(SDL_QUERY);
}

void tr::mouse::showCursor(bool show) noexcept
{
    SDL_ShowCursor(show);
}