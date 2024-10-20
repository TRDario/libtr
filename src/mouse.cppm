/**
 * @file keyboard.cppm
 * @brief Provides mouse functionality.
 */

module;
#include <cassert>
#include <SDL2/SDL.h>

export module tr:mouse;

import std;
import glm;
import :bitmap;
import :geometry;
import :sdl;

export namespace tr {
    class Cursor;


    /******************************************************************************************************************
     * System mouse cursor icons.
     ******************************************************************************************************************/
	enum class SysCursor {
        /**************************************************************************************************************
         * Default arrow cursor.
         **************************************************************************************************************/
		ARROW,

        /**************************************************************************************************************
         * I-shaped cursor.
         **************************************************************************************************************/
		IBEAM,

        /**************************************************************************************************************
         * Waiting cursor.
         **************************************************************************************************************/
		WAIT,

        /**************************************************************************************************************
         * Crosshair cursor.
         **************************************************************************************************************/
		CROSSHAIR,

        /**************************************************************************************************************
         * Waiting arrow cursor.
         **************************************************************************************************************/
		WAIT_ARROW,

        /**************************************************************************************************************
         * Resizing (northwest-southeast) cursor.
         **************************************************************************************************************/
		SIZE_NWSE,

        /**************************************************************************************************************
         * Resizing (northeast-southwest) cursor.
         **************************************************************************************************************/
		SIZE_NESW,

        /**************************************************************************************************************
         * Resizing (west-east) cursor.
         **************************************************************************************************************/
		SIZE_WE,

        /**************************************************************************************************************
         * Resizing (north-south) cursor.
         **************************************************************************************************************/
		SIZE_NS,

        /**************************************************************************************************************
         * Resizing (all directions) cursor.
         **************************************************************************************************************/
		SIZE_ALL,

        /**************************************************************************************************************
         * Forbidden action cursor.
         **************************************************************************************************************/
		NO,

        /**************************************************************************************************************
         * Pointing hand cursor.
         **************************************************************************************************************/
		HAND
	};


    /******************************************************************************************************************
     * Error thrown when cursor allocation fails.
     ******************************************************************************************************************/
    struct CursorBadAlloc : std::bad_alloc {
        /**************************************************************************************************************
         * Gets an error message.
         *
         * @return An explanatory error message.
	     **************************************************************************************************************/
        virtual const char* what() const noexcept;
    };


    /******************************************************************************************************************
     * Mouse button. Expressions like Button { [5 - 32] } are allowed.
     ******************************************************************************************************************/
	enum class MouseButton {
		LEFT = 1,
		MIDDLE,
		RIGHT,
		X1,
		X2
	};

    /******************************************************************************************************************
     * Mouse button mask. Opaque, meant to be interacted with with mouse::held.
     ******************************************************************************************************************/
	enum class MouseButtonMask : unsigned int {};


    /******************************************************************************************************************
     * Mouse-related functionality.
	 *
	 * Nothing in this namespace can be used before SDL is initialized.
     ******************************************************************************************************************/
    namespace mouse {
        /**************************************************************************************************************
         * Gets the position of the mouse.
         *
         * @return The position of the mouse relative to the top left corner of the window.
	     **************************************************************************************************************/
        glm::ivec2 pos() noexcept;

        /**************************************************************************************************************
         * Gets the delta of the mouse position.
         *
         * @return The mouse position delta between the last call to this function and now.
	     **************************************************************************************************************/
        glm::ivec2 delta() noexcept;


        /**************************************************************************************************************
         * Gets the held mouse buttons.
         *
         * @return The mouse button state in the form of a button mask.
	     **************************************************************************************************************/
        MouseButtonMask buttons() noexcept;

        /**************************************************************************************************************
         * Gets whether a specific mouse button is held.
         *
         * @param button A mouse button. May be any of the named ones or in the range [1, 32].
         *
         * @return True if the button is held, and false otherwise.
	     **************************************************************************************************************/
        bool held(MouseButton button) noexcept;
        
        /**************************************************************************************************************
         * Gets whether a specific mouse button is held for a given mask.
         *
         * @param button A mouse button. May be any of the named ones or in the range [1, 32].
         * @param mask The button mask to check.
         *
         * @return True if the button is held, and false otherwise.
	     **************************************************************************************************************/
        bool held(MouseButton button, MouseButtonMask mask) noexcept;


        /**************************************************************************************************************
         * Gets whether relative mouse mode is enabled.
         *
         * In relative mouse mode, only deltas are reported, mouse position isn't changed.
         *
         * @return True if relative mode is enabled, false otherwise.
	     **************************************************************************************************************/
        bool inRelativeMode() noexcept;
        
        /**************************************************************************************************************
         * Sets whether relative mouse mode is enabled.
         *
         * In relative mouse mode, only deltas are reported, mouse position isn't changed.
         *
         * @param relative Whether relative mouse mode should be enabled.
	     **************************************************************************************************************/
        bool setRelativeMode(bool relative) noexcept;


        /**************************************************************************************************************
         * Sets whether the mouse is captured.
         *
         * When captured, this application's window gets all the mouse events, good for dragging and such but not
         * for long periods of time.
         *
         * @param captured Whether the mouse should be captured.
	     **************************************************************************************************************/
        bool setCaptured(bool captured) noexcept;


        /**************************************************************************************************************
         * Gets whether the mouse cursor is visible.
         *
         * @return True if the cursor is shown, and false otherwise.
	     **************************************************************************************************************/
        bool cursorShown() noexcept;
        
        /**************************************************************************************************************
         * Sets whether the mouse cursor is visible.
         *
         * @param show Whether to show the mouse cursor.
	     **************************************************************************************************************/
        void showCursor(bool show) noexcept;

        /**************************************************************************************************************
         * Sets the mouse cursor.
         *
         * @param cursor A reference to a mouse cursor.
	     **************************************************************************************************************/
        void setCursor(const Cursor& cursor) noexcept;
    }

	/******************************************************************************************************************
     * Mouse cursor graphic.
     ******************************************************************************************************************/
	class Cursor {
	public:
		/**************************************************************************************************************
         * Creates the default mouse cursor.
         *
         * @exception CursorBadAlloc If allocating the cursor failed.
	     **************************************************************************************************************/
		Cursor();

        /**************************************************************************************************************
         * Creates a system cursor.
         *
         * @exception CursorBadAlloc If allocating the cursor failed.
         *
         * @param icon The cursor icon to use.
	     **************************************************************************************************************/
		Cursor(SysCursor icon);

        /**************************************************************************************************************
         * Creates a cursor from a bitmap.
         *
         * @exception CursorBadAlloc If allocating the cursor failed.
         *
         * @param bitmap The cursor bitmap.
         * @param focus The focus point of the cursor (where the actual mouse position is relative to the graphic).
	     **************************************************************************************************************/
		Cursor(const Bitmap& bitmap, glm::ivec2 focus);

        /**************************************************************************************************************
         * Creates a simple black-and-white cursor from color and mask bitfields.
         *
         * @exception CursorBadAlloc If allocating the cursor failed.
         *
         * @param color A span of bits where 0 is white and 1 black.
         * @param mask A span of bits determining the mask: 1 is opaque, 0 + white = transparent,
         *             0 + black = inverted color.
         * @param size The size of the cursor graphic, both coordinates must be multiples of 8 and match the sizes of
         *             the bitfields.
         * @param focus The focus point of the cursor (where the actual mouse position is relative to the graphic).
	     **************************************************************************************************************/
		Cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus);
	private:
		struct Deleter { void operator()(SDL_Cursor* ptr) noexcept; /**< @private */ };
		std::unique_ptr<SDL_Cursor, Deleter> _impl;

		friend void mouse::setCursor(const Cursor& cursor) noexcept;
	};
}

/// @cond IMPLEMENTATION

namespace tr {
    // Checks if a cursor if not null and throws CursorBadAlloc otherwise.
    SDL_Cursor* checkNotNull(SDL_Cursor* ptr);
}

SDL_Cursor* tr::checkNotNull(SDL_Cursor* ptr)
{
    if (ptr == nullptr) {
        throw CursorBadAlloc {};
    }
    return ptr;
}

const char* tr::CursorBadAlloc::what() const noexcept {
    return "failed cursor allocation";
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
    return std::uint32_t(mask) & SDL_BUTTON(std::uint32_t(button));
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

tr::Cursor::Cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus)
{
    assert(color.size() == mask.size() && color.size() == size.x * size.y / 64);
    _impl.reset(checkNotNull(SDL_CreateCursor((const std::uint8_t*)(color.data()), (const std::uint8_t*)(mask.data()), size.x, size.y, focus.x, focus.y)));
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

/// @endcond